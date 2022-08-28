
// Magnetometer.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CMagnetometerApp:
// � ���������� ������� ������ ��. Magnetometer.cpp
//

class CMagnetometerApp : public CWinApp
{
public:
	CMagnetometerApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAbout();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

public:
	BOOL m_bChangeFol;			// ���� �� ��������� ����� ���������� ������
	int nCountPoints = 0;	// ������� ���������� ����� ��� ������ � ����
	int SaveAllData();		// ������ ����� ����� � �����
protected:
	int nRunTime = 0;		// ������� ������� ���������� �����������
	TCHAR* GetTimeFrom(TCHAR* sTime);		// ��������� ������� ���������� �� nStartTime
	void MessageDots(TCHAR* sString);
};

extern CMagnetometerApp theApp;