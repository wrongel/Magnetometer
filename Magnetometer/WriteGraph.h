#pragma once
#define Block_size 64
class WriteGraph
{
public:
	WriteGraph();
	~WriteGraph();

	HANDLE m_hlog;
	int CreateGraphFile(TCHAR* sNameFile);			// Создать файл для графера
	void SetGraphData(float fDat, int nNewFlag);	// Записать данные в массив
	int SaveGraphData(BOOL bLog);					// Сохранить файл для графера, параметр - записать в лог или нет

	TCHAR* GetFilePath();		// Получение адреса файла

protected:
	TCHAR m_sFilePath[MAX_PATH];		// Путь к папке с файлом
	float m_fData[Block_size / 4];			// Массив данных
	int nBytesWriten;			// Количество байт для записи и текущий элемент в массиве
	int nBlocksWriten;			// Количество записанных блоков в файл
};

