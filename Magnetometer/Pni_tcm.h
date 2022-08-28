#pragma once

#include "dmatrix.h"
#include "MagnetometerDlg.h"		// ������ ��� ��������� � �����������
#include "Pni_MA.h"

class Pni_tcm
{
public:
	Pni_tcm();
	~Pni_tcm();
	
	int InitCom(TCHAR* sCommName, int nBaudPort, int nBaudMagn);	// ������������� COM �����
	void UpdateMData();		// �������� ������ �����������

	TCHAR* GetC();	// �������� ��������� �������
	TCHAR* GetP();	// �������� ��������� �������
	TCHAR* GetR();	// �������� ��������� �����
	TCHAR* GetX();	// �������� ������ ������������ �� ��� �
	TCHAR* GetY();	// �������� ������ ������������ �� ��� Y
	TCHAR* GetZ();	// �������� ������ ������������ �� ��� Z
	TCHAR* GetT();	// �������� �����������
	TCHAR* GetMod();	// �������� ������ �� ��������� ������������ ���������� ����
	TCHAR* GetEr();  // �������� ��������� �� ������
	int MCalib();

	TCHAR* GetComName();	// �������� ��� �����
	int GetBaudPort();		// �������� ������� �����
	TCHAR* GetBaudMagn();	// �������� ������� ������������

	//void Calibr();			// ������������ ������� ������������� ����, �������� � ������� ������
	void Calibr2();
	double Compens();		// ����������� ������ ������� ������������
	int SaveCalibData(TCHAR* sFilePath);	// ��������� �������� ������ ������������� ����, �������� � ������� ������ � ����
	int LoadCalibData(TCHAR* sFilePath);	// ��������� �������� ������ ������������� ����, �������� � ������� ������ �� �����

	// ������ ���� � protected
	TCHAR* WriteCOM(TCHAR* sQuery, TCHAR* sReceivedData, int nBufSize);	// ������� � ����� ���������

protected:
	HANDLE m_hport;		// ���������� �����
	TCHAR m_sCommName[20];	// ��� �����
	int m_nBaudPort;	// ������� �����
	TCHAR m_sBaudMagn[7];	// ������� ������������
	TCHAR m_sOutchar[MAX_PATH];	// ������ ��� ���������� �������������
	TCHAR m_sComp[20];
	TCHAR m_sRoll[20];
	TCHAR m_sPitch[20];
	TCHAR m_sX[20];
	TCHAR m_sY[20];
	TCHAR m_sZ[20];
	TCHAR m_sT[20];
	TCHAR m_sSqrt[20];
	TCHAR* m_sError;	// ��������� �� ������

	DMatrix Bp;			// ������� ��� ����������
	DMatrix Vp;
	DMatrix Wp;
	DMatrix *U;
	DMatrix *R;
	int OldCalibPoints;

	//TCHAR* WriteCOM(TCHAR* sQuery, TCHAR* sReceivedData, int nBufSize);	// ������� � ����� ���������
	TCHAR* SingleData(TCHAR* sInput, TCHAR cSepar);	// ��������� ������ �� ������������ �� ���������
	void SetEr();		// ������� �������� ������ �� ������ ������ �� �������

	TCHAR* ConvertBaudRate(int nBaudRate);		// ����������� ����� �������� � �� �������������� � ������������
};
