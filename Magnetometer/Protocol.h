#pragma once
class Protocol
{
public:
	Protocol();
	~Protocol();

	void WriteLog(TCHAR* sMes);							// �������� ������ � �����
protected:	
	HANDLE m_hlog;										// ���������� ����� ������
	TCHAR m_sLogPath[MAX_PATH];							// ���� � ����� � �������

	void CreateLogPath();								// ������� ����� � exe ��������
	TCHAR* GetTimeStr(TCHAR* sTime, TCHAR cSepar);		// �������� ������ ���� � �������
};

