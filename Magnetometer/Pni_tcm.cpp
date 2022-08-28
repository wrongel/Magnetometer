#include "stdafx.h"
#include "Pni_tcm.h"
#include "Protocol.h"
#include "Magnetometer.hxx"

Pni_tcm::Pni_tcm()
{
	m_hport = NULL;
	m_sError = NULL;
	*m_sCommName = NULL;
	m_nBaudPort = 0;
	*m_sBaudMagn = NULL;

	R = NULL;
	U = NULL;
}

Pni_tcm::~Pni_tcm()
{
	if (m_hport != INVALID_HANDLE_VALUE)
		CloseHandle(m_hport);

	if (R != NULL)
	{ 
		for (int i = 0; i < CalibPoints; i++)
			R[i].de_allocate();
		delete[] R;
		R = NULL;
	}
	if (U != NULL)
	{
		for (int i = 0; i < CalibPoints; i++)
			U[i].de_allocate();
		delete[] U;
		U = NULL;
	}
	if (Bp.data)
		Bp.de_allocate();
	if (Vp.data)
		Vp.de_allocate();
	if (Wp.data)
		Wp.de_allocate();
}


int Pni_tcm::InitCom(TCHAR* sCommName, int nBaudPort, int nBaudMagn)
{
	// Определения имени COM порта
	HKEY hKey;	// Дескриптор ключа реестра
	LONG lResult;	// Результат операции
	DWORD MaxData = MAX_PATH;	// Размер буффера под имя порта
	TCHAR achValue[DNS_MAX_NAME_LENGTH];	// Название значения в ключе
	DWORD cchValue = DNS_MAX_NAME_LENGTH;	// Размер названия значения
	DWORD cValues;		// Количество значений для ключа
	
	// Открытие ключа
	//if (!sCommName)
	if (sCommName[0] == '\0')
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			pMainDlg->AddMessage("Не удалось открыть файл реестра", 1);
			//pProtocol->WriteLog("Не удалось открыть файл реестра\r\n");
			return 0;
		}
		pMainDlg->AddMessage("Успешно открыт файл реестра", 0);
		//pProtocol->WriteLog("Успешно открыт файл реестра\r\n");

		// Получение количества значений
		lResult = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &cValues, NULL, NULL, NULL, NULL);
		// Получение имени COM порта
		if (cValues)
		{
			for (unsigned int i = 0, lResult = ERROR_SUCCESS; i < cValues; i++)
			{
				achValue[0] = '\0';
				lResult = RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, (BYTE*)m_sCommName, &MaxData);
			}
		}
		RegCloseKey(hKey);	// Закрытие ключа
	}
	else
		_tcscpy_s(m_sCommName, sCommName);
	// Открытие порта
	if (m_hport != NULL)
		CloseHandle(m_hport);
	m_hport = ::CreateFile(m_sCommName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	//Проверка
	if (m_hport == INVALID_HANDLE_VALUE)
	{
		pMainDlg->AddMessage("Ошибка открытия порта", 1);
		//pProtocol->WriteLog("Ошибка открытия порта\r\n");
		return 0;
	}
	
	_stprintf_s(pMes, "%s%s%s", "Успешно открыт порт ", m_sCommName, "\r\n");
	//pProtocol->WriteLog(pMes);
	pMainDlg->AddMessage(pMes, 0);

	// Выставление параметров порта
	DCB param = { 0 };
	param.DCBlength = sizeof(param);
	if (!GetCommState(m_hport, &param))
	{
		pMainDlg->AddMessage("Не удалось получить параметры управляющей структуры порта", 1);
		//pProtocol->WriteLog("Не удалось получить параметры управляющей структуры порта\r\n");
		return 0;
	}

	m_nBaudPort = nBaudPort;
	param.BaudRate = nBaudPort;		// Частота
	param.ByteSize = 8;			// Размер байта
	param.StopBits = ONESTOPBIT;	// Один стоповый бит
	param.Parity = NOPARITY;	// Нет контроля четности
	if (!SetCommState(m_hport, &param))
	{
		pMainDlg->AddMessage("Не удалось применить управляющую структуру к порту", 1);
		//pProtocol->WriteLog("Не удалось применить управляющую структуру к порту\r\n");
		return 0;
	}
	pMainDlg->AddMessage("Успешно применена управляющая структура к порту", 0);
	//pProtocol->WriteLog("Успешно применена управляющая структура к порту\r\n");		

	// Выставление тайм-аутов
	COMMTIMEOUTS commTimeouts;
	GetCommTimeouts(m_hport, &commTimeouts);
	commTimeouts.ReadIntervalTimeout = 50;			// Между двумя принимаемыми символами
	commTimeouts.ReadTotalTimeoutConstant = 500;//200;	// Константа для вычисления полного тайм-аута
	commTimeouts.ReadTotalTimeoutMultiplier = 0;// 50;	// Множитель для вычисления полного тайм-аута
													// Полный максимальный тайм-аут операции чтения ReadTotalTimeoutConstant + (ReadIntervalTimeout * количество считываемых байт)

	if (!SetCommTimeouts(m_hport, &commTimeouts))
	{
		pMainDlg->AddMessage("Не удалось выставить тайм-ауты порта", 1);
		//pProtocol->WriteLog("Не удалось выставить тайм-ауты порта\r\n");
		return 0;
	}
	pMainDlg->AddMessage("Успешно выставлены тайм-ауты порта", 0);
	//pProtocol->WriteLog("Успешно выставлены тайм-ауты порта\r\n");

	// Проверка частоты
	WriteCOM("b?\n", m_sBaudMagn, _countof(m_sBaudMagn));
	if (_tcsrchr(m_sBaudMagn, 'E'))
		WriteCOM("b?\n", m_sBaudMagn, _countof(m_sBaudMagn));		// Повторное считывание для очистки мусора, после неудачного подключения

	if (nBaudPort != _tstoi(GetBaudMagn()))		// Если частота порта не совпадает частотой магнитометра
	{
		_stprintf_s(pMes, "%s%d%s", "Не удалось подключиться к магнитометру при заданной частоте порта ", nBaudPort, "\r\n");
		//pProtocol->WriteLog(pMes);
		pMainDlg->AddMessage(pMes, 1);
		return 0;
	}

	// Задание новой частоты
	if (nBaudPort != nBaudMagn)		// Если задана новая частота магнитометра
	{
		TCHAR sBaudMes[6];
		_stprintf_s(sBaudMes, "%s%s%s", "b=", ConvertBaudRate(nBaudMagn), "\n");
		WriteCOM(sBaudMes, sBaudMes, _countof(sBaudMes));

		if (_tcsrchr(sBaudMes, 'E'))
			//pProtocol->WriteLog("Не удалось настроить магнитометр\r\n");
			pMainDlg->AddMessage("Не удалось настроить магнитометр", 1);
		else
		{
			_stprintf_s(m_sBaudMagn, "%d", nBaudMagn);	// Сохранение новой частоты
			_stprintf_s(pMes, "%s%s%s", "Магнитометр настроен на новую частоту ", m_sBaudMagn, "\r\n");
			//pProtocol->WriteLog(pMes);
			pMainDlg->AddMessage(pMes, 1);
		}
		return 0;
	}
	
	// Задание дополнительных настроек
	TCHAR sMAns[10];
	TCHAR sTAns[10];		
	WriteCOM("em=e\n", sMAns, _countof(sMAns));		// Включить показания магнитометра в выходное слово
	WriteCOM("et=e\n", sTAns, _countof(sTAns));		// Включить показания температуры в выходное слово
	if (_tcsrchr(sMAns, 'E') || _tcsrchr(sTAns, 'E'))
	{
		//pProtocol->WriteLog("Не удалось настроить магнитометр\r\n");
		pMainDlg->AddMessage("Не удалось настроить магнитометр", 1);
		return 0;
	}

	_stprintf_s(pMes, "%s%d%s", "Успешно настроен магнитометр c частотой ", nBaudMagn, "\r\n");
	//pProtocol->WriteLog(pMes);
	pMainDlg->AddMessage(pMes, 1);

	_tcscpy_s(sCommName, _tcslen(m_sCommName) + 1, m_sCommName);
	return 1;
}


TCHAR* Pni_tcm::ConvertBaudRate(int nBaudRate)
{
	switch (nBaudRate)
	{
		case 300:
			return "1";
		case 1200:
			return "2";
		case 2400:
			return "3";
		case 4800:
			return "4";
		case 9600:
			return "5";
		case 19200:
			return "6";
		case 38400:
			return "7";
		case 115200:
			return "8";
		default:
			return "";
	}
}


TCHAR* Pni_tcm::WriteCOM(TCHAR* sQuery, TCHAR* sReceivedData, int nBufSize)
{
	DWORD dwBytesWritten;	// Количество байт, полученных портом
	if (!WriteFile(m_hport, sQuery, _tcslen(sQuery), &dwBytesWritten, NULL))
		return NULL;

	DWORD iSize = 0;	// Количество принятых байт
	ReadFile(m_hport, sReceivedData, nBufSize, &iSize, NULL);

	return sReceivedData;		//sReceivedData и так вернется, тут можно возвращать все что угодно
}

void Pni_tcm::UpdateMData()
{
	TCHAR sReceivedData[100] = { 0 };
	
	if (!bModePr)
		WriteCOM("s?\n", sReceivedData, _countof(sReceivedData));			// Если тронуть металлом контакт магнитометра, то в качестве ответа может придти 
																			// строка информации или /r или пустая строка
	else
	{
		std::string sQuery;
		std::string sReceiveData;
		sQuery = "s?";
		pMA->SendCommand(sQuery, sReceiveData);
		_tcscpy_s(sReceivedData, sReceiveData.c_str());
	}

	sReceivedData[1] = '\n';
	_tcscpy_s(m_sComp, SingleData(sReceivedData, 'P'));
	_tcscpy_s(m_sPitch, SingleData(_tcsrchr(sReceivedData, 'P'), 'R'));
	_tcscpy_s(m_sRoll, SingleData(_tcsrchr(sReceivedData, 'R'), 'X'));
	_tcscpy_s(m_sX, SingleData(_tcsrchr(sReceivedData, 'X'), 'Y'));
	/*_tcscpy_s(m_sY, SingleData(_tcsrchr(sReceivedData, 'Y'), 'Z'));
	_tcscpy_s(m_sZ, SingleData(_tcsrchr(sReceivedData, 'Z'), 'T'));*/
	_tcscpy_s(m_sZ, SingleData(_tcsrchr(sReceivedData, 'Y'), 'Z'));			// Y магнитометра = Zт
	_tcscpy_s(m_sY, SingleData(_tcsrchr(sReceivedData, 'Z'), 'T'));			// Z магнитометра = -Yт
	_tcscpy_s(m_sT, SingleData(_tcsrchr(sReceivedData, 'T'), '*'));

	double temp = _ttof(m_sY);
	temp *= (-1);
	_stprintf_s(m_sY, "%.2f", temp);
}

TCHAR* Pni_tcm::SingleData(TCHAR* sInput, TCHAR cSepar)
{
	try {
		int i = 0;
		while (1)
		{
			if (sInput[i + 1] == cSepar)
			{
				m_sOutchar[i] = '\0';
				if (sInput[0] == 'T')
					SetEr();
				break;
			}
			m_sOutchar[i] = sInput[i + 1];
			i++;
		}
	}
	catch (...)
	{
		AfxMessageBox("sad");
	}
	return m_sOutchar;
}

void Pni_tcm::SetEr()
{
	if (TCHAR* temp = _tcsrchr(m_sOutchar, 'E'))
	{
		int npos = (int)(temp - m_sOutchar);
		if (!_tcscmp(temp, "E002"))
			m_sError = "Превышено максимальное значение по крену или дифференту ±50°";
		else if (!_tcscmp(temp, "E004"))
			m_sError = "Превышено максимальное значение на одной из осей ±80 мкТл";
		else if (!_tcscmp(temp, "E006"))
			m_sError = "Превышены максимальные значения инклинометра и магнитометра";
		m_sOutchar[npos] = '\0';
	}
	else
		m_sError = NULL;
}

int Pni_tcm::MCalib()
{
	/*std::string sQuery;
	std::string sReceiveData;
	sQuery = "s?";*/
	//_tcscpy(sReceivedData, sReceiveData.c_str());
	//char a = sReceiveData.front();


	TCHAR sReceivedData[100] = "";
	//TCHAR sCalibRate[15];
	WriteCOM("cc\r", sReceivedData, _countof(sReceivedData));
	WriteCOM("mpcal=e\n", sReceivedData, _countof(sReceivedData));
	WriteCOM("go\r", sReceivedData, _countof(sReceivedData));
	AfxMessageBox("Вращайте магнитометр. Нажмите кнопку ОК по окончании", MB_ICONINFORMATION);
	WriteCOM("h\r", sReceivedData, _countof(sReceivedData));
	WriteCOM("mpcal=d\n", sReceivedData, _countof(sReceivedData));
	//Цикл очистка от накопившегося мусора
	while (sReceivedData[0] != 'H')
		WriteCOM("lc?\r", sReceivedData, _countof(sReceivedData));

	// Получение показателей калибровки
	int nPos = _tcschr(sReceivedData, '\r') - sReceivedData;
	_tcsncpy_s(sLastCalib, sReceivedData, nPos);
	sLastCalib[nPos] = '\0';

	// Получение показателя калибровки магнитного поля
	_tcsncpy_s(sReceivedData, sLastCalib + 5, 10);

	// Проверка успешности калибровки
	if (_ttoi(&sLastCalib[1]) < 9 || _ttoi(&sLastCalib[3]) < 9 )//|| _ttof(sReceivedData) > 10)
		return 0;
	return 1;
}

//void Pni_tcm::Calibr()
//{
//	double pi = 3.1415926535897;
//	double S0 = 0;
//
//	// Очистить память если калибровка запущена не в первый раз
//	if (R != NULL)
//	{
//		for (int i = 0; i < CalibPoints; i++)
//			R[i].de_allocate();
//		delete[] R;
//		R = NULL;
//	}
//	if (U != NULL)
//	{
//		for (int i = 0; i < CalibPoints; i++)
//			U[i].de_allocate();
//		delete[] U;
//		U = NULL;
//	}
//	if (Bp.data)
//		Bp.de_allocate();
//	if (Vp.data)
//		Vp.de_allocate();
//	if (Wp.data)
//		Wp.de_allocate();
//
//	U = new DMatrix[CalibPoints];
//	R = new DMatrix[CalibPoints];
//
//	DMatrix Rx, Ry, Rz;
//	DMatrix SR;
//	Rx.Ini(3, 3, 3, 3);
//	Ry.Ini(3, 3, 3, 3);
//	Rz.Ini(3, 3, 3, 3);
//	SR.Ini(3, 3, 3, 3);
//
//	SR = SR * 0;
//	for (int i = 0; i < CalibPoints; i++)
//	{
//		double nSumCol = 0;
//		CalibData[0][i] *= pi / 180;
//		CalibData[1][i] *= pi / 180;
//		CalibData[2][i] *= pi / 180;
//
//		// Нескомпенсированный сигнал
//		U[i].Ini(3, 1, 3, 3);
//		for (int j = 0; j < 3; j++)
//		{
//			U[i].data[j][0] = CalibData[j + 3][i];
//			nSumCol += pow(U[i].data[j][0], 2);
//		}
//		S0 += sqrt(nSumCol);	// Um в новой калибровке
//
//		Rx.data[0][0] = 1;						Rx.data[0][1] = 0;						 Rx.data[0][2] = 0;
//		Rx.data[1][0] = 0;						Rx.data[1][1] = cos(CalibData[1][i]);    Rx.data[1][2] = sin(CalibData[1][i]);
//		Rx.data[2][0] = 0;						Rx.data[2][1] = -sin(CalibData[1][i]);   Rx.data[2][2] = cos(CalibData[1][i]);
//
//		Ry.data[0][0] = cos(CalibData[0][i]);	Ry.data[0][1] = 0;						 Ry.data[0][2] = sin(CalibData[0][i]);
//		Ry.data[1][0] = 0;						Ry.data[1][1] = 1;						 Ry.data[1][2] = 0;
//		Ry.data[2][0] = -sin(CalibData[0][i]);	Ry.data[2][1] = 0;						 Ry.data[2][2] = cos(CalibData[0][i]);
//
//		Rz.data[0][0] = cos(CalibData[2][i]);	Rz.data[0][1] = sin(CalibData[2][i]);    Rz.data[0][2] = 0;
//		Rz.data[1][0] = -sin(CalibData[2][i]);	Rz.data[1][1] = cos(CalibData[2][i]);    Rz.data[1][2] = 0;
//		Rz.data[2][0] = 0;						Rz.data[2][1] = 0;						 Rz.data[2][2] = 1;
//
//		R[i].Ini(3, 3, 3, 3);
//		R[i] = Rx * Rz;
//		R[i] = R[i] * Ry;
//
//		SR = SR + R[i];
//	}
//
//	S0 /= CalibPoints;
//
//	if (Rx.data)
//		Rx.de_allocate();
//	if (Ry.data)
//		Ry.de_allocate();
//	if (Rz.data)
//		Rz.de_allocate();
//
//	DMatrix a11, a12, a21, a22;
//	DMatrix b1, b2;
//	double sp = S0;
//	int pp = 0;
//	Wp.Ini(3, 3, 3, 3);
//	Bp.Ini(3, 1, 3, 3);
//	Vp.Ini(3, 1, 3, 3);
//	a11.Ini(3, 3, 3, 3);
//	a12.Ini(3, 3, 3, 3);
//	a21.Ini(3, 3, 3, 3);
//	a22.Ini(3, 3, 3, 3);
//	b1.Ini(3, 1, 3, 3);
//	b2.Ini(3, 1, 3, 3);
//
//	Wp = Wp * 0;
//	Wp.data[0][0] = 1;
//	Wp.data[1][1] = 1;
//	Wp.data[2][2] = 1;
//
//	a22 = a22 * 0;
//	a22.data[0][0] = 1;
//	a22.data[1][1] = 1;
//	a22.data[2][2] = 1;
//
//	b2 = b2 * 0;
//	for (int i = 0; i < CalibPoints; i++)
//	{
//		b2.data[0][0] += U[i].data[0][0];
//		b2.data[1][0] += U[i].data[1][0];
//		b2.data[2][0] += U[i].data[2][0];
//	}
//	b2 = b2 * (1. / CalibPoints);
//
//	while (/*sp > S0 / pow(10, 4) ||*/ pp < 15)	// В комментарии ограничение по точности, вместе с г.к. решили отказаться от этого
//	{
//		pp++;
//		//a21 = Wp*SR *(1. / nSize);
//		a21 = Wp*SR;
//		a21 = a21*(1. / CalibPoints);
//		a12 = a21.T();
//		b1 = b1 * 0;
//		a11 = a11 * 0;
//
//		DMatrix WW;
//		WW.Ini(3, 3, 3, 3);
//		WW = Wp.T();
//		WW = WW * Wp;
//		for (int i = 0; i < CalibPoints; i++)
//		{
//			//b1 = b1 + R[i].T()*Wp.T()*U[i] * (1. / nSize);
//			DMatrix Temp1, Temp2, Temp3;
//			Temp1.Ini(3, 3, 3, 3);
//			Temp2.Ini(3, 3, 3, 3);
//			Temp3.Ini(3, 1, 3, 3);
//			Temp1 = R[i].T();
//			Temp2 = Wp.T();
//			Temp1 = Temp1 * Temp2;
//			Temp3 = Temp1 * U[i];
//			Temp3 = Temp3 * (1. / CalibPoints);
//			b1 = b1 + Temp3;
//
//			//a11 = a11 + ((Wp*R[i]).T()*(Wp*R[i])) * (1. / nSize); Старый вариант
//			/*Temp = Wp*R[i];
//			Temp2 = Temp.T();
//			Temp2 = Temp2 * Temp;
//			Temp2 = Temp2 * (1. / CalibPoints);
//			a11 = a11 + Temp2;*/
//
//			//WW=Wp'*Wp; a11=a11+R(:,:,k)'*WW*R(:,:,k)/Q; Новый вариант
//
//			Temp1 = R[i].T();
//			Temp1 = Temp1 * WW;
//			Temp1 = Temp1 * R[i];
//			Temp1 = Temp1 * (1. / CalibPoints);
//			a11 = a11 + Temp1;
//
//			Temp1.de_allocate();
//			Temp2.de_allocate();
//			Temp3.de_allocate();
//		}
//		WW.de_allocate();
//		
//		DMatrix AA;
//		AA.Ini(6, 6, 6, 6);
//		AA.data[0][0] = a11.data[0][0]; AA.data[0][1] = a11.data[0][1]; AA.data[0][2] = a11.data[0][2];  AA.data[0][3] = a12.data[0][0]; AA.data[0][4] = a12.data[0][1]; AA.data[0][5] = a12.data[0][2];
//		AA.data[1][0] = a11.data[1][0]; AA.data[1][1] = a11.data[1][1]; AA.data[1][2] = a11.data[1][2];  AA.data[1][3] = a12.data[1][0]; AA.data[1][4] = a12.data[1][1]; AA.data[1][5] = a12.data[1][2];
//		AA.data[2][0] = a11.data[2][0]; AA.data[2][1] = a11.data[2][1]; AA.data[2][2] = a11.data[2][2];  AA.data[2][3] = a12.data[2][0]; AA.data[2][4] = a12.data[2][1]; AA.data[2][5] = a12.data[2][2];
//
//		AA.data[3][0] = a21.data[0][0]; AA.data[3][1] = a21.data[0][1]; AA.data[3][2] = a21.data[0][2];  AA.data[3][3] = a22.data[0][0]; AA.data[3][4] = a22.data[0][1]; AA.data[3][5] = a22.data[0][2];
//		AA.data[4][0] = a21.data[1][0]; AA.data[4][1] = a21.data[1][1]; AA.data[4][2] = a21.data[1][2];  AA.data[4][3] = a22.data[1][0]; AA.data[4][4] = a22.data[1][1]; AA.data[4][5] = a22.data[1][2];
//		AA.data[5][0] = a21.data[2][0]; AA.data[5][1] = a21.data[2][1]; AA.data[5][2] = a21.data[2][2];  AA.data[5][3] = a22.data[2][0]; AA.data[5][4] = a22.data[2][1]; AA.data[5][5] = a22.data[2][2];
//
//		DMatrix BB;
//		BB.Ini(6, 1, 6, 6);
//		BB.data[0][0] = b1.data[0][0];
//		BB.data[1][0] = b1.data[1][0];
//		BB.data[2][0] = b1.data[2][0];
//		BB.data[3][0] = b2.data[0][0];
//		BB.data[4][0] = b2.data[1][0];
//		BB.data[5][0] = b2.data[2][0];
//
//		AA = AA.Inverse();
//		DMatrix VB;
//		VB.Ini(6, 1, 6, 6);
//		VB = AA * BB;
//
//		Bp.data[0][0] = VB.data[0][0];
//		Bp.data[1][0] = VB.data[1][0];
//		Bp.data[2][0] = VB.data[2][0];
//		Vp.data[0][0] = VB.data[3][0];
//		Vp.data[1][0] = VB.data[4][0];
//		Vp.data[2][0] = VB.data[5][0];
//
//		//Оценка мягкого железа
//		DMatrix SRB, SUB, XR;
//		SRB.Ini(3, 3, 3, 3); SUB.Ini(3, 3, 3, 3); XR.Ini(3, 1, 3, 3);
//		SRB = SRB * 0;
//		SUB = SUB * 0;
//
//		for (int i = 0; i < CalibPoints; i++)
//		{
//			// Неправильный 1 вариант
//			/*XR = (R[i] + (SR * (-1. / nSize)))*Bp;
//			SRB = SRB + XR * XR.T();
//			SUB = SUB + (U[i] + (b2 * (-1))) * XR.T();*/
//			
//			// XR = R[i] * Bp; Было исправлено на это
//
//			// XR=(R(:,:,k)-SR/Q)*Bp;
//			DMatrix Temp;
//			Temp.Ini(3, 3, 3, 3);
//			Temp = SR * (-1. / CalibPoints);
//			Temp = R[i] + Temp;
//			XR = Temp * Bp;
//			Temp.de_allocate();
//			
//			//SRB = SRB + XR * XR.T();
//			Temp.Ini(1, 3, 1, 1);
//			DMatrix Temp2;
//			Temp2.Ini(3, 3, 3, 3);
//			Temp = XR.T();
//			Temp2 = XR * Temp;
//			SRB = SRB + Temp2;
//
//			//SUB = SUB + (U[i] + (b2 * (-1))) * XR.T();
//			// 09.11.2016 не ясно почему я использовал такую формулу SUB = SUB + (U[i] + (Vp * (-1))) * XR.T() когда у г.к. везде SUB=SUB+(U(:,k)-b2)*XR'. Спросить если будут проблемы
//			DMatrix Temp3;
//			Temp3.Ini(3, 1, 3, 3);
//			Temp3 = b2 * (-1);
//			Temp3 = U[i] + Temp3;
//			Temp2 = Temp3 * Temp;
//			SUB = SUB + Temp2;
//
//			Temp.de_allocate();
//			Temp2.de_allocate();
//			Temp3.de_allocate();
//		}
//
//		//Wp = SUB * SRB.Inverse();
//		SRB = SRB.Inverse();	// Затирается так как больше нигде не используется
//		Wp = SUB * SRB;
//
//		// Нормировка
//		double kn = pow(Wp.data[0][0] * Wp.data[1][1] * Wp.data[1][1], 1. / 3);
//		Wp = Wp * (1. / kn);
//		Bp = Bp * kn;
//
//		// Конец нового алгоритма, дальше проверка точности
//		//DMatrix SP;
//		//SP.Ini(3, 1, 3, 3);
//		//sp = 0;
//		//for (int i = 0; i < CalibPoints; i++)
//		//{
//		//	//SP = U[i] + Vp * (-1) + (Wp * R[i] * Bp) * (-1);
//		//	DMatrix Temp;
//		//	Temp.Ini(3, 1, 3, 3);
//		//	Temp = Vp * (-1);
//		//	DMatrix Temp2;
//		//	Temp2.Ini(3, 3, 3, 3);
//		//	Temp2 = Wp * R[i];
//		//	DMatrix Temp3;
//		//	Temp3.Ini(3, 1, 3, 3);
//		//	Temp3 = Temp2 * Bp;
//		//	Temp3 = Temp3 * (-1);
//		//	SP = U[i] + Temp;
//		//	SP = SP + Temp3;
//		//	Temp.de_allocate();
//		//	Temp2.de_allocate();
//		//	//sp = sp + (SP.T() * SP).data[0][0];
//		//	Temp.Ini(1, 3, 1, 1);
//		//	Temp = SP.T();
//		//	Temp2.Ini(1, 1, 1, 1);
//		//	Temp2 = Temp * SP;
//		//	sp = sp + Temp.data[0][0];
//		//	Temp.de_allocate();
//		//	Temp2.de_allocate();
//		//	Temp3.de_allocate();
//		//}
//		//sp = sqrt(sp)ж
//		//if (SP.data)
//		//	SP.de_allocate();
//
//		if (AA.data)
//			AA.de_allocate();
//		if (BB.data)
//			BB.de_allocate();
//		if (VB.data)
//			VB.de_allocate();
//
//		if (SRB.data)
//			SRB.de_allocate();
//		if (SUB.data)
//			SUB.de_allocate();
//		if (XR.data)
//			XR.de_allocate();	
//	}
//
//	TCHAR a[500] = "";
//	TCHAR sTemp[100] = "";
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			_stprintf_s(sTemp, "%6.4f ", Wp.data[i][j]);
//			_tcscat(a, sTemp);
//		}
//		_tcscat_s(a, "\r\n");
//	}
//
//	AfxMessageBox(a);
//
//	//TCHAR a[500] = "";
//	//	TCHAR sTemp[100] = "";
//	//	for (int i = 0; i < 3; i++)
//	//	{
//	//	/*for (int j = 0; j < 3; j++)
//	//	{*/
//	//	_stprintf_s(sTemp, "%6.4f ", Bp.data[i][0]);
//	//	_tcscat(a, sTemp);
//	//	//}
//	//	_tcscat_s(a, "\r\n");
//	//	}
//	//
//	//	AfxMessageBox(a);
//
//	if (SR.data)
//		SR.de_allocate();
//	if (a11.data)
//		a11.de_allocate();
//	if (a12.data)
//		a12.de_allocate();
//	if (a21.data)
//		a21.de_allocate();
//	if (a22.data)
//		a22.de_allocate();
//	if (b1.data)
//		b1.de_allocate();
//	if (b2.data)
//		b2.de_allocate();
//}

void Pni_tcm::Calibr2()
{
	double pi = 3.1415926535897;
	double S0 = 0;

	// Очистить память если калибровка запущена не в первый раз
	if (R != NULL)
	{
		for (int i = 0; i < OldCalibPoints; i++)
		{
			R[i].de_allocate();
		}
		delete[] R;
		R = NULL;
	}
	if (U != NULL)
	{
		for (int i = 0; i < OldCalibPoints; i++)
			U[i].de_allocate();
		delete[] U;
		U = NULL;
	}
	if (Bp.data)
		Bp.de_allocate();
	if (Vp.data)
		Vp.de_allocate();
	if (Wp.data)
		Wp.de_allocate();

	U = new DMatrix[CalibPoints];
	R = new DMatrix[CalibPoints];

	OldCalibPoints = CalibPoints;
	DMatrix Rx, Ry, Rz;
	DMatrix SR;
	Rx.Ini(3, 3, 3, 3);
	Ry.Ini(3, 3, 3, 3);
	Rz.Ini(3, 3, 3, 3);
	SR.Ini(3, 3, 3, 3);

	SR = SR * 0;
	for (int i = 0; i < CalibPoints; i++)
	{
		double nSumCol = 0;
		CalibComp.front() *= pi / 180;
		CalibRoll.front() *= pi / 180;
		CalibPitch.front() *= pi / 180;

		// Нескомпенсированный сигнал
		U[i].Ini(3, 1, 3, 3);
		U[i].data[0][0] = CalibX.front();
		U[i].data[1][0] = CalibY.front();
		U[i].data[2][0] = CalibZ.front();
		for (int j = 0; j < 3; j++)
		{
			//U[i].data[j][0] = CalibData[j + 3][i];
			nSumCol += pow(U[i].data[j][0], 2);
		}
		S0 += sqrt(nSumCol);	// Um в новой калибровке

		Rx.data[0][0] = 1;						Rx.data[0][1] = 0;						 Rx.data[0][2] = 0;
		Rx.data[1][0] = 0;						Rx.data[1][1] = cos(CalibRoll.front());    Rx.data[1][2] = sin(CalibRoll.front());
		Rx.data[2][0] = 0;						Rx.data[2][1] = -sin(CalibRoll.front());   Rx.data[2][2] = cos(CalibRoll.front());

		Ry.data[0][0] = cos(CalibComp.front());	Ry.data[0][1] = 0;						 Ry.data[0][2] = sin(CalibComp.front());
		Ry.data[1][0] = 0;						Ry.data[1][1] = 1;						 Ry.data[1][2] = 0;
		Ry.data[2][0] = -sin(CalibComp.front());	Ry.data[2][1] = 0;						 Ry.data[2][2] = cos(CalibComp.front());

		Rz.data[0][0] = cos(CalibPitch.front());	Rz.data[0][1] = sin(CalibPitch.front());    Rz.data[0][2] = 0;
		Rz.data[1][0] = -sin(CalibPitch.front());	Rz.data[1][1] = cos(CalibPitch.front());    Rz.data[1][2] = 0;
		Rz.data[2][0] = 0;						Rz.data[2][1] = 0;						 Rz.data[2][2] = 1;

		R[i].Ini(3, 3, 3, 3);
		R[i] = Rx * Rz;
		R[i] = R[i] * Ry;

		SR = SR + R[i];

		CalibComp.pop_front();
		CalibRoll.pop_front();
		CalibPitch.pop_front();
		CalibX.pop_front();
		CalibY.pop_front();
		CalibZ.pop_front();
		int a = CalibComp.size();
	}

	S0 /= CalibPoints;

	if (Rx.data)
		Rx.de_allocate();
	if (Ry.data)
		Ry.de_allocate();
	if (Rz.data)
		Rz.de_allocate();

	DMatrix a11, a12, a21, a22;
	DMatrix b1, b2;
	double sp = S0;
	int pp = 0;
	Wp.Ini(3, 3, 3, 3);
	Bp.Ini(3, 1, 3, 3);
	Vp.Ini(3, 1, 3, 3);
	a11.Ini(3, 3, 3, 3);
	a12.Ini(3, 3, 3, 3);
	a21.Ini(3, 3, 3, 3);
	a22.Ini(3, 3, 3, 3);
	b1.Ini(3, 1, 3, 3);
	b2.Ini(3, 1, 3, 3);

	Wp = Wp * 0;
	Wp.data[0][0] = 1;
	Wp.data[1][1] = 1;
	Wp.data[2][2] = 1;

	a22 = a22 * 0;
	a22.data[0][0] = 1;
	a22.data[1][1] = 1;
	a22.data[2][2] = 1;

	b2 = b2 * 0;
	for (int i = 0; i < CalibPoints; i++)
	{
		b2.data[0][0] += U[i].data[0][0];
		b2.data[1][0] += U[i].data[1][0];
		b2.data[2][0] += U[i].data[2][0];
	}
	b2 = b2 * (1. / CalibPoints);

	while (/*sp > S0 / pow(10, 4) ||*/ pp < 15)	// Количество итераций можно ставить меньше, например 15
	{
		pp++;
		//a21 = Wp*SR *(1. / nSize);
		a21 = Wp*SR;
		a21 = a21*(1. / CalibPoints);
		a12 = a21.T();
		b1 = b1 * 0;
		a11 = a11 * 0;

		DMatrix WW;
		WW.Ini(3, 3, 3, 3);
		WW = Wp.T();
		WW = WW * Wp;
		for (int i = 0; i < CalibPoints; i++)
		{
			//b1 = b1 + R[i].T()*Wp.T()*U[i] * (1. / nSize);
			DMatrix Temp1, Temp2, Temp3;
			Temp1.Ini(3, 3, 3, 3);
			Temp2.Ini(3, 3, 3, 3);
			Temp3.Ini(3, 1, 3, 3);
			Temp1 = R[i].T();
			Temp2 = Wp.T();
			Temp1 = Temp1 * Temp2;
			Temp3 = Temp1 * U[i];
			Temp3 = Temp3 * (1. / CalibPoints);
			b1 = b1 + Temp3;

			//a11 = a11 + ((Wp*R[i]).T()*(Wp*R[i])) * (1. / nSize); Старый вариант
			/*Temp = Wp*R[i];
			Temp2 = Temp.T();
			Temp2 = Temp2 * Temp;
			Temp2 = Temp2 * (1. / CalibPoints);
			a11 = a11 + Temp2;*/

			//WW=Wp'*Wp; a11=a11+R(:,:,k)'*WW*R(:,:,k)/Q; Новый вариант
			Temp1 = R[i].T();
			Temp1 = Temp1 * WW;
			Temp1 = Temp1 * R[i];
			Temp1 = Temp1 * (1. / CalibPoints);
			a11 = a11 + Temp1;

			Temp1.de_allocate();
			Temp2.de_allocate();
			Temp3.de_allocate();
		}
		WW.de_allocate();

		DMatrix AA;
		AA.Ini(6, 6, 6, 6);
		AA.data[0][0] = a11.data[0][0]; AA.data[0][1] = a11.data[0][1]; AA.data[0][2] = a11.data[0][2];  AA.data[0][3] = a12.data[0][0]; AA.data[0][4] = a12.data[0][1]; AA.data[0][5] = a12.data[0][2];
		AA.data[1][0] = a11.data[1][0]; AA.data[1][1] = a11.data[1][1]; AA.data[1][2] = a11.data[1][2];  AA.data[1][3] = a12.data[1][0]; AA.data[1][4] = a12.data[1][1]; AA.data[1][5] = a12.data[1][2];
		AA.data[2][0] = a11.data[2][0]; AA.data[2][1] = a11.data[2][1]; AA.data[2][2] = a11.data[2][2];  AA.data[2][3] = a12.data[2][0]; AA.data[2][4] = a12.data[2][1]; AA.data[2][5] = a12.data[2][2];

		AA.data[3][0] = a21.data[0][0]; AA.data[3][1] = a21.data[0][1]; AA.data[3][2] = a21.data[0][2];  AA.data[3][3] = a22.data[0][0]; AA.data[3][4] = a22.data[0][1]; AA.data[3][5] = a22.data[0][2];
		AA.data[4][0] = a21.data[1][0]; AA.data[4][1] = a21.data[1][1]; AA.data[4][2] = a21.data[1][2];  AA.data[4][3] = a22.data[1][0]; AA.data[4][4] = a22.data[1][1]; AA.data[4][5] = a22.data[1][2];
		AA.data[5][0] = a21.data[2][0]; AA.data[5][1] = a21.data[2][1]; AA.data[5][2] = a21.data[2][2];  AA.data[5][3] = a22.data[2][0]; AA.data[5][4] = a22.data[2][1]; AA.data[5][5] = a22.data[2][2];

		DMatrix BB;
		BB.Ini(6, 1, 6, 6);
		BB.data[0][0] = b1.data[0][0];
		BB.data[1][0] = b1.data[1][0];
		BB.data[2][0] = b1.data[2][0];
		BB.data[3][0] = b2.data[0][0];
		BB.data[4][0] = b2.data[1][0];
		BB.data[5][0] = b2.data[2][0];

		AA = AA.Inverse();
		DMatrix VB;
		VB.Ini(6, 1, 6, 6);
		VB = AA * BB;

		Bp.data[0][0] = VB.data[0][0];
		Bp.data[1][0] = VB.data[1][0];
		Bp.data[2][0] = VB.data[2][0];
		Vp.data[0][0] = VB.data[3][0];
		Vp.data[1][0] = VB.data[4][0];
		Vp.data[2][0] = VB.data[5][0];

		//Оценка мягкого железа
		DMatrix SRB, SUB, XR;
		SRB.Ini(3, 3, 3, 3); SUB.Ini(3, 3, 3, 3); XR.Ini(3, 1, 3, 3);
		SRB = SRB * 0;
		SUB = SUB * 0;

		for (int i = 0; i < CalibPoints; i++)
		{
			// Неправильный 1 вариант
			/*XR = (R[i] + (SR * (-1. / nSize)))*Bp;
			SRB = SRB + XR * XR.T();
			SUB = SUB + (U[i] + (b2 * (-1))) * XR.T();*/

			// XR = R[i] * Bp; Было исправлено на это

			// XR=(R(:,:,k)-SR/Q)*Bp;
			DMatrix Temp;
			Temp.Ini(3, 3, 3, 3);
			Temp = SR * (-1. / CalibPoints);
			Temp = R[i] + Temp;
			XR = Temp * Bp;
			Temp.de_allocate();

			//SRB = SRB + XR * XR.T();
			Temp.Ini(1, 3, 1, 1);
			DMatrix Temp2;
			Temp2.Ini(3, 3, 3, 3);
			Temp = XR.T();
			Temp2 = XR * Temp;
			SRB = SRB + Temp2;

			//SUB = SUB + (U[i] + (b2 * (-1))) * XR.T();
			// 09.11.2016 не ясно почему я использовал такую формулу SUB = SUB + (U[i] + (Vp * (-1))) * XR.T() когда у г.к. везде SUB=SUB+(U(:,k)-b2)*XR'. Спросить если будут проблемы
			DMatrix Temp3;
			Temp3.Ini(3, 1, 3, 3);
			Temp3 = b2 * (-1);
			Temp3 = U[i] + Temp3;
			Temp2 = Temp3 * Temp;
			SUB = SUB + Temp2;

			Temp.de_allocate();
			Temp2.de_allocate();
			Temp3.de_allocate();
		}

		//Wp = SUB * SRB.Inverse();
		SRB = SRB.Inverse();	// Затирается так как больше нигде не используется
		Wp = SUB * SRB;

		/*double kn = pow(Wp.data[0][0] * Wp.data[1][1] * Wp.data[1][1], 1. / 3);
		Wp = Wp * (1. / kn);
		Bp = Bp * kn;*/

		// Конец нового алгоритма, дальше проверка точности
		//DMatrix SP;
		//SP.Ini(3, 1, 3, 3);
		//sp = 0;
		//for (int i = 0; i < CalibPoints; i++)
		//{
		//	//SP = U[i] + Vp * (-1) + (Wp * R[i] * Bp) * (-1);
		//	DMatrix Temp;
		//	Temp.Ini(3, 1, 3, 3);
		//	Temp = Vp * (-1);
		//	DMatrix Temp2;
		//	Temp2.Ini(3, 3, 3, 3);
		//	Temp2 = Wp * R[i];
		//	DMatrix Temp3;
		//	Temp3.Ini(3, 1, 3, 3);
		//	Temp3 = Temp2 * Bp;
		//	Temp3 = Temp3 * (-1);
		//	SP = U[i] + Temp;
		//	SP = SP + Temp3;
		//	Temp.de_allocate();
		//	Temp2.de_allocate();
		//	//sp = sp + (SP.T() * SP).data[0][0];
		//	Temp.Ini(1, 3, 1, 1);
		//	Temp = SP.T();
		//	Temp2.Ini(1, 1, 1, 1);
		//	Temp2 = Temp * SP;
		//	sp = sp + Temp.data[0][0];
		//	Temp.de_allocate();
		//	Temp2.de_allocate();
		//	Temp3.de_allocate();
		//}
		//sp = sqrt(sp);
		//if (SP.data)
		//	SP.de_allocate();

		if (AA.data)
			AA.de_allocate();
		if (BB.data)
			BB.de_allocate();
		if (VB.data)
			VB.de_allocate();

		if (SRB.data)
			SRB.de_allocate();
		if (SUB.data)
			SUB.de_allocate();
		if (XR.data)
			XR.de_allocate();
	}

	// Для отладки вывод одной из матриц
	/*TCHAR a[500] = "";
	TCHAR sTemp[100] = "";
	for (int i = 0; i < 3; i++)
	{
	for (int j = 0; j < 3; j++)
	{
	_stprintf_s(sTemp, "%6.4f ", Wp.data[i][j]);
	_tcscat(a, sTemp);
	}
	_tcscat_s(a, "\r\n");
	}

	AfxMessageBox(a);*/

	if (SR.data)
		SR.de_allocate();
	if (a11.data)
		a11.de_allocate();
	if (a12.data)
		a12.de_allocate();
	if (a21.data)
		a21.de_allocate();
	if (a22.data)
		a22.de_allocate();
	if (b1.data)
		b1.de_allocate();
	if (b2.data)
		b2.de_allocate();
}

double Pni_tcm::Compens()
{
	//UX(:, k) = Wp\(U(:, k) - Vp) - R(:, : , k)*Bp;
	// Тестовый вариант - компенсация сигналов при калибровке
	//DMatrix *UXx = new DMatrix[CALIBPOINTS];
	//double SC = 0;

	//for (int i = 0; i < CALIBPOINTS; i++)
	//{
	//	double nSumCol = 0;
	//	UXx[i].Ini(3, 1, 3, 3);
	//	//UX[i] = Wp.Inverse() * (U[i] + (Vp * (-1))) + ((R[i] * Bp) * (-1));
	//	DMatrix Temp, Temp2;
	//	Temp.Ini(3, 3, 3, 3);
	//	Temp2.Ini(3, 1, 3, 3);
	//	Temp = Wp.Inverse();
	//	UXx[i] = Vp * (-1);
	//	UXx[i] = U[i] + UXx[i];
	//	Temp2 = R[i] * Bp;
	//	Temp2 = Temp2 * (-1);
	//	UXx[i] = Temp * UXx[i];
	//	UXx[i] = UXx[i] + Temp2;

	//	Temp.de_allocate();
	//	Temp2.de_allocate();
	//	for (int j = 0; j < 3; j++)
	//	{
	//		//UX[i].data[j][0] = Data[j + 3][i];
	//		nSumCol += pow(UXx[i].data[j][0], 2);
	//	}
	//	SC += sqrt(nSumCol);
	//}

	//SC /= CALIBPOINTS;

	//for (int i = 0; i < CALIBPOINTS; i++)
	//{
	//	if (UXx != NULL)
	//		UXx[i].de_allocate();
	//}
	//delete[] UXx;

	//sprintf_s(pMes, "%6.4f", SC);
	//AfxMessageBox(pMes);

	double pi = 3.1415926535897;
	double SC = 0;
	DMatrix UX, UOne, RxOne, RyOne, RzOne, ROne;
	UX.Ini(3, 1, 3, 3);
	UOne.Ini(3, 1, 3, 3);
	RxOne.Ini(3, 3, 3, 3);
	RyOne.Ini(3, 3, 3, 3);
	RzOne.Ini(3, 3, 3, 3);
	ROne.Ini(3, 3, 3, 3);

	// Задание параметров исходного сигнала
	UOne.data[0][0] = _ttof(m_sX);
	UOne.data[1][0] = _ttof(m_sY);
	UOne.data[2][0] = _ttof(m_sZ);

	RxOne.data[0][0] = 1;						RxOne.data[0][1] = 0;								 RxOne.data[0][2] = 0;
	RxOne.data[1][0] = 0;						RxOne.data[1][1] = cos(_ttof(m_sRoll) * pi / 180);   RxOne.data[1][2] = sin(_ttof(m_sRoll)* pi / 180);
	RxOne.data[2][0] = 0;						RxOne.data[2][1] = -sin(_ttof(m_sRoll)* pi / 180);   RxOne.data[2][2] = cos(_ttof(m_sRoll)* pi / 180);

	RyOne.data[0][0] = cos(_ttof(m_sComp)* pi / 180);	RyOne.data[0][1] = 0;						 RyOne.data[0][2] = sin(_ttof(m_sComp)* pi / 180);
	RyOne.data[1][0] = 0;								RyOne.data[1][1] = 1;						 RyOne.data[1][2] = 0;
	RyOne.data[2][0] = -sin(_ttof(m_sComp)* pi / 180);	RyOne.data[2][1] = 0;						 RyOne.data[2][2] = cos(_ttof(m_sComp)* pi / 180);

	RzOne.data[0][0] = cos(_ttof(m_sPitch)* pi / 180);	RzOne.data[0][1] = sin(_ttof(m_sPitch)* pi / 180);    RzOne.data[0][2] = 0;
	RzOne.data[1][0] = -sin(_ttof(m_sPitch)* pi / 180);	RzOne.data[1][1] = cos(_ttof(m_sPitch)* pi / 180);    RzOne.data[1][2] = 0;
	RzOne.data[2][0] = 0;								RzOne.data[2][1] = 0;						 RzOne.data[2][2] = 1;

	ROne = RxOne * RzOne;
	ROne = ROne * RyOne;

	//UX(:, k) = Wp\(U(:, k) - Vp) - R(:, : , k)*Bp;
	//UX[i] = Wp.Inverse() * (U[i] + (Vp * (-1))) + ((R[i] * Bp) * (-1)); Старый вариант
	/*DMatrix Temp, Temp2;
	Temp.Ini(3, 3, 3, 3);
	Temp2.Ini(3, 1, 3, 3);
	Temp = Wp.Inverse();
	UX = Vp * (-1);
	UX = UOne + UX;
	Temp2 = ROne * Bp;
	Temp2 = Temp2 * (-1);
	UX = Temp * UX;
	UX = UX + Temp2;*/

	//  UX(:,k)=(U(:,k)-Wp*R(:,:,k)*Bp-Vp) Новый вариант
	DMatrix Temp, Temp2;
	Temp.Ini(3, 3, 3, 3);
	Temp = Wp * ROne;
	UX = Temp * Bp;
	UX = UX + Vp;
	UX = UX * (-1);
	UX = UOne + UX;

	SC = sqrt(pow(UX.data[0][0], 2) + pow(UX.data[1][0], 2) + pow(UX.data[2][0], 2));

	Temp.de_allocate();
	Temp2.de_allocate();
	RxOne.de_allocate();
	RyOne.de_allocate();
	RzOne.de_allocate();
	ROne.de_allocate();
	UOne.de_allocate();
	UX.de_allocate();
	return SC;
}

int Pni_tcm::SaveCalibData(TCHAR* sFilePath)
{
	//if (AfxMessageBox("Сохранить данные для калибровки?", MB_YESNO) == IDYES)
	//{
		CStdioFile pFile;
		//TCHAR m_sFilePath[MAX_PATH];
		float CalibSave[5][3];
		// Для текстового файла
		/*TCHAR m_sFilePath2[MAX_PATH];
		_stprintf_s(m_sFilePath2, "%s%s", sFolderPath, "\\Test.txt");
		HANDLE m_hlog = ::CreateFile(m_sFilePath2, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);*/

		//_stprintf_s(sFilePath, "%s%s", sFolderPath, "\\CalibData.n");
		if (!pFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			_stprintf_s(pMes, "%s%s%s", "Не удалось создать файл ", sFilePath, "\r\n");
			pProtocol->WriteLog(pMes);
			return 0;
		}
		// Заполнение массива
		//pFile.SeekToBegin();
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == 0)
					CalibSave[i][j] = Bp.data[j][0];
					//pFile.Write(&Bp.data[j][0], 4);
				else if (i == 1)
					CalibSave[i][j] = Vp.data[j][0];
					//pFile.Write(&Vp.data[j][0], 4);
				else
					CalibSave[i][j] = Wp.data[i - 2][j];
					//pFile.Write(&Wp.data[i - 2][j], 4);
			}	
		}
		pFile.SeekToBegin();
		pFile.Write(CalibSave, 60);
		pFile.Close();
		_stprintf_s(pMes, "%s%s%s", "Создан файл ", sFilePath, "\r\n");
		pProtocol->WriteLog(pMes);

		/*for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				TCHAR a[20];
				_stprintf_s(a, "%f; ", CalibSave[i][j]);
				WriteFile(m_hlog, a, _tcslen(a), NULL, NULL);
			}
		}
		if (m_hlog != INVALID_HANDLE_VALUE)
			CloseHandle(m_hlog);*/
		return 1;
	//}
	//return 0;
}

int Pni_tcm::LoadCalibData(TCHAR* sFilePath)
{
	CStdioFile pFile;
	//TCHAR sFilePath[MAX_PATH];

	//_stprintf_s(sFilePath, "%s%s", sFolderPath, "\\CalibData.n");
	if (!pFile.Open(sFilePath, CFile::modeRead | CFile::modeNoTruncate | CFile::typeBinary))
	{
		_stprintf_s(pMes, "%s%s%s", "Не удалось открыть файл ", sFilePath, "\r\n");
		//pProtocol->WriteLog(pMes);
		pMainDlg->AddMessage(pMes, 1);
		return 0;
	}
	
	if (pFile.GetLength() != 60)
	{
		_stprintf_s(pMes, "%s%s%s", "Ошибка формата файла ", sFilePath, "\r\n");
		//pProtocol->WriteLog(pMes);
		pMainDlg->AddMessage(pMes, 1);
		return 0;
	}

	//if (AfxMessageBox("Загрузить данные прошлой калибровки из файла CalibData.n?", MB_YESNO) == IDYES)
	//{
		float temp;
		Bp.Ini(3, 1, 3, 3);
		Vp.Ini(3, 1, 3, 3);
		Wp.Ini(3, 3, 3, 3);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pFile.Read(&temp, 4);
				if (i == 0)
					//pFile.Read(&Bp.data[j][0], 4);
					Bp.data[j][0] = temp;
				else if (i == 1)
					//pFile.Read(&Vp.data[j][0], 4);
					Vp.data[j][0] = temp;
				else
					//pFile.Read(&Wp.data[i - 2][j], 4);
					Wp.data[i - 2][j] = temp;
			}
		}
		_stprintf_s(pMes, "%s%s", "Успешно загружены данные калибровки из файла ", sFilePath);
		//pProtocol->WriteLog("Успешно загружены данные калибровки из файла CalibData.n\r\n");
		pMainDlg->AddMessage(pMes, 1);
		pFile.Close();
		return 1;
	//}
	pFile.Close();
	return 0;
}

TCHAR* Pni_tcm::GetC()
{
	return m_sComp;
}

TCHAR* Pni_tcm::GetR()
{
	return m_sRoll;
}

TCHAR* Pni_tcm::GetP()
{
	return m_sPitch;
}

TCHAR* Pni_tcm::GetX()
{
	return m_sX;
}

TCHAR* Pni_tcm::GetY()
{
	return m_sY;
}

TCHAR* Pni_tcm::GetZ()
{
	return m_sZ;			// Zт = Y магнитометра
}

TCHAR* Pni_tcm::GetT()
{
	return m_sT;
}

TCHAR* Pni_tcm::GetMod()
{
	double dSqr = sqrt(pow(_ttof(m_sX), 2) + pow(_ttof(m_sY), 2) + pow(_ttof(m_sZ), 2));
	_stprintf_s(m_sSqrt, "%4.2f", dSqr);
	return m_sSqrt;
}

TCHAR* Pni_tcm::GetEr()
{
	return m_sError;
}

TCHAR* Pni_tcm::GetComName()
{
	return m_sCommName;
}

int Pni_tcm::GetBaudPort()
{
	return m_nBaudPort;
}

TCHAR* Pni_tcm::GetBaudMagn()
{
	TCHAR cValue = m_sBaudMagn[3];
	switch (cValue)
	{
	case '1':
		_tcscpy_s(m_sBaudMagn, "300");
		break;
	case '2':
		_tcscpy_s(m_sBaudMagn, "1200");
		break;
	case '3':
		_tcscpy_s(m_sBaudMagn, "2400");
		break;
	case '4':
		_tcscpy_s(m_sBaudMagn, "4800");
		break;
	case '5':
		_tcscpy_s(m_sBaudMagn, "9600");
		break;
	case '6':
		_tcscpy_s(m_sBaudMagn, "19200");
		break;
	case '7':
		_tcscpy_s(m_sBaudMagn, "38400");
		break;
	case '8':
		_tcscpy_s(m_sBaudMagn, "115200");
		break;
	}

	return m_sBaudMagn;
}
