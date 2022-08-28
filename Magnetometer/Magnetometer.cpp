
// Magnetometer.cpp : Определяет поведение классов для приложения.
//

#include "stdafx.h"
#include "Magnetometer.h"
#include "MagnetometerDlg.h"
#include "Magnetometer.hpp"
#include "Protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagnetometerApp

BEGIN_MESSAGE_MAP(CMagnetometerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_ABOUTMENU, &CMagnetometerApp::OnAbout)
END_MESSAGE_MAP()


// создание CMagnetometerApp

CMagnetometerApp::CMagnetometerApp()
{
	// поддержка диспетчера перезагрузки
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CMagnetometerApp

CMagnetometerApp theApp;


// инициализация CMagnetometerApp

BOOL CMagnetometerApp::InitInstance()
{
	CWinApp::InitInstance();

	// Активация визуального диспетчера "Классический Windows" для включения элементов управления MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Путь к файлам по умолчанию
	GetModuleFileName(NULL, sFolderPath, _countof(sFolderPath));
	sFolderPath[_tcsrchr(sFolderPath, '\\') - sFolderPath] = '\0';

	pMainDlg = (CMagnetometerDlg*) new class CMagnetometerDlg();
	m_pMainWnd = pMainDlg;

	pProtocol = (Protocol*) new class Protocol();
	
	pPni = (Pni_tcm*) new class Pni_tcm();

	pMA = new class Pni_MA();

	//pMainDlg->GetMenu()->EnableMenuItem(ID_TESTMA, MF_DISABLED);	// Заглушка для билда Г.К.
	nRunFlag = 1;
	pMainDlg->CheckDlgButton(IDC_PNIMODE, 1);			// Режим магнитометра по умолчанию
	// Заглушка для билда Г.К.
	/*if (pPni->LoadCalibData())
	{
		bComCalib = 1;
		pMainDlg->GetDlgItem(IDC_OFFSET)->EnableWindow(1);
		pMainDlg->GetDlgItem(IDC_OFFSETCHECK)->EnableWindow(1);
		AfxMessageBox("Данные калибровки успешно загружены", MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox("Данные прошлой калибровки не загружены");
	}*/

	// Поскольку диалоговое окно закрыто, возвратите значение FALSE, чтобы можно было выйти из
	//  приложения вместо запуска генератора сообщений приложения.
	return TRUE;
}

// Класс окна о программе
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

														// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClearcalib();
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_COMMAND(ID_CLEARCALIB, &CAboutDlg::OnClearcalib)
END_MESSAGE_MAP()


void CMagnetometerApp::OnAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


int CMagnetometerApp::ExitInstance()
{
	if (pPni != NULL)
		delete pPni;
	if (pMA != NULL)
		delete pMA;
	if (pProtocol != NULL)
		delete pProtocol;
	if (pMainDlg != NULL)
		delete pMainDlg;
	if (pAllDataFile != NULL)
	{
		pAllDataFile->Close();
		delete pAllDataFile;
		pAllDataFile = NULL;
	}

	return CWinApp::ExitInstance();
}


BOOL CMagnetometerApp::OnIdle(LONG lCount)
{
	int ping;				// Задержка для фиксированного интервала записи
	TCHAR pComName[20] = "\0";
	std::string err_str("Ошибка: ");

	switch (nRunFlag)
	{
	case 1:
		if (/*TCHAR* pComName =*/ pPni->InitCom(pComName, 38400, 38400))		// Инициализация магнитометра
		{
			_stprintf_s(pMes, "%s%s", "Подключен магнитометр PNI TCM 2.5 в порт ", pComName);
			//pMainDlg->SetDlgItemText(IDC_HEADMES, pMes);
			pMainDlg->GetDlgItem(IDC_GETDATA)->EnableWindow(1);
			pMainDlg->GetDlgItem(IDC_CHANGEFOL)->EnableWindow(1);
			pMainDlg->EnableMenuCalib(1);
		}
		else
		{
			_stprintf_s(pMes, "%s", "Произошла ошибка подключения магнитометра");
			//pMainDlg->SetDlgItemText(IDC_HEADMES, "Произошла ошибка подключения магнитометра");
			pMainDlg->GetDlgItem(IDC_GETDATA)->EnableWindow(0);
			pMainDlg->GetDlgItem(IDC_CHANGEFOL)->EnableWindow(0);
			pMainDlg->EnableMenuCalib(0);
		}
		pMainDlg->AddMessage(pMes, 1);
		nRunFlag = 0;
		break;
	case 2:
		nStartTime = GetTickCount() - nCurTime;			// Запуск таймера		
		nRunFlag++; 
		break;
	case 3:
		TCHAR sTime[10];
		if (!nRunTime)
			nRunTime = GetTickCount();
		
		/*if (lCount == 1 || lCount % 5 == 0)
			MessageDots("Идет считывание данных магнитометра");*/

		// Получение данных с магнитометра
		pPni->UpdateMData();
		pMainDlg->SetDlgItemText(IDC_COMP, pPni->GetC());
		pMainDlg->SetDlgItemText(IDC_ROLL, pPni->GetR());
		pMainDlg->SetDlgItemText(IDC_PITCH, pPni->GetP());
		pMainDlg->SetDlgItemText(IDC_TEMP, pPni->GetT());
		pMainDlg->SetDlgItemText(IDC_X, pPni->GetX());
		pMainDlg->SetDlgItemText(IDC_Y, pPni->GetY());
		pMainDlg->SetDlgItemText(IDC_Z, pPni->GetZ());
		pMainDlg->SetDlgItemText(IDC_MOD, pPni->GetMod());
		pMainDlg->SetDlgItemText(IDC_TIMEEDIT, GetTimeFrom(sTime));
		// Вывод поля компенсации
		if (bComCalib)
		{
			_stprintf_s(pMes, "%6.4f", pPni->Compens());
			pMainDlg->SetDlgItemText(IDC_OFFSET, pMes);
		}

		// Вывод текущего состояния магнитометра
		if (lCount == 1 || lCount % 5 == 0)
		{
			if (TCHAR* sMes = pPni->GetEr())
				MessageDots(sMes);
			else if (bCalib)
				MessageDots("Идет калибровка. Вращайте магнитометр");
			else
				MessageDots("Идет считывание данных магнитометра");
		}

		ping = 150 - GetTickCount() + nRunTime;		
		if (ping > 0)
			Sleep(ping);		// Задержка для фиксированного интервала записи, константа 0.15, после 2:30 - погрешность секунда, после 5 - 2 секунды

		nRunTime = GetTickCount();
		nRunFlag++;
		break;
	case 4:
		// Сохранение в файлы по компонентам
		if (pMainDlg->IsDlgButtonChecked(IDC_COMPCHECK))
			GraphFile[0].SetGraphData(_ttof(pPni->GetC()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_ROLLCHECK))
			GraphFile[1].SetGraphData(_ttof(pPni->GetR()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_PITCHCHECK))
			GraphFile[2].SetGraphData(_ttof(pPni->GetP()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_TEMPCHECK))
			GraphFile[3].SetGraphData(_ttof(pPni->GetT()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_XCHECK))
			GraphFile[4].SetGraphData(_ttof(pPni->GetX()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_YCHECK))
			GraphFile[5].SetGraphData(_ttof(pPni->GetY()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_ZCHECK))
			GraphFile[6].SetGraphData(_ttof(pPni->GetZ()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_MODCHECK))
			GraphFile[7].SetGraphData(_ttof(pPni->GetMod()), m_bChangeFol);
		if (pMainDlg->IsDlgButtonChecked(IDC_OFFSETCHECK) && bComCalib)
			GraphFile[8].SetGraphData(pPni->Compens(), m_bChangeFol);
		m_bChangeFol = 0;

		if (bCalib)
			nRunFlag++;
		else
			nRunFlag = 6;
		break;
	case 5:
		//MessageDots("Идет калибровка. Вращайте магнитометр");

		CalibComp.push_back(_ttof(pPni->GetC()));
		CalibRoll.push_back(_ttof(pPni->GetR()));
		CalibPitch.push_back(_ttof(pPni->GetP()));
		CalibX.push_back(_ttof(pPni->GetX()));
		CalibY.push_back(_ttof(pPni->GetY()));
		CalibZ.push_back(_ttof(pPni->GetZ()));

		_stprintf_s(pMes, "%d", CalibComp.size());
		pMainDlg->SetDlgItemText(IDC_CPOINTS, pMes);

		nRunFlag++;
		break;
	case 6:
		// Запись точек в общий файл
		if (pMainDlg->IsDlgButtonChecked(IDC_CHECKALL) && pAllDataFile != NULL)
			SaveAllData();

		if (nCountPoints == nFilePoints)// && !bCalib)		// С комментарием можно одновременно калибровать и записывать требуемое количество точек в файл
			nRunFlag++;
		else
			nRunFlag = 3;
		break;
	case 7:
		// Сохранение точек
		_stprintf_s(pMes, "%s%d%s", "Записано ", nFilePoints, " точек");
		AfxMessageBox(pMes, MB_ICONINFORMATION);
		nCountPoints = 0;
		nFilePoints = -1;
		pMainDlg->OnBnClickedButton2();		// Нажатие кнопки стоп для восстановления кнопок
		pAllDataFile->Close();
		delete pAllDataFile;
		pAllDataFile = NULL;

		if (bCalib)
			nRunFlag = 12;
		else
			nRunFlag = 10;
		break;
	case 8:
		// Инициализация МА
		//std::string err_str("Ошибка: ");
		try
		{
			pMA->LoadSettings();
			pMA->OpenIO();
			pMA->LoadProgram();
			pMA->CheckConnect();
			pMA->CheckMag();

			pMainDlg->GetDlgItem(IDC_GETDATA)->EnableWindow(1);
			pMainDlg->GetDlgItem(IDC_CHANGEFOL)->EnableWindow(1);
			pMainDlg->EnableMenuCalib(1);
		}
		catch (const std::exception &err)
		{
			err_str += err.what();
			pMainDlg->AddMessage((TCHAR*)err_str.c_str(), 1);
			//AfxMessageBox(err_str.c_str());
			//pMainDlg->SetDlgItemText(IDC_HEADMES, err_str.c_str());

			pMainDlg->GetDlgItem(IDC_GETDATA)->EnableWindow(0);
			pMainDlg->GetDlgItem(IDC_CHANGEFOL)->EnableWindow(0);
			pMainDlg->EnableMenuCalib(0);
		}
		catch (...)
		{
			err_str += "Прочие ошибки";
			pMainDlg->AddMessage((TCHAR*)err_str.c_str(), 1);
			//AfxMessageBox(err_str.c_str());
			//pMainDlg->SetDlgItemText(IDC_HEADMES, err_str.c_str());

			pMainDlg->GetDlgItem(IDC_GETDATA)->EnableWindow(0);
			pMainDlg->GetDlgItem(IDC_CHANGEFOL)->EnableWindow(0);
			pMainDlg->EnableMenuCalib(0);
		}
		nRunFlag = 0;
		break;
	case 10:
		if (pMainDlg->IsDlgButtonChecked(IDC_COMPCHECK) && !GraphFile[0].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_ROLLCHECK) && !GraphFile[1].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_PITCHCHECK) && !GraphFile[2].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_TEMPCHECK) && !GraphFile[3].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_XCHECK) && !GraphFile[4].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_YCHECK) && !GraphFile[5].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_ZCHECK) && !GraphFile[6].SaveGraphData(1))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_MODCHECK) && !GraphFile[7].SaveGraphData(1))
			AfxMessageBox(pMes);

		nRunFlag = 0;
		if (pMainDlg->IsDlgButtonChecked(IDC_CHECKALL) && bCalib)				// Если запущена калибровка
		{
			nFilePoints = CalibComp.size();
			nRunFlag = 7;
		}
		else if (bCalib)
			nRunFlag = 12;
		break;
	case 12:
		CalibPoints = CalibComp.size();
		pPni->Calibr2();

		_stprintf_s(pMes, "Проведена калибровка на основе %0.2d точек\r\n", CalibPoints);
		pMainDlg->AddMessage(pMes, 1);
		//pMainDlg->SetDlgItemText(IDC_HEADMES, "Калибровка закончена");
		//AfxMessageBox(pMes, MB_ICONINFORMATION);
		//pProtocol->WriteLog(pMes);
		bCalib = 0;
		bComCalib = 1;
		nRunFlag = 0;
		pMainDlg->EnableModes(1);
		pMainDlg->EnableButtons(1);
		pMainDlg->EnableChecks(1);
		pMainDlg->EnableMenuCalib(1);
		pMainDlg->GetDlgItem(IDC_STOP)->EnableWindow(0);
		pMainDlg->GetDlgItem(IDC_OFFSET)->EnableWindow(1);
		break;
	default:
		break;
	}

	return TRUE; //CWinApp::OnIdle(lCount);
}

TCHAR * CMagnetometerApp::GetTimeFrom(TCHAR* sTime)
{
	nCurTime = GetTickCount() - nStartTime;
	int nSec = nCurTime / 1000;
	div_t res_div = div(nSec, 60);

	int nCurSec = nSec - res_div.quot * 60;
	int nCurMin = nSec / 60;
	int nCurHour = nCurMin / 60;
	_stprintf_s(sTime, 10, "%02d:%02d:%02d", nCurHour, nCurMin, nCurSec);
	return sTime;
}

void CMagnetometerApp::MessageDots(TCHAR* sString)
{
	TCHAR sBuf[MAX_PATH];
	MessageList.GetText(MessageList.GetCount() - 1, sBuf);

	if (_tcsstr(sBuf, sString))
	{
		if (TCHAR* sTemp = _tcsstr(sBuf, "..."))
			sBuf[sTemp - sBuf] = '\0';
		else
			_tcscat_s(sBuf, ".");

		MessageList.DeleteString(MessageList.GetCount() - 1);
		pMainDlg->AddMessage(sBuf, 0);
	}
	else
		pMainDlg->AddMessage(sString, 0);
}

int CMagnetometerApp::SaveAllData()
{
	float pAllData[8];
	pAllData[0] = _ttof(pPni->GetC()); pAllData[1] = _ttof(pPni->GetR()); pAllData[2] = _ttof(pPni->GetP()); pAllData[3] = _ttof(pPni->GetT());
	pAllData[4] = _ttof(pPni->GetX()); pAllData[5] = _ttof(pPni->GetY()); pAllData[6] = _ttof(pPni->GetZ()); pAllData[7] = _ttof(pPni->GetMod());
	// 4 байта на каждое число
	pAllDataFile->Write(pAllData, 32);
	nCountPoints++;
	return 1;
}
