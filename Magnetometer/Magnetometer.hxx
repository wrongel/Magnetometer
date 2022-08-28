//
#include "list"
//#define	CALIBPOINTS 30					// ���������� ����� ��� ����������

extern class CMagnetometerDlg *pMainDlg;	// ����� �������� ����
extern class Pni_tcm *pPni;					// ����� ������������
extern class Pni_MA *pMA;					// ����� ������ �������������
extern class Protocol *pProtocol;			// ����� ���������
extern class WriteGraph GraphFile[9];

extern int nRunFlag;
extern int nStartTime;							// ��������� ����� ��� �������
extern int nCurTime;							// ������� ����� �������
extern TCHAR pMes[MAX_PATH];				// ������ ��� ������ � �������� � ������ ���������
extern TCHAR sFolderPath[MAX_PATH];			// ��������� ���� � ����� � ������ �������
extern BOOL bModePr;					// ����� ������ ��������� 0 - �����������, 1 - ��

extern std::list<double> CalibComp;
extern std::list<double> CalibPitch;
extern std::list<double> CalibRoll;
extern std::list<double> CalibX;
extern std::list<double> CalibY;
extern std::list<double> CalibZ;
extern int CalibPoints;

extern BOOL bCalib;								// ���� � ������������� ����������
extern BOOL bComCalib;						// ���� �� �������� ����������
extern CStdioFile *pAllDataFile;				// ���� �� ����� ���������� �������
extern int nFilePoints;							// ���������� ����� ��� ������ � ����
extern TCHAR sLastCalib[15];						// ��������� ��������� ���������� ����������

extern CListBox MessageList;