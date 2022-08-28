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

// Подключить библиотеку
void CMkioUsbElcus::Open( void )
{
	// Библиотека уже подключена
	if( bOpen_ )
	{
		return;
	}

	// Подключить библиотеку
	if( !::MkioUsb_Redan_Open( 0 ) )
	{
		throw ( std::runtime_error( "Ошибка инициализации библиотеки ввода/вывода" ) );
	}

	// Очистить приемный блок драйвера
	::MkioUsb_Redan_ReceiverClean();

	// Пауза
	::Sleep( 100 );

	// Библиотека подключена
	bOpen_ = true;
}

// Закрыть библиотеку
void CMkioUsbElcus::Close( void )
{
	// Библиотека уже закрыта
	if( !bOpen_ )
	{
		return;
	}

	// Запретить получение прерываний
	::MkioUsb_Redan_IrqDisable();

	// Закрыть библиотеку
	::MkioUsb_Redan_Close();

	// Библиотека отключена
	bOpen_ = false;
}

// Загрузить программы в процессоры
void CMkioUsbElcus::Load( const std::vector<loadConfig_s> &load )
{
	// Нет записей в конфигурации загрузки
	if( load.empty() )
	{
		throw ( std::runtime_error( "Конфигурация загрузки пуста" ) );
	}

	// Найти адрес ОУ
	addr_ = FindAddr();

	// Загрузка программ
	std::for_each( load.begin(), load.end(), load_for( this, false ) );
}

// Выбрать линию связи
void CMkioUsbElcus::SelectLine( MKIO_LINE line ) const
{
	if( !MkioUsb_Bc_SelectLine( line ) )
	{
		throw ( std::runtime_error( "Ошибка операции выбора линии связи" ) );
	}
}

// Выбрать процессор
void CMkioUsbElcus::SelectProc( WORD proc ) const
{
	// Выбрать процессор
	SelectProc( proc, false );
}

// Прием/передача
void CMkioUsbElcus::SendCommand( WORD *send_ptr, size_t send_sz, WORD *recv_ptr, size_t recv_sz ) const
{
	const WORD subAddr = 28;

	Send( subAddr, send_ptr, send_sz );
	Recv( subAddr, recv_ptr, recv_sz );
}

// Найти адрес ОУ
WORD CMkioUsbElcus::FindAddr( void ) const
{
	WORD addr[31];
	if( ::MkioUsb_Bc_FindSlaves( addr ) == 0 )
	{
		 throw ( std::runtime_error( "Не найдено ОУ" ) );
	}
	return addr[0];
}

// Передать данные
void CMkioUsbElcus::Send( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const
{
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, buf_sz, buf_ptr ) )
	{
		throw ( std::runtime_error( "Ошибка операции передачи данных" ) );
	}
}

// Принять данные
void CMkioUsbElcus::Recv( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const
{
	// Установить режим приема
	if( ::MkioUsb_Redan_SetReceiveMode( 1, buf_sz ) )
	{
		throw ( std::runtime_error( "Ошибка операции установки режима приема данных" ) );
	}

	// Очистить приемный блок
	::MkioUsb_Redan_ReceiverClean();

	// Разрешить прерывания
	if( !::MkioUsb_Redan_IrqEnable() )
	{
		throw ( std::runtime_error( "Ошибка операции включения прерываний" ) );
	}

	// Начало отсчета тайм-аута
	DWORD timeBegin = ::GetTickCount();

	// Ожидание приема данных
	while( ::MkioUsb_Redan_GetNumReceivedIrq()<1 )
	{
		// Пауза
		::Sleep( 10 );

		// Тайм-аут
		if( ( ::GetTickCount()-timeBegin ) > timeOut_ )
		{
			throw ( std::runtime_error( "Тайм-аут" ) );
		}
	};

	// Очистить приемный блок
	::MkioUsb_Redan_ReceiverClean();

	// Запретить прерывания
	if( !::MkioUsb_Redan_IrqDisable() )
	{
		throw ( std::runtime_error( "Ошибка операции получения данных" ) );
	}

	// Извлечь данные
	::MkioUsb_Redan_GetReceivedInfo( buf_sz, buf_ptr );
}

// Выбрать процессор
void CMkioUsbElcus::SelectProc( WORD proc, bool isProcChild ) const
{
	WORD send = proc&0x07;

	// Команда для дочернего процессора
	if( isProcChild )
	{
		send = 0x0600|( proc&0x07 );
	}

	if( !::MkioUsb_Bc_SendCommand( addr_, 30, 1, &send ) )
	{
		throw ( std::runtime_error( "Ошибка операции выбора номера процессора" ) );
	}
}

// Сбросить процессор
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

	// Команда
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		if( isProcChild )
		{
			throw ( std::runtime_error( "Ошибка операции сброса процессора" ) );
		}
	}

	if( isProcChild )
	{
		return;
	}

	// Ожидание
	::Sleep( 10 );

	// Взвод процессора
	send[0] = 0x06;
	addr_   = 0x11;

	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		throw ( std::runtime_error( "Ошибка операции взвода процессора" ) );
	}
}

// Загрузить процессор
void CMkioUsbElcus::LoadProc( WORD proc, bool isProcChild, const std::string &fileName ) const
{
	// Открыть файл
	std::basic_ifstream<char> file( fileName.c_str(), std::ios::binary );
	if( file.bad() )
	{
		throw ( std::runtime_error( "Ошибка открытия файла бортовой программы" ) );
	};

	// Получить фактический размер файла
	file.seekg( 0, std::ios::end );
	size_t file_sz = static_cast<size_t>( file.tellg() );
	file.seekg( 0, std::ios::beg );

	std::vector<char> buf( file_sz );
	file.read( &buf[0], buf.size() );
	file.close();

	// Считать размер файла из файла
	union
	{
		char  byte[4];
		DWORD size;
	} size_u;
	size_u.byte[0] = buf[3];
	size_u.byte[1] = buf[2];
	size_u.byte[2] = buf[1];
	size_u.byte[3] = buf[0];

	// Сравнить фактический и считанный размер файла плюс 4 BYTE
	if( file_sz != size_u.size+4 )
	{
		throw ( "Неизвестный формат файла бортовой программы" );
	}

	int bRet = false;

	// Загрузить родительский процессор
	if( !isProcChild )
	{
		bRet = ::MkioUsb_LoadProc_Ex( addr_, reinterpret_cast<const unsigned char *>( &buf[4] ), buf.size()-4 );
	}

	// Загрузить дочерний процессор
	else
	{
		bRet = ::MkioUsb_LoadProcTransit_Ex( addr_, proc, reinterpret_cast<const unsigned char *>( &buf[4] ), buf.size()-4 );
	}

	if( !bRet )
	{
		throw ( std::runtime_error( "Ошибка операции загрузки процессора" ) );
	}
}

// Запустить процессор
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

	// Команда
	if( !::MkioUsb_Bc_SendCommand( addr_, subAddr, send_sz, send ) )
	{
		throw ( std::runtime_error( "Ошибка операции запуска процессора" ) );
	}
}

// !!! Добавить загрузку дочерних процессоров !!!
void CMkioUsbElcus::load_for::operator ()( const loadConfig_s &load )
{
	// Пауза
	::Sleep( 100 );

	class_ptr->SelectProc( load.numProc, bProcChild );
	class_ptr->ResetProc( load.numProc, bProcChild );
	class_ptr->LoadProc( load.numProc, bProcChild, load.fileName );
/*
	// Режим по-умолчанию -> <ФИЗИК>
	if((cfg.wProc==2)&&(cfg.wLevel==1)&&(cfg.wSubProc>=0&&cfg.wSubProc<=6)) return;
	if((cfg.wProc==2)&&(cfg.wLevel==2)&&(cfg.wSubProc>=0&&cfg.wSubProc<=8)) return;
*/

	// Пауза
	::Sleep( 10 );

	// Запустить процессор
	class_ptr->StartProc( load.numProc, bProcChild );

/*
	// Пауза
	::Sleep(10);

	// МПТ
	if(0 < cfg.wLevel)
	{
		// Выбрать процессор
		pClass->mkioProcSelect(cfg.wProc, cfg.wLevel);
		// Запустить процессор
		pClass->mkioProcStartEx();
	}
*/
};
