#pragma once

#include "dmatrix.h"
#include "MagnetometerDlg.h"		// Убрать как разберусь с сообщениями
#include "Pni_MA.h"

class Pni_tcm
{
public:
	Pni_tcm();
	~Pni_tcm();
	
	int InitCom(TCHAR* sCommName, int nBaudPort, int nBaudMagn);	// Инициализация COM порта
	void UpdateMData();		// Получить строку показателей

	TCHAR* GetC();	// Получить показания компаса
	TCHAR* GetP();	// Получить показания тангажа
	TCHAR* GetR();	// Получить показания крена
	TCHAR* GetX();	// Получить данные магнитометра по оси Х
	TCHAR* GetY();	// Получить данные магнитометра по оси Y
	TCHAR* GetZ();	// Получить данные магнитометра по оси Z
	TCHAR* GetT();	// Получить температуру
	TCHAR* GetMod();	// Получить корень из квадратов составляющих магнитного поля
	TCHAR* GetEr();  // Получить сообщение об ошибке
	int MCalib();

	TCHAR* GetComName();	// Получить имя порта
	int GetBaudPort();		// Получить частоту порта
	TCHAR* GetBaudMagn();	// Получить частоту магнитометра

	//void Calibr();			// Сформировать матрицы геомагнитного поля, жесткого и мягкого железа
	void Calibr2();
	double Compens();		// Компенсация модуля сигнала магнитометра
	int SaveCalibData(TCHAR* sFilePath);	// Сохранить значения матриц геомагнитного поля, жесткого и мягкого железа в файл
	int LoadCalibData(TCHAR* sFilePath);	// Загрузить значения матриц геомагнитного поля, жесткого и мягкого железа из файла

	// Должно быть в protected
	TCHAR* WriteCOM(TCHAR* sQuery, TCHAR* sReceivedData, int nBufSize);	// Посылка и прием сообщения

protected:
	HANDLE m_hport;		// Дескриптор порта
	TCHAR m_sCommName[20];	// Имя порта
	int m_nBaudPort;	// Частота порта
	TCHAR m_sBaudMagn[7];	// Частота магнитометра
	TCHAR m_sOutchar[MAX_PATH];	// Данные для одиночного представления
	TCHAR m_sComp[20];
	TCHAR m_sRoll[20];
	TCHAR m_sPitch[20];
	TCHAR m_sX[20];
	TCHAR m_sY[20];
	TCHAR m_sZ[20];
	TCHAR m_sT[20];
	TCHAR m_sSqrt[20];
	TCHAR* m_sError;	// Сообщение об ошибке

	DMatrix Bp;			// Матрицы для калибровки
	DMatrix Vp;
	DMatrix Wp;
	DMatrix *U;
	DMatrix *R;
	int OldCalibPoints;

	//TCHAR* WriteCOM(TCHAR* sQuery, TCHAR* sReceivedData, int nBufSize);	// Посылка и прием сообщения
	TCHAR* SingleData(TCHAR* sInput, TCHAR cSepar);	// Разбиение ответа от магнитометра на подстроки
	void SetEr();		// Очистка выходных данных от ошибки выхода за границы

	TCHAR* ConvertBaudRate(int nBaudRate);		// Соотношение между частотой и ее представлением в магнитометре
};
