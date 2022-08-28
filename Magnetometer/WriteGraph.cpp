#include "stdafx.h"
#include "WriteGraph.h"
#include "Protocol.h"
#include "Magnetometer.hxx"

WriteGraph::WriteGraph()
{
	nBytesWriten = 0;
	nBlocksWriten = 0;
	*m_sFilePath = NULL;
	memset(m_fData, 0, sizeof(m_fData));
}

WriteGraph::~WriteGraph()
{
}

int WriteGraph::CreateGraphFile(TCHAR* sNameFile)
 {
	// Для текстового файла
	/*_stprintf_s(m_sFilePath, "%s%s", sFolderPath, "\\MagnetometerGraph\\");
	CreateDirectory(m_sFilePath, NULL);
	_tcscat_s(m_sFilePath, sNameFile);
	m_hlog = ::CreateFile(m_sFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);*/

	CStdioFile pFile;
	char cHead[512];

	_stprintf_s(m_sFilePath, "%s%s", sFolderPath, "\\MagnetometerGraph\\");
	CreateDirectory(m_sFilePath, NULL);
	_tcscat_s(m_sFilePath, sNameFile);

	memset(cHead, 0xAA, sizeof(cHead));
	if (!pFile.Open(m_sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		_stprintf_s(pMes, "%s%s", "Не удалось создать файл ", m_sFilePath);
		//_stprintf_s(pMes, "%s%s%s", "Не удалось создать файл ", m_sFilePath, "\r\n");
		pProtocol->WriteLog(pMes);
		return 0;
	}
	pFile.SeekToBegin();
	pFile.Write(cHead, sizeof(cHead));
	pFile.Close();
	_stprintf_s(pMes, "%s%s%s", "Создан файл ", m_sFilePath, "\r\n");
	pProtocol->WriteLog(pMes);
	
	return 1;
}

void WriteGraph::SetGraphData(float fDat, int nNewFlag)
{
	/*TCHAR sLogMes[200];
	if (count % 10 == 0 && count != 0)
		_stprintf_s(sLogMes, "%s;\r\n", fDat);
	else
		_stprintf_s(sLogMes, "%f; ", fDat);
	WriteFile(m_hlog, sLogMes, _tcslen(sLogMes), NULL, NULL);*/

	if (nNewFlag)
	{
		nBytesWriten = 0;
		nBlocksWriten = 0;
	}
	m_fData[nBytesWriten / 4] = fDat;
	if (nBytesWriten + 4 == Block_size)
	{
		SaveGraphData(0);
		nBytesWriten = 0;
		nBlocksWriten++;
	}
	else
		nBytesWriten += 4;
}

int WriteGraph::SaveGraphData(BOOL bLog)
{
	/*if (m_hlog != INVALID_HANDLE_VALUE)
		CloseHandle(m_hlog);*/

	CStdioFile pFile;

	if (!pFile.Open(m_sFilePath, CFile::modeWrite | CFile::modeNoTruncate | CFile::typeBinary))
	{
		_stprintf_s(pMes, "%s%s%s", "Не удалось сохранить файл ", m_sFilePath, "\r\n");
		pProtocol->WriteLog(pMes);
		return 0;
	}
	if (nBlocksWriten)
		pFile.Seek(512 + Block_size * nBlocksWriten, CFile::begin);
	else
		pFile.Seek(512, CFile::begin);
	if (nBytesWriten + 4 == Block_size)
		pFile.Write(m_fData, Block_size);
	else
		pFile.Write(m_fData, nBytesWriten);
	pFile.Close();

	if (bLog)
	{
		_stprintf_s(pMes, "%s%s%s", "Сохранен файл ", m_sFilePath, "\r\n");
		pProtocol->WriteLog(pMes);
	}
	return 1;
}

TCHAR* WriteGraph::GetFilePath()
{
	return m_sFilePath;
}

