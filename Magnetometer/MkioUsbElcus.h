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
		LINE_MAIN   = 0, // основная линия связи
		LINE_RESERV = 1  // резервная линия связи
	};

	// Структура конфигурации программ для загрузки
	struct loadConfig_s
	{
		// Номер процессора
		WORD numProc;
		// Путь к файлу программы
		std::string fileName;
		// Ссылка на дочерние процессоры
//		std::vector<loadConfig_s *> child_ptr;
	};

///////////////////////////////////////////////////////////
public:

	// Подключить библиотеку
	void Open( void );
	// Закрыть библиотеку
	void Close( void );

	// Загрузить программы в процессоры
	void Load( const std::vector<loadConfig_s> &load );

	// Установить тайм-аут
	void SetTimeOut( DWORD timeOut ) { timeOut_ = timeOut; };

	// Выбрать линию связи
	void SelectLine( MKIO_LINE line ) const;

	// Выбрать процессор
	void SelectProc( WORD wProc ) const;

	// Прием/передача
	void SendCommand( WORD *send_ptr, size_t send_sz, WORD *recv_ptr, size_t recv_sz ) const;

///////////////////////////////////////////////////////////
private:

	// Найти адрес ОУ
	WORD FindAddr( void ) const;

	// Передать данные
	void Send( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const;
	// Получить данные
	void Recv( WORD subAddr, WORD *buf_ptr, size_t buf_sz ) const;

	// Выбрать процессор
	void SelectProc( WORD proc, bool isProcChild ) const;
	// Сбросить процессор
	void ResetProc( WORD proc, bool isProcChild );
	// Загрузить процессор
	void LoadProc( WORD proc, bool isProcChild, const std::string &fileName ) const;
	// Запустить процессор
	void StartProc( WORD proc, bool isProcChild ) const;
/*
	void mkioProcStartEx() throw(Err_Cmd_Proc_Start, Err_User_Break);
*/
///////////////////////////////////////////////////////////
private:

	// Загрузка программ в процессор
// !!! Добавить загрузку дочерних процессоров !!!
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
// Свойства
///////////////////////////////////////////////////////////
private:

	// Флаг подключения библиотеки
	bool bOpen_;

	// Адрес ОУ
	WORD addr_;

	// Тайм-аут
	DWORD timeOut_;
};
