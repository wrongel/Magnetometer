#include "stdafx.h"
#include "Protocol.h"
#include "Magnetometer.hxx"


Protocol::Protocol()
{
	CreateLogPath();
	m_hlog = ::CreateFile(m_sLogPath, GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hlog == INVALID_HANDLE_VALUE)
		AfxMessageBox("Не удалось создать файл с отчетом");
	else
		WriteLog("Начало работы\r\n");
}

Protocol::~Protocol()
{
	WriteLog("Завершение работы\r\n");
	if (m_hlog != INVALID_HANDLE_VALUE)
		CloseHandle(m_hlog);
}

void Protocol::CreateLogPath()
{
	TCHAR sProtName[25];
	_tcscpy_s(m_sLogPath, sFolderPath);
	_tcscat_s(m_sLogPath, "\\MagnetometerLogs\\");
	CreateDirectory(m_sLogPath, NULL);
	_tcscat_s(m_sLogPath, GetTimeStr(sProtName, '.'));
	_tcscat_s(m_sLogPath, ".log");
}

TCHAR* Protocol::GetTimeStr(TCHAR * sTime, TCHAR cSepar)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	if (cSepar == '.')
		_stprintf_s(sTime, 25, "%02d-%02d-%d_%02d.%02d.%02d", time.wDay, time.wMonth, time.wYear,
			time.wHour, time.wMinute, time.wSecond);
	else
		_stprintf_s(sTime, 25, "%02d-%02d-%d_%02d:%02d:%02d", time.wDay, time.wMonth, time.wYear,
			time.wHour, time.wMinute, time.wSecond);	// Если написать _tcslen(sTime) вместо явного 25 то теряется указатель на this

	return sTime;
}

void Protocol::WriteLog(TCHAR * sMes)
{
	TCHAR sLogMes[MAX_PATH];
	DWORD dwBytesWritten;
	_stprintf_s(sLogMes, "%s%s%s", GetTimeStr(sLogMes, ':'), "\t", sMes);
	WriteFile(m_hlog, sLogMes, _tcslen(sLogMes), &dwBytesWritten, NULL);
}
