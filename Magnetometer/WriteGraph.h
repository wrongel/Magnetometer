#pragma once
#define Block_size 64
class WriteGraph
{
public:
	WriteGraph();
	~WriteGraph();

	HANDLE m_hlog;
	int CreateGraphFile(TCHAR* sNameFile);			// ������� ���� ��� �������
	void SetGraphData(float fDat, int nNewFlag);	// �������� ������ � ������
	int SaveGraphData(BOOL bLog);					// ��������� ���� ��� �������, �������� - �������� � ��� ��� ���

	TCHAR* GetFilePath();		// ��������� ������ �����

protected:
	TCHAR m_sFilePath[MAX_PATH];		// ���� � ����� � ������
	float m_fData[Block_size / 4];			// ������ ������
	int nBytesWriten;			// ���������� ���� ��� ������ � ������� ������� � �������
	int nBlocksWriten;			// ���������� ���������� ������ � ����
};

