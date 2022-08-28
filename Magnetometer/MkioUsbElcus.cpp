// MkioUsbElcus.cpp: implementation of the CMkioUsbElcus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MkioUsbElcusDrv_v3.h"
#include "MkioUsbElcus.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMkioUsbElcus::CMkioUsbElcus( void ):
	bOpen_( false ),
	addr_( 0x11 ),
	timeOut_( 500 )
{
}

CMkioUsbElcus::~CMkioUsbElcus( void )
{
	Close();
}

// ���������� ����������
void CMkioUsbElcus::Open( void )
{
	// ���������� ��� ����������
	if( bOpen_ )
	{
		return;
	}

	// ���������� ����������
	if( !::MkioUsb_Redan_Open( 0 ) )
	{
		throw ( std::runtime_error( "������ ������������� ���������� �����/������" ) );
	}

	// �������� �������� ���� ��������
	::MkioUsb_Redan_ReceiverClean();

	// �����
	::Sleep( 100 );

	// ���������� ����������
	bOpen_ = true;
}

// ������� ����������
void CMkioUsbElcus::Close( void )
{
	// ���������� ��� �������
	if( !bOpen_ )
	{
		return;
	}

	// ��������� ��������� ����������
	::MkioUsb_Redan_IrqDisable();

	// ������� ����������
	::MkioUsb_Redan_Close();

	// ���������� ���������
	bOpen_ = false;
}

// ��������� ��������� � ����������
void CMkioUsbElcus::Load( const std::vector<loadConfig_s> &load )
{
	// ��� ������� � ������������ ��������
	if( load.empty() )
	{
		throw ( std::runtime_error( "������������ �������� �����" ) );
	}

	// ����� ����� ��
	addr_ = FindAddr();

	// �������� ��������
	std::for_each( load.begin(), load.end(), load_for( this, false ) );
}

// ������� ����� �����
void CMkioUsbElcus::SelectLine( MKIO_LINE line ) const
{
	if( !MkioUsb_Bc_SelectLine( line ) )
	{
		throw ( std::runtime_error( "������ �������� ������ ����� �����" ) );
	}
}

// ������� ���������
void CMkioUsbElcus::SelectProc( WORD proc ) const
{
	// ������� ���������
	SelectProc( proc, false );
}

// �����/��������
void CMkioUsbElcus::SendCommand( WORD *send_ptr, size_t send_sz, WORD *recv_ptr, size_t recv_sz ) const
{
	const WORD subAddr = 28;

	Send( subAddr, send_ptr, send_sz );
	Recv( subAddr, recv_ptr, recv_sz );
}

// ����� ����� ��
WORD CMkioUsbElcus::FindAddr( void ) const
{
	WORD addr[31];
	if( ::MkioUsb_Bc_FindSlaves( addr ) == 0 )
	{
		 throw ( std::runtime_error( "�� ������� ��" ) );
	}
	return addr[0];
}

// �������� ������
void CMkioUsbElcus::Send( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const
{
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, buf_sz, buf_ptr ) )
	{
		throw ( std::runtime_error( "������ �������� �������� ������" ) );
	}
}

// ������� ������
void CMkioUsbElcus::Recv( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const
{
	// ���������� ����� ������
	if( ::MkioUsb_Redan_SetReceiveMode( 1, buf_sz ) )
	{
		throw ( std::runtime_error( "������ �������� ��������� ������ ������ ������" ) );
	}

	// �������� �������� ����
	::MkioUsb_Redan_ReceiverClean();

	// ��������� ����������
	if( !::MkioUsb_Redan_IrqEnable() )
	{
		throw ( std::runtime_error( "������ �������� ��������� ����������" ) );
	}

	// ������ ������� ����-����
	DWORD timeBegin = ::GetTickCount();

	// �������� ������ ������
	while( ::MkioUsb_Redan_GetNumReceivedIrq()<1 )
	{
		// �����
		::Sleep( 10 );

		// ����-���
		if( ( ::GetTickCount()-timeBegin ) > timeOut_ )
		{
			throw ( std::runtime_error( "����-���" ) );
		}
	};

	// �������� �������� ����
	::MkioUsb_Redan_ReceiverClean();

	// ��������� ����������
	if( !::MkioUsb_Redan_IrqDisable() )
	{
		throw ( std::runtime_error( "������ �������� ��������� ������" ) );
	}

	// ������� ������
	::MkioUsb_Redan_GetReceivedInfo( buf_sz, buf_ptr );
}

// ������� ���������
void CMkioUsbElcus::SelectProc( WORD proc, bool isProcChild ) const
{
	WORD send = proc&0x07;

	// ������� ��� ��������� ����������
	if( isProcChild )
	{
		send = 0x0600|( proc&0x07 );
	}

	if( !::MkioUsb_Bc_SendCommand( addr_, 30, 1, &send ) )
	{
		throw ( std::runtime_error( "������ �������� ������ ������ ����������" ) );
	}
}

// �������� ���������
void CMkioUsbElcus::ResetProc( WORD proc, bool isProcChild )
{
	WORD   send[2];
	WORD   subAddr = 29;
	size_t send_sz = 1;
	       send[0] = 0x07;

	if( isProcChild )
	{
		subAddr = 28;
		send_sz = 2;
		send[0] = 0xCA00|( send_sz-1 );
		send[1] = proc;
	}

	// �������
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		if( isProcChild )
		{
			throw ( std::runtime_error( "������ �������� ������ ����������" ) );
		}
	}

	if( isProcChild )
	{
		return;
	}

	// ��������
	::Sleep( 10 );

	// ����� ����������
	send[0] = 0x06;
	addr_   = 0x11;

	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		throw ( std::runtime_error( "������ �������� ������ ����������" ) );
	}
}

// ��������� ���������
void CMkioUsbElcus::LoadProc( WORD proc, bool isProcChild, const std::string &fileName ) const
{
	// ������� ����
	std::basic_ifstream<char> file( fileName.c_str(), std::ios::binary );
	if( file.bad() )
	{
		throw ( std::runtime_error( "������ �������� ����� �������� ���������" ) );
	};

	// �������� ����������� ������ �����
	file.seekg( 0, std::ios::end );
	size_t file_sz = static_cast<size_t>( file.tellg() );
	file.seekg( 0, std::ios::beg );

	std::vector<char> buf( file_sz );
	file.read( &buf[0], buf.size() );
	file.close();

	// ������� ������ ����� �� �����
	union
	{
		char  byte[4];
		DWORD size;
	} size_u;
	size_u.byte[0] = buf[3];
	size_u.byte[1] = buf[2];
	size_u.byte[2] = buf[1];
	size_u.byte[3] = buf[0];

	// �������� ����������� � ��������� ������ ����� ���� 4 BYTE
	if( file_sz != size_u.size+4 )
	{
		throw ( "����������� ������ ����� �������� ���������" );
	}

	int bRet = false;

	// ��������� ������������ ���������
	if( !isProcChild )
	{
		bRet = ::MkioUsb_LoadProc_Ex( addr_, reinterpret_cast<const unsigned char *>( &buf[4] ), buf.size()-4 );
	}

	// ��������� �������� ���������
	else
	{
		bRet = ::MkioUsb_LoadProcTransit_Ex( addr_, proc, reinterpret_cast<const unsigned char *>( &buf[4] ), buf.size()-4 );
	}

	if( !bRet )
	{
		throw ( std::runtime_error( "������ �������� �������� ����������" ) );
	}
}

// ��������� ���������
void CMkioUsbElcus::StartProc( WORD proc, bool isProcChild ) const
{
	WORD   send[2];
	WORD   subAddr = 29;
	size_t send_sz = 1;
	       send[0] = 0x00;

	if( isProcChild )
	{
		subAddr = 28;
		send_sz = 2;
		send[0] = 0xCC00|( send_sz-1 );
		send[1] = proc;
	}

	// �������
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		throw ( std::runtime_error( "������ �������� ������� ����������" ) );
	}
}

// !!! �������� �������� �������� ����������� !!!
void CMkioUsbElcus::load_for::operator ()( const loadConfig_s &load )
{
	// �����
	::Sleep( 100 );

	class_ptr->SelectProc( load.numProc, bProcChild );
	class_ptr->ResetProc( load.numProc, bProcChild );
	class_ptr->LoadProc( load.numProc, bProcChild, load.fileName );
/*
	// ����� ��-��������� -> <�����>
	if((cfg.wProc==2)&&(cfg.wLevel==1)&&(cfg.wSubProc>=0&&cfg.wSubProc<=6)) return;
	if((cfg.wProc==2)&&(cfg.wLevel==2)&&(cfg.wSubProc>=0&&cfg.wSubProc<=8)) return;
*/

	// �����
	::Sleep( 10 );

	// ��������� ���������
	class_ptr->StartProc( load.numProc, bProcChild );

/*
	// �����
	::Sleep(10);

	// ���
	if(0 < cfg.wLevel)
	{
		// ������� ���������
		pClass->mkioProcSelect(cfg.wProc, cfg.wLevel);
		// ��������� ���������
		pClass->mkioProcStartEx();
	}
*/
};
