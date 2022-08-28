#if !defined(__INCLUDED_MKIOUSBLIBELCUS_H__)
#define __INCLUDED_MKIOUSBLIBELCUS_H__

#define MKIO_MAX_BC 2
#define MKIO_MAX_RT 2
#define MKIO_MAX_MT 1

#define GREK_INC


/*        Расширенное слово результата обмена(wResultX)

|15 14 13 12|11 10 09 08|07 06 05 04|03 02 01 00|
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
¦BS|ME|    SCC    ¦K1|K2| X| X|G2|G1|IB| ERROR  ¦
+--------------+--+--+-----------+--------------+

 IB (SX_IB_MASK ) установлен бит в ОС
 G1 (SX_G1_MASK ) генерация по ЛПИ A
 G2 (SX_G2_MASK ) генерация по ЛПИ B
 K2 (SX_K2_MASK ) ошибка во 2-м ОС (МТ)
 K1 (SX_K1_MASK ) ошибка в 1-м ОС (МТ)
 SCC(SX_SCC_MASK) поле кода формата сообщения (МТ)
 ME (SX_ME_MASK	) интегрированный признак ошибки в сообщении (МТ)
 BS (SX_BUS_MASK) номер ЛПИ (МТ)

------------------------------------------------|
|Знач. | Имя      | Смысл                       |
|ERROR | Const    | Ошибки                      |
|-----------------------------------------------|
|  0   | SX_NOERR | Нет ошибок;                 |
|  1   | SX_MEO   | Ошибка кода Манчестер-2     |
|  2   | SX_TOA   | Не пришло ОС                |
|  3   | SX_TOD   | Не пришло ИС( для запроса ) |
|  4   | SX_ELN   | Лишние ИС( для запроса )    |
|  5   | SX_ERAO  | Ошибка адреса ОУ в ОС       |
|  6   | SX_ESYN  | Ошибка типа синхроимпульса  |
|  7   | SX_EBC   | Ош.эхоконтроля              |
-------------------------------------------------*/

typedef struct tagMKIO_BC_STATEWORD
{
  union
  {
    struct // Расширенное  слово результата обмена(wResultX)
    {
     unsigned short uError     : 3;
     unsigned short uAwBitError: 1;
     unsigned short uGenLine1  : 1;
     unsigned short uGenLine2  : 1;
     unsigned short uUnuse     : 2;
     unsigned short uAw2Error  : 1;
     unsigned short uAw1Error  : 1;
     unsigned short MT_Format  : 4;
     unsigned short MT_Error   : 1;
     unsigned short MT_Line    : 1;
    } ResX;
    struct // Cлово результата обмена(wResult)
    {
      unsigned short uAdrError  :  1;//  S_ERAO_MASK
      unsigned short uCodeError :  1;//  S_MEO_MASK
      unsigned short uAwBitError:  1;//  S_IB_MASK
      unsigned short uPauseError:  1;//  S_TO_MASK
      unsigned short uMemError  :  1;//  S_EM_MASK
      unsigned short uAbsError  :  1;//  S_EBC_MASK
      unsigned short uDi        :  1;//  S_DI_MASK
      unsigned short uEln       :  1;//  S_ELN_MASK
      unsigned short uG1        :  1;//  S_G1_MASK
      unsigned short uG2        :  1;//  S_G2_MASK
      unsigned short uUnuse     :  6;//  S_G2_MASK
    } Res;
    WORD uWord;
  };
  WORD uInt;
} BC_STATEWORD;


/*               Командное слово (КС)
 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---
¦     ADR      ¦K ¦    SA/CI     ¦    N/COP     ¦
+--------------+--+--+-----------+--------------+
| uAddress     |Di|   uSubAddr   |    uNumData  |
+--------------+--+--+-----------+--------------+

  ADR - поле адреса адресуемого OУ:
       00000-11110 - адрес ОУ;
       11111(31) - групповой адрес;
  К - бит "прием/передача", указывает на действия OУ:
       0 - OУ принимает информацию (RT_RECEIVE),
       1 - ОУ передает информацию (RT_TRANSMIT);
  SA/CI - подадрес/режим управления (вместе с битом B):
       00000, 11111 - задает команду режима управления в поле N/COP в режиме без аппаратного бита,
       11111 - задает команду режима управления в поле N/COP в режиме с аппаратным битом (CI_MASK),
       00001-11110 - задает подадрес в режиме без аппаратного бита,
       10000-11110 - задает подадрес в режиме с аппаратным битом;
  N/COP - число информационных слов/код команды:
       00000 - 32 слова в режиме передачи данных,
       00001-11111 - 1-31 слово в режиме передачи данных,
       00000-11111 - код команды в режиме управления.*/
typedef struct tagMKIO_BC_COMMAND_WORD
{
  union
  {
    struct
    {
      unsigned short uNumData: 5;
      unsigned short uSubAddr: 5;
      unsigned short uDirect : 1;
      unsigned short uAddress: 5;
    } Bf;
    WORD uWord;
  };
} BC_CMDWORD;

/*            Ответное слово (ОС)
 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---
¦    ADR       ¦A ¦B ¦C ¦X  X  X ¦D ¦E ¦F ¦G ¦H ¦
+--------------+--+--+--+--------+--+--+--+--+--+
      ADR - поле адреса OУ ответившего ОУ:
          00000-11110 - адрес ОУ (ADDR_MASK);
      A - бит ошибки сообщения (ERROR_MASK);
      В - аппаратный бит (HBIT_MASK);
      C - бит "Запрос обслуживания подсистемы" (SREQ_MASK);
      XXX - резерв (000) (NULL_MASK);
      D - бит "Принята групповая команда" (BRCST_MASK);
      E - бит "Подсистема занята" (BUSY_MASK);
      F - бит "Неисправность подсистемы" (SSFL_MASK);
      G - бит "Принято управление каналом" (DNBA_MASK);
      H - бит "Неисправность терминала" (RTFL_MASK);   */
typedef struct tagMKIO_BC_ANSWER_WORD
{
  union
  {
    struct
    {
      unsigned short  uRtInvalid      : 1;
      unsigned short  uGetControl     : 1;
      unsigned short  uAbonentInvalid : 1;
      unsigned short  uBusy           : 1;
      unsigned short  uGroupCommand   : 1;
      unsigned short  uReserv         : 3;
      unsigned short  uServiceRequest : 1;
      unsigned short  uTransmitAw     : 1;
      unsigned short  uMessageError   : 1;
      unsigned short  uAddress        : 5;
    } Bf;
    WORD uWord;
  };
} BC_ANSWORD;


/*                    Формат флагового слова ОУ.
  15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
¦FLG¦ X ¦ X ¦ X ¦ X ¦T/R¦        SA         ¦         N         ¦
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                    +-----------------------v-------------------+
                                Разряды командного слова        */
typedef struct tagMKIO_RT_FLAG_WORD
{
  union
  {
    struct
    {
      unsigned short uNumData: 5;
      unsigned short uSubAddr: 5;
      unsigned short uDirect : 1;
      unsigned short uUnused : 4;
      unsigned short uFlag   : 1;
    } Bf;
    WORD uWord;
  };
} RT_FLAGWORD;

/*             Формат слова состояния ОУ.

15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
¦ X ¦EM ¦ X ¦ER ¦BP ¦T/R¦      SA/CI        ¦       N/COP       ¦
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                    +-----------------------v-------------------+
                            Разряды последнего командного слова МК

  ПРИМЕЧАНИЕ: слово состояния ОУ возвращается функцией rtgetstate, а также передается в качестве параметра прерываний rtIntErr, rtIntData.

     T/R - разряд "прием/передача" последнего КС (RT_DIR_MASK);
     SA/CI - разряды подадрес/режим управления последнего КС (SUBADDR_MASK, CI_MASK);
     N/COP - разряды число слов/код команды последнего КС (NWORS_MASK, CMD_MASK);
     BP - бит занятости выбранного подадреса в режиме без флагов устанавливается, если выбранный подадрес ведет в данный момент обмен с МК;
     ER - бит "Ошибка обмена с подсистемой", установка этого бита может указывать на неисправность в устройстве (этот бит может устанавливаться только в платах TMK400, RTMK400, TMKMPC);
     EM - бит "Ошибка сообщения МК" устанавливается, если в адресуемом данному ОУ сообщении обнаружена ошибка формата сообщения, ошибка манчестерского кода или ошибка четности (RT_ERROR_MASK).
*/
typedef struct tagMKIO_RT_STATE_WORD
{
  union
  {
    struct
    {
      unsigned short uNumData     : 5;
      unsigned short uSubAddr     : 5;
      unsigned short uDirect      : 1;
      unsigned short uBP          : 1;
      unsigned short uDeviceError : 1;
      unsigned short uUnused1     : 1;
      unsigned short uMessageError: 1;
      unsigned short uUnused2     : 1;
    } Bf;
    WORD uWord;
  };
} RT_STATEWORD;


/*
                   Формат слова состояния сообщения (МТ).


     15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    ¦BUS¦ME ¦      SCC      ¦K1 ¦K2 ¦ X ¦ X ¦G1 ¦G2 ¦IB ¦    ERR    ¦
    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
ПРИМЕЧАНИЕ: слово состояния сообщения МТ возвращается функцией mtgetsw для текущей выбранной базы, а также передается в качестве параметра прерывания mtIntX для последнего принятого сообщения (слово состояния, передаваемое при останове монитора, в дальнейшем невозможно получить функцией mtgetsw).

     BUS - бит номера ЛПИ, по которой принято сообщение (SX_BUS_MASK);
     ME - бит интегрированного признака ошибки в сообщении (SX_ME_MASK);
     SCC - поле кода формата сообщения, при его сдвиге на 10 разрядов вправо совпадает с кодом управления КК (SX_SCC_MASK);
     K1 - бит признака ошибки в первом КС (SX_K1_MASK);
     K2 - бит признака ошибки во втором КС (SX_K2_MASK);
     G1 - бит признака обнаружения генерации в основной ЛПИ (SX_G1_MASK);
     G2 - бит признака обнаружения генерации в резервной ЛПИ (SX_G2_MASK);
     IB - бит признака установленных битов в поле флагов любого ОС (SX_IB_MASK);
     ERR - поле кода ошибки сообщения (SX_ERR_MASK).
*/
typedef struct tagMKIO_MT_STATE_WORD
{
  union
  {
    struct
    {
      unsigned short uError       : 3;
      unsigned short uAwBitError  : 1;
      unsigned short uGenLine1    : 1;
      unsigned short uGenLine2    : 1;
      unsigned short uUnuse       : 2;
      unsigned short uK2Error     : 1;
      unsigned short uK1Error     : 1;
      unsigned short uCommand     : 4;
      unsigned short uIntgError   : 1;
      unsigned short uBus         : 1;
    } StdBf;
    struct
    {
      unsigned short uUnuse       : 10;
      unsigned short uSize        :  6;
    } McoBf;
    WORD uWord;
  };
  WORD uInt;
} MT_STATEWORD;




typedef struct tagMKIO_MT_MSGINFO
{
  int          iType;   // 0- получено из MkioUsb_Mt_GetMessageData
                        // 1- получено из MT_МСОFRAME
  DWORD        dwTime;
  MT_STATEWORD Sw   ;   // Слово статуса
  BC_CMDWORD   Cw   ;   // командное слово
  BC_ANSWORD   Aw   ;   // Отв.слово
  BOOL         bAw  ;   // Валидность ОС
  int          iFmt ;   // = -1 - нераспознанный формат
                        // =  1 - Формат 1 КК->ОУ
                        // =  2 - Формат 2 КК<-ОУ и т.д.
  BOOL         bMcoError; // Признак наличия ошибки( неполный кадр ) в кадре МСО                      
  WORD         nData;
  WORD         uData[32];
} MT_MSGINFO;




typedef struct tagMKIO_LOADER_FILE
{
  WORD uProcessor   ;
  BOOL bRunProcessor;
  char pszName[1024];
} MKIO_LOADER_FILE;


typedef struct tagMKIO_LOADER_INFO
{
  WORD uTmk;
  WORD uAddress;
  WORD uNumFiles;
  MKIO_LOADER_FILE uFile[32];
	CProgressCtrl* pProgress;
  BOOL Service_bDriverOpened;
  BOOL Service_bBcCreated;
} MKIO_LOADER_INFO;




BOOL MkioUsb_Open();
void MkioUsb_Close();
int  MkioUsb_GetMaxDeviceAmount();
void MkioUsb_Bc_ErrorMessageBox( int iBcIdent, char* pExString=NULL );
BOOL MkioUsb_Bc_CreateErrorListString( int iBcIdent, char* pString, char* pExString );
int  MkioUsb_Bc_GetLastError();
int  MkioUsb_Bc_CreateDevice( int iTmkDevice );
void MkioUsb_Bc_Free( int iBcIdent );
BOOL MkioUsb_Bc_Select( int iBcIdent );
BOOL MkioUsb_Bc_SendCommand( WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData );
BOOL MkioUsb_Bc_SetCommandData( WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData );
BOOL MkioUsb_Bc_RunCommand();
BOOL MkioUsb_Bc_SelectLine( WORD uLine );
BOOL MkioUsb_Bc_RequestAnswWord( WORD uRtAdr, WORD* pAnsWord );
int  MkioUsb_Bc_FindSlaves( WORD* pAdrSlaves );
BOOL MkioUsb_Bc_SetDataAtBase( WORD uBase, WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData );
BOOL MkioUsb_Bc_LinkBases( WORD uBaseStart, WORD uBaseEnd );
BOOL MkioUsb_Bc_SendData( WORD uBase );
BOOL MkioUsb_Bc_UnlinkBases( WORD uBaseStart, WORD uBaseEnd );
BOOL MkioUsb_Bc_SendRequest( WORD uBase, WORD uRtAddress, WORD uRtSubAddress, int nData, WORD* pData );
BOOL MkioUsb_Bc_GetLastAnsWord( int iBcIdent, WORD* pAnsWord );
BOOL MkioUsb_Bc_CreateErrorListString( int iBcIdent, char* pString, char* pExString=NULL );
WORD MkioUsb_Bc_SetAwIgnoreErrorMask( int iBcIdent, WORD uIgnoreAwErrorMask );
BOOL MkioUsb_Bc_SendControlCommand( WORD uCommandID, WORD uRtAdr, WORD* pAnsWord );
BOOL MkioUsb_Bc_SelectBoardProcessor( int iBcIdent, WORD uAddress, WORD uProcessor );
BOOL MkioUsb_Bc_RequestUnlinkBases();
BOOL MkioUsb_Bc_RequestLinkBases();
BOOL MkioUsb_Rt_Test();



BOOL MkioUsb_Rt_CreateDevice( int iTmkDevice, WORD uRtAddress );
BOOL MkioUsb_Rt_Select( int iRtIdent );
BOOL MkioUsb_Rt_SetRequestData( WORD uSubAddress, WORD nData, WORD* pData  );
void MkioUsb_Rt_Free( int iRtIdent );
BOOL MkioUsb_Rt_WaitEvent( int iRtIdent );
BOOL MkioUsb_Rt_Test( WORD uTmk, WORD uSubAddr );
BOOL MkioUsb_Rt_GetLastIoState( int iRtIdent, WORD* pState );
BOOL MkioUsb_Rt_IsUpdate( int iRtIdent, WORD uDirect, WORD uSubAddr, WORD* pNumData=NULL );
BOOL MkioUsb_Rt_Read( int iRtIdent, WORD uSubAddr, WORD numData, WORD* pData, BOOL bFlagOff=TRUE );
BOOL MkioUsb_Rt_Write( int iRtIdent, WORD uSubAddr, WORD numData, WORD* pData, BOOL bFlagSet=TRUE );
BOOL MkioUsb_Rt_CreateErrorListString( int iRtIdent, char* pString );



int  MkioUsb_Mt_CreateDevice( int iTmkDevice );
void MkioUsb_Mt_Free( int iMtIdent );
BOOL MkioUsb_Mt_LinkBases( WORD uBaseStart, WORD uBaseEnd );
BOOL MkioUsb_Mt_UnlinkBases( WORD uBaseStart );
BOOL MkioUsb_Mt_Select( int iMtIdent );
BOOL MkioUsb_Mt_SetMode( int uNumBases );
BOOL MkioUsb_Mt_Run();
BOOL MkioUsb_Mt_Stop();
BOOL MkioUsb_Mt_WaitEvent( int iMtIdent );
BOOL MkioUsb_Mt_GetEventInfo( int iMtIdent, WORD* pIntType, WORD* pStatus, WORD* pBase );
BOOL MkioUsb_Mt_GetMessageData( int iMtIdent, WORD uBase, MT_MSGINFO* pMs );

BOOL MkioUsb_Mt_McoOpen( int iMtIdent, WORD nSize );
BOOL MkioUsb_Mt_McoRun( int iMtIdent, DWORD* pNumFrames );
WORD MkioUsb_Mt_McoGetNumFrames( int iMtIdent );
//OL MkioUsb_Mt_McoGetFirstFrame(  int iMtIdent, MT_MCOFRAME* pFrame );
//OL MkioUsb_Mt_McoGetNextFrame(  int iMtIdent, MT_MCOFRAME* pFrame );
int  MkioUsb_Mt_McoReadFrames( int iMtIdent, MT_MSGINFO* pFrameArr[], int nMaxFrames );
BOOL MkioUsb_Mt_McoStop( int iMtIdent );




BOOL MkioUsb_RunLoad_Adsp2181( MKIO_LOADER_INFO* pLoadInfo );
BOOL MkioUsb_RunLoad_Adsp2181( MKIO_LOADER_INFO* pLoadInfo );


BOOL    MkioUsb_Redan_Open( int iTmkDevice );
BOOL    MkioUsb_Redan_Close();
void    MkioUsb_Redan_GetReceivedInfo( WORD Len, WORD volatile *pInfo );
BOOL    MkioUsb_SetMessageMode( BOOL bMessageMode );
void    MkioUsb_Redan_GetReceivedInfoEx( DWORD nStartWord, DWORD nWords, PWORD pDest );
__int16 MkioUsb_Redan_GetChannelStatus( DWORD *pStatusWord );
__int16 MkioUsb_Redan_AbortReceive();
void    MkioUsb_Redan_ReceiverClean();
__int16 MkioUsb_Redan_SetAskOneProcessor();
__int16 MkioUsb_Redan_SetAskAnyProcessor();
int     MkioUsb_Redan_GetNumReceivedIrq();
BOOL    MkioUsb_Redan_IrqEnable();
BOOL    MkioUsb_Redan_IrqDisable();
__int16 MkioUsb_Redan_SetReceiveMode( WORD uNumBlocks, WORD uSizeBlock );
int     MkioUsb_Redan_SetReceiveModeEx( WORD uNumBlocks, WORD uSizeBlock );
__int16 MkioUsb_Redan_SendCommandSequence( WORD uCmdID, WORD uCmdLen, WORD* pCmdData );
__int16 MkioUsb_Redan_SetProcessor( WORD uProcessor  );
BOOL    MkioUsb_Redan_SetAddress( WORD uBoardAddress  );
__int16 MkioUsb_VJ2_SetAddress( WORD uAddress );
__int16 MkioUsb_VJ2_SetSubAddress( WORD uSubAddress );
BOOL    MkioUsb_VJ2_SendCommand( WORD wLenSeq, WORD* pInfo );
__int16 MkioUsb_Manch_WriteWord( WORD uData );
BOOL    MkioUsb_Redan_IsDataReady( BOOL bMode );
BOOL    MkioUsb_ResetAbonent( WORD uRTCurrentAddress, WORD uRTAfterResetAddress=0x11 );
BOOL    MkioUsb_ResetProcessor( WORD uProcessor, WORD uRTCurrentAddress, WORD uRTAfterResetAddress=0x11  );

// Grek >> (31.05.2011)
#ifdef GREK_INC
/*
BOOL MkioUsb_Bc_WaitEvent( int iBcIdent, WORD uDirect );
BOOL MkioUsb_Bc_SetRequestMode( WORD uAddress, WORD uSubAddress, WORD uSizeBlock );
BOOL MkioUsb_Bc_SendBaseChain(int iBcIdent, WORD uBaseStart, WORD numBases);
*/
BOOL MkioUsb_Redan_SetSubAddress(WORD uBoardSubAddress);
BOOL MkioUsb_LoadProc_Ex(WORD wAddr, const BYTE* pbtBuf, long lszBuf);
BOOL MkioUsb_LoadProcTransit_Ex(WORD wAddr, WORD wProc, const BYTE* pbtBuf, long lszBuf);
#endif
// Grek <<

#endif // !defined(__INCLUDED_MKIOUSBLIBELCUS_H__)