//
#include "list"
//#define	CALIBPOINTS 30					// ���������� ����� ��� ����������

class CMagnetometerDlg *pMainDlg;		// ����� �������� ����
class Pni_tcm *pPni;					// ����� ������������
class Pni_MA *pMA;						// ����� ������ �������������
class Protocol *pProtocol;				// ����� ���������
class WriteGraph GraphFile[9];

int nRunFlag;
int nStartTime = 0;						// ��������� ����� ��� �������
int nCurTime = 0;						// ������� ����� �������
TCHAR pMes[MAX_PATH];					// ������ ��� ������ � �������� � ������ ���������
TCHAR sFolderPath[MAX_PATH];			// ��������� ���� � ����� � ������ �������
BOOL bModePr = 0;					// ����� ������ ��������� 0 - �����������, 1 - ��

std::list<double> CalibComp;
std::list<double> CalibPitch;
std::list<double> CalibRoll;
std::list<double> CalibX;
std::list<double> CalibY;
std::list<double> CalibZ;
int CalibPoints;

BOOL bCalib;							// ���� � ������������� ����������
BOOL bComCalib;							// ���� �� �������� ����������
CStdioFile *pAllDataFile;				// ���� �� ����� ���������� �������
int nFilePoints = -1;							// ���������� ����� ��� ������ � ����
TCHAR sLastCalib[15];						// ��������� ��������� ���������� ����������

CListBox MessageList;