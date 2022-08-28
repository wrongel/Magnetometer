//
#include "list"
//#define	CALIBPOINTS 30					// Количество точек для калибровки

class CMagnetometerDlg *pMainDlg;		// Класс главного окна
class Pni_tcm *pPni;					// Класс магнитометра
class Pni_MA *pMA;						// Класс модуля аппаратурного
class Protocol *pProtocol;				// Класс протокола
class WriteGraph GraphFile[9];

int nRunFlag;
int nStartTime = 0;						// Начальная точка для таймера
int nCurTime = 0;						// Текущее время таймера
TCHAR pMes[MAX_PATH];					// Строка для записи в протокол и вывода сообщений
TCHAR sFolderPath[MAX_PATH];			// Начальный путь к логам и файлам графера
BOOL bModePr = 0;					// Режим работы программы 0 - магнитометр, 1 - МА

std::list<double> CalibComp;
std::list<double> CalibPitch;
std::list<double> CalibRoll;
std::list<double> CalibX;
std::list<double> CalibY;
std::list<double> CalibZ;
int CalibPoints;

BOOL bCalib;							// Флаг о необходимости калибровки
BOOL bComCalib;							// Флаг об успешной калибровке
CStdioFile *pAllDataFile;				// Файл со всеми возможными данными
int nFilePoints = -1;							// Количество точек для записи в файл
TCHAR sLastCalib[15];						// Показания последней внутренней калибровки

CListBox MessageList;