#include "stdafx.h"
#include "Pni_MA.h"
#include "Magnetometer.hxx"


Pni_MA::Pni_MA()
{
	sIniSet.append(".\\MagCalib.ini");
}

Pni_MA::~Pni_MA()
{
	// �������� ����� �������� ������ �� �������
	CloseIO();
}

void Pni_MA::copy_word_to_string(const WORD * source_ptr, size_t dest_sz, char * dest_ptr)
{
	for (size_t i = 0; i<dest_sz; ++i)
	{
		*dest_ptr++ = static_cast<char>(*source_ptr++);
	}
}

void Pni_MA::LoadSettings()
{
	typedef std::pair<std::string, std::string> pair_t;
	settingProgram_.insert(pair_t("proc_0", ""));
	settingProgram_.insert(pair_t("proc_1", ""));
	settingProgram_.insert(pair_t("proc_2", ""));
	settingProgram_.insert(pair_t("flag_turn", "0"));
	settingProgram_.insert(pair_t("path_koef_file", ""));
	settingProgram_.insert(pair_t("operator_name", ""));
	settingProgram_.insert(pair_t("customer_name", ""));
	settingProgram_.insert(pair_t("control_name", ""));

	for (auto it = settingProgram_.begin(); it != settingProgram_.end(); ++it)
	{
		TCHAR sBuf[_MAX_PATH + 1];

		::GetPrivateProfileString("Settings", it->first.c_str(), "", sBuf, sizeof(sBuf), sIniSet.c_str());

		if (_tcslen(sBuf) == 0)
		{
			continue;
		}

		it->second = sBuf;
	}
}

void Pni_MA::OpenIO()
{
	m_IO.Open();
}

void Pni_MA::CloseIO()
{
	m_IO.Close();
}

void Pni_MA::LoadProgram()
{
	std::vector<CMkioUsbElcus::loadConfig_s> load;
	CMkioUsbElcus::loadConfig_s config;

	config.numProc = 0;
	config.fileName = settingProgram_["proc_0"];
	load.push_back(config);

	config.numProc = 1;
	config.fileName = settingProgram_["proc_1"];
	load.push_back(config);

	config.numProc = 2;
	config.fileName = settingProgram_["proc_2"];
	load.push_back(config);

	m_IO.Load(load);
	m_IO.SelectProc(1);
}

void Pni_MA::CheckConnect()
{
	// ������ �������
	const size_t array_sz = 7;

	std::vector<WORD> send(array_sz + 1);
	// �������
	send[0] = (CMD_CHECK_CONNECT * 0x100) | (send.size() - 1);
	// ������
	for (size_t i = 1; i<send.size(); ++i)
	{
		send[i] = static_cast<WORD>(rand());
	}

	std::vector<WORD> recv(array_sz);

	m_IO.SetTimeOut(CalculateTimeOut(recv.size()));
	m_IO.SendCommand(&send[0], send.size(), &recv[0], recv.size());

	// ������
	if (!std::equal(send.begin() + 1, send.end(), recv.begin()))
	{
		throw std::runtime_error("������: �������� ����� �����");
	}
}

void Pni_MA::CheckMag()
{
	std::vector<WORD> send(2);
	// �������
	send[0] = (CMD_CHECK_MAG * 0x100) | (send.size() - 1);
	// ������
	send[1] = 0;

	std::vector<WORD> recv(2);

	m_IO.SetTimeOut(CalculateTimeOut(recv.size()));
	m_IO.SendCommand(&send[0], send.size(), &recv[0], recv.size());

	// ������
	if (recv[0] != CMD_CHECK_MAG)
	{
		throw std::runtime_error("������: ������� �������� ������");
	}
	if (recv[1] != 1)
	{
		throw std::runtime_error("������: ��� ����� � �������������");
	}
}

int Pni_MA::MACalib()
{
	std::string sQuery;
	std::string sReceiveData;
	sQuery = "cc?";
	SendCommand(sQuery, sReceiveData);
	sQuery = "mpcal=e";
	SendCommand(sQuery, sReceiveData);
	sQuery = "go";
	SendCommand(sQuery, sReceiveData);
	AfxMessageBox("�������� �����������. ������� ������ �� �� ���������", MB_ICONINFORMATION);
	sQuery = "h";
	SendCommand(sQuery, sReceiveData);
	sQuery = "mpcal=d";
	SendCommand(sQuery, sReceiveData);
	while (sReceiveData.front() != 'H')
	{
		sQuery = "lc?";
		SendCommand(sQuery, sReceiveData);
	}

	TCHAR sReceivedData[100] = "";
	_tcscpy_s(sReceivedData, sReceiveData.c_str());
	// ��������� ����������� ����������
	int nPos = _tcschr(sReceivedData, '\r') - sReceivedData;
	_tcsncpy_s(sLastCalib, sReceivedData, nPos);
	sLastCalib[nPos] = '\0';

	// ��������� ���������� ���������� ���������� ����
	_tcsncpy_s(sReceivedData, sLastCalib + 5, 10);

	// �������� ���������� ����������
	if (_ttoi(&sLastCalib[1]) < 9 || _ttoi(&sLastCalib[3]) < 9)//|| _ttof(sReceivedData) > 10)
		return 0;
	return 1;
}


//void Pni_MA::SendCommand(CMkioUsbElcus &io, std::string &cmd_str, std::string &ret_str)
void Pni_MA::SendCommand(std::string &cmd_str, std::string &ret_str)
{
	// ���������� ������������ ����
	const size_t recv_sz = 100;

	// [0]:      �������/���-�� ������������ ����
	// [1]:      ���-�� ������������ ����
	// [2]:      ����-��� ( == [2]*10��)
	// [3]:      ���-�� ���� � �������
	// [4..n-1]: �������
	// [n]:      <cr>
	std::vector<WORD> send(cmd_str.size() + 5);
	send[0] = (CMD_SEND_COMMAND * 0x100) | (send.size() - 1);
	send[1] = recv_sz;
	send[2] = 20;
	send[3] = cmd_str.size() + 1;
	std::copy(cmd_str.begin(), cmd_str.end(), send.begin() + 4);
	send[send.size() - 1] = '\r';

	// [0]: CMD_SEND_COMMAND ( ������ )
	// [1]: ����������� ���-�� �������� ����
	// [2..n]: ������ �������� ����
	std::vector<WORD> recv(recv_sz);

	m_IO.SetTimeOut(CalculateTimeOut(recv.size()) + send[2] * 10);
	m_IO.SendCommand(&send[0], send.size(), &recv[0], recv.size());

	if (recv[0] != CMD_SEND_COMMAND)
	{
		throw std::runtime_error("������: ������� �������� ������");
	}

	ret_str.resize(recv[1]);
	copy_word_to_string(&recv[2], recv[1], &ret_str[0]);
}

int Pni_MA::CalculateTimeOut(size_t words)
{
	return static_cast<int> (1.0f*words / (MKIO_SPEED / 16.0f/*bits in word*/ / 1000.0f/*ms in per sec*/) + 1.0/*rounded up*/ + MAG_TIME_OUT);
}
