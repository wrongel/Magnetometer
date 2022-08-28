// MkioUsbElcus.h: interface for the CMkioUsbElcus class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

class CMkioUsbElcus
{
public:
	CMkioUsbElcus( void );
	virtual ~CMkioUsbElcus( void );

///////////////////////////////////////////////////////////
public:

	enum MKIO_LINE
	{
		LINE_MAIN   = 0, // �������� ����� �����
		LINE_RESERV = 1  // ��������� ����� �����
	};

	// ��������� ������������ �������� ��� ��������
	struct loadConfig_s
	{
		// ����� ����������
		WORD numProc;
		// ���� � ����� ���������
		std::string fileName;
		// ������ �� �������� ����������
//		std::vector<loadConfig_s *> child_ptr;
	};

///////////////////////////////////////////////////////////
public:

	// ���������� ����������
	void Open( void );
	// ������� ����������
	void Close( void );

	// ��������� ��������� � ����������
	void Load( const std::vector<loadConfig_s> &load );

	// ���������� ����-���
	void SetTimeOut( DWORD timeOut ) { timeOut_ = timeOut; };

	// ������� ����� �����
	void SelectLine( MKIO_LINE line ) const;

	// ������� ���������
	void SelectProc( WORD wProc ) const;

	// �����/��������
	void SendCommand( WORD *send_ptr, size_t send_sz, WORD *recv_ptr, size_t recv_sz ) const;

///////////////////////////////////////////////////////////
private:

	// ����� ����� ��
	WORD FindAddr( void ) const;

	// �������� ������
	void Send( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const;
	// �������� ������
	void Recv( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const;

	// ������� ���������
	void SelectProc( WORD proc, bool isProcChild ) const;
	// �������� ���������
	void ResetProc( WORD proc, bool isProcChild );
	// ��������� ���������
	void LoadProc( WORD proc, bool isProcChild, const std::string &fileName ) const;
	// ��������� ���������
	void StartProc( WORD proc, bool isProcChild ) const;
/*
	void mkioProcStartEx() throw(Err_Cmd_Proc_Start, Err_User_Break);
*/
///////////////////////////////////////////////////////////
private:

	// �������� �������� � ���������
// !!! �������� �������� �������� ����������� !!!
	class load_for
	{
		private:

			CMkioUsbElcus *class_ptr;
			bool           bProcChild;

		public:

			load_for( CMkioUsbElcus *ptr, bool isProcChild ): class_ptr( ptr ), bProcChild( isProcChild )
			{
			};

			virtual ~load_for( void )
			{
			};

			void operator ()( const loadConfig_s &load );
	};
	friend class load_for;

///////////////////////////////////////////////////////////
// ��������
///////////////////////////////////////////////////////////
private:

	// ���� ����������� ����������
	bool bOpen_;

	// ����� ��
	WORD addr_;

	// ����-���
	DWORD timeOut_;
};
