#pragma once
class Protocol
{
public:
	Protocol();
	~Protocol();

	void WriteLog(TCHAR* sMes);							// Записать строку в отчет
protected:	
	HANDLE m_hlog;										// Дескриптор файла отчета
	TCHAR m_sLogPath[MAX_PATH];							// Путь к папке с отчетом

	void CreateLogPath();								// Создать папку в exe каталоге
	TCHAR* GetTimeStr(TCHAR* sTime, TCHAR cSepar);		// Получить строку даты и времени
};

