//
#include "list"
//#define	CALIBPOINTS 30					// Количество точек для калибровки

extern class CMagnetometerDlg *pMainDlg;	// Класс главного окна
extern class Pni_tcm *pPni;					// Класс магнитометра
extern class Pni_MA *pMA;					// Класс модуля аппаратурного
extern class Protocol *pProtocol;			// Класс протокола
extern class WriteGraph GraphFile[9];

extern int nRunFlag;
extern int nStartTime;							// Начальная точка для таймера
extern int nCurTime;							// Текущее время таймера
extern TCHAR pMes[MAX_PATH];				// Строка для записи в протокол и вывода сообщений
extern TCHAR sFolderPath[MAX_PATH];			// Начальный путь к логам и файлам графера
extern BOOL bModePr;					// Режим работы программы 0 - магнитометр, 1 - МА

extern std::list<double> CalibComp;
extern std::list<double> CalibPitch;
extern std::list<double> CalibRoll;
extern std::list<double> CalibX;
extern std::list<double> CalibY;
extern std::list<double> CalibZ;
extern int CalibPoints;

extern BOOL bCalib;								// Флаг о необходимости калибровки
extern BOOL bComCalib;						// Флаг об успешной калибровке
extern CStdioFile *pAllDataFile;				// Файл со всеми возможными данными
extern int nFilePoints;							// Количество точек для записи в файл
extern TCHAR sLastCalib[15];						// Показания последней внутренней калибровки

extern CListBox MessageList;