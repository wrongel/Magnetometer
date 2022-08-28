#pragma once

#include "MkioUsbElcus.h"
#include <map>

class Pni_MA
{
public:
	Pni_MA();
	~Pni_MA();

	void LoadSettings();
	void OpenIO();
	void CloseIO();
	void LoadProgram();
	void CheckConnect();
	void CheckMag();
	int MACalib();
	
	void SendCommand(std::string &cmd_str, std::string &ret_str);
	int CalculateTimeOut(size_t words);

private:

	// Константы
	enum
	{
		// Скорость обмена по МКИО ( бит/сек )
		MKIO_SPEED = 38400,
		// Таймаут АНВ ( мс )
		MAG_TIME_OUT = 200,
	};

	// Команды
	enum MAG_COMMAND
	{
		CMD_CHECK_CONNECT = 0x81,
		CMD_CHECK_MAG = 0x82,
		CMD_MAG_QUERY = 0x83,
		CMD_MAG_AND_ANGLE_QUERY = 0x84,
		CMD_SEND_COMMAND = 0x89,
	};

	CMkioUsbElcus m_IO;
	std::string sIniSet;		// Имя *.ini-файла
	// Настройки программы
	typedef std::map<std::string, std::string> settingProgram_t;		// Попробывать потом избавиться от map и в include тоже
	settingProgram_t settingProgram_;

	void copy_word_to_string(const WORD *source_ptr, size_t dest_sz, char *dest_ptr);
};

