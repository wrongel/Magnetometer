
// Magnetometer.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMagnetometerApp:
// О реализации данного класса см. Magnetometer.cpp
//

class CMagnetometerApp : public CWinApp
{
public:
	CMagnetometerApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAbout();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

public:
	BOOL m_bChangeFol;			// Флаг об изменении папки сохранения файлов
	int nCountPoints = 0;	// Счетчик количества точек для записи в файл
	int SaveAllData();		// Запись одной точки в файлы
protected:
	int nRunTime = 0;		// Счетчик времени считывания показателей
	TCHAR* GetTimeFrom(TCHAR* sTime);		// Получение времени прошедшего от nStartTime
	void MessageDots(TCHAR* sString);
};

extern CMagnetometerApp theApp;