/*****************************************************************************/
/*    WDMTMK API v4.00 for Microsoft Windows 98/2000/XP. ELCUS, 1998, 2008.  */
/*    Interface to driver TMK1553B.SYS v4.00 for Windows 98/ME/2000/XP       */
/*    Interface to driver USB_TA_DRV.dll v1.7 for Windows 98/ME/2000/XP      */ 
/*****************************************************************************/
#include "stdafx.h"

#include <cstdio>
#include "MkioUsbElcusDrv_v3.h"

#ifdef GREK_INC
#define sprintf sprintf_s
#define ReadHuge Read
#endif

//#ifndef _TMK1553B_
//#define _TMK1553B_
#define TMK_DATA unsigned short

#define tmkError *ptmkError

#ifndef _TMK1553B_MRT
#define _TMK1553B_MRT
#endif

#ifndef MAX_USBTA_NUM
#define MAX_USBTA_NUM (8-1)
#endif

#define TMK_VERSION_MIN 0x0400 /* v4.00 */
#define TMK_VERSION 0x0400     /* v4.00 */

#define EZUSB_MAJOR_VERSION_MIN 02
#define EZUSB_MINOR_VERSION_MIN 01

#ifdef _TMK1553B_MRT
#define MAX_TMK_NUMBER (8+64-1)
#else
#define MAX_TMK_NUMBER (8-1)
#endif //def TMK1553B_MRT

#define MIN_TMK_TYPE 2
#define MAX_TMK_TYPE 12

#define TMK400 2
#define TMKMPC 3
#define RTMK400 4
#define TMKX 5
#define TMKXI 6
#define MRTX 7
#define MRTXI 8
#define TA 9
#define TAI 10
#define MRTA 7
#define MRTAI 8

#define ALL_TMKS 0x00FF

#define GET_TIMEOUT 0xFFFF

#define SWTIMER_OFF   0x0000
#define SWTIMER_ON    0x0400
#define SWTIMER_EVENT 0x8000
#define SWTIMER_RESET 0xFBFF

#define GET_SWTIMER_CTRL 0xFFFF

#define TIMER_RESET 0xFBFF
#define TIMER_OFF   0x0000
#define TIMER_16BIT 0x1400
#define TIMER_32BIT 0x0400
#define TIMER_1US   0x0000
#define TIMER_2US   0x0080
#define TIMER_4US   0x0100
#define TIMER_8US   0x0180
#define TIMER_16US  0x0200
#define TIMER_32US  0x0280
#define TIMER_64US  0x0300
#define TIMER_STOP  0x0380
#define TIMER_SYN   0x0040
#define TIMER_SYND  0x0020
#define TIMER_SA    0x001F

#define TIMER_MASK  0x17FF
#define TIMER_STEP  0x0380
#define TIMER_BITS  0x1400

#define GET_TIMER_CTRL 0xFFFF

#define DATA_BC_RT 0x00
#define DATA_BC_RT_BRCST 0x08
#define DATA_RT_BC 0x01
#define DATA_RT_RT 0x02
#define DATA_RT_RT_BRCST 0x0A
#define CTRL_C_A 0x03
#define CTRL_C_BRCST 0x0B
#define CTRL_CD_A 0x04
#define CTRL_CD_BRCST 0x0C
#define CTRL_C_AD 0x05

#define BUS_A 0
#define BUS_B 1
#define BUS_1 0
#define BUS_2 1

#define S_ERAO_MASK 0x01
#define S_MEO_MASK 0x02
#define S_IB_MASK 0x04
#define S_TO_MASK 0x08
#define S_EM_MASK 0x10
#define S_EBC_MASK 0x20
#define S_DI_MASK 0x40
#define S_ELN_MASK 0x80
#define S_G1_MASK 0x1000
#define S_G2_MASK 0x2000

#define NWORDS_MASK 0x001F
#define CMD_MASK 0x041F
#define SUBADDR_MASK 0x03E0
#define CI_MASK 0x03E0
#define HBIT_MASK 0x0200
#define RT_DIR_MASK 0x0400
#define ADDRESS_MASK 0xF800
#define RTFL_MASK 0x0001
#define DNBA_MASK 0x0002
#define SSFL_MASK 0x0004
#define BUSY_MASK 0x0008
#define BRCST_MASK 0x0010
#define NULL_MASK 0x00E0
#define SREQ_MASK 0x0100
#define ERROR_MASK 0x0400

#define SREQ 0x01
#define BUSY 0x02
#define SSFL 0x04
#define RTFL 0x08
#define DNBA 0x10

#define CWB0 0x20
#define CWB1 0x40

#define BC_MODE 0x00
#define RT_MODE 0x80
#define MT_MODE 0x100
#define MRT_MODE 0x280
#define UNDEFINED_MODE 0xFFFF

#define RT_TRANSMIT 0x0400
#define RT_RECEIVE 0x0000

#define RT_ERROR_MASK 0x4000

#define RT_FLAG 0x8000
#define RT_FLAG_MASK 0x8000

#define RT_HBIT_MODE 0x0001
#define RT_FLAG_MODE 0x0002
#define RT_BRCST_MODE 0x0004
#define RT_DATA_BL 0x2000
#define RT_GENER1_BL 0x0004
#define RT_GENER2_BL 0x4000
#define BC_GENER1_BL 0x0004
#define BC_GENER2_BL 0x4000
#define MT_GENER1_BL 0x0004
#define MT_GENER2_BL 0x4000
#define TMK_IRQ_OFF 0x8000

#define CX_CC_MASK 0x000F
#define CX_CONT_MASK 0x0010
#define CX_BUS_MASK 0x0020
#define CX_SIG_MASK 0x8000
#define CX_INT_MASK 0x0020

#define CX_CONT 0x0010
#define CX_STOP 0x0000
#define CX_BUS_0 0x0000
#define CX_BUS_A 0x0000
#define CX_BUS_1 0x0020
#define CX_BUS_B 0x0020
#define CX_NOSIG 0x0000
#define CX_SIG 0x8000
#define CX_INT 0x0000
#define CX_NOINT 0x0020

#define SX_NOERR 0
#define SX_MEO 1
#define SX_TOA 2
#define SX_TOD 3
#define SX_ELN 4
#define SX_ERAO 5
#define SX_ESYN 6
#define SX_EBC 7

#define SX_ERR_MASK 0x0007
#define SX_IB_MASK 0x0008
#define SX_G1_MASK 0x0010
#define SX_G2_MASK 0x0020
#define SX_K2_MASK 0x0100
#define SX_K1_MASK 0x0200
#define SX_SCC_MASK 0x3C00
#define SX_ME_MASK 0x4000
#define SX_BUS_MASK 0x8000

#define SX_BUS_0 0x0000
#define SX_BUS_A 0x0000
#define SX_BUS_1 0x8000
#define SX_BUS_B 0x8000

#define GET_IO_DELAY 0xFFFF

#define RT_ENABLE 0x0000
#define RT_DISABLE 0x001F
#define RT_GET_ENABLE 0xFFFF

#define CW(ADDR,DIR,SUBADDR,NWORDS) ((TMK_DATA)(((ADDR)<<11)|(DIR)|((SUBADDR)<<5)|((NWORDS)&0x1F)))
#define CWM(ADDR,COMMAND) ((TMK_DATA)(((ADDR)<<11)|(CI_MASK)|(COMMAND)))
#define CWMC(ADDR,CI,COMMAND) ((TMK_DATA)(((ADDR)<<11)|((CI)&0x03E0)|(COMMAND)))

/*#define CMD_ILLEGAL 0x000*/
#define CMD_DYNAMIC_BUS_CONTROL 0x400
#define CMD_SYNCHRONIZE 0x401
#define CMD_TRANSMIT_STATUS_WORD 0x402
#define CMD_INITIATE_SELF_TEST 0x403
#define CMD_TRANSMITTER_SHUTDOWN 0x404
#define CMD_OVERRIDE_TRANSMITTER_SHUTDOWN 0x405
#define CMD_INHIBIT_TERMINAL_FLAG_BIT 0x406
#define CMD_OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT 0x407
#define CMD_RESET_REMOTE_TERMINAL 0x408
#define CMD_TRANSMIT_VECTOR_WORD 0x410
#define CMD_SYNCHRONIZE_WITH_DATA_WORD 0x011
#define CMD_TRANSMIT_LAST_COMMAND_WORD 0x412
#define CMD_TRANSMIT_BUILT_IN_TEST_WORD 0x413

#define TMK_ERROR_0      0xE0040000
#define TMK_BAD_TYPE     (1 + TMK_ERROR_0)
#define TMK_BAD_IRQ      (2 + TMK_ERROR_0)
#define TMK_BAD_NUMBER   (3 + TMK_ERROR_0)
#define BC_BAD_BUS       (4 + TMK_ERROR_0)
#define BC_BAD_BASE      (5 + TMK_ERROR_0)
#define BC_BAD_LEN       (6 + TMK_ERROR_0)
#define RT_BAD_PAGE      (7 + TMK_ERROR_0)
#define RT_BAD_LEN       (8 + TMK_ERROR_0)
#define RT_BAD_ADDRESS   (9 + TMK_ERROR_0)
#define RT_BAD_FUNC      (10 + TMK_ERROR_0)
#define BC_BAD_FUNC      (11 + TMK_ERROR_0)
#define TMK_BAD_FUNC     (12 + TMK_ERROR_0)
#define VTMK_BAD_VERSION (13 + TMK_ERROR_0)

#define VTMK_tmkconfig 2
#define VTMK_tmkdone 3
#define VTMK_tmkgetmaxn 4
#define VTMK_tmkselect 5
#define VTMK_tmkselected 6
#define VTMK_tmkgetmode 7
#define VTMK_tmksetcwbits 8
#define VTMK_tmkclrcwbits 9
#define VTMK_tmkgetcwbits 10
#define VTMK_tmkdefevent 11
#define VTMK_tmkgetevd 12

#define VTMK_bcreset 13
#define VTMK_bc_def_tldw 14
#define VTMK_bc_enable_di 15
#define VTMK_bc_disable_di 16
#define VTMK_bcdefirqmode 17
#define VTMK_bcgetirqmode 18
#define VTMK_bcgetmaxbase 19
#define VTMK_bcdefbase 20
#define VTMK_bcgetbase 21
#define VTMK_bcputw 22
#define VTMK_bcgetw 23
#define VTMK_bcgetansw 24
#define VTMK_bcputblk 25
#define VTMK_bcgetblk 26
#define VTMK_bcdefbus 27
#define VTMK_bcgetbus 28
#define VTMK_bcstart 29
#define VTMK_bcstartx 30
#define VTMK_bcdeflink 31
#define VTMK_bcgetlink 32
#define VTMK_bcstop 33
#define VTMK_bcgetstate 34

#define VTMK_rtreset 35
#define VTMK_rtdefirqmode 36
#define VTMK_rtgetirqmode 37
#define VTMK_rtdefmode 38
#define VTMK_rtgetmode 39
#define VTMK_rtgetmaxpage 40
#define VTMK_rtdefpage 41
#define VTMK_rtgetpage 42
#define VTMK_rtdefpagepc 43
#define VTMK_rtdefpagebus 44
#define VTMK_rtgetpagepc 45
#define VTMK_rtgetpagebus 46
#define VTMK_rtdefaddress 47
#define VTMK_rtgetaddress 48
#define VTMK_rtdefsubaddr 49
#define VTMK_rtgetsubaddr 50
#define VTMK_rtputw 51
#define VTMK_rtgetw 52
#define VTMK_rtputblk 53
#define VTMK_rtgetblk 54
#define VTMK_rtsetanswbits 55
#define VTMK_rtclranswbits 56
#define VTMK_rtgetanswbits 57
#define VTMK_rtgetflags 58
#define VTMK_rtputflags 59
#define VTMK_rtsetflag 60
#define VTMK_rtclrflag 61
#define VTMK_rtgetflag 62
#define VTMK_rtgetstate 63
#define VTMK_rtbusy 64
#define VTMK_rtlock 65
#define VTMK_rtunlock 66
#define VTMK_rtgetcmddata 67
#define VTMK_rtputcmddata 68

#define VTMK_mtreset 69
#define VTMK_mtdefirqmode 70
#define VTMK_mtgetirqmode 71
#define VTMK_mtgetmaxbase 72
#define VTMK_mtdefbase 73
#define VTMK_mtgetbase 74
#define VTMK_mtputw 75
#define VTMK_mtgetw 76
#define VTMK_mtgetsw 77
#define VTMK_mtputblk 78
#define VTMK_mtgetblk 79
#define VTMK_mtstartx 80
#define VTMK_mtdeflink 81
#define VTMK_mtgetlink 82
#define VTMK_mtstop 83
#define VTMK_mtgetstate 84

#define VTMK_tmkgetinfo 85
#define VTMK_GetVersion 86

#define VTMK_rtenable 87

#define VTMK_mrtgetmaxn 88
#define VTMK_mrtconfig 89
#define VTMK_mrtselected 90
#define VTMK_mrtgetstate 91
#define VTMK_mrtdefbrcsubaddr0 92
#define VTMK_mrtreset 93

#define VTMK_tmktimer 94
#define VTMK_tmkgettimer 95
#define VTMK_tmkgettimerl 96
#define VTMK_bcgetmsgtime 97
#define VTMK_mtgetmsgtime 98
#define VTMK_rtgetmsgtime 99

#define VTMK_tmkgethwver 100
 
#define VTMK_tmkgetevtime 101
#define VTMK_tmkswtimer 102
#define VTMK_tmkgetswtimer 103

#define VTMK_tmktimeout 104

#define VTMK_mrtdefbrcpage 105
#define VTMK_mrtgetbrcpage 106

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)


#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3
#define FILE_ANY_ACCESS                 0
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe
#define TMK_KRNLDRVR 0x8000
#define IOCTL_TMK_tmkconfig CTL_CODE(TMK_KRNLDRVR, VTMK_tmkconfig+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkdone CTL_CODE(TMK_KRNLDRVR, VTMK_tmkdone+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgetmaxn CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetmaxn+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkselect CTL_CODE(TMK_KRNLDRVR, VTMK_tmkselect+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkselected CTL_CODE(TMK_KRNLDRVR, VTMK_tmkselected+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgetmode CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmksetcwbits CTL_CODE(TMK_KRNLDRVR, VTMK_tmksetcwbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkclrcwbits CTL_CODE(TMK_KRNLDRVR, VTMK_tmkclrcwbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgetcwbits CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetcwbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkdefevent CTL_CODE(TMK_KRNLDRVR, VTMK_tmkdefevent+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgetevd CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetevd+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcreset CTL_CODE(TMK_KRNLDRVR, VTMK_bcreset+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bc_def_tldw CTL_CODE(TMK_KRNLDRVR, VTMK_bc_def_tldw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bc_enable_di CTL_CODE(TMK_KRNLDRVR, VTMK_bc_enable_di+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bc_disable_di CTL_CODE(TMK_KRNLDRVR, VTMK_bc_disable_di+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcdefirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_bcdefirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetmaxbase CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetmaxbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcdefbase CTL_CODE(TMK_KRNLDRVR, VTMK_bcdefbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetbase CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcputw CTL_CODE(TMK_KRNLDRVR, VTMK_bcputw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetw CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetansw CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetansw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcputblk CTL_CODE(TMK_KRNLDRVR, VTMK_bcputblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetblk CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcdefbus CTL_CODE(TMK_KRNLDRVR, VTMK_bcdefbus+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetbus CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetbus+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcstart CTL_CODE(TMK_KRNLDRVR, VTMK_bcstart+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcstartx CTL_CODE(TMK_KRNLDRVR, VTMK_bcstartx+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcdeflink CTL_CODE(TMK_KRNLDRVR, VTMK_bcdeflink+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetlink CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetlink+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcstop CTL_CODE(TMK_KRNLDRVR, VTMK_bcstop+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetstate CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetstate+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_rtreset CTL_CODE(TMK_KRNLDRVR, VTMK_rtreset+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefmode CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetmode CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetmaxpage CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetmaxpage+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefpage CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefpage+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetpage CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetpage+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefpagepc CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefpagepc+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefpagebus CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefpagebus+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetpagepc CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetpagepc+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetpagebus CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetpagebus+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefaddress CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefaddress+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetaddress CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetaddress+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtdefsubaddr CTL_CODE(TMK_KRNLDRVR, VTMK_rtdefsubaddr+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetsubaddr CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetsubaddr+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtputw CTL_CODE(TMK_KRNLDRVR, VTMK_rtputw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetw CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtputblk CTL_CODE(TMK_KRNLDRVR, VTMK_rtputblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetblk CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtsetanswbits CTL_CODE(TMK_KRNLDRVR, VTMK_rtsetanswbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtclranswbits CTL_CODE(TMK_KRNLDRVR, VTMK_rtclranswbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetanswbits CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetanswbits+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetflags CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetflags+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtputflags CTL_CODE(TMK_KRNLDRVR, VTMK_rtputflags+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtsetflag CTL_CODE(TMK_KRNLDRVR, VTMK_rtsetflag+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtclrflag CTL_CODE(TMK_KRNLDRVR, VTMK_rtclrflag+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetflag CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetflag+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetstate CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetstate+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtbusy CTL_CODE(TMK_KRNLDRVR, VTMK_rtbusy+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtlock CTL_CODE(TMK_KRNLDRVR, VTMK_rtlock+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtunlock CTL_CODE(TMK_KRNLDRVR, VTMK_rtunlock+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetcmddata CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetcmddata+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtputcmddata CTL_CODE(TMK_KRNLDRVR, VTMK_rtputcmddata+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_mtreset CTL_CODE(TMK_KRNLDRVR, VTMK_mtreset+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtdefirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_mtdefirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetirqmode CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetirqmode+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetmaxbase CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetmaxbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtdefbase CTL_CODE(TMK_KRNLDRVR, VTMK_mtdefbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetbase CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetbase+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtputw CTL_CODE(TMK_KRNLDRVR, VTMK_mtputw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetw CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetsw CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetsw+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtputblk CTL_CODE(TMK_KRNLDRVR, VTMK_mtputblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetblk CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetblk+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtstartx CTL_CODE(TMK_KRNLDRVR, VTMK_mtstartx+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtdeflink CTL_CODE(TMK_KRNLDRVR, VTMK_mtdeflink+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetlink CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetlink+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtstop CTL_CODE(TMK_KRNLDRVR, VTMK_mtstop+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetstate CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetstate+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_tmkgetinfo CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetinfo+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_GetVersion CTL_CODE(TMK_KRNLDRVR, VTMK_GetVersion+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_rtenable              CTL_CODE(TMK_KRNLDRVR, VTMK_rtenable+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_mrtgetmaxn    CTL_CODE(TMK_KRNLDRVR, VTMK_mrtgetmaxn+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtconfig             CTL_CODE(TMK_KRNLDRVR, VTMK_mrtconfig+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtselected   CTL_CODE(TMK_KRNLDRVR, VTMK_mrtselected+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtgetstate   CTL_CODE(TMK_KRNLDRVR, VTMK_mrtgetstate+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtdefbrcsubaddr0 CTL_CODE(TMK_KRNLDRVR, VTMK_mrtdefbrcsubaddr0+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtreset              CTL_CODE(TMK_KRNLDRVR, VTMK_mrtreset+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_tmktimer CTL_CODE(TMK_KRNLDRVR, VTMK_tmktimer+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgettimer CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgettimer+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgettimerl CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgettimerl+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_bcgetmsgtime CTL_CODE(TMK_KRNLDRVR, VTMK_bcgetmsgtime+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mtgetmsgtime CTL_CODE(TMK_KRNLDRVR, VTMK_mtgetmsgtime+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_rtgetmsgtime CTL_CODE(TMK_KRNLDRVR, VTMK_rtgetmsgtime+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_tmkgethwver CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgethwver+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_tmkgetevtime CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetevtime+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkswtimer CTL_CODE(TMK_KRNLDRVR, VTMK_tmkswtimer+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_tmkgetswtimer CTL_CODE(TMK_KRNLDRVR, VTMK_tmkgetswtimer+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_tmktimeout CTL_CODE(TMK_KRNLDRVR, VTMK_tmktimeout+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_TMK_mrtdefbrcpage CTL_CODE(TMK_KRNLDRVR, VTMK_mrtdefbrcpage+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TMK_mrtgetbrcpage CTL_CODE(TMK_KRNLDRVR, VTMK_mrtgetbrcpage+0x800, METHOD_NEITHER, FILE_ANY_ACCESS)


#define Ezusb_IOCTL_INDEX  0x0800
#define FILE_DEVICE_UNKNOWN  0x00000022

#define IOCTL_EZUSB_GET_DRIVER_VERSION   CTL_CODE(FILE_DEVICE_UNKNOWN, Ezusb_IOCTL_INDEX+29, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EZUSB_SET_CURPROC         CTL_CODE(FILE_DEVICE_UNKNOWN, Ezusb_IOCTL_INDEX+34, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _EZUSB_DRIVER_VERSION
{
   WORD     MajorVersion;
   WORD     MinorVersion;
   WORD     BuildVersion;
} EZUSB_DRIVER_VERSION, *PEZUSB_DRIVER_VERSION;

typedef struct
{
  short nType;
  char szName[10];
  unsigned short wPorts1;
  unsigned short wPorts2;
  unsigned short wIrq1;
  unsigned short wIrq2;
  unsigned short wIODelay;
} TTmkConfigData;

typedef struct
{
  int nInt;
  unsigned short wMode;
  union
  {
    struct
    {
      unsigned short wResult;
      unsigned short wAW1;
      unsigned short wAW2;
    } bc;
    struct
    {
      unsigned short wBase;
      unsigned short wResultX;
    } bcx;
    struct
    {
      unsigned short wStatus;
      unsigned short wCmd;
    } rt;
    struct
    {
      unsigned short wBase;
      unsigned short wResultX;
    } mt;
    struct
    {
      unsigned short wStatus;
    } mrt;
  };
} TTmkEventData;

DWORD TmkOpen();
void TmkClose();
void tmkgetinfo(TTmkConfigData *pConfD);
void tmkgetevd( TTmkEventData *pEvD );
void tmkdefevent( HANDLE hEvent, BOOL fEventSet );
int tmkgetmaxn();
int tmkconfig( int tmkNumber );
int tmkdone( int tmkNumber );
int tmkselect( int tmkNumber );
int tmkselected();
TMK_DATA tmkgetmode();
int tmkdefdac(int dacValue);
int tmkgetdac(int *dacValue, int *dacMode);
void tmksetcwbits( TMK_DATA tmkSetControl );
void tmkclrcwbits( TMK_DATA tmkClrControl );
TMK_DATA tmkgetcwbits();
int bcreset();
void bc_def_tldw( TMK_DATA wTLDW );
void bc_enable_di();
void bc_disable_di();
int bcdefirqmode( TMK_DATA bcIrqMode );
TMK_DATA bcgetirqmode();
TMK_DATA bcgetmaxbase();
int bcdefbase( TMK_DATA bcBasePC );
TMK_DATA bcgetbase();
void bcputw( TMK_DATA bcAddr, TMK_DATA bcData );
TMK_DATA bcgetw( TMK_DATA bcAddr );
DWORD bcgetansw( TMK_DATA bcCtrlCode );
void bcputblk( TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength );
void bcgetblk( TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength );
int bcdefbus( TMK_DATA bcBus );
TMK_DATA bcgetbus();
int bcstart( TMK_DATA bcBase, TMK_DATA bcCtrlCode );
int bcstartx( TMK_DATA bcBase, TMK_DATA bcCtrlCode );
int bcdeflink( TMK_DATA bcBase, TMK_DATA bcCtrlCode );
DWORD bcgetlink();
TMK_DATA bcstop();
DWORD bcgetstate();
int rtreset();
int rtdefirqmode( TMK_DATA rtIrqMode );
TMK_DATA rtgetirqmode();
int rtdefmode( TMK_DATA rtMode );
TMK_DATA rtgetmode();
TMK_DATA rtgetmaxpage();
int rtdefpage( TMK_DATA rtPage );
TMK_DATA rtgetpage();
int rtdefpagepc( TMK_DATA rtPagePC );
int rtdefpagebus( TMK_DATA rtPageBus );
TMK_DATA rtgetpagepc();
TMK_DATA rtgetpagebus();
int rtdefaddress( TMK_DATA rtAddress );
TMK_DATA rtgetaddress();
void rtdefsubaddr( TMK_DATA rtDir, TMK_DATA rtSubAddr);
TMK_DATA rtgetsubaddr();
void rtputw( TMK_DATA rtAddr, TMK_DATA rtData );
TMK_DATA rtgetw( TMK_DATA rtAddr );
void rtputblk( TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength );
void rtgetblk( TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength );
void rtsetanswbits( TMK_DATA rtSetControl );
void rtclranswbits( TMK_DATA rtClrControl );
TMK_DATA rtgetanswbits();
void rtgetflags( void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax );
void rtputflags( void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax );
void rtsetflag();
void rtclrflag();
TMK_DATA rtgetflag( TMK_DATA rtDir, TMK_DATA rtSubAddr );
TMK_DATA rtgetstate();
TMK_DATA rtbusy();
void rtlock( TMK_DATA rtDir, TMK_DATA rtSubAddr );
void rtunlock();
TMK_DATA rtgetcmddata( TMK_DATA rtBusCommand );
void rtputcmddata( TMK_DATA rtBusCommand, TMK_DATA rtData );
int mtreset();
TMK_DATA mtgetsw();
#define mtdefirqmode bcdefirqmode
#define mtgetirqmode bcgetirqmode
#define mtgetmaxbase bcgetmaxbase
#define mtdefbase bcdefbase
#define mtgetbase bcgetbase
#define mtputw bcputw
#define mtgetw bcgetw
#define mtputblk bcputblk
#define mtgetblk bcgetblk
#define mtstartx bcstartx
#define mtdeflink bcdeflink
#define mtgetlink bcgetlink
#define mtstop bcstop
#define mtgetstate bcgetstate

TMK_DATA rtenable(TMK_DATA rtEnable);

int mrtgetmaxn();
DWORD mrtconfig(int tmkNumber);
int mrtselected();
TMK_DATA mrtgetstate();
void mrtdefbrcsubaddr0();
int mrtreset();

TMK_DATA tmktimer(TMK_DATA tmkTimerCtrl);
DWORD tmkgettimer();
TMK_DATA tmkgettimerl();
DWORD bcgetmsgtime();
#define mtgetmsgtime bcgetmsgtime
DWORD rtgetmsgtime();
TMK_DATA tmkgethwver();

DWORD tmkgetevtime();
TMK_DATA tmkswtimer(TMK_DATA tmkTimerCtrl);
DWORD tmkgetswtimer();

TMK_DATA tmktimeout(TMK_DATA tmkTimeOut);

int mrtdefbrcpage(TMK_DATA mrtBrcPage);
TMK_DATA mrtgetbrcpage();

DWORD MonitorHwStart(DWORD dwBufSize);
DWORD MonitorHwGetMessage(WORD * Data, DWORD dwBufSize, BOOL FillFlag, DWORD * dwMsWritten);
DWORD MonitorHwStop();
//#endif
typedef struct
{
  int nInt;
  unsigned int empty;
  unsigned int wMode;
  unsigned int awEvData[3];
//  PEPROCESS hProc;
  unsigned int empty1;
  unsigned int empty2;
} TListEvD, *pTListEvD;

typedef int (__cdecl *htmkconfig_usb) (int tmkNumber, int wType, int PortsAddr1, int PortsAddr2);
typedef int (__cdecl *htmkdone_usb) (int tmkNumber);
typedef int (__cdecl *htmkselect_usb)(int tmkNumber);
typedef void (__cdecl *htmkdefevent_usb)(void *hEvent);
typedef int (__cdecl *hRead_DLL_EvD_usb)(TListEvD *evd);
typedef void (__cdecl *htmksetcwbits_usb)(TMK_DATA tmkSetControl);
typedef void (__cdecl *htmkclrcwbits_usb)(TMK_DATA tmkClrControl);
typedef TMK_DATA (__cdecl *htmkgetcwbits_usb)();
typedef TMK_DATA (__cdecl *htmkgetmode_usb)();
typedef int (__cdecl *htmkdefdac_usb)(int dacValue);
typedef int (__cdecl *htmkgetdac_usb)(int *value, int *mode);
typedef int (__cdecl *hbcreset_usb)();
typedef int (__cdecl *hbcdefirqmode_usb)(TMK_DATA bcIrqMode);
typedef TMK_DATA (__cdecl *hbcgetirqmode_usb)();
typedef TMK_DATA (__cdecl *hbcgetmaxbase_usb)();
typedef int (__cdecl *hbcdefbase_usb)(TMK_DATA bcBasePC);
typedef TMK_DATA (__cdecl *hbcgetbase_usb)();
typedef void (__cdecl *hbcputw_usb)(TMK_DATA bcAddr, TMK_DATA bcData);
typedef TMK_DATA (__cdecl *hbcgetw_usb)(TMK_DATA bcAddr);
typedef DWORD (__cdecl *hbcgetansw_usb)(TMK_DATA bcCtrlCode);
typedef void (__cdecl *hbcputblk_usb)(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength);
typedef void (__cdecl *hbcgetblk_usb)(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength);
typedef int (__cdecl *hbcdefbus_usb)(TMK_DATA bcBus);
typedef TMK_DATA (__cdecl *hbcgetbus_usb)();
typedef int (__cdecl *hbcstart_usb)(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
typedef int (__cdecl *hbcstartx_usb)(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
typedef int (__cdecl *hbcdeflink_usb)(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
typedef DWORD (__cdecl *hbcgetlink_usb)();
typedef TMK_DATA (__cdecl *hbcstop_usb)();
typedef DWORD (__cdecl *hbcgetstate_usb)();
typedef int (__cdecl *hrtreset_usb)();
typedef int (__cdecl *hrtdefirqmode_usb)(TMK_DATA rtIrqMode);
typedef TMK_DATA (__cdecl *hrtgetirqmode_usb)();
typedef int (__cdecl *hrtdefmode_usb)(TMK_DATA rtMode);
typedef TMK_DATA (__cdecl *hrtgetmode_usb)();
typedef TMK_DATA (__cdecl *hrtgetmaxpage_usb)();
typedef int (__cdecl *hrtdefpage_usb)(TMK_DATA rtPage);
typedef TMK_DATA (__cdecl *hrtgetpage_usb)();
typedef int (__cdecl *hrtdefpagepc_usb)(TMK_DATA rtPagePC);
typedef int (__cdecl *hrtdefpagebus_usb)(TMK_DATA rtPageBus);
typedef TMK_DATA (__cdecl *hrtgetpagepc_usb)();
typedef TMK_DATA (__cdecl *hrtgetpagebus_usb)();
typedef int (__cdecl *hrtdefaddress_usb)(TMK_DATA rtAddress);
typedef TMK_DATA (__cdecl *hrtgetaddress_usb)();
typedef void (__cdecl *hrtdefsubaddr_usb)(TMK_DATA rtDir, TMK_DATA rtSubAddr);
typedef TMK_DATA (__cdecl *hrtgetsubaddr_usb)();
typedef void (__cdecl *hrtputw_usb)(TMK_DATA rtAddr, TMK_DATA rtData);
typedef TMK_DATA (__cdecl *hrtgetw_usb)(TMK_DATA rtAddr);
typedef void (__cdecl *hrtputblk_usb)(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength);
typedef void (__cdecl *hrtgetblk_usb)(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength);
typedef void (__cdecl *hrtsetanswbits_usb)(TMK_DATA rtSetControl);
typedef void (__cdecl *hrtclranswbits_usb)(TMK_DATA rtClrControl);
typedef TMK_DATA (__cdecl *hrtgetanswbits_usb)();
typedef void (__cdecl *hrtgetflags_usb)(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax);
typedef void (__cdecl *hrtputflags_usb)(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax);
typedef void (__cdecl *hrtsetflag_usb)();
typedef void (__cdecl *hrtclrflag_usb)();
typedef TMK_DATA (__cdecl *hrtgetflag_usb)(TMK_DATA rtDir, TMK_DATA rtSubAddr);
typedef void (__cdecl *hrtputflag_usb)(TMK_DATA rtDir, TMK_DATA rtSubAddr, TMK_DATA rtFlag);
typedef TMK_DATA (__cdecl *hrtgetstate_usb)();
typedef TMK_DATA (__cdecl *hrtbusy_usb)();
typedef void (__cdecl *hrtlock_usb)(TMK_DATA rtDir, TMK_DATA rtSubAddr);
typedef void (__cdecl *hrtunlock_usb)();
typedef TMK_DATA (__cdecl *hrtgetcmddata_usb)(TMK_DATA rtBusCommand);
typedef void (__cdecl *hrtputcmddata_usb)(TMK_DATA rtBusCommand, TMK_DATA rtData);
typedef int (__cdecl *hmtreset_usb)();
typedef TMK_DATA (__cdecl *hmtgetsw_usb)();
typedef TMK_DATA (__cdecl *hrtenable_usb)(TMK_DATA rtEnable);
typedef int (__cdecl *hmrtselected_usb)();
typedef TMK_DATA (__cdecl *hmrtgetstate_usb)();
typedef void (__cdecl *hmrtdefbrcsubaddr0_usb)();
typedef int (__cdecl *hmrtreset_usb)();
typedef TMK_DATA (__cdecl *htmktimer_usb)(TMK_DATA TimerCtrl);
typedef DWORD (__cdecl *htmkgettimer_usb)();
typedef DWORD (__cdecl *htmkgettimerl_usb)();
typedef DWORD (__cdecl *hbcgetmsgtime_usb)();
typedef DWORD (__cdecl *hrtgetmsgtime_usb)();
typedef TMK_DATA (__cdecl *htmkgethwver_usb)();
typedef DWORD (__cdecl *htmkgetevtime_usb)();
typedef TMK_DATA (__cdecl *htmkswtimer_usb)(TMK_DATA tmkTimerCtrl);
typedef DWORD (__cdecl *htmkgetswtimer_usb)();
typedef TMK_DATA (__cdecl *htmktimeout_usb)(TMK_DATA tmkTimeOut);
typedef int (__cdecl *hmrtdefbrcpage_usb)(TMK_DATA mrtBrcPage);
typedef TMK_DATA (__cdecl *hmrtgetbrcpage_usb)();

typedef DWORD (__cdecl *hMonitorHwStart_usb)(DWORD dwBufSize, HANDLE hEvent);
typedef DWORD (__cdecl *hMonitorHwGetMessage_usb)(WORD * Data, DWORD dwBufSize, BOOL FillFlag, DWORD * dwMsWritten);
typedef DWORD (__cdecl *hMonitorHwStop_usb)(void);
	
int ftmkError = -1;

int ftmkconfig_usb (int tmkNumber, int wType, int PortsAddr1, int PortsAddr2){return -1;}
int ftmkdone_usb (int tmkNumber){return -1;}
int ftmkselect_usb(int tmkNumber){return -1;}
void ftmkdefevent_usb(void *hEvent){}
int fRead_DLL_EvD_usb(TListEvD *evd){return -1;}
void ftmksetcwbits_usb(TMK_DATA tmkSetControl){}
void ftmkclrcwbits_usb(TMK_DATA tmkClrControl){}
TMK_DATA ftmkgetcwbits_usb(){return -1;}
TMK_DATA ftmkgetmode_usb(){return -1;}
int ftmkdefdac_usb(int dacValue){return -1;}
int ftmkgetdac_usb(int *value, int *mode){return -1;}
int fbcreset_usb(){return -1;}
int fbcdefirqmode_usb(TMK_DATA bcIrqMode){return -1;}
TMK_DATA fbcgetirqmode_usb(){return -1;}
TMK_DATA fbcgetmaxbase_usb(){return -1;}
int fbcdefbase_usb(TMK_DATA bcBasePC){return -1;}
TMK_DATA fbcgetbase_usb(){return -1;}
void fbcputw_usb(TMK_DATA bcAddr, TMK_DATA bcData){}
TMK_DATA fbcgetw_usb(TMK_DATA bcAddr){return -1;}
DWORD fbcgetansw_usb(TMK_DATA bcCtrlCode){return -1;}
void fbcputblk_usb(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength){}
void fbcgetblk_usb(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength){}
int  fbcdefbus_usb(TMK_DATA bcBus){return -1;}
TMK_DATA fbcgetbus_usb(){return -1;}
int fbcstart_usb(TMK_DATA bcBase, TMK_DATA bcCtrlCode){return -1;}
int fbcstartx_usb(TMK_DATA bcBase, TMK_DATA bcCtrlCode){return -1;}
int fbcdeflink_usb(TMK_DATA bcBase, TMK_DATA bcCtrlCode){return -1;}
DWORD fbcgetlink_usb(){return -1;}
TMK_DATA fbcstop_usb(){return -1;}
DWORD fbcgetstate_usb(){return -1;}
int frtreset_usb(){return -1;}
int frtdefirqmode_usb(TMK_DATA rtIrqMode){return -1;}
TMK_DATA frtgetirqmode_usb(){return -1;}
int frtdefmode_usb(TMK_DATA rtMode){return -1;}
TMK_DATA frtgetmode_usb(){return -1;}
TMK_DATA frtgetmaxpage_usb(){return -1;}
int frtdefpage_usb(TMK_DATA rtPage){return -1;}
TMK_DATA frtgetpage_usb(){return -1;}
int frtdefpagepc_usb(TMK_DATA rtPagePC){return -1;}
int frtdefpagebus_usb(TMK_DATA rtPageBus){return -1;}
TMK_DATA frtgetpagepc_usb(){return -1;}
TMK_DATA frtgetpagebus_usb(){return -1;}
int frtdefaddress_usb(TMK_DATA rtAddress){return -1;}
TMK_DATA frtgetaddress_usb(){return -1;}
void frtdefsubaddr_usb(TMK_DATA rtDir, TMK_DATA rtSubAddr){}
TMK_DATA frtgetsubaddr_usb(){return -1;}
void frtputw_usb(TMK_DATA rtAddr, TMK_DATA rtData){}
TMK_DATA frtgetw_usb(TMK_DATA rtAddr){return -1;}
void frtputblk_usb(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength){}
void frtgetblk_usb(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength){}
void frtsetanswbits_usb(TMK_DATA rtSetControl){}
void frtclranswbits_usb(TMK_DATA rtClrControl){}
TMK_DATA frtgetanswbits_usb(){return -1;}
void frtgetflags_usb(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax){}
void frtputflags_usb(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax){}
void frtsetflag_usb(){}
void frtclrflag_usb(){}
TMK_DATA frtgetflag_usb(TMK_DATA rtDir, TMK_DATA rtSubAddr){return -1;}
void frtputflag_usb(TMK_DATA rtDir, TMK_DATA rtSubAddr, TMK_DATA rtFlag){}
TMK_DATA frtgetstate_usb(){return -1;}
TMK_DATA frtbusy_usb(){return -1;}
void frtlock_usb(TMK_DATA rtDir, TMK_DATA rtSubAddr){}
void frtunlock_usb(){}
TMK_DATA frtgetcmddata_usb(TMK_DATA rtBusCommand){return -1;}
void frtputcmddata_usb(TMK_DATA rtBusCommand, TMK_DATA rtData){}
int fmtreset_usb(){return -1;}
TMK_DATA fmtgetsw_usb(){return -1;}
TMK_DATA frtenable_usb(TMK_DATA rtEnable){return -1;}
int fmrtselected_usb(){return -1;}
TMK_DATA fmrtgetstate_usb(){return -1;}
void fmrtdefbrcsubaddr0_usb(){}
int fmrtreset_usb(){return -1;}
TMK_DATA ftmktimer_usb(TMK_DATA TimerCtrl){return -1;}
DWORD ftmkgettimer_usb(){return -1;}
DWORD ftmkgettimerl_usb(){return -1;}
DWORD fbcgetmsgtime_usb(){return -1;}
DWORD frtgetmsgtime_usb(){return -1;}
TMK_DATA ftmkgethwver_usb(){return -1;}
DWORD ftmkgetevtime_usb(){return -1;}
TMK_DATA ftmkswtimer_usb(TMK_DATA tmkTimerCtrl){return -1;}
DWORD ftmkgetswtimer_usb(){return -1;}
TMK_DATA ftmktimeout_usb(WORD tmkTimeOut){return -1;}
int fmrtdefbrcpage_usb(TMK_DATA mrtBrcPage){return -1;}
TMK_DATA fmrtgetbrcpage_usb(){return 0;}

DWORD fMonitorHwStart_usb(DWORD dwBufSize, HANDLE hEvent){return -1;}
DWORD fMonitorHwGetMessage_usb(WORD * Data, DWORD dwBufSize, BOOL FillFlag, DWORD * dwMsWritten){return -1;}
DWORD fMonitorHwStop_usb(void){return -1;}

int *ptmkError;

htmkconfig_usb tmkconfig_usb = ftmkconfig_usb;
htmkdone_usb tmkdone_usb = ftmkdone_usb;
htmkselect_usb tmkselect_usb = ftmkselect_usb;
htmkdefevent_usb tmkdefevent_usb = ftmkdefevent_usb;
hRead_DLL_EvD_usb Read_DLL_EvD_usb = fRead_DLL_EvD_usb;
htmksetcwbits_usb tmksetcwbits_usb = ftmksetcwbits_usb;
htmkclrcwbits_usb tmkclrcwbits_usb = ftmkclrcwbits_usb;
htmkgetcwbits_usb tmkgetcwbits_usb = ftmkgetcwbits_usb;
htmkgetmode_usb tmkgetmode_usb = ftmkgetmode_usb;
htmkdefdac_usb tmkdefdac_usb = ftmkdefdac_usb;
htmkgetdac_usb tmkgetdac_usb = ftmkgetdac_usb;
hbcreset_usb bcreset_usb = fbcreset_usb;
hbcdefirqmode_usb bcdefirqmode_usb = fbcdefirqmode_usb;
hbcgetirqmode_usb bcgetirqmode_usb = fbcgetirqmode_usb;
hbcgetmaxbase_usb bcgetmaxbase_usb = fbcgetmaxbase_usb;
hbcdefbase_usb bcdefbase_usb = fbcdefbase_usb;
hbcgetbase_usb bcgetbase_usb = fbcgetbase_usb;
hbcputw_usb bcputw_usb = fbcputw_usb;
hbcgetw_usb bcgetw_usb = fbcgetw_usb;
hbcgetansw_usb bcgetansw_usb = fbcgetansw_usb;
hbcputblk_usb bcputblk_usb = fbcputblk_usb;
hbcgetblk_usb bcgetblk_usb = fbcgetblk_usb;
hbcdefbus_usb bcdefbus_usb = fbcdefbus_usb;
hbcgetbus_usb bcgetbus_usb = fbcgetbus_usb;
hbcstart_usb bcstart_usb = fbcstart_usb;
hbcstartx_usb bcstartx_usb = fbcstartx_usb;
hbcdeflink_usb bcdeflink_usb = fbcdeflink_usb;
hbcgetlink_usb bcgetlink_usb = fbcgetlink_usb;
hbcstop_usb bcstop_usb = fbcstop_usb;
hbcgetstate_usb bcgetstate_usb = fbcgetstate_usb;
hrtreset_usb rtreset_usb = frtreset_usb;
hrtdefirqmode_usb rtdefirqmode_usb = frtdefirqmode_usb;
hrtgetirqmode_usb rtgetirqmode_usb = frtgetirqmode_usb;
hrtdefmode_usb rtdefmode_usb = frtdefmode_usb;
hrtgetmode_usb rtgetmode_usb = frtgetmode_usb;
hrtgetmaxpage_usb rtgetmaxpage_usb = frtgetmaxpage_usb;
hrtdefpage_usb rtdefpage_usb = frtdefpage_usb;
hrtgetpage_usb rtgetpage_usb = frtgetpage_usb;
hrtdefpagepc_usb rtdefpagepc_usb = frtdefpagepc_usb;
hrtdefpagebus_usb rtdefpagebus_usb = frtdefpagebus_usb;
hrtgetpagepc_usb rtgetpagepc_usb = frtgetpagepc_usb;
hrtgetpagebus_usb rtgetpagebus_usb = frtgetpagebus_usb;
hrtdefaddress_usb rtdefaddress_usb = frtdefaddress_usb;
hrtgetaddress_usb rtgetaddress_usb = frtgetaddress_usb;
hrtdefsubaddr_usb rtdefsubaddr_usb = frtdefsubaddr_usb;
hrtgetsubaddr_usb rtgetsubaddr_usb = frtgetsubaddr_usb;
hrtputw_usb rtputw_usb = frtputw_usb;
hrtgetw_usb rtgetw_usb = frtgetw_usb;
hrtputblk_usb rtputblk_usb = frtputblk_usb;
hrtgetblk_usb rtgetblk_usb = frtgetblk_usb;
hrtsetanswbits_usb rtsetanswbits_usb = frtsetanswbits_usb;
hrtclranswbits_usb rtclranswbits_usb = frtclranswbits_usb;
hrtgetanswbits_usb rtgetanswbits_usb = frtgetanswbits_usb;
hrtgetflags_usb rtgetflags_usb = frtgetflags_usb;
hrtputflags_usb rtputflags_usb = frtputflags_usb;
hrtsetflag_usb rtsetflag_usb = frtsetflag_usb;
hrtclrflag_usb rtclrflag_usb = frtclrflag_usb;
hrtgetflag_usb rtgetflag_usb = frtgetflag_usb;
hrtputflag_usb rtputflag_usb = frtputflag_usb;
hrtgetstate_usb rtgetstate_usb = frtgetstate_usb;
hrtbusy_usb rtbusy_usb = frtbusy_usb;
hrtlock_usb rtlock_usb = frtlock_usb;
hrtunlock_usb rtunlock_usb = frtunlock_usb;
hrtgetcmddata_usb rtgetcmddata_usb = frtgetcmddata_usb;
hrtputcmddata_usb rtputcmddata_usb = frtputcmddata_usb;
hmtreset_usb mtreset_usb = fmtreset_usb;
hmtgetsw_usb mtgetsw_usb = fmtgetsw_usb;
hrtenable_usb rtenable_usb = frtenable_usb;
hmrtselected_usb mrtselected_usb = fmrtselected_usb;
hmrtgetstate_usb mrtgetstate_usb = fmrtgetstate_usb;
hmrtdefbrcsubaddr0_usb mrtdefbrcsubaddr0_usb = fmrtdefbrcsubaddr0_usb;
hmrtreset_usb mrtreset_usb = fmrtreset_usb;
htmktimer_usb tmktimer_usb = ftmktimer_usb;
htmkgettimer_usb tmkgettimer_usb = ftmkgettimer_usb;
htmkgettimerl_usb tmkgettimerl_usb = ftmkgettimerl_usb;
hbcgetmsgtime_usb bcgetmsgtime_usb = fbcgetmsgtime_usb;
hrtgetmsgtime_usb rtgetmsgtime_usb = frtgetmsgtime_usb;
htmkgethwver_usb tmkgethwver_usb = ftmkgethwver_usb;
htmkgetevtime_usb tmkgetevtime_usb = ftmkgetevtime_usb;
htmkswtimer_usb tmkswtimer_usb = ftmkswtimer_usb;
htmkgetswtimer_usb tmkgetswtimer_usb = ftmkgetswtimer_usb;
htmktimeout_usb tmktimeout_usb = ftmktimeout_usb;
hmrtdefbrcpage_usb mrtdefbrcpage_usb = fmrtdefbrcpage_usb;
hmrtgetbrcpage_usb mrtgetbrcpage_usb = fmrtgetbrcpage_usb;

hMonitorHwStart_usb MonitorHwStart_usb = fMonitorHwStart_usb;
hMonitorHwGetMessage_usb MonitorHwGetMessage_usb = fMonitorHwGetMessage_usb;
hMonitorHwStop_usb MonitorHwStop_usb = fMonitorHwStop_usb;

#define DeviceIoControlCase(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped)\
{\
  if (!_VTMK4usbFlag[_VTMK4tmkCurNum])\
  {\
    switch(dwIoControlCode)\
    {\
      case (DWORD)IOCTL_TMK_bcreset:\
        return(DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcdefirqmode:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefirqmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcdefbase:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefbase, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcdefbus:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefbus, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcstart:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcstart, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcstartx:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcstartx, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_bcdeflink:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdeflink, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtreset:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefirqmode:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefirqmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefmode:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefpage:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpage, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefpagepc:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpagepc, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefpagebus:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpagebus, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_rtdefaddress:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefaddress, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_mtreset:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_mrtreset:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      case (DWORD)IOCTL_TMK_mrtdefbrcpage:\
        return (DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtdefbrcpage, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();\
      break;\
      default: DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);\
    }\
  }\
  else\
  {\
    if(_hVTMK4VxD[_VTMK4tmkCurNum])\
      switch(dwIoControlCode)\
      {\
        case (DWORD)IOCTL_TMK_tmkgetmode:\
          return tmkgetmode_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmkgetcwbits:\
          return tmkgetcwbits_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcreset:\
          return bcreset_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcdefirqmode:\
          return bcdefirqmode_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetirqmode:\
          return bcgetirqmode_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcgetmaxbase:\
        return bcgetmaxbase_usb();\
          break;\
        case (DWORD)IOCTL_TMK_bcdefbase:\
          return bcdefbase_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetbase:\
          return bcgetbase_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcgetw:\
          return bcgetw_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetansw:\
          return bcgetansw_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_bcdefbus:\
          return bcdefbus_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetbus:\
          return bcgetbus_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcstart:\
          return bcstart_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_bcstartx:\
          return bcstartx_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_bcdeflink:\
          return bcdeflink_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetlink:\
          return bcgetlink_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcstop:\
          return bcstop_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcgetstate:\
          return bcgetstate_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtreset:\
          return rtreset_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtdefirqmode:\
          return rtdefirqmode_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetirqmode:\
          return rtgetirqmode_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtdefmode:\
          return rtdefmode_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetmode:\
          return rtgetmode_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetmaxpage:\
          return rtgetmaxpage_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtdefpage:\
          return rtdefpage_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetpage:\
          return rtgetpage_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtdefpagepc:\
          return rtdefpagepc_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtdefpagebus:\
          return rtdefpagebus_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetpagepc:\
          return rtgetpagepc_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetpagebus:\
          return rtgetpagebus_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtdefaddress:\
          return rtdefaddress_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetaddress:\
          return rtgetaddress_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetsubaddr:\
          return rtgetsubaddr_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetw:\
          return rtgetw_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetanswbits:\
          return rtgetanswbits_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetflag:\
          return rtgetflag_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetstate:\
          return rtgetstate_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtbusy:\
          return rtbusy_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetcmddata:\
          return rtgetcmddata_usb (lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_mtreset:\
          return mtreset_usb();\
        break;\
        case (DWORD)IOCTL_TMK_mtgetsw:\
          return mtgetsw_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtenable:\
          return rtenable_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_mrtselected:\
          return mrtselected_usb();\
        break;\
        case (DWORD)IOCTL_TMK_mrtgetstate:\
          return mrtgetstate_usb();\
        break;\
        case (DWORD)IOCTL_TMK_mrtreset:\
          return mrtreset_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmktimer:\
          return tmktimer_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_tmkgettimer:\
          return tmkgettimer_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmkgettimerl:\
          return tmkgettimerl_usb();\
        break;\
        case (DWORD)IOCTL_TMK_bcgetmsgtime:\
          return bcgetmsgtime_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtgetmsgtime:\
          return rtgetmsgtime_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmkgethwver:\
          return tmkgethwver_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmkgetevtime:\
          return tmkgetevtime_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmkswtimer:\
          return tmkswtimer_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_tmkgetswtimer:\
          return tmkgetswtimer_usb();\
        break;\
        case (DWORD)IOCTL_TMK_tmktimeout:\
          return tmktimeout_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_mrtdefbrcpage:\
          return mrtdefbrcpage_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_mrtgetbrcpage:\
          return mrtgetbrcpage_usb();\
        break;\
      }\
    else\
      return -1;\
  }\
}  

#define DeviceIoControlCaseVoid(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped)\
{\
  if (!_VTMK4usbFlag[_VTMK4tmkCurNum])\
    DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);\
  else\
  {\
    if(_hVTMK4VxD[_VTMK4tmkCurNum])\
      switch(dwIoControlCode)\
      {\
        case (DWORD)IOCTL_TMK_tmksetcwbits:\
          tmksetcwbits_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_tmkclrcwbits:\
          tmkclrcwbits_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_tmkdefevent:\
          tmkdefevent_usb((void *)(lpInBuffer[0]|(lpInBuffer[1]<<16)));\
        break;\
        case (DWORD)IOCTL_TMK_bcputw:\
          bcputw_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_bcputblk:\
          bcputblk_usb(lpInBuffer[0],(void *)(lpInBuffer[1]|(lpInBuffer[2]<<16)),lpInBuffer[3]);\
        break;\
        case (DWORD)IOCTL_TMK_bcgetblk:\
          bcgetblk_usb(lpInBuffer[0],(void *)(lpInBuffer[1]|(lpInBuffer[2]<<16)),lpInBuffer[3]);\
        break;\
        case (DWORD)IOCTL_TMK_rtdefsubaddr:\
          rtdefsubaddr_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_rtputw:\
          rtputw_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_rtputblk:\
          rtputblk_usb(lpInBuffer[0],(void *)(lpInBuffer[1]|(lpInBuffer[2]<<16)),lpInBuffer[3]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetblk:\
          rtgetblk_usb(lpInBuffer[0],(void *)(lpInBuffer[1]|(lpInBuffer[2]<<16)),lpInBuffer[3]);\
        break;\
        case (DWORD)IOCTL_TMK_rtsetanswbits:\
          rtsetanswbits_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtclranswbits:\
          rtclranswbits_usb(lpInBuffer[0]);\
        break;\
        case (DWORD)IOCTL_TMK_rtgetflags:\
          rtgetflags_usb((void *)(lpInBuffer[0]|(lpInBuffer[1]<<16)),lpInBuffer[2],lpInBuffer[3],lpInBuffer[4]);\
        break;\
        case (DWORD)IOCTL_TMK_rtputflags:\
          rtputflags_usb((void *)(lpInBuffer[0]|(lpInBuffer[1]<<16)),lpInBuffer[2],lpInBuffer[3],lpInBuffer[4]);\
        break;\
        case (DWORD)IOCTL_TMK_rtsetflag:\
          rtsetflag_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtclrflag:\
          rtclrflag_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtlock:\
          rtlock_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_rtunlock:\
          rtunlock_usb();\
        break;\
        case (DWORD)IOCTL_TMK_rtputcmddata:\
          rtputcmddata_usb(lpInBuffer[0],lpInBuffer[1]);\
        break;\
        case (DWORD)IOCTL_TMK_mrtdefbrcsubaddr0:\
          mrtdefbrcsubaddr0_usb();\
        break;\
      }\
  }\
}

#pragma warning (disable: 4244) 
#pragma warning (disable: 4700) 
#pragma warning (disable: 4715) 
#pragma warning (disable: 4996) 

int _VTMK4tmkNumber;
HANDLE _ahVTMK4Event[MAX_TMK_NUMBER+1+MAX_USBTA_NUM+1];
HANDLE _hVTMK4VxD[MAX_TMK_NUMBER+1+MAX_USBTA_NUM+1];
int _VTMK4tmkMaxNum = -1;
int _VTMK4mrtMaxNum = -1;
int _VTMK4tmkCurNum;
char _VTMK4usbFlag[MAX_TMK_NUMBER+1+MAX_USBTA_NUM+1];
int _VTMK4tmkDrvnum[MAX_TMK_NUMBER+1+MAX_USBTA_NUM+1];

HINSTANCE hUSBDrvDll = NULL;

DWORD TmkOpen()
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[6];
  DWORD _cbVTMK4Ret;
  PEZUSB_DRIVER_VERSION version;
  int iTMK;
  int nTMK = -1;
  int nMRT = -1;
  int loaded = 1;
  int nTMKb = -1;
  char tmkName[32];
  char cc = 0x5c;

  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    _VTMK4usbFlag[iTMK] = 0;
    sprintf (tmkName,"%c%c.%cTMK1553BDevice%d",cc,cc,cc,iTMK);
    _hVTMK4VxD[iTMK] = CreateFile(tmkName,
                                  GENERIC_READ | GENERIC_WRITE,
                                  0,
                                  NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL
                                  );

    if (_hVTMK4VxD[iTMK] != INVALID_HANDLE_VALUE)
    {
      if (!DeviceIoControl(_hVTMK4VxD[iTMK], (DWORD)IOCTL_TMK_GetVersion, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL) ||
              _awVTMK4OutBuf[0] < TMK_VERSION_MIN)
      {
        CloseHandle(_hVTMK4VxD[iTMK]);
        _hVTMK4VxD[iTMK] = 0;
         return VTMK_BAD_VERSION;
      }
      _VTMK4tmkDrvnum[iTMK] = iTMK;
      nTMK = iTMK;
      if (iTMK < 8)
        nMRT = iTMK;
    }
  }

  if(!hUSBDrvDll)
    hUSBDrvDll = LoadLibrary("USB_TA_DRV.dll");


  if(hUSBDrvDll != NULL)
  {
    if(!(ptmkError = (int*)GetProcAddress(hUSBDrvDll, "tmkError")))
        {ptmkError = &ftmkError;loaded = 0;}
    if(!(tmkconfig_usb = (htmkconfig_usb) GetProcAddress(hUSBDrvDll, "tmkconfig_usb")))
        {tmkconfig_usb = ftmkconfig_usb;loaded = 0;}
    if(!(tmkdone_usb = (htmkdone_usb) GetProcAddress(hUSBDrvDll, "tmkdone_usb")))
        {tmkdone_usb = ftmkdone_usb;loaded = 0;}
    if(!(tmkselect_usb = (htmkselect_usb) GetProcAddress(hUSBDrvDll, "tmkselect_usb")))
        {tmkselect_usb = ftmkselect_usb;loaded = 0;}
    if(!(Read_DLL_EvD_usb = (hRead_DLL_EvD_usb) GetProcAddress(hUSBDrvDll, "Read_DLL_EvD_usb")))
        {Read_DLL_EvD_usb = fRead_DLL_EvD_usb;loaded = 0;}
    if(!(tmktimeout_usb = (htmktimeout_usb) GetProcAddress(hUSBDrvDll, "tmktimeout_usb")))
        {tmktimeout_usb = ftmktimeout_usb;loaded = 0;}
    if(!(tmksetcwbits_usb = (htmksetcwbits_usb) GetProcAddress(hUSBDrvDll, "tmksetcwbits_usb")))
        {tmksetcwbits_usb = ftmksetcwbits_usb;loaded = 0;}
    if(!(tmkclrcwbits_usb = (htmkclrcwbits_usb) GetProcAddress(hUSBDrvDll, "tmkclrcwbits_usb")))
        {tmkclrcwbits_usb = ftmkclrcwbits_usb;loaded = 0;}
    if(!(tmkgetcwbits_usb = (htmkgetcwbits_usb) GetProcAddress(hUSBDrvDll, "tmkclrcwbits_usb")))
        {tmkgetcwbits_usb = ftmkgetcwbits_usb;loaded = 0;}
    if(!(tmkgetmode_usb = (htmkgetmode_usb) GetProcAddress(hUSBDrvDll, "tmkgetmode_usb")))
        {tmkgetmode_usb = ftmkgetmode_usb;loaded = 0;}
    if(!(tmkdefdac_usb = (htmkdefdac_usb) GetProcAddress(hUSBDrvDll, "tmkdefdac_usb")))
        {tmkdefdac_usb = ftmkdefdac_usb;loaded = 0;}
    if(!(tmkgetdac_usb = (htmkgetdac_usb) GetProcAddress(hUSBDrvDll, "tmkgetdac_usb")))
        {tmkgetdac_usb = ftmkgetdac_usb;loaded = 0;}
    if(!(bcreset_usb = (hbcreset_usb) GetProcAddress(hUSBDrvDll, "bcreset_usb")))
        {bcreset_usb = fbcreset_usb;loaded = 0;}
    if(!(bcdefirqmode_usb = (hbcdefirqmode_usb) GetProcAddress(hUSBDrvDll, "bcdefirqmode_usb")))
        {bcdefirqmode_usb = fbcdefirqmode_usb;loaded = 0;}
    if(!(bcgetirqmode_usb = (hbcgetirqmode_usb) GetProcAddress(hUSBDrvDll, "bcgetirqmode_usb")))
        {bcgetirqmode_usb = fbcgetirqmode_usb;loaded = 0;}
    if(!(bcgetmaxbase_usb = (hbcgetmaxbase_usb) GetProcAddress(hUSBDrvDll, "bcgetmaxbase_usb")))
        {bcgetmaxbase_usb = fbcgetmaxbase_usb;loaded = 0;}
    if(!(bcgetmaxbase_usb = (hbcgetmaxbase_usb) GetProcAddress(hUSBDrvDll, "bcgetmaxbase_usb")))
        {bcgetmaxbase_usb = fbcgetmaxbase_usb;loaded = 0;}
    if(!(bcdefbase_usb = (hbcdefbase_usb) GetProcAddress(hUSBDrvDll, "bcdefbase_usb")))
        {bcdefbase_usb = fbcdefbase_usb;loaded = 0;}
    if(!(bcgetbase_usb = (hbcgetbase_usb) GetProcAddress(hUSBDrvDll, "bcgetbase_usb")))
        {bcgetbase_usb = fbcgetbase_usb;loaded = 0;}
    if(!(bcputw_usb = (hbcputw_usb) GetProcAddress(hUSBDrvDll, "bcputw_usb")))
        {bcputw_usb = fbcputw_usb;loaded = 0;}
    if(!(bcgetw_usb = (hbcgetw_usb) GetProcAddress(hUSBDrvDll, "bcgetw_usb")))
        {bcgetw_usb = fbcgetw_usb;loaded = 0;}
    if(!(bcgetansw_usb = (hbcgetansw_usb) GetProcAddress(hUSBDrvDll, "bcgetansw_usb")))
        {bcgetansw_usb = fbcgetansw_usb;loaded = 0;}
    if(!(bcputblk_usb = (hbcputblk_usb) GetProcAddress(hUSBDrvDll, "bcputblk_usb")))
        {bcputblk_usb = fbcputblk_usb;loaded = 0;}
    if(!(bcgetblk_usb = (hbcgetblk_usb) GetProcAddress(hUSBDrvDll, "bcgetblk_usb")))
        {bcgetblk_usb = fbcgetblk_usb;loaded = 0;}
    if(!(bcdefbus_usb = (hbcdefbus_usb) GetProcAddress(hUSBDrvDll, "bcdefbus_usb")))
        {bcdefbus_usb = fbcdefbus_usb;loaded = 0;}
    if(!(bcgetbus_usb = (hbcgetbus_usb) GetProcAddress(hUSBDrvDll, "bcgetbus_usb")))
        {bcgetbus_usb = fbcgetbus_usb;loaded = 0;}
    if(!(bcstart_usb = (hbcstart_usb) GetProcAddress(hUSBDrvDll, "bcstart_usb")))
        {bcstart_usb = fbcstart_usb;loaded = 0;}
    if(!(bcstartx_usb = (hbcstartx_usb) GetProcAddress(hUSBDrvDll, "bcstartx_usb")))
        {bcstartx_usb = fbcstartx_usb;loaded = 0;}
    if(!(bcdeflink_usb = (hbcdeflink_usb) GetProcAddress(hUSBDrvDll, "bcdeflink_usb")))
        {bcdeflink_usb = fbcdeflink_usb;loaded = 0;}
    if(!(bcgetlink_usb = (hbcgetlink_usb) GetProcAddress(hUSBDrvDll, "bcgetlink_usb")))
        {bcgetlink_usb = fbcgetlink_usb;loaded = 0;}
    if(!(bcstop_usb = (hbcstop_usb) GetProcAddress(hUSBDrvDll, "bcstop_usb")))
        {bcstop_usb = fbcstop_usb;loaded = 0;}
    if(!(bcgetstate_usb = (hbcgetstate_usb) GetProcAddress(hUSBDrvDll, "bcgetstate_usb")))
        {bcgetstate_usb = fbcgetstate_usb;loaded = 0;}
    if(!(rtreset_usb = (hrtreset_usb) GetProcAddress(hUSBDrvDll, "rtreset_usb")))
        {rtreset_usb = frtreset_usb;loaded = 0;}
    if(!(rtdefirqmode_usb = (hrtdefirqmode_usb) GetProcAddress(hUSBDrvDll, "rtdefirqmode_usb")))
        {rtdefirqmode_usb = frtdefirqmode_usb;loaded = 0;}
    if(!(rtgetirqmode_usb = (hrtgetirqmode_usb) GetProcAddress(hUSBDrvDll, "rtgetirqmode_usb")))
        {rtgetirqmode_usb = frtgetirqmode_usb;loaded = 0;}
    if(!(rtdefmode_usb = (hrtdefmode_usb) GetProcAddress(hUSBDrvDll, "rtdefmode_usb")))
        {rtdefmode_usb = frtdefmode_usb;loaded = 0;}
    if(!(rtgetmode_usb = (hrtgetmode_usb) GetProcAddress(hUSBDrvDll, "rtgetmode_usb")))
        {rtgetmode_usb = frtgetmode_usb;loaded = 0;}
    if(!(rtgetmaxpage_usb = (hrtgetmaxpage_usb) GetProcAddress(hUSBDrvDll, "rtgetmaxpage_usb")))
        {rtgetmaxpage_usb = frtgetmaxpage_usb;loaded = 0;}
    if(!(rtdefpage_usb = (hrtdefpage_usb) GetProcAddress(hUSBDrvDll, "rtdefpage_usb")))
        {rtdefpage_usb = frtdefpage_usb;loaded = 0;}
    if(!(rtgetpage_usb = (hrtgetpage_usb) GetProcAddress(hUSBDrvDll, "rtgetpage_usb")))
        {rtgetpage_usb = frtgetpage_usb;loaded = 0;}
    if(!(rtdefpagepc_usb = (hrtdefpagepc_usb) GetProcAddress(hUSBDrvDll, "rtdefpagepc_usb")))
        {rtdefpagepc_usb = frtdefpagepc_usb;loaded = 0;}
    if(!(rtdefpagebus_usb = (hrtdefpagebus_usb) GetProcAddress(hUSBDrvDll, "rtdefpagebus_usb")))
        {rtdefpagebus_usb = frtdefpagebus_usb;loaded = 0;}
    if(!(rtgetpagepc_usb = (hrtgetpagepc_usb) GetProcAddress(hUSBDrvDll, "rtgetpagepc_usb")))
        {rtgetpagepc_usb = frtgetpagepc_usb;loaded = 0;}
    if(!(rtgetpagebus_usb = (hrtgetpagebus_usb) GetProcAddress(hUSBDrvDll, "rtgetpagebus_usb")))
        {rtgetpagebus_usb = frtgetpagebus_usb;loaded = 0;}
    if(!(rtdefaddress_usb = (hrtdefaddress_usb) GetProcAddress(hUSBDrvDll, "rtdefaddress_usb")))
        {rtdefaddress_usb = frtdefaddress_usb;loaded = 0;}
    if(!(rtgetaddress_usb = (hrtgetaddress_usb) GetProcAddress(hUSBDrvDll, "rtgetaddress_usb")))
        {rtgetaddress_usb = frtgetaddress_usb;loaded = 0;}
    if(!(rtdefsubaddr_usb = (hrtdefsubaddr_usb) GetProcAddress(hUSBDrvDll, "rtdefsubaddr_usb")))
        {rtdefsubaddr_usb = frtdefsubaddr_usb;loaded = 0;}
    if(!(rtgetsubaddr_usb = (hrtgetsubaddr_usb) GetProcAddress(hUSBDrvDll, "rtgetsubaddr_usb")))
        {rtgetsubaddr_usb = frtgetsubaddr_usb;loaded = 0;}
    if(!(rtputw_usb = (hrtputw_usb) GetProcAddress(hUSBDrvDll, "rtputw_usb")))
        {rtputw_usb = frtputw_usb;loaded = 0;}
    if(!(rtgetw_usb = (hrtgetw_usb) GetProcAddress(hUSBDrvDll, "rtgetw_usb")))
        {rtgetw_usb = frtgetw_usb;loaded = 0;}
    if(!(rtputblk_usb = (hrtputblk_usb) GetProcAddress(hUSBDrvDll, "rtputblk_usb")))
        {rtputblk_usb = frtputblk_usb;loaded = 0;}
    if(!(rtgetblk_usb = (hrtgetblk_usb) GetProcAddress(hUSBDrvDll, "rtgetblk_usb")))
        {rtgetblk_usb = frtgetblk_usb;loaded = 0;}
    if(!(rtsetanswbits_usb = (hrtsetanswbits_usb) GetProcAddress(hUSBDrvDll, "rtsetanswbits_usb")))
        {rtsetanswbits_usb = frtsetanswbits_usb;loaded = 0;}
    if(!(rtclranswbits_usb = (hrtclranswbits_usb) GetProcAddress(hUSBDrvDll, "rtclranswbits_usb")))
        {rtclranswbits_usb = frtclranswbits_usb;loaded = 0;}
    if(!(rtgetanswbits_usb = (hrtgetanswbits_usb) GetProcAddress(hUSBDrvDll, "rtgetanswbits_usb")))
        {rtgetanswbits_usb = frtgetanswbits_usb;loaded = 0;}
    if(!(rtgetflags_usb = (hrtgetflags_usb) GetProcAddress(hUSBDrvDll, "rtgetflags_usb")))
        {rtgetflags_usb = frtgetflags_usb;loaded = 0;}
    if(!(rtputflags_usb = (hrtputflags_usb) GetProcAddress(hUSBDrvDll, "rtputflags_usb")))
        {rtputflags_usb = frtputflags_usb;loaded = 0;}
    if(!(rtsetflag_usb = (hrtsetflag_usb) GetProcAddress(hUSBDrvDll, "rtsetflag_usb")))
        {rtsetflag_usb = frtsetflag_usb;loaded = 0;}
    if(!(rtclrflag_usb = (hrtclrflag_usb) GetProcAddress(hUSBDrvDll, "rtclrflag_usb")))
        {rtclrflag_usb = frtclrflag_usb;loaded = 0;}
    if(!(rtgetflag_usb = (hrtgetflag_usb) GetProcAddress(hUSBDrvDll, "rtgetflag_usb")))
        {rtgetflag_usb = frtgetflag_usb;loaded = 0;}
    if(!(rtputflag_usb = (hrtputflag_usb) GetProcAddress(hUSBDrvDll, "rtputflag_usb")))
        {rtputflag_usb = frtputflag_usb;loaded = 0;}
    if(!(rtgetstate_usb = (hrtgetstate_usb) GetProcAddress(hUSBDrvDll, "rtgetstate_usb")))
        {rtgetstate_usb = frtgetstate_usb;loaded = 0;}
    if(!(rtbusy_usb = (hrtbusy_usb) GetProcAddress(hUSBDrvDll, "rtbusy_usb")))
        {rtbusy_usb = frtbusy_usb;loaded = 0;}
    if(!(rtlock_usb = (hrtlock_usb) GetProcAddress(hUSBDrvDll, "rtlock_usb")))
        {rtlock_usb = frtlock_usb;loaded = 0;}
    if(!(rtunlock_usb = (hrtunlock_usb) GetProcAddress(hUSBDrvDll, "rtunlock_usb")))
        {rtunlock_usb = frtunlock_usb;loaded = 0;}
    if(!(rtgetcmddata_usb = (hrtgetcmddata_usb) GetProcAddress(hUSBDrvDll, "rtgetcmddata_usb")))
        {rtgetcmddata_usb = frtgetcmddata_usb;loaded = 0;}
    if(!(rtputcmddata_usb = (hrtputcmddata_usb) GetProcAddress(hUSBDrvDll, "rtputcmddata_usb")))
        {rtputcmddata_usb = frtputcmddata_usb;loaded = 0;}
    if(!(mtreset_usb = (hmtreset_usb) GetProcAddress(hUSBDrvDll, "mtreset_usb")))
        {mtreset_usb = fmtreset_usb;loaded = 0;}
    if(!(mtgetsw_usb = (hmtgetsw_usb) GetProcAddress(hUSBDrvDll, "mtgetsw_usb")))
        {mtgetsw_usb = fmtgetsw_usb;loaded = 0;}
    if(!(rtenable_usb = (hrtenable_usb) GetProcAddress(hUSBDrvDll, "rtenable_usb")))
        {rtenable_usb = frtenable_usb;loaded = 0;}
    if(!(mrtselected_usb = (hmrtselected_usb) GetProcAddress(hUSBDrvDll, "mrtselected_usb")))
        {mrtselected_usb = fmrtselected_usb;loaded = 0;}
    if(!(mrtgetstate_usb = (hmrtgetstate_usb) GetProcAddress(hUSBDrvDll, "mrtgetstate_usb")))
        {mrtgetstate_usb = fmrtgetstate_usb;loaded = 0;}
    if(!(mrtdefbrcsubaddr0_usb = (hmrtdefbrcsubaddr0_usb) GetProcAddress(hUSBDrvDll, "mrtdefbrcsubaddr0_usb")))
        {mrtdefbrcsubaddr0_usb = fmrtdefbrcsubaddr0_usb;loaded = 0;}
    if(!(mrtreset_usb = (hmrtreset_usb) GetProcAddress(hUSBDrvDll, "mrtreset_usb")))
        {mrtreset_usb = fmrtreset_usb;loaded = 0;}
    if(!(tmktimer_usb = (htmktimer_usb) GetProcAddress(hUSBDrvDll, "tmktimer_usb")))
        {tmktimer_usb = ftmktimer_usb;loaded = 0;}
    if(!(tmkgettimer_usb = (htmkgettimer_usb) GetProcAddress(hUSBDrvDll, "tmkgettimer_usb")))
        {tmkgettimer_usb = ftmkgettimer_usb;loaded = 0;}
    if(!(tmkgettimerl_usb = (htmkgettimerl_usb) GetProcAddress(hUSBDrvDll, "tmkgettimerl_usb")))
        {tmkgettimerl_usb = ftmkgettimerl_usb;loaded = 0;}
    if(!(bcgetmsgtime_usb = (hbcgetmsgtime_usb) GetProcAddress(hUSBDrvDll, "bcgetmsgtime_usb")))
        {bcgetmsgtime_usb = fbcgetmsgtime_usb;loaded = 0;}
    if(!(rtgetmsgtime_usb = (hrtgetmsgtime_usb) GetProcAddress(hUSBDrvDll, "rtgetmsgtime_usb")))
        {rtgetmsgtime_usb = frtgetmsgtime_usb;loaded = 0;}
    if(!(tmkgethwver_usb = (htmkgethwver_usb) GetProcAddress(hUSBDrvDll, "tmkgethwver_usb")))
        {tmkgethwver_usb = ftmkgethwver_usb;loaded = 0;}
    if(!(tmkgetevtime_usb = (htmkgetevtime_usb) GetProcAddress(hUSBDrvDll, "tmkgetevtime_usb")))
        {tmkgetevtime_usb = ftmkgetevtime_usb;loaded = 0;}
    if(!(tmkdefevent_usb = (htmkdefevent_usb) GetProcAddress(hUSBDrvDll, "tmkdefevent_usb")))
        {tmkdefevent_usb = ftmkdefevent_usb;loaded = 0;}
    if(!(tmkswtimer_usb = (htmkswtimer_usb) GetProcAddress(hUSBDrvDll, "tmkswtimer_usb")))
        {tmkswtimer_usb = ftmkswtimer_usb;loaded = 0;}
    if(!(tmkgetswtimer_usb = (htmkgetswtimer_usb) GetProcAddress(hUSBDrvDll, "tmkgetswtimer_usb")))
        {tmkgetswtimer_usb = ftmkgetswtimer_usb;loaded = 0;}
    if(!(tmktimeout_usb = (htmktimeout_usb) GetProcAddress(hUSBDrvDll, "tmktimeout_usb")))
        {tmktimeout_usb = ftmktimeout_usb;loaded = 0;}
    if(!(mrtdefbrcpage_usb = (hmrtdefbrcpage_usb) GetProcAddress(hUSBDrvDll, "mrtdefbrcpage_usb")))
        {mrtdefbrcpage_usb = fmrtdefbrcpage_usb;loaded = 0;}
    if(!(mrtgetbrcpage_usb = (hmrtgetbrcpage_usb) GetProcAddress(hUSBDrvDll, "mrtgetbrcpage_usb")))
        {mrtgetbrcpage_usb = fmrtgetbrcpage_usb;loaded = 0;}

    if(!(MonitorHwStart_usb = (hMonitorHwStart_usb) GetProcAddress(hUSBDrvDll, "MonitorHwStart_usb")))
        {MonitorHwStart_usb = fMonitorHwStart_usb;loaded = 0;}
    if(!(MonitorHwGetMessage_usb = (hMonitorHwGetMessage_usb) GetProcAddress(hUSBDrvDll, "MonitorHwGetMessage_usb")))
        {MonitorHwGetMessage_usb = fMonitorHwGetMessage_usb;loaded = 0;}
    if(!(MonitorHwStop_usb = (hMonitorHwStop_usb) GetProcAddress(hUSBDrvDll, "MonitorHwStop_usb")))
        {MonitorHwStop_usb = fMonitorHwStop_usb;loaded = 0;}
                
    if(loaded)
    {
      nTMKb = nTMK;
      for(iTMK = 0; iTMK <= MAX_USBTA_NUM; ++iTMK)
      {
        _VTMK4usbFlag[nTMKb+1+iTMK] = 0;
        sprintf (tmkName,"%c%c.%cEZUSB-%d",cc,cc,cc,iTMK);
        _hVTMK4VxD[nTMKb+1+iTMK] = CreateFile(tmkName,
                                              GENERIC_READ | GENERIC_WRITE,
                                              0,
                                              NULL,
                                              OPEN_EXISTING,
                                              FILE_ATTRIBUTE_NORMAL,
                                              NULL
                                              );

        if (_hVTMK4VxD[nTMKb+1+iTMK] != INVALID_HANDLE_VALUE)
        {
          DeviceIoControl(_hVTMK4VxD[nTMKb+1+iTMK], (DWORD)IOCTL_EZUSB_GET_DRIVER_VERSION, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 6, &_cbVTMK4Ret, NULL);
          version = (PEZUSB_DRIVER_VERSION) _awVTMK4OutBuf;

          if(((DWORD)(version->MajorVersion << 16)+(DWORD)(version->MinorVersion)) < ((DWORD)(EZUSB_MAJOR_VERSION_MIN << 16)+(DWORD)(EZUSB_MINOR_VERSION_MIN)))
          {
            CloseHandle(_hVTMK4VxD[nTMKb+1+iTMK]);
          }
          else
          {
            _VTMK4usbFlag[iTMK+1 + nTMKb] = 1;
            _VTMK4tmkDrvnum[iTMK+1+nTMKb]=iTMK;
            nTMK = iTMK+1+nTMKb;
            if (nTMK < 8)
              nMRT = nTMK;
            CloseHandle(_hVTMK4VxD[nTMKb+1+iTMK]);
          }
        }
      }
    }
  }

  if (nTMK == -1) 
    return 1;

  _VTMK4tmkMaxNum = nTMK;
  _VTMK4mrtMaxNum = nMRT;
/*
  for (iTMK = 0; iTMK <= _VTMK4tmkMaxNum; ++iTMK)
  {
    if (_hVTMK4VxD[iTMK] != INVALID_HANDLE_VALUE) 
    {
      CloseHandle(_hVTMK4VxD[iTMK]);
    }
    _ahVTMK4Event[iTMK] = 0;
    _hVTMK4VxD[iTMK] = 0;
  }
*/
  for (iTMK = 0; iTMK <= _VTMK4tmkMaxNum; ++iTMK)
  {
    if (_hVTMK4VxD[iTMK] != INVALID_HANDLE_VALUE) 
    {
     DWORD dwHandleFlag;
     if( ::GetHandleInformation(_hVTMK4VxD[iTMK],&dwHandleFlag) )
     {
       CloseHandle(_hVTMK4VxD[iTMK]);
     };
    }
    _ahVTMK4Event[iTMK] = 0;
    _hVTMK4VxD[iTMK] = 0;
  }

  return 0;
}

void TmkClose()
{
  int iTMK;
  for (iTMK = 0; iTMK <= _VTMK4tmkMaxNum; ++iTMK)
  {
    if (_hVTMK4VxD[iTMK])
    CloseHandle(_hVTMK4VxD[iTMK]);
    _ahVTMK4Event[iTMK] = 0;
    _hVTMK4VxD[iTMK] = 0;
    _VTMK4usbFlag[iTMK] = 0;
    _VTMK4tmkDrvnum[iTMK] = -1;
  }

  _VTMK4tmkMaxNum = -1;

  tmkconfig_usb = ftmkconfig_usb;
  tmkdone_usb = ftmkdone_usb;
  tmkselect_usb = ftmkselect_usb;
  tmkdefevent_usb = ftmkdefevent_usb;
  Read_DLL_EvD_usb = fRead_DLL_EvD_usb;
  tmksetcwbits_usb = ftmksetcwbits_usb;
  tmkclrcwbits_usb = ftmkclrcwbits_usb;
  tmkgetcwbits_usb = ftmkgetcwbits_usb;
  tmkgetmode_usb = ftmkgetmode_usb;
  tmkdefdac_usb = ftmkdefdac_usb;
  tmkgetdac_usb = ftmkgetdac_usb;
  bcreset_usb = fbcreset_usb;
  bcdefirqmode_usb = fbcdefirqmode_usb;
  bcgetirqmode_usb = fbcgetirqmode_usb;
  bcgetmaxbase_usb = fbcgetmaxbase_usb;
  bcdefbase_usb = fbcdefbase_usb;
  bcgetbase_usb = fbcgetbase_usb;
  bcputw_usb = fbcputw_usb;
  bcgetw_usb = fbcgetw_usb;
  bcgetansw_usb = fbcgetansw_usb;
  bcputblk_usb = fbcputblk_usb;
  bcgetblk_usb = fbcgetblk_usb;
  bcdefbus_usb = fbcdefbus_usb;
  bcgetbus_usb = fbcgetbus_usb;
  bcstart_usb = fbcstart_usb;
  bcstartx_usb = fbcstartx_usb;
  bcdeflink_usb = fbcdeflink_usb;
  bcgetlink_usb = fbcgetlink_usb;
  bcstop_usb = fbcstop_usb;
  bcgetstate_usb = fbcgetstate_usb;
  rtreset_usb = frtreset_usb;
  rtdefirqmode_usb = frtdefirqmode_usb;
  rtgetirqmode_usb = frtgetirqmode_usb;
  rtdefmode_usb = frtdefmode_usb;
  rtgetmode_usb = frtgetmode_usb;
  rtgetmaxpage_usb = frtgetmaxpage_usb;
  rtdefpage_usb = frtdefpage_usb;
  rtgetpage_usb = frtgetpage_usb;
  rtdefpagepc_usb = frtdefpagepc_usb;
  rtdefpagebus_usb = frtdefpagebus_usb;
  rtgetpagepc_usb = frtgetpagepc_usb;
  rtgetpagebus_usb = frtgetpagebus_usb;
  rtdefaddress_usb = frtdefaddress_usb;
  rtgetaddress_usb = frtgetaddress_usb;
  rtdefsubaddr_usb = frtdefsubaddr_usb;
  rtgetsubaddr_usb = frtgetsubaddr_usb;
  rtputw_usb = frtputw_usb;
  rtgetw_usb = frtgetw_usb;
  rtputblk_usb = frtputblk_usb;
  rtgetblk_usb = frtgetblk_usb;
  rtsetanswbits_usb = frtsetanswbits_usb;
  rtclranswbits_usb = frtclranswbits_usb;
  rtgetanswbits_usb = frtgetanswbits_usb;
  rtgetflags_usb = frtgetflags_usb;
  rtputflags_usb = frtputflags_usb;
  rtsetflag_usb = frtsetflag_usb;
  rtclrflag_usb = frtclrflag_usb;
  rtgetflag_usb = frtgetflag_usb;
  rtputflag_usb = frtputflag_usb;
  rtgetstate_usb = frtgetstate_usb;
  rtbusy_usb = frtbusy_usb;
  rtlock_usb = frtlock_usb;
  rtunlock_usb = frtunlock_usb;
  rtgetcmddata_usb = frtgetcmddata_usb;
  rtputcmddata_usb = frtputcmddata_usb;
  mtreset_usb = fmtreset_usb;
  mtgetsw_usb = fmtgetsw_usb;
  rtenable_usb = frtenable_usb;
  mrtselected_usb = fmrtselected_usb;
  mrtgetstate_usb = fmrtgetstate_usb;
  mrtdefbrcsubaddr0_usb = fmrtdefbrcsubaddr0_usb;
  mrtreset_usb = fmrtreset_usb;
  tmktimer_usb = ftmktimer_usb;
  tmkgettimer_usb = ftmkgettimer_usb;
  tmkgettimerl_usb = ftmkgettimerl_usb;
  bcgetmsgtime_usb = fbcgetmsgtime_usb;
  rtgetmsgtime_usb = frtgetmsgtime_usb;
  tmkgethwver_usb = ftmkgethwver_usb;
  tmkgetevtime_usb = ftmkgetevtime_usb;
  tmkswtimer_usb = ftmkswtimer_usb;
  tmkgetswtimer_usb = ftmkgetswtimer_usb;
  tmktimeout_usb = ftmktimeout_usb;
  mrtdefbrcpage_usb = fmrtdefbrcpage_usb;
  mrtgetbrcpage_usb = fmrtgetbrcpage_usb;

  MonitorHwStart_usb = fMonitorHwStart_usb;
  MonitorHwGetMessage_usb = fMonitorHwGetMessage_usb;
  MonitorHwStop_usb = fMonitorHwStop_usb;

  if(hUSBDrvDll)
    FreeLibrary(hUSBDrvDll);
  hUSBDrvDll = NULL;
}

int tmkgetmaxn()
{
  return _VTMK4tmkMaxNum;
}

int tmkconfig(int tmkNumber)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  char tmkName[32];

  if (_VTMK4tmkMaxNum == -1)
		return TMK_BAD_NUMBER;

  if (_hVTMK4VxD[tmkNumber] != 0)
		return TMK_BAD_NUMBER;

  if (_VTMK4usbFlag[tmkNumber])
  {
    sprintf (tmkName,"%c%c.%cEZUSB-%d",0x5c,0x5c,0x5c,_VTMK4tmkDrvnum[tmkNumber]);
    _hVTMK4VxD[tmkNumber] = CreateFile(tmkName,
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                       );
    if (_hVTMK4VxD[tmkNumber] == INVALID_HANDLE_VALUE)
      return TMK_BAD_NUMBER;
    else
    {
      if(DeviceIoControl(_hVTMK4VxD[tmkNumber], (DWORD)IOCTL_EZUSB_SET_CURPROC, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL))
      {
        _VTMK4tmkCurNum = tmkNumber;
        return tmkconfig_usb(_VTMK4tmkDrvnum[tmkNumber], 9, 0, 0);
      }
      else
      {
        CloseHandle (_hVTMK4VxD[tmkNumber]);
        _hVTMK4VxD[tmkNumber] = 0;
        return TMK_BAD_NUMBER;
      }
        
    }
  }

  sprintf (tmkName,"%c%c.%cTMK1553BDevice%d",0x5c,0x5c,0x5c,tmkNumber);
  _hVTMK4VxD[tmkNumber] = CreateFile(tmkName,
                                     GENERIC_READ | GENERIC_WRITE,
                                     0,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL
                                     );
  if (_hVTMK4VxD[tmkNumber] == INVALID_HANDLE_VALUE)
    return TMK_BAD_NUMBER;
  else
  {
    _awVTMK4InBuf[0] = (WORD)tmkNumber;
    DeviceIoControl(_hVTMK4VxD[tmkNumber], (DWORD)IOCTL_TMK_tmkconfig, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
    if (_awVTMK4OutBuf[0] != 0) 
    {
      CloseHandle (_hVTMK4VxD[tmkNumber]);
      _hVTMK4VxD[tmkNumber] = 0;
    }
    _VTMK4tmkCurNum = tmkNumber;
    return (int)_awVTMK4OutBuf[0];
  }
}

int tmkdone(int tmkNumber)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  int iTMK;

  if (tmkNumber == ALL_TMKS)
  {
    for (iTMK = 0; iTMK <= _VTMK4tmkMaxNum; ++iTMK)
    {
      if(_VTMK4usbFlag[iTMK])
      {
        _ahVTMK4Event[iTMK] = 0;
        if(_hVTMK4VxD[tmkNumber])
          CloseHandle (_hVTMK4VxD[tmkNumber]);
        _hVTMK4VxD[iTMK] = 0;
        tmkdone_usb(_VTMK4tmkDrvnum[iTMK]);
      }
      else
      {
        _awVTMK4InBuf[0] = (WORD)iTMK;
        DeviceIoControl(_hVTMK4VxD[iTMK], (DWORD)IOCTL_TMK_tmkdone, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);

        _ahVTMK4Event[iTMK] = 0;
        CloseHandle(_hVTMK4VxD[iTMK]);
        _hVTMK4VxD[iTMK] = 0;
      }
    }
  }
  else if (tmkNumber >= 0 && tmkNumber <= _VTMK4tmkMaxNum)
  {
    if(_VTMK4usbFlag[tmkNumber])
    {
      _ahVTMK4Event[tmkNumber] = 0;
      if(_hVTMK4VxD[tmkNumber])
        CloseHandle (_hVTMK4VxD[tmkNumber]);
      _hVTMK4VxD[tmkNumber] = 0;
      tmkdone_usb(_VTMK4tmkDrvnum[tmkNumber]);
    }
    else
    {
      _awVTMK4InBuf[0] = (WORD)tmkNumber;
      DeviceIoControl(_hVTMK4VxD[tmkNumber], (DWORD)IOCTL_TMK_tmkdone, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);

      _ahVTMK4Event[tmkNumber] = 0;
      CloseHandle (_hVTMK4VxD[tmkNumber]);
      _hVTMK4VxD[tmkNumber] = 0;
    }
  }

  return 0;
}

int tmkselect(int tmkNumber)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;

  if ((_hVTMK4VxD[tmkNumber] != INVALID_HANDLE_VALUE) && (_hVTMK4VxD[tmkNumber] != 0))
  {
    if (_VTMK4usbFlag[tmkNumber])
    {
      _VTMK4tmkCurNum = tmkNumber;
      tmkselect_usb(_VTMK4tmkDrvnum[tmkNumber]);
      return 0;
    }

    _VTMK4tmkCurNum = tmkNumber;

     //**************добавлено для поддержки двухканальных плат**********************************************************
    _awVTMK4InBuf[0] = _VTMK4tmkCurNum; 
    DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkselect, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
    return _awVTMK4OutBuf[0];
    //*******************************************************************************************************************
    //return 0;
  }
  else 
  return TMK_BAD_NUMBER;
}

int tmkselected()
{
	return _VTMK4tmkCurNum;
}

TMK_DATA tmkgetmode()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetmode, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int tmkdefdac(int dacValue)
{

  if ((_hVTMK4VxD[_VTMK4tmkCurNum])&&(_VTMK4usbFlag[_VTMK4tmkCurNum]))
    return tmkdefdac_usb(dacValue);
  else
    return TMK_BAD_NUMBER;

}

int tmkgetdac(int *dacValue, int *dacMode)
{
  if ((_hVTMK4VxD[_VTMK4tmkCurNum])&&(_VTMK4usbFlag[_VTMK4tmkCurNum]))
    return tmkgetdac_usb(dacValue, dacMode);
  else
  {
    *dacValue = 0;
    *dacMode = 0;
    return TMK_BAD_NUMBER;
  }
}

void tmksetcwbits(TMK_DATA tmkSetControl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = tmkSetControl;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmksetcwbits, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void tmkclrcwbits(TMK_DATA tmkClrControl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = tmkClrControl;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkclrcwbits, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA tmkgetcwbits()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetcwbits, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void tmkdefevent(HANDLE hEvent, BOOL fEventSet)
{
  WORD _awVTMK4InBuf[4], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DWORD hVxDEvent;

  int iTMK;
  if (hEvent != 0)
  {
    for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
    {
      if (hEvent == _ahVTMK4Event[iTMK])
        break;
    }
    if (iTMK > MAX_TMK_NUMBER)
    {
      hVxDEvent = (DWORD)hEvent;
      _ahVTMK4Event[_VTMK4tmkNumber] = hEvent;
    }
  }
  else
  {
    hVxDEvent = 0;
    _ahVTMK4Event[_VTMK4tmkNumber] = 0;
  }

//  hVxDEvent = (DWORD)hEvent;
  _ahVTMK4Event[_VTMK4tmkNumber] = hEvent;
  _awVTMK4InBuf[0] = LOWORD(hVxDEvent);
  _awVTMK4InBuf[1] = HIWORD(hVxDEvent);
  _awVTMK4InBuf[2] = LOWORD(fEventSet);
  _awVTMK4InBuf[3] = HIWORD(fEventSet);
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkdefevent, _awVTMK4InBuf, 8, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void tmkgetevd(TTmkEventData *pEvD)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[6];
  DWORD _cbVTMK4Ret;
  if (!_VTMK4usbFlag[_VTMK4tmkCurNum])
  {
        DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetevd, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 12, &_cbVTMK4Ret, NULL);

	    pEvD->nInt = ((DWORD*)(_awVTMK4OutBuf))[0];
		switch (pEvD->wMode = _awVTMK4OutBuf[2])
		{
		case BC_MODE:
			switch (pEvD->nInt)
			{
			case 1:
			pEvD->bc.wResult = _awVTMK4OutBuf[3];
			break;
			case 2:
			pEvD->bc.wResult = _awVTMK4OutBuf[3];
			pEvD->bc.wAW1 = _awVTMK4OutBuf[4];
			pEvD->bc.wAW2 = _awVTMK4OutBuf[5];
			break;
			case 3:
			pEvD->bcx.wResultX = _awVTMK4OutBuf[3];
			pEvD->bcx.wBase = _awVTMK4OutBuf[4];
			break;
			case 4:
			pEvD->bcx.wBase = _awVTMK4OutBuf[3];
			break;
			}
			break;
		case MT_MODE:
			switch (pEvD->nInt)
			{
			case 3:
			pEvD->mt.wResultX = _awVTMK4OutBuf[3];
			pEvD->mt.wBase = _awVTMK4OutBuf[4];
			break;
			case 4:
			pEvD->mt.wBase = _awVTMK4OutBuf[3];
			break;
			}
			break;
		case RT_MODE:
			switch (pEvD->nInt)
			{
			case 1:
			pEvD->rt.wCmd = _awVTMK4OutBuf[3];
			break;
			case 2:
			case 3:
			pEvD->rt.wStatus = _awVTMK4OutBuf[3];
			break;
			}
			break;
		}
  }
  else
  {
        TListEvD evd_s;
	Read_DLL_EvD_usb(&evd_s);

	pEvD->nInt = evd_s.nInt;
	pEvD->wMode = evd_s.wMode;
	switch (pEvD->wMode)
	{
	case BC_MODE:
	    switch (pEvD->nInt)
	    {
	    case 1:
		pEvD->bc.wResult = evd_s.awEvData[0];
		break;
		case 2:
		pEvD->bc.wResult = evd_s.awEvData[0];
		pEvD->bc.wAW1 = evd_s.awEvData[1];
		pEvD->bc.wAW2 = evd_s.awEvData[2];
		break;
		case 3:
		pEvD->bcx.wResultX = evd_s.awEvData[0];
		pEvD->bcx.wBase = evd_s.awEvData[1];
		break;
		case 4:
		pEvD->bcx.wBase = evd_s.awEvData[0];
		break;
		}
		break;
	case MT_MODE:
	    switch (pEvD->nInt)
	    {
	    case 3:
		pEvD->mt.wResultX = evd_s.awEvData[0];
		pEvD->mt.wBase = evd_s.awEvData[1];
		break;
		case 4:
		pEvD->mt.wBase = evd_s.awEvData[0];
		break;
		}
		break;
	case RT_MODE:
	    switch (pEvD->nInt)
	    {
	    case 1:
		pEvD->rt.wCmd = evd_s.awEvData[0];
		break;
		case 2:
		case 3:
		pEvD->rt.wStatus = evd_s.awEvData[0];
		break;
		}
		break;
	}
  }
}

void tmkgetinfo(TTmkConfigData *pConfD)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = LOWORD(pConfD);
  _awVTMK4InBuf[1] = HIWORD(pConfD);
  if (!_VTMK4usbFlag[_VTMK4tmkCurNum])
  {
      DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetinfo, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
  }
  else
  {
	  pConfD->nType = TA;
	  strcpy(pConfD->szName, "TA1USB");
	  pConfD->wPorts1 = 0;//__tmkPortsAddr1[realnum];
	  pConfD->wPorts2 = 0;//__tmkPortsAddr2[realnum];
	  pConfD->wIrq1 = 0;
	  pConfD->wIrq2 = 0;
	  pConfD->wIODelay = 0;
  }

}

int bcreset()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void bc_def_tldw(TMK_DATA wTLDW)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = wTLDW;
  DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bc_def_tldw, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void bc_enable_di()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bc_enable_di, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void bc_disable_di()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bc_disable_di, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int bcdefirqmode(TMK_DATA bcIrqMode)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcIrqMode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefirqmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA bcgetirqmode()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetirqmode, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA bcgetmaxbase()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetmaxbase, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int bcdefbase(TMK_DATA bcBasePC)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcBasePC;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefbase, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA bcgetbase()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetbase, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void bcputw(TMK_DATA bcAddr, TMK_DATA bcData)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcAddr;
  _awVTMK4InBuf[1] = bcData;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcputw, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA bcgetw(TMK_DATA bcAddr)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcAddr;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetw, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

DWORD bcgetansw(TMK_DATA bcCtrlCode)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcCtrlCode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetansw, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}

void bcputblk(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength)
{
  WORD _awVTMK4InBuf[4], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcAddr;
  _awVTMK4InBuf[1] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[2] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[3] = cwLength;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcputblk, _awVTMK4InBuf, 8, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void bcgetblk(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength)
{
  WORD _awVTMK4InBuf[4], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcAddr;
  _awVTMK4InBuf[1] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[2] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[3] = cwLength;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetblk, _awVTMK4InBuf, 8, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int bcdefbus(TMK_DATA bcBus)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcBus;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdefbus, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA bcgetbus()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetbus, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int bcstart(TMK_DATA bcBase, TMK_DATA bcCtrlCode)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcBase;
  _awVTMK4InBuf[1] = bcCtrlCode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcstart, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int bcstartx(TMK_DATA bcBase, TMK_DATA bcCtrlCode)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcBase;
  _awVTMK4InBuf[1] = bcCtrlCode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcstartx, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int bcdeflink(TMK_DATA bcBase, TMK_DATA bcCtrlCode)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = bcBase;
  _awVTMK4InBuf[1] = bcCtrlCode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcdeflink, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

DWORD bcgetlink()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetlink, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}

TMK_DATA bcstop()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcstop, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

DWORD bcgetstate()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetstate, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}

int rtreset()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int rtdefirqmode(TMK_DATA rtIrqMode)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtIrqMode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefirqmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL)
}

TMK_DATA rtgetirqmode()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetirqmode, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int rtdefmode(TMK_DATA rtMode)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtMode;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefmode, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetmode()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetmode, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA rtgetmaxpage()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetmaxpage, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int rtdefpage(TMK_DATA rtPage)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtPage;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpage, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetpage()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetpage, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int rtdefpagepc(TMK_DATA rtPagePC)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtPagePC;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpagepc, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int rtdefpagebus(TMK_DATA rtPageBus)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtPageBus;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefpagebus, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetpagepc()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetpagepc, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA rtgetpagebus()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetpagebus, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int rtdefaddress(TMK_DATA rtAddress)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtAddress;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefaddress, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetaddress()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetaddress, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void rtdefsubaddr(TMK_DATA rtDir, TMK_DATA rtSubAddr)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtDir;
  _awVTMK4InBuf[1] = rtSubAddr;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtdefsubaddr, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetsubaddr()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetsubaddr, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void rtputw(TMK_DATA rtAddr, TMK_DATA rtData)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtAddr;
  _awVTMK4InBuf[1] = rtData;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtputw, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetw(TMK_DATA rtAddr)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtAddr;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetw, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void rtputblk(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength)
{
  WORD _awVTMK4InBuf[4], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtAddr;
  _awVTMK4InBuf[1] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[2] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[3] = cwLength;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtputblk, _awVTMK4InBuf, 8, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtgetblk(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength)
{
  WORD _awVTMK4InBuf[4], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtAddr;
  _awVTMK4InBuf[1] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[2] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[3] = cwLength;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetblk, _awVTMK4InBuf, 8, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtsetanswbits(TMK_DATA rtSetControl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtSetControl;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtsetanswbits, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtclranswbits(TMK_DATA rtClrControl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtClrControl;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtclranswbits, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetanswbits()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetanswbits, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void rtgetflags(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax)
{
  WORD _awVTMK4InBuf[5], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[1] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[2] = rtDir;
  _awVTMK4InBuf[3] = rtFlagMin;
  _awVTMK4InBuf[4] = rtFlagMax;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetflags, _awVTMK4InBuf, 10, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtputflags(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax)
{
  WORD _awVTMK4InBuf[5], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = (WORD)(LOWORD(pcBuffer));
  _awVTMK4InBuf[1] = (WORD)(HIWORD(pcBuffer));
  _awVTMK4InBuf[2] = rtDir;
  _awVTMK4InBuf[3] = rtFlagMin;
  _awVTMK4InBuf[4] = rtFlagMax;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtputflags, _awVTMK4InBuf, 10, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtsetflag()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtsetflag, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtclrflag()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtclrflag, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetflag(TMK_DATA rtDir, TMK_DATA rtSubAddr)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtDir;
  _awVTMK4InBuf[1] = rtSubAddr;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetflag, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA rtgetstate()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetstate, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA rtbusy()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtbusy, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void rtlock(TMK_DATA rtDir, TMK_DATA rtSubAddr)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtDir;
  _awVTMK4InBuf[1] = rtSubAddr;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtlock, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

void rtunlock()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtunlock, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA rtgetcmddata(TMK_DATA rtBusCommand)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtBusCommand;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetcmddata, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}
void rtputcmddata(TMK_DATA rtBusCommand, TMK_DATA rtData)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtBusCommand;
  _awVTMK4InBuf[1] = rtData;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtputcmddata, _awVTMK4InBuf, 4, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int mtreset()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA mtgetsw()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mtgetsw, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

TMK_DATA rtenable(TMK_DATA rtEnable)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = rtEnable;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtenable, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int mrtgetmaxn()
{
  return _VTMK4mrtMaxNum;
/*
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControl(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtgetmaxn, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return ((int)(_awVTMK4OutBuf[0]));
*/
}

DWORD mrtconfig(int mrtNumber)
{
  WORD _awVTMK4InBuf[2], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  WORD nRT,RT0,iRT;
  char tmkName[32];


  if (_hVTMK4VxD[mrtNumber] != 0)
    return 0;

  if (!_VTMK4usbFlag[mrtNumber])
  {	

  sprintf (tmkName,"%c%c.%cTMK1553BDevice%d",0x5c,0x5c,0x5c,mrtNumber);
  _hVTMK4VxD[mrtNumber] = CreateFile(tmkName,
                                     GENERIC_READ | GENERIC_WRITE,
                                     0,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL
                                    );
  if (_hVTMK4VxD[mrtNumber] == INVALID_HANDLE_VALUE)
    return 0;

  _awVTMK4InBuf[0] = (WORD)mrtNumber;
  DeviceIoControl(_hVTMK4VxD[mrtNumber], (DWORD)IOCTL_TMK_mrtconfig, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  if (_awVTMK4OutBuf[0] != 0&&_awVTMK4OutBuf[1] != 0)
  {
    RT0 =  _awVTMK4OutBuf[0];
    nRT =  _awVTMK4OutBuf[1];
    //открыть виртуальные устройства
    for (iRT = RT0; iRT < (RT0+nRT); ++iRT)
    {
      if (_hVTMK4VxD[iRT] != 0)
        return 0;
      sprintf (tmkName,"%c%c.%cTMK1553BDevice%d",0x5c,0x5c,0x5c,iRT);
      _hVTMK4VxD[iRT] = CreateFile(tmkName,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL
                        );
      if (_hVTMK4VxD[iRT] == INVALID_HANDLE_VALUE)
        return 0;
    }//for(iRT = RT0;iRT < (RT0+nRT);iRT++)
  }//if(_awVTMK4OutBuf[0] != 0&&_awVTMK4OutBuf[1] != 0)
  return (_awVTMK4OutBuf[0]+(_awVTMK4OutBuf[1]<<16));
  }
  else 
	return 0;						
}

int mrtselected()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtselected, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return ((int)(_awVTMK4OutBuf[0]));
}

TMK_DATA mrtgetstate()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtgetstate, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

void mrtdefbrcsubaddr0()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCaseVoid(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtdefbrcsubaddr0, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

int mrtreset()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtreset, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}

TMK_DATA tmktimer(TMK_DATA tmkTimerCtrl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = tmkTimerCtrl;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmktimer, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}
DWORD tmkgettimer()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgettimer, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}
TMK_DATA tmkgettimerl()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgettimerl, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}
DWORD bcgetmsgtime()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_bcgetmsgtime, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}
DWORD rtgetmsgtime()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_rtgetmsgtime, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}

TMK_DATA tmkgethwver()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgethwver, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

DWORD tmkgetevtime()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetevtime, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}
TMK_DATA tmkswtimer(TMK_DATA tmkTimerCtrl)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = tmkTimerCtrl;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkswtimer, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}
DWORD tmkgetswtimer()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[2];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmkgetswtimer, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 4, &_cbVTMK4Ret, NULL);
  return (((DWORD*)(_awVTMK4OutBuf))[0]);
}

TMK_DATA tmktimeout(TMK_DATA tmkTimeOut)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = tmkTimeOut;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_tmktimeout, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

int mrtdefbrcpage(TMK_DATA mrtBrcPage)
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  _awVTMK4InBuf[0] = mrtBrcPage;
//  return (DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtdefbrcpage, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL))?0:GetLastError();
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtdefbrcpage, _awVTMK4InBuf, 2, _awVTMK4OutBuf, 0, &_cbVTMK4Ret, NULL);
}
TMK_DATA mrtgetbrcpage()
{
  WORD _awVTMK4InBuf[1], _awVTMK4OutBuf[1];
  DWORD _cbVTMK4Ret;
  DeviceIoControlCase(_hVTMK4VxD[_VTMK4tmkCurNum], (DWORD)IOCTL_TMK_mrtgetbrcpage, _awVTMK4InBuf, 0, _awVTMK4OutBuf, 2, &_cbVTMK4Ret, NULL);
  return _awVTMK4OutBuf[0];
}

DWORD MT_Start(DWORD dwBufSize, HANDLE hEvent)
{
  if (_VTMK4usbFlag[_VTMK4tmkCurNum])
  {
    return MonitorHwStart_usb(dwBufSize, hEvent);
  }
  else
    return TMK_BAD_NUMBER;
}

DWORD MT_GetMessage(WORD * Data, DWORD dwBufSize, int FillFlag, DWORD * dwMsWritten)
{
  if (_VTMK4usbFlag[_VTMK4tmkCurNum])
  {
    return MonitorHwGetMessage_usb(Data, dwBufSize, FillFlag, dwMsWritten);
  }
  else
    return TMK_BAD_NUMBER;
}

DWORD MT_Stop()
{
  if (_VTMK4usbFlag[_VTMK4tmkCurNum])
  {
    return MonitorHwStop_usb();
  }
  else
    return TMK_BAD_NUMBER;
}

#pragma warning (default: 4244) 
#pragma warning (default: 4700) 
#pragma warning (default: 4715) 
#pragma warning (default: 4996) 

//#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define TRANSMIT_BASE               0
#define REQUEST_BASE                1
#define COMMAND_BASE                2
#define REQUEST_CHAIN_START_BASE    8
#define REQUEST_CHAIN_NUM_BASES   256
#define TRANSMIT_CHAIN_START_BASE 264
#define TRANSMIT_CHAIN_NUM_BASES  256
#define MT_START_BASE               0

typedef struct tagMKIOUSB_BC
{
  BOOL           bOpen;
  int            iTmkDevice;
  int            iLastError;
  WORD           uLine;
  WORD           uMaxBaseIndx;
  BOOL           bAnsWord;
  BC_ANSWORD     bfAnsWord;
  BOOL           bState;
  BC_STATEWORD   bfState;
  WORD           uNumLinkedBases[512];
  HANDLE         hEvent;
  TTmkEventData  Evd;
  WORD           uIgnoreAwErrorMask;
  WORD           Cx_Sig;
  WORD           uReqSize ;
  WORD           uReqChainSize ;
  WORD           uReqLinkedBase;
  BOOL           bChain;
  WORD           uLastCommandBaseWord;
} MKIOUSB_BC;

typedef struct tagMKIOUSB_RT
{
  BOOL           bOpen;
  int            iTmkDevice;
  int            iLastError;
  WORD           uMaxPageIndx;
  WORD           uAddress;
  HANDLE         hEvent;
  TTmkEventData  Evd;
  BOOL           bState;
  RT_STATEWORD   bfState;
} MKIOUSB_RT;

typedef struct tagMKIOUSB_MT
{
  BOOL           bOpen;
  int            iTmkDevice;
  int            iLastError;
  WORD           uLine;
  WORD           uMaxBaseIndx;
  BOOL           bState;
  MT_STATEWORD   bfState;
  WORD           uNumLinkedBases[512];
  WORD           uBase;
  int            nIrqCntr;
  HANDLE         hEvent;
  WORD*          pMcoBuffer;
  WORD           uMcoBufSize;
  WORD           uMcoNumFrames;
//  WORD*          pMcoNextFrame;
//  int            iMcoIndxFrame;
  TTmkEventData  Evd;
} MKIOUSB_MT;





typedef struct tagMKIOUSBCTRL
{
  BOOL           bOpen;
  int            numTmkDevices;
  int            iTmkSelected;
  int            iBcSelected;
  int            iRtSelected;
  int            iMtSelected;
  int            numActivatedTmkDevices;
  int            numActivatedBc;
  int            numActivatedRt;
  int            numActivatedMt;
  int            iBcTmk[MKIO_MAX_BC];
  int            iRtTmk[MKIO_MAX_RT];
  int            iMtTmk[MKIO_MAX_MT];
  MKIOUSB_BC     bc[MKIO_MAX_BC];
  MKIOUSB_RT     rt[MKIO_MAX_RT];
  MKIOUSB_MT     mt[MKIO_MAX_MT];
} MKIOUSBCTRL;

typedef struct tagMKIOWRITEWORDFRAME
{
  int     iIndx;
  WORD    uCommand;
  WORD    uSizeFrame;
  __int16 iCountDown;
  WORD    uFrame[256];
} MKIOWRITEWORDFRAME;


MKIOUSBCTRL         MkioUsbCtrl;
SECURITY_ATTRIBUTES MkioUsb_saEvent;
MKIOWRITEWORDFRAME  MkioWriteWordFrame;
BOOL                MkioUsb_bMessageMode=TRUE;


void MkioUsb_Bc_ResetInfo( int iBcIdent )
{
  MkioUsbCtrl.iBcTmk[iBcIdent]                 = -1;
  MkioUsbCtrl.bc[iBcIdent].bOpen               = FALSE;
  MkioUsbCtrl.bc[iBcIdent].iTmkDevice          = -1;
  MkioUsbCtrl.bc[iBcIdent].iLastError          = 0;
//  MkioUsbCtrl.bc[iBcIdent].uLine               = CX_BUS_A;
  MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx= 0;
  MkioUsbCtrl.bc[iBcIdent].bAnsWord            = FALSE;
  MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord     = NULL;
  MkioUsbCtrl.bc[iBcIdent].bState              = FALSE;
  MkioUsbCtrl.bc[iBcIdent].bfState.uWord       = NULL;
  MkioUsbCtrl.bc[iBcIdent].bfState.uInt        = NULL;
  memset( &MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases,1024,0 );
  MkioUsbCtrl.bc[iBcIdent].hEvent              = NULL;
  memset( &MkioUsbCtrl.bc[iBcIdent].Evd,sizeof(TTmkEventData),0 );
  MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask=0xffff;
  MkioUsbCtrl.bc[iBcIdent].Cx_Sig              = CX_SIG;

  MkioUsbCtrl.bc[iBcIdent].bChain              = FALSE;
  MkioUsbCtrl.bc[iBcIdent].uReqSize            = 0;
  MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase      = 0;
  MkioUsbCtrl.bc[iBcIdent].uReqChainSize       = 0;
  MkioUsbCtrl.bc[iBcIdent].uLastCommandBaseWord=0;

  MkioWriteWordFrame.iIndx                     = 0;
  MkioWriteWordFrame.uCommand                  = 0;
  MkioWriteWordFrame.uSizeFrame                = 0;
  MkioWriteWordFrame.iCountDown                = 0;
};


void MkioUsb_Rt_ResetInfo( int iRtIdent )
{
  MkioUsbCtrl.iRtTmk[iRtIdent]         =       -1;
  MkioUsbCtrl.rt[iRtIdent].bOpen       =    FALSE;
  MkioUsbCtrl.rt[iRtIdent].iTmkDevice  =       -1;
  MkioUsbCtrl.rt[iRtIdent].iLastError  =        0;
  MkioUsbCtrl.rt[iRtIdent].uMaxPageIndx=        0;
  MkioUsbCtrl.rt[iRtIdent].uAddress    =        1;
  MkioUsbCtrl.rt[iRtIdent].hEvent      =     NULL;
  MkioUsbCtrl.rt[iRtIdent].bState      =    FALSE;
  MkioUsbCtrl.rt[iRtIdent].bfState.uWord =   NULL;
  memset( &MkioUsbCtrl.rt[iRtIdent].Evd,sizeof(TTmkEventData),0 );
};


void MkioUsb_Mt_ResetInfo( int iMtIdent )
{
  MkioUsbCtrl.iMtTmk[iMtIdent]         =       -1;
  MkioUsbCtrl.mt[iMtIdent].bOpen       =    FALSE;
  MkioUsbCtrl.mt[iMtIdent].iTmkDevice  =       -1;
  MkioUsbCtrl.mt[iMtIdent].iLastError  =        0;
//  MkioUsbCtrl.mt[iMtIdent].uLine       = CX_BUS_A;
  MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx=        0;
  MkioUsbCtrl.mt[iMtIdent].bState        =  FALSE;
  MkioUsbCtrl.mt[iMtIdent].bfState.uWord =   NULL;
  MkioUsbCtrl.mt[iMtIdent].bfState.uInt  =   NULL;
  memset( &MkioUsbCtrl.mt[iMtIdent].uNumLinkedBases,1024,0 );
  MkioUsbCtrl.mt[iMtIdent].hEvent       =     NULL;
  MkioUsbCtrl.mt[iMtIdent].uBase        =     NULL;
  MkioUsbCtrl.mt[iMtIdent].nIrqCntr     =     NULL;
  MkioUsbCtrl.mt[iMtIdent].pMcoBuffer   =     NULL;
  MkioUsbCtrl.mt[iMtIdent].uMcoBufSize  =     NULL;
  MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames=     NULL;
//  MkioUsbCtrl.mt[iMtIdent].pMcoNextFrame=     NULL;
//  MkioUsbCtrl.mt[iMtIdent].iMcoIndxFrame=     NULL;

  memset( &MkioUsbCtrl.mt[iMtIdent].Evd,sizeof(TTmkEventData),0 );
};




BOOL MkioUsb_Open()
{
  int i;
  MkioUsbCtrl.bOpen                  =FALSE;
  MkioUsbCtrl.numTmkDevices          =  0;
  MkioUsbCtrl.iTmkSelected           = -1;
  MkioUsbCtrl.iBcSelected            = -1;
  MkioUsbCtrl.iRtSelected            = -1;
  MkioUsbCtrl.numActivatedTmkDevices =  0;
  MkioUsbCtrl.numActivatedBc         =  0;
  MkioUsbCtrl.numActivatedRt         =  0;
  MkioUsbCtrl.numActivatedMt         =  0;

  for( i=0; i<MKIO_MAX_BC; i++ )  MkioUsbCtrl.iBcTmk[i]= -1;
  for( i=0; i<MKIO_MAX_RT; i++ )  MkioUsbCtrl.iRtTmk[i]= -1;
  for( i=0; i<MKIO_MAX_MT; i++ )  MkioUsbCtrl.iMtTmk[i]= -1;


  for( i=0; i<MKIO_MAX_BC; i++ )  MkioUsb_Bc_ResetInfo( i );
  for( i=0; i<MKIO_MAX_RT; i++ )  MkioUsb_Rt_ResetInfo( i );
  for( i=0; i<MKIO_MAX_MT; i++ )  MkioUsb_Mt_ResetInfo( i );

  if( TmkOpen()!=0 ) return FALSE;

  MkioUsbCtrl.bOpen=TRUE;
  MkioUsbCtrl.numTmkDevices= tmkgetmaxn()+1;
  return TRUE ;
};

//  BOOL Service_bDriverOpened;
//  BOOL Service_bBcCreated;


void MkioUsb_Close()
{
  int i;
  MkioUsbCtrl.bOpen                  =FALSE;
  MkioUsbCtrl.numTmkDevices          =  0;
  MkioUsbCtrl.iTmkSelected           = -1;
  MkioUsbCtrl.iBcSelected            = -1;
  MkioUsbCtrl.iRtSelected            = -1;
  MkioUsbCtrl.numActivatedTmkDevices =  0;
  MkioUsbCtrl.numActivatedBc         =  0;
  MkioUsbCtrl.numActivatedRt         =  0;

  for( i=0; i<MKIO_MAX_BC; i++ )  MkioUsbCtrl.iBcTmk[i]= -1;
  for( i=0; i<MKIO_MAX_RT; i++ )  MkioUsbCtrl.iRtTmk[i]= -1;

  for( i=0; i<MKIO_MAX_BC; i++ )  MkioUsb_Bc_ResetInfo( i );
  for( i=0; i<MKIO_MAX_RT; i++ )  MkioUsb_Rt_ResetInfo( i );
  for( i=0; i<MKIO_MAX_MT; i++ )  MkioUsb_Mt_ResetInfo( i );
  TmkClose();
};


int MkioUsb_GetMaxDeviceAmount()
{
  return MkioUsbCtrl.numTmkDevices;
};

BOOL MkioUsb_SetMessageMode( BOOL bMessageMode )
{
 BOOL                   bRet;
 bRet                 = MkioUsb_bMessageMode;
 MkioUsb_bMessageMode = bMessageMode;
 return bRet;
};


int MkioUsb_MessageBox(
                        HWND hWnd,          // handle of owner window
                        LPCTSTR lpText,     // address of text in message box
                        LPCTSTR lpCaption,  // address of title of message box
                        UINT uType          // style of message box
                      )
{
  int iRet;
  if( MkioUsb_bMessageMode )
  {
    iRet = ::MessageBox( hWnd, lpText, lpCaption, uType );
  }
  else
  {
    iRet = MB_OK;
  };
  return iRet;
};



BOOL MkioUsb_IsDeviceFree( int iTmkDevice )
{
  int i;
  for( i=0; i<MKIO_MAX_BC; i++ )
  {
    if( MkioUsbCtrl.iBcTmk[i]       ==iTmkDevice ) return FALSE;// это одно
//  if( MkioUsbCtrl.bc[i].iTmkDevice==iTmkDevice ) return FALSE;// и то  же
  };
  for( i=0; i<MKIO_MAX_RT; i++ )
  {
    if( MkioUsbCtrl.iRtTmk[i]       ==iTmkDevice ) return FALSE;// это одно
//  if( MkioUsbCtrl.rt[i].iTmkDevice==iTmkDevice ) return FALSE;// и то  же
  };
  for( i=0; i<MKIO_MAX_MT; i++ )
  {
    if( MkioUsbCtrl.iMtTmk[i]       ==iTmkDevice ) return FALSE;// это одно
//  if( MkioUsbCtrl.rt[i].iTmkDevice==iTmkDevice ) return FALSE;// и то  же
  };
  return TRUE;
};

int MkioUsb_Bc_GetIdent( int iTmkDevice )
{
  int iBcIdent;
  int i;

  iBcIdent= -1;
  if( MkioUsb_IsDeviceFree(iTmkDevice)==TRUE ) return -1;
  for( i=0; i<MKIO_MAX_BC; i++ )
  {
    if( MkioUsbCtrl.iBcTmk[i]==iTmkDevice )
    {
      iBcIdent=i;
      return iBcIdent;
    };
  };
  return -1;
};


int MkioUsb_TmkSelect( int iTmkDevice )
{
  int iRet;
  if( iTmkDevice==MkioUsbCtrl.iTmkSelected )
  {
    return 0;
  }
  iRet = tmkselect( iTmkDevice );
  if( iRet == 0 )
  {
    MkioUsbCtrl.iTmkSelected=iTmkDevice;
    return 1;
  }
  else
  {
    MkioUsbCtrl.iTmkSelected=        -1;
    return -1;
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////

int MkioUsb_Bc_GetLastError()
{
  return 0;
};

BOOL MkioUsb_Bc_Select( int iBcIdent )
{
  int iRet,iTmkDevice;
  if( iBcIdent<0 || iBcIdent>=MKIO_MAX_BC ) return FALSE;
  iTmkDevice = MkioUsbCtrl.bc[iBcIdent].iTmkDevice;
  iRet       = MkioUsb_TmkSelect( iTmkDevice );
  switch( iRet )
  {
    case  0: break;
    case  1: tmkdefevent( MkioUsbCtrl.bc[iBcIdent].hEvent,TRUE); break;
    case -1: 
    default: MkioUsbCtrl.iBcSelected =-1; return FALSE;
  };
  MkioUsbCtrl.iBcSelected = iBcIdent;
  return TRUE;
};




int MkioUsb_Bc_CreateDevice( int iTmkDevice )
{
  WORD i;
  int  iRet,iBcIdent;

  if( MkioUsbCtrl.numActivatedTmkDevices>=MkioUsbCtrl.numTmkDevices )   goto error_0;
  if( iTmkDevice<0 || iTmkDevice>=MkioUsbCtrl.numTmkDevices         )   goto error_0;
  if( MkioUsb_IsDeviceFree(iTmkDevice )== FALSE                     )   goto error_0;

  iBcIdent = -1;
  for( i=0; i<MKIO_MAX_BC; i++ )
  {
    if( MkioUsbCtrl.iBcTmk[i]==-1 )
    {
      iBcIdent = i;
      break;
    };
  };  if( iBcIdent<0 ) goto error_0;

      MkioUsbCtrl.bc[iBcIdent].hEvent= CreateEvent(NULL, TRUE, FALSE, NULL);
  if( MkioUsbCtrl.bc[iBcIdent].hEvent== NULL )                          goto error_1;

  iRet = tmkconfig( iTmkDevice );                         if( iRet!=0 ) goto error_2;
  if( !MkioUsb_TmkSelect(iTmkDevice) )                                  goto error_3;
  bcreset();

  MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx = bcgetmaxbase();
  MkioUsbCtrl.bc[iBcIdent].iTmkDevice   = MkioUsbCtrl.iBcTmk[iBcIdent]= iTmkDevice;
  MkioUsbCtrl.bc[iBcIdent].bOpen        = TRUE;
  tmkdefevent( MkioUsbCtrl.bc[iBcIdent].hEvent,TRUE);
  MkioUsbCtrl.iBcSelected = iBcIdent;
  MkioUsbCtrl.numActivatedBc++;
  MkioUsbCtrl.numActivatedTmkDevices++;

  return iBcIdent;
error_3:  tmkdone( iTmkDevice );
error_2:  CloseHandle(MkioUsbCtrl.bc[iBcIdent].hEvent);
error_1:
error_0:  MkioUsbCtrl.iBcSelected = -1;
  return -1;
};


void MkioUsb_Bc_Free( int iBcIdent )
{
  int iTmkDevice;
  if( iBcIdent<0 || iBcIdent>=MKIO_MAX_BC ) return;

  iTmkDevice = MkioUsbCtrl.bc[iBcIdent].iTmkDevice;

  if( MkioUsb_IsDeviceFree( iTmkDevice )  ) return;

  MkioUsb_Bc_Select(iBcIdent);
  tmkdefevent( NULL,NULL);//TRUE );
  ResetEvent(MkioUsbCtrl.bc[iBcIdent].hEvent);
  CloseHandle( MkioUsbCtrl.bc[iBcIdent].hEvent );
//+++  tmkdone( iTmkDevice );

  MkioUsbCtrl.iBcTmk[iBcIdent]= MkioUsbCtrl.bc[iBcIdent].iTmkDevice  =-1;
  if( iTmkDevice==MkioUsbCtrl.iTmkSelected ) MkioUsbCtrl.iTmkSelected=-1;

  MkioUsbCtrl.numActivatedTmkDevices--;
  MkioUsbCtrl.numActivatedBc--;

  MkioUsbCtrl.bc[iBcIdent].bOpen=FALSE;
  MkioUsb_Bc_ResetInfo( iBcIdent );
};

BOOL MkioUsb_Bc_SelectLine( WORD uLine )
{
  int j = MkioUsbCtrl.iBcSelected;
  switch( uLine )
  {
    default:
    case  0: MkioUsbCtrl.bc[j].uLine = CX_BUS_A; break;
    case  1: MkioUsbCtrl.bc[j].uLine = CX_BUS_B; break;
  };
  return TRUE;
};

BOOL MkioUsb_Bc_SelectLineForStart( int iBcIdent )
{
 int iUseBus;
 int iRet;
 switch( MkioUsbCtrl.bc[iBcIdent].uLine )
 {
   default       :
   case CX_BUS_A : iUseBus = BUS_A;break;
   case CX_BUS_B : iUseBus = BUS_B;break;
 };
 iRet = bcdefbus( iUseBus );
 if( iRet!=0 ) return FALSE;
 return FALSE;
};



BOOL MkioUsb_Bc_SetCommandData( WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData )
{
  int   iBcIdent,i, iRet;
  WORD  uCmdWord ;
  WORD  uNumNeedBases, uBase, uTail, uSzPortion ;
  WORD  uPortion[33];
  WORD* pWord;

  if(!MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  iBcIdent = MkioUsbCtrl.iBcSelected;

  uNumNeedBases = nData/32; if((nData%32)>0) uNumNeedBases++; // max(32)
  if( uNumNeedBases >  TRANSMIT_CHAIN_NUM_BASES ) return FALSE;
  if( uNumNeedBases > (MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx+1) ) return FALSE ;
  if( uNumNeedBases > 1 )
  {
    uTail = nData;
    pWord = pData;
    for( uBase=0; uBase<uNumNeedBases; uBase++ )
    {
      uSzPortion = uTail; if( uSzPortion>32 ) uSzPortion=32;
      uCmdWord   = CW( uAdrRT, RT_RECEIVE, uSubAdr, uSzPortion );
// Grek >> (01.06.2011)
#ifdef GREK_INC
			uPortion[i=0] = uCmdWord;
			memcpy(static_cast<void*>(&uPortion[1]), static_cast<const void*>(pWord), uSzPortion*sizeof(WORD));
			pWord += uSzPortion;
#else
      for( i=0; i<(uSzPortion+1); i++ )
      {
        if( i==0 ) uPortion[i]=  uCmdWord;
        else
        {
          uPortion[i]= *pWord;
          pWord++;
        };
      };
#endif
// Grek <<

      iRet = bcdefbase( TRANSMIT_CHAIN_START_BASE + uBase );if( iRet!=0 ) return FALSE;
      bcputblk(  0, uPortion, uSzPortion+1 );
      uTail -= uSzPortion;
    };
    MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[TRANSMIT_CHAIN_START_BASE]= uNumNeedBases;
  }
  else
  {
    pWord      = pData;
    uSzPortion = nData;
    uCmdWord   = uPortion[0]= CW( uAdrRT, RT_RECEIVE, uSubAdr, uSzPortion );
// Grek >> (01.06.2011)
#ifdef GREK_INC
		memcpy(static_cast<void*>(&uPortion[1]), static_cast<const void*>(pWord), uSzPortion*sizeof(WORD));
		pWord += uSzPortion;
#else
    for( i=0; i<uSzPortion; i++ )
    {
      uPortion[i+1]= *pWord;
      pWord++;
    };
#endif
// Grek <<
    bcdefbase( TRANSMIT_CHAIN_START_BASE );
    bcputblk(  0, uPortion, uSzPortion+1 );
    MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[TRANSMIT_CHAIN_START_BASE]= 1;
  };
  return TRUE;
};



BOOL MkioUsb_Bc_WaitEvent( int iBcIdent, WORD uDirect )
{
//  WORD uAwErrorMask;
//  WORD uAwIgnoreErrorResult;
  BC_STATEWORD sw;
  BC_ANSWORD   aw;

  MkioUsbCtrl.bc[iBcIdent].bAnsWord = FALSE;
  MkioUsbCtrl.bc[iBcIdent].bState   = FALSE;
  MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = 0xffff;

  switch ( WaitForSingleObject(MkioUsbCtrl.bc[iBcIdent].hEvent,1000))
  {
    default           :
      {
      return FALSE;
      };
    case WAIT_TIMEOUT :
      {
      return FALSE;
      };
    case WAIT_OBJECT_0: ResetEvent(MkioUsbCtrl.bc[iBcIdent].hEvent); break;
  };
  tmkgetevd(&MkioUsbCtrl.bc[iBcIdent].Evd);

  switch( MkioUsbCtrl.bc[iBcIdent].Evd.nInt )
  {
    default: return FALSE;
    case  1: // bcIntNorm - Нормальное завершение одиночного обмена
      {
        MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
        MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
        MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = LOWORD(bcgetansw(uDirect));
        return TRUE;
      };break;
    case  2: // bcIntExec - завершение с исключит.ситуацией, в т.ч. с битами в ОС
      {
        MkioUsbCtrl.bc[iBcIdent].bState                    = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfState.uInt              = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
        MkioUsbCtrl.bc[iBcIdent].bfState.uWord  = sw.uWord = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
        MkioUsbCtrl.bc[iBcIdent].bAnsWord                  = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord= aw.uWord = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wAW1;

        // Есть ли другие ошибки в слове рез.обмена, кроме ошибки выст.битов ОС?
        sw.Res.uAwBitError = 0; 
        if( sw.uWord!=0 ) return FALSE;// Да, есть!

        // Выставлена ли маска игнорирования битов ОС
        if( MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask!=0xffff )
        {// Да, выставлена

          // Выставлен ли бит наличия ош.битов в ОС 
          if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uAwBitError!=0 )
          { // Да, выставлен!

            // Остались ли биты  в ОС после наложения маски игнор-ия    
            aw.uWord &= 0x40D;// Погасим все биты ОС, кроме битов ошибок!
            aw.uWord &= MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask;
            if( aw.uWord != 0 ) return FALSE;// Да, остались!
            return TRUE;// Нет, не остались
          }
          else
          { // Нет, не выставлен
            return TRUE;
          };
        }
        else
        {// Нет, не выставлена, т.е. ==0xFFFF
          // Выставлен ли бит наличия ош.битов в ОС 
          if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uAwBitError!=0 )
          { // Да, выставлен!
            return FALSE;
          }
          else
          { // Нет, не выставлен
            return TRUE;
          };
        };
      };break;
    case  3: // bcIntX    - завершение(останов цепочки) обмена
      {
        MkioUsbCtrl.bc[iBcIdent].bState                    = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfState.uInt              = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
        MkioUsbCtrl.bc[iBcIdent].bfState.uWord  = sw.uWord = MkioUsbCtrl.bc[iBcIdent].Evd.bcx.wResultX;
        MkioUsbCtrl.bc[iBcIdent].bAnsWord                  = TRUE;
        MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord= aw.uWord = LOWORD(bcgetansw(uDirect));
        // Есть ли другие ошибки в слове рез.обмена, кроме ошибки выст.битов ОС?
        sw.ResX.uAwBitError = 0; 
        if( sw.uWord!=0 ) return FALSE;// Да, есть!

        // Выставлена ли маска игнорирования битов ОС
        if( MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask!=0xffff )
        {// Да, выставлена

          // Выставлен ли бит наличия ош.битов в ОС 
          if( MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uAwBitError!=0 )
          { // Да, выставлен!

            // Остались ли биты  в ОС после наложения маски игнор-ия 
            aw.uWord &= 0x40D;// Погасим все биты ОС, кроме битов ошибок!
            aw.uWord &= MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask;
            if( aw.uWord != 0 ) return FALSE;// Да, остались!
            return TRUE;// Нет, не остались
          }
          else
          { // Нет, не выставлен
            return TRUE;
          };
        }
        else
        {// Нет, не выставлена, т.е. ==0xFFFF
          // Выставлен ли бит наличия ош.битов в ОС 
          if( MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uAwBitError!=0 )
          { // Да, выставлен!
            return FALSE;
          }
          else
          { // Нет, не выставлен
            return TRUE;
          };
        };
      };break;
  };
  return FALSE;
};





char* MkioUsb_pszBcStateBitList[]=
{
 "    - установлен бит ошибки в ОС\n",
 "    - генерация по основной ЛПИ\n",
 "    - генерация по резервной ЛПИ B\n"
};

char* MkioUsb_pszBcStateErrorList[]=
{
 "    - нет ошибки статуса \n",
 "    - ошибка кода <Манчестер-2>\n",
 "    - ошибка паузы до ОС (нет ответа ОУ)\n",
 "    - ошибка паузы до ИС (число ИС меньше заданного)\n",
 "    - ошибка число ИС больше заданного\n",
 "    - ошибка адреса ОУ в ОС\n",
 "    - ошибка типа синхроимпульса\n",
 "    - ошибка эхоконтроля\n"
};


char* MkioUsb_pszBcAwList[]=
{
 "    - Неисправность ОУ\n",
 "    - Принято управление\n",
 "    - Неисправность Абонента\n",
 "    - Абонент Занят \n",
 "    - Принята групповая команда\n",
 "    - Запрос обслуживания\n",
 "    - Передача ОС\n",
 "    - Ошибка в сообщении\n"
};


BOOL MkioUsb_Bc_CreateErrorListStringX( int iBcIdent, char* pString, char* pExString )
{
  char pszString[1024];
  char s[1024];
		unsigned short uErr;

		lstrcpy( pszString,"\0");

  if( MkioUsbCtrl.bc[iBcIdent].bState==FALSE )
		{
    lstrcpy( pszString,"Нет событий!");
				lstrcpy( pString,pszString );
			 return FALSE;
		};
  lstrcpy(pszString,"      РЕЗУЛЬТАТ ОБМЕНА ПО MKИО\n");
  lstrcat(pszString,"  \n");
		if( pExString!=NULL )
		{
    lstrcat(pszString,pExString);
    lstrcat(pszString,"  \n");
		};

  lstrcat(pszString," > Слово расширенного результата обмена:\n");

  if( MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uAwBitError ) lstrcat( pszString,MkioUsb_pszBcStateBitList[0] );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uGenLine1   ) lstrcat( pszString,MkioUsb_pszBcStateBitList[1] );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uGenLine2   ) lstrcat( pszString,MkioUsb_pszBcStateBitList[2] );
		uErr = MkioUsbCtrl.bc[iBcIdent].bfState.ResX.uError;	
		if( uErr > 0 ) lstrcat( pszString,MkioUsb_pszBcStateErrorList[uErr] );

  if( MkioUsbCtrl.bc[iBcIdent].bAnsWord ==FALSE )
		{
    lstrcat(pszString,"\n     - Отсутствие Ответного Слова");
				lstrcpy( pString,pszString );
			 return TRUE;
		};
  lstrcat(pszString,"  \n");
  lstrcat(pszString," > Ответное Слово ОУ:\n");
		sprintf( s,"    - Адрес ОУ: %6u (0x%2X)\n",MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAddress,
			                                          MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAddress);lstrcat(pszString,s);
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uRtInvalid     ) lstrcat(pszString,MkioUsb_pszBcAwList[0] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uGetControl    ) lstrcat(pszString,MkioUsb_pszBcAwList[1] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAbonentInvalid) lstrcat(pszString,MkioUsb_pszBcAwList[2] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uBusy          ) lstrcat(pszString,MkioUsb_pszBcAwList[3] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uGroupCommand  ) lstrcat(pszString,MkioUsb_pszBcAwList[4] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uServiceRequest) lstrcat(pszString,MkioUsb_pszBcAwList[5] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uTransmitAw    ) lstrcat(pszString,MkioUsb_pszBcAwList[6] );
		lstrcpy( pString,pszString );
	 return TRUE;
};

BOOL MkioUsb_Bc_CreateErrorListStringS( int iBcIdent, char* pString, char* pExString )
{
  char pszString[1024];
  char s[1024];
//		unsigned short uErr;

		lstrcpy( pszString,"\0");

  if( MkioUsbCtrl.bc[iBcIdent].bState==FALSE )
		{
    lstrcpy( pszString,"Нет событий!");
				lstrcpy( pString,pszString );
			 return FALSE;
		};
  lstrcpy(pszString,"      РЕЗУЛЬТАТ ОБМЕНА ПО MKИО\n");
  lstrcat(pszString,"  \n");
		if( pExString!=NULL )
		{
    lstrcat(pszString,pExString);
    lstrcat(pszString,"  \n");
		};

  lstrcat(pszString," > Слово результата обмена:\n");

  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uAdrError   ) lstrcat( pszString,"S_ERAO_MASK -ошибка адреса ОУ в ОС" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uCodeError  ) lstrcat( pszString,"S_MEO_MASK  -ошибка кода Манчестер-2" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uAwBitError ) lstrcat( pszString,"S_IB_MASK   -установлен бит в ОС" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uPauseError ) lstrcat( pszString,"S_TO_MASK   -ошибка паузы до ОС или ИС (нет ответа)" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uMemError   ) lstrcat( pszString,"S_EM_MASK   -ошхибка обмена с ДОЗУ" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uAbsError   ) lstrcat( pszString,"S_EBC_MASK  -ошибка самоконтроля или составная ошибка" );
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uDi         ) lstrcat( pszString,"S_DI_MASK   -не описана в документации");
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uEln        ) lstrcat( pszString,"S_ELN_MASK  -не описана в документации");  
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uG1         ) lstrcat( pszString,"S_G1_MASKM  -не описана в документации");
  if( MkioUsbCtrl.bc[iBcIdent].bfState.Res.uG2         ) lstrcat( pszString,"S_G2_MASKM  -не описана в документации");
  if( MkioUsbCtrl.bc[iBcIdent].bAnsWord ==FALSE )
  lstrcat(pszString,"  \n");
  lstrcat(pszString," > Ответное Слово ОУ:\n");
  sprintf( s,"    - Адрес ОУ: %6u (0x%2X)\n",MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAddress,
                                             MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAddress);
                                                             lstrcat(pszString,s);
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uRtInvalid     ) lstrcat(pszString,MkioUsb_pszBcAwList[0] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uGetControl    ) lstrcat(pszString,MkioUsb_pszBcAwList[1] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uAbonentInvalid) lstrcat(pszString,MkioUsb_pszBcAwList[2] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uBusy          ) lstrcat(pszString,MkioUsb_pszBcAwList[3] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uGroupCommand  ) lstrcat(pszString,MkioUsb_pszBcAwList[4] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uServiceRequest) lstrcat(pszString,MkioUsb_pszBcAwList[5] );
  if( MkioUsbCtrl.bc[iBcIdent].bfAnsWord.Bf.uTransmitAw    ) lstrcat(pszString,MkioUsb_pszBcAwList[6] );
  lstrcpy( pString,pszString );
  return TRUE;
};



BOOL MkioUsb_Bc_CreateErrorListString( int iBcIdent, char* pString, char* pExString )
{
  switch(MkioUsbCtrl.bc[iBcIdent].bfState.uInt)
  {
  default: 
		{
          lstrcpy( pString,"Нет событий!");
		  return FALSE;
		};
  case 1:
  case 2: return MkioUsb_Bc_CreateErrorListStringS( iBcIdent, pString, pExString );
  case 3: return MkioUsb_Bc_CreateErrorListStringX( iBcIdent, pString, pExString );
  };
};





BOOL MkioUsb_Bc_GetLastAnsWord( int iBcIdent, WORD* pAnsWord )
{
		if( MkioUsbCtrl.bc[iBcIdent].bAnsWord==FALSE ) return FALSE;
 *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
  return TRUE;
};


BOOL MkioUsb_Bc_RunCommand()
{
			WORD  uMode;
			BOOL  bRet ;
			int   iRet;
			WORD  uBase;

   if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
			int iBcIdent = MkioUsbCtrl.iBcSelected;

  	uMode   = DATA_BC_RT|CX_STOP|CX_SIG|(MkioUsbCtrl.bc[iBcIdent].uLine);
//			uMode  |=(MkioUsbCtrl.bc[iBcIdent].Cx_Sig);
//   uMode  |=(MkioUsbCtrl.bc[iBcIdent].uLine);
			for( uBase=0; uBase<MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[TRANSMIT_CHAIN_START_BASE]; uBase++ )
			{
				iRet = bcdefbase( TRANSMIT_CHAIN_START_BASE+uBase );    if( iRet!=0 ) return FALSE;
				iRet = bcstartx(  TRANSMIT_CHAIN_START_BASE+uBase,uMode );
				switch( iRet )
					{
							case            0: break;
							case  BC_BAD_BASE: return FALSE;
							case  BC_BAD_FUNC: return FALSE;
							default          : return FALSE;
					};
     bRet= MkioUsb_Bc_WaitEvent(iBcIdent,DATA_BC_RT);
					                                    if( !bRet   ) return FALSE;
			};
			return TRUE;
};


BOOL MkioUsb_Bc_SendCommand( WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData )
{
  if( !MkioUsb_Bc_SetCommandData( uAdrRT,uSubAdr,nData,pData ) ) return FALSE;
  if( !MkioUsb_Bc_RunCommand() ) return FALSE;
  return TRUE;
};

BOOL MkioUsb_Bc_SendControlCommand( WORD uCommandID, WORD uRtAdr, WORD* pAnsWord )
{
  WORD uMode;
  WORD uCmdWord;
  WORD uAdr;
  WORD uCommand;
  int  iBcIdent;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  iBcIdent = MkioUsbCtrl.iBcSelected;

// Контролируем готовность канала к передаче запрошенных данных, готовность
//        канала индицируется снятием бита "Неисправность ОУ" в ОС.
//
  ResetEvent(MkioUsbCtrl.bc[iBcIdent].hEvent);
  uCommand =   uCommandID; // Команда "Установить ОУ в исх.состояние"
  uAdr     =  (uRtAdr&0x1F)<<11; 
  uCmdWord =   uAdr  |   0x07e0;
  uCmdWord|=   uCommand;

  uMode    = CTRL_C_A;// | CX_STOP | CX_SIG | MkioUsbCtrl.bc[iBcIdent].uLine;//CX_BUS_A;//+++MkioUsbCtrl.bc[iBcIdent].uLine;

  bcdefbase( COMMAND_BASE );
  if( uCmdWord!=MkioUsbCtrl.bc[iBcIdent].uLastCommandBaseWord )
  {
    bcputblk( 0, &uCmdWord, 1 );
    MkioUsbCtrl.bc[iBcIdent].uLastCommandBaseWord= uCmdWord;
  };

  bcdefbase( COMMAND_BASE );
  MkioUsb_Bc_SelectLineForStart( iBcIdent );

  bcstart( COMMAND_BASE,uMode );
  switch ( WaitForSingleObject(MkioUsbCtrl.bc[iBcIdent].hEvent,1000))
  {
   default           :
	   {
		   return FALSE;
	   };
   case WAIT_TIMEOUT :
	   {
		   return FALSE;
	   };
   case WAIT_OBJECT_0: ResetEvent(MkioUsbCtrl.bc[iBcIdent].hEvent);break;
  };
  tmkgetevd(&MkioUsbCtrl.bc[iBcIdent].Evd);
/*
S_ERAO_MASK	ошибка адреса ОУ в ОС
S_MEO_MASK	ошибка кода "Манчестер-2"
S_IB_MASK		установлен бит в ОС
S_TO_MASK	ошибка паузы до ОС (нет ответа) или до любого ИС (нет или мало ИС)
S_EM_MASK	ошибка обмена с ДОЗУ (только на TMK400, RTMK400, TMKMPC)
S_EBC_MASK	ошибка самоконтроля при передаче или составная ошибка при приеме
*/
  switch( MkioUsbCtrl.bc[iBcIdent].Evd.nInt )
  {
  default: return FALSE;
  case  1: // bcIntNorm - Нормальное завершение одиночного обмена, этот ти прерывания возникает
   {       //             при наличии готовности данных на изделии( reset FL2 )
           //             Норма: ОУ присутствует, биты ОС не выставлены
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
   	 MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = LOWORD(bcgetansw(CTRL_C_A));
    *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
     return TRUE;
   };break;
  case  2: // bcIntExec - завершение с исключит.ситуацией, это прерывание возникает при выставленном
   {       //             бите <Неисправность ОУ> в ОС, который сигнализирует о неготовности данных
           //             на изделии( set FL2 )
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_ERAO_MASK )
     {//S_ERAO_MASK	ошибка адреса ОУ в ОС
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wAW1;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return FALSE;
     };
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_MEO_MASK )
     {//S_MEO_MASK	ошибка кода "Манчестер-2"
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         =  FALSE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = 0x0000;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return FALSE;
     };
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_IB_MASK )
     {//S_IB_MASK		установлен бит в ОС
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wAW1;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return TRUE;
     };
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_TO_MASK )
     {//S_TO_MASK	ошибка паузы до ОС (нет ответа) или до любого ИС (нет или мало ИС)
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         =  FALSE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = 0x0000;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return FALSE;
     };
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_EM_MASK )
     {//S_EM_MASK	ошибка обмена с ДОЗУ (только на TMK400, RTMK400, TMKMPC)
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         =  FALSE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = 0x0000;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return FALSE;
     };
     if( MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult & S_EBC_MASK )
     {//S_EBC_MASK	ошибка самоконтроля при передаче или составная ошибка при приеме
  	    MkioUsbCtrl.bc[iBcIdent].bAnsWord         =  FALSE;
       MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = 0x0000;
      *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
       return FALSE;
     };
     return FALSE;
   };break;
  case  3: // bcIntX    - завершение(останов) цепочки обмена
   {// При запросе Готовности этоn тип прерывания возникнуть не может, поэтому ошибка
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bcx.wResultX;
  	  MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = LOWORD(bcgetansw(CTRL_C_A));
    *pAnsWord = MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord;
     return FALSE;
   };break;
  };

  return TRUE;
};



BOOL MkioUsb_Bc_RequestAnswWord( WORD uRtAdr, WORD* pAnsWord )
{
		BOOL bRet;
		bRet = MkioUsb_Bc_SendControlCommand( 0x0002, uRtAdr, pAnsWord );
//+++		bRet = MkioUsb_Bc_SendControlCommand( 0x0008, uRtAdr, pAnsWord );
  return bRet;
};


int  MkioUsb_Bc_FindSlaves( WORD* pAdrSlaves )
{
	WORD uAdr;
	WORD uAnsWord;
	int  iNumSlaves;
	BOOL bRet;

 iNumSlaves = 0;

	for( uAdr=0; uAdr<=30; uAdr++ )
	{
   bRet = MkioUsb_Bc_RequestAnswWord( uAdr, &uAnsWord );
			if( bRet )
			{
				 pAdrSlaves[iNumSlaves] = uAdr;
     iNumSlaves++;
			};
	};
	return iNumSlaves;
};

BOOL MkioUsb_Bc_SetDataAtBase( WORD uBase, WORD uAdrRT, WORD uSubAdr, WORD nData, WORD* pData )
{
  int   i;
  WORD  uCmdWord ;
  WORD  uPortion[33];
  WORD* pWord;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  int iBcIdent = MkioUsbCtrl.iBcSelected;

  if( uBase > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx )  return FALSE;
  if( nData > 32                 )  return FALSE;
  if( bcreset()                  )  return FALSE;

  pWord      = pData    ;
  uCmdWord   = uPortion[0]= CW( uAdrRT, RT_RECEIVE, uSubAdr, nData );
// Grek >> (01.06.2011)
#ifdef GREK_INC
	memcpy(static_cast<void*>(&uPortion[i=1]), static_cast<const void*>(pWord), nData*sizeof(WORD));
	pWord += nData;
#else
  for( i=0; i<nData; i++ )
  {
    uPortion[i+1]= *pWord;
    pWord++;
  };
#endif
// Grek <<
  bcdefbase( uBase );
  bcputblk(  0, uPortion, nData+1 );
  return TRUE;
};

/*
   На ТА1-USB-01 эта функция работает, но для работы по МКИО с нашими
			изделиями ее применение бессмысленно, т.к. наличие бита "Неисправность ОУ" 
			в ОС остановит передачу цепочки после первой же порции данных.
*/
BOOL MkioUsb_Bc_LinkBases( WORD uBaseStart, WORD uBaseEnd )
{
 int  iRet;
	WORD uBase, uMode;

 if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
	int iBcIdent = MkioUsbCtrl.iBcSelected;

 if( uBaseStart > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseEnd   > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseStart>=uBaseEnd ) return FALSE;

 for( uBase=uBaseStart+1; uBase<=uBaseEnd; uBase++ )
	{
   uMode   = DATA_BC_RT ;
   uMode  |= (MkioUsbCtrl.bc[iBcIdent].uLine);
			if( uBase==uBaseEnd )
			{
   			uMode |= CX_STOP;
   			uMode |=(MkioUsbCtrl.bc[iBcIdent].Cx_Sig);
			}
			else
			{
   			uMode |= CX_CONT;
   			uMode |= CX_NOSIG;
			};
   bcdefbase( uBase-1 );	iRet = bcdeflink( uBase,uMode );
			switch( iRet )
			{
					case            0: break;
					case  BC_BAD_BASE: return FALSE;
					case  BC_BAD_FUNC: return FALSE;
					default          : return FALSE;
			};
	};
	MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[uBaseStart]= uBaseEnd - uBaseStart + 1;
	return TRUE;
};







/*
   На ТА1-USB-01 эта функция работает, но для работы по МКИО с нашими
			изделиями ее применение бессмысленно, т.к. бессмысленно применение 
			функции BOOL MkioUsb_Bc_LinkBases( WORD uBaseStart, WORD uBaseEnd )
*/
BOOL MkioUsb_Bc_UnlinkBases( WORD uBaseStart, WORD uBaseEnd )
{
 int  iRet;
	WORD uBase, uMode;

 if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
	int iBcIdent = MkioUsbCtrl.iBcSelected;

 if( uBaseStart > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseEnd   > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseStart >=uBaseEnd ) return FALSE;

 for( uBase=uBaseStart; uBase<=uBaseEnd; uBase++ )
	{
   uMode  = DATA_BC_RT  ;
   uMode |=(MkioUsbCtrl.bc[iBcIdent].uLine);
			uMode |= CX_STOP;
			uMode |=(MkioUsbCtrl.bc[iBcIdent].Cx_Sig);
   bcdefbase( uBase-1 );	iRet = bcdeflink( uBase,uMode );
			switch( iRet )
			{
					case            0: break;
					case  BC_BAD_BASE: return FALSE;
					case  BC_BAD_FUNC: return FALSE;
					default          : return FALSE;
			};
	};
	MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[uBaseStart]= 0;
 MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase=0;
	return TRUE;
};



BOOL MkioUsb_Bc_SendData( WORD uBase )
{
  WORD  uMode;
  BOOL  bRet ;
  int   iRet;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  int iBcIdent = MkioUsbCtrl.iBcSelected;


  if( uBase > MkioUsbCtrl.bc[iBcIdent].uMaxBaseIndx ) return FALSE;

  uMode   = DATA_BC_RT;
  uMode  |=(MkioUsbCtrl.bc[iBcIdent].Cx_Sig);
  if( MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[uBase] <=1 ) uMode |=CX_STOP;
  else                                                      uMode |=CX_CONT;
  uMode  |=(MkioUsbCtrl.bc[iBcIdent].uLine);
  iRet    = bcdefbase(uBase);                    if( iRet!=0 ) return FALSE;
  iRet    = bcstartx(  uBase,uMode );
  switch( iRet )
  {
    case            0: break;
    case  BC_BAD_BASE: return FALSE;
    case  BC_BAD_FUNC: return FALSE;
    default          : return FALSE;
  };

  bRet= MkioUsb_Bc_WaitEvent(iBcIdent,DATA_BC_RT);
  if( !bRet ) return FALSE;
  return TRUE;
};




BOOL MkioUsb_Bc_SendRequest( WORD* pData )
{
  int   iBcIdent;
  WORD  uMode, uBase, uChainBase, uTail, uSizePortion;
  BOOL  bRet    ;
  WORD* pDest   ;
  int   iRet    ;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  iBcIdent = MkioUsbCtrl.iBcSelected;

  if( MkioUsbCtrl.bc[iBcIdent].bChain==FALSE )
  {
    uMode = DATA_RT_BC|CX_STOP|CX_SIG|(MkioUsbCtrl.bc[iBcIdent].uLine);
    bcdefbase( REQUEST_BASE );
    MkioUsb_Bc_SelectLineForStart( iBcIdent );
    iRet  = bcstart(  REQUEST_BASE,uMode );
    switch( iRet )
    {
      case            0: break;
      case  BC_BAD_BASE: return FALSE;
      case  BC_BAD_FUNC: return FALSE;
      default          : return FALSE;
    };
    bRet= MkioUsb_Bc_WaitEvent(iBcIdent,DATA_RT_BC);  if( !bRet ) return FALSE;
    bcdefbase( REQUEST_BASE );
    bcgetblk( 2, pData, MkioUsbCtrl.bc[iBcIdent].uReqSize );
    return TRUE;
  }
  else
  {
    uMode = DATA_RT_BC|CX_CONT|CX_SIG|(MkioUsbCtrl.bc[iBcIdent].uLine);
    bcdefbase( REQUEST_CHAIN_START_BASE );
    iRet  = bcstartx( REQUEST_CHAIN_START_BASE,uMode ); if( iRet!=0 ) return FALSE;
    bRet  = MkioUsb_Bc_WaitEvent(iBcIdent,DATA_RT_BC );
    if( !bRet   )
    {
      return FALSE;
    };
    uTail = MkioUsbCtrl.bc[iBcIdent].uReqChainSize;
    pDest = pData;
    for( uBase=0; uBase<MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase; uBase++ )
    {
      uChainBase   = REQUEST_CHAIN_START_BASE + uBase;
      uSizePortion = uTail                     ; if( uSizePortion>32 ) uSizePortion = 32;
      bcdefbase( uChainBase );
      bcgetblk(  2, pDest, uSizePortion );
      uTail-= uSizePortion;
      pDest+= uSizePortion;
    };
    return TRUE;
  };
};


WORD MkioUsb_Bc_SetAwIgnoreErrorMask( int iBcIdent, WORD uIgnoreAwErrorMask )
{
  WORD uPrevMask;
  if( iBcIdent<0 || iBcIdent>MKIO_MAX_BC ) return 0;
  uPrevMask                                   = MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask;
  MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask = uIgnoreAwErrorMask;
  return uPrevMask;
};

WORD MkioUsb_Bc_GetAwIgnoreErrorMask( int iBcIdent )
{
  if( iBcIdent<0 || iBcIdent>MKIO_MAX_BC ) return 0;
  return MkioUsbCtrl.bc[iBcIdent].uIgnoreAwErrorMask;
};


void MkioUsb_Bc_ErrorMessageBox( int iBcIdent, char* pExString )
{
  char pszMessage[1024];
  MkioUsb_Bc_CreateErrorListString( iBcIdent,pszMessage,pExString );
  MkioUsb_MessageBox( NULL,pszMessage,"Ошибка МКИО-канала",MB_OK|MB_ICONERROR);
};

void MkioUsb_Bc_IrqEnanle( int iBcIdent, BOOL bMode )
{
  if( bMode ) MkioUsbCtrl.bc[iBcIdent].Cx_Sig=CX_SIG;
  else        MkioUsbCtrl.bc[iBcIdent].Cx_Sig=CX_NOSIG;
};

BOOL MkioUsb_Bc_SelectBoardProcessor( int iBcIdent, WORD uAddress, WORD uProcessor )
{
  WORD uCmdCode;
  BOOL bRet    ;
  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  uCmdCode     = uProcessor & 0x0007;
  bRet         = MkioUsb_Bc_SendCommand( uAddress, 30, 1, &uCmdCode );
  return TRUE;
};



/////////////////////////////////////////////////////////////////////////////////////

BOOL MkioUsb_Rt_Select( int iRtIdent )
{
  int iRet,iTmkDevice;
  if( iRtIdent<0 || iRtIdent>=MKIO_MAX_RT ) return FALSE;
  iTmkDevice = MkioUsbCtrl.rt[iRtIdent].iTmkDevice; 
  iRet = MkioUsb_TmkSelect( iTmkDevice );
  switch( iRet )
  {
    case  0: break;
    case  1: tmkdefevent( MkioUsbCtrl.rt[iRtIdent].hEvent,TRUE); break;
    case -1: 
    default: MkioUsbCtrl.iRtSelected =-1; return FALSE;
  };
  MkioUsbCtrl.iRtSelected = iRtIdent;
  return TRUE;
};

int MkioUsb_Rt_CreateDevice( int iTmkDevice, WORD uRtAddress )
{
  BOOL     bRet;
  WORD     uSubAddr;
  int      iRet,iRtIdent;
  int      i;

  iRtIdent = -1;

  if( MkioUsbCtrl.numActivatedTmkDevices>=MkioUsbCtrl.numTmkDevices )   goto error_0;
  if( iTmkDevice<0 || iTmkDevice>=MkioUsbCtrl.numTmkDevices         )   goto error_0;
  if( MkioUsb_IsDeviceFree(iTmkDevice )== FALSE                     )   goto error_0;

  for( i=0; i<MKIO_MAX_RT; i++ )
  {
    if( MkioUsbCtrl.iRtTmk[i]==-1 )
    {
      iRtIdent = i;
      break;
    };
  };  if( iRtIdent<0 ) goto error_0;

      MkioUsbCtrl.rt[iRtIdent].hEvent= CreateEvent( NULL, TRUE, FALSE, NULL);
  if( MkioUsbCtrl.rt[iRtIdent].hEvent== NULL )                          goto error_1;

  iRet = tmkconfig( iTmkDevice );                         if( iRet!=0 ) goto error_2;
  tmkselect(        iTmkDevice );
  rtreset();

  bRet= MkioUsb_TmkSelect(iTmkDevice);                    if( !bRet   ) goto error_3;
  rtreset();
  tmkdefevent( MkioUsbCtrl.rt[iRtIdent].hEvent,TRUE);
  iRet= rtdefaddress( uRtAddress );                       if( iRet!=0 ) goto error_3;
  iRet= rtdefmode( RT_FLAG_MODE );                        if( iRet!=0 ) goto error_3;
  iRet= rtdefirqmode(rtgetirqmode()&~RT_DATA_BL);         if( iRet!=0 ) goto error_3;
  iRet= rtenable(RT_ENABLE);                              if( iRet!=0 ) goto error_3;
  iRet= rtdefpage(0);                                     if( iRet!=0 ) goto error_3;

  for ( uSubAddr= 1; uSubAddr<= 30; uSubAddr++ )
  {
    rtdefsubaddr(RT_RECEIVE , uSubAddr );  rtclrflag();
    rtdefsubaddr(RT_TRANSMIT, uSubAddr );  rtclrflag();
  };

  MkioUsbCtrl.rt[iRtIdent].uMaxPageIndx = rtgetmaxpage();
  MkioUsbCtrl.rt[iRtIdent].iTmkDevice   = MkioUsbCtrl.iRtTmk[iRtIdent]= iTmkDevice;
  MkioUsbCtrl.rt[iRtIdent].uAddress     = uRtAddress;
  MkioUsbCtrl.rt[iRtIdent].bOpen        = TRUE;

  MkioUsbCtrl.iRtSelected = iRtIdent;
  MkioUsbCtrl.numActivatedRt++;
  MkioUsbCtrl.numActivatedTmkDevices++;

  return iRtIdent;
error_3:  tmkdone( iTmkDevice );
error_2:  CloseHandle(MkioUsbCtrl.rt[iRtIdent].hEvent);
error_1:
error_0:  MkioUsbCtrl.iRtSelected = -1;
  return -1;
};

BOOL MkioUsb_Rt_SetRequestData( WORD uSubAddress, WORD nData, WORD* pData  )
{
  if( !MkioUsb_Rt_Select( MkioUsbCtrl.iRtSelected ) ) return FALSE;
  int iRtIdent = MkioUsbCtrl.iRtSelected;
  rtdefsubaddr( RT_TRANSMIT, uSubAddress );
  rtputblk( 0, pData, nData );
  return TRUE;
};


void MkioUsb_Rt_Free( int iRtIdent )
{
  int iTmkDevice;
  if( iRtIdent<0 || iRtIdent>=MKIO_MAX_RT ) return;

  iTmkDevice = MkioUsbCtrl.rt[iRtIdent].iTmkDevice; 
  if( MkioUsb_IsDeviceFree( iTmkDevice )  ) return;

  MkioUsb_Rt_Select(iRtIdent);
  tmkdefevent( NULL,TRUE);
  CloseHandle( MkioUsbCtrl.rt[iRtIdent].hEvent );

  MkioUsbCtrl.iRtTmk[iRtIdent]= MkioUsbCtrl.rt[iRtIdent].iTmkDevice  =-1;
  if( iTmkDevice==MkioUsbCtrl.iTmkSelected ) MkioUsbCtrl.iTmkSelected=-1;

  MkioUsbCtrl.numActivatedTmkDevices--;
  MkioUsbCtrl.numActivatedRt--;

  MkioUsb_Rt_ResetInfo( iRtIdent );
  tmkdone( iTmkDevice );
  MkioUsbCtrl.rt[iRtIdent].bOpen=FALSE;
  return;
};


BOOL MkioUsb_Rt_WaitEvent( int iRtIdent )
{
  MkioUsb_Rt_Select( iRtIdent );
  MkioUsbCtrl.rt[iRtIdent].bState= FALSE;
  MkioUsbCtrl.rt[iRtIdent].bfState.uWord= NULL;
  switch ( WaitForSingleObject(MkioUsbCtrl.rt[iRtIdent].hEvent,1000))
  {
    default           : return FALSE;
    case WAIT_TIMEOUT : return FALSE;
    case WAIT_OBJECT_0: ResetEvent(MkioUsbCtrl.rt[iRtIdent].hEvent); break;
  };
  tmkgetevd( &MkioUsbCtrl.rt[iRtIdent].Evd );
  MkioUsbCtrl.rt[iRtIdent].bState= TRUE;
  MkioUsbCtrl.rt[iRtIdent].bfState.uWord = MkioUsbCtrl.rt[iRtIdent].Evd.rt.wStatus;
  return TRUE;
};

BOOL MkioUsb_Rt_GetLastIoState( int iRtIdent, WORD* pState )
{
  if( MkioUsbCtrl.rt[iRtIdent].bState==FALSE ) return FALSE;
  *pState= MkioUsbCtrl.rt[iRtIdent].bfState.uWord;
  return TRUE;
};



BOOL MkioUsb_Rt_IsUpdate( int iRtIdent, WORD uDirect, WORD uSubAddr, WORD* pNumData )
{
  RT_FLAGWORD fw;
  MkioUsb_Rt_Select( iRtIdent );
  switch( uDirect )
  {
    default:           return FALSE;
    case  RT_RECEIVE : break;
    case  RT_TRANSMIT: break;
  };
  fw.uWord = rtgetflag( uDirect, uSubAddr );
  if( pNumData!=0 )
  {
    *pNumData= fw.Bf.uNumData;
  };
  if( fw.Bf.uFlag!=0 )
  {
    return TRUE ;
  }
  else
  {
    return FALSE;
  };
};


BOOL MkioUsb_Rt_Read( int iRtIdent, WORD uSubAddr, WORD numData, WORD* pData, BOOL bFlagOff )
{
  MkioUsb_Rt_Select( iRtIdent );
  rtdefsubaddr( RT_RECEIVE, uSubAddr );
  rtgetblk(0,pData,numData);
  if( bFlagOff )
  {
    rtclrflag();
  };
  return TRUE;
};

BOOL MkioUsb_Rt_Write( int iRtIdent, WORD uSubAddr, WORD numData, WORD* pData, BOOL bFlagSet )
{
  MkioUsb_Rt_Select( iRtIdent );
  rtdefsubaddr( RT_TRANSMIT, uSubAddr  );
  rtputblk(0,pData,numData);
  if( bFlagSet ) rtsetflag();
  return TRUE;
};


BOOL MkioUsb_Rt_Test()
{
  return TRUE;
}



BOOL MkioUsb_Rt_CreateErrorListString( int iRtIdent, char* pString )
{
  char pszString[1024];
  char s[1024];

  lstrcpy( pszString,"\0");

  if( MkioUsbCtrl.rt[iRtIdent].bState==FALSE )
  {
    lstrcpy( pszString,"Нет событий!");
    lstrcpy( pString,pszString );
    return FALSE;
  };
  lstrcpy(pszString,"      (OУ) РЕЗУЛЬТАТ ОБМЕНА MKIO\n");
  lstrcat(pszString,"  \n");
  lstrcat(pszString,"(.) СЛОВО СОСТОЯНИЯ ОУ:\n");

  sprintf( s,"    - ПодАдрес  : %u\n", MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uSubAddr );
  lstrcat( pszString,s );
  sprintf( s,"    - К-во Слов : %u\n", MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uNumData );
  lstrcat( pszString,s );
  if( MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uDirect==0 )
  {
    lstrcat( pszString,"    - Тип Обмена: Прием Данных\n");
  }
  else
  {
    lstrcat( pszString,"    - Тип Обмена: Передача Данных\n");
  };

  if( MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uBP          ) lstrcat( pszString,"    - внутренний конфликт аппаратуры ОУ\n" );
  if( MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uDeviceError ) lstrcat( pszString,"    - Ошибка обмена с подсистемой ОУ\n" );
  if( MkioUsbCtrl.rt[iRtIdent].bfState.Bf.uMessageError) lstrcat( pszString,"    - Ошибка в сообщении\n" );
  lstrcpy( pString,pszString );
  return TRUE;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Существует две функции загрузки :
		1.BOOL MkioUsb_RunLoad_Adsp2181_File(...) - т.е. эта
		2.BOOL MkioUsb_RunLoad_Adsp2181_File_Ex(...)
	Эти функции отличаются только дисциплиной загрузки, но результаты загрузки одинаковы!
		-Функция (1) все грузит мелкими порциями с одной базы ДОЗУ КК- с базы №0.
		-Функция (2) все грузит крупными порциями по 2К байт, предварительно записав необходимое
		количество баз в ДОЗУ КК; затем она "пачкой" выдает эти базы на передачу.

	Вроде бы функция (2) работает побыстрее
*/
BOOL MkioUsb_RunLoad_Adsp2181_File( int iBcIdent, WORD uAddress, MKIO_LOADER_FILE* pFileInfo, CProgressCtrl* pProgress )
{
	CFile hf;
	BOOL  bRet;
	DWORD dwSizeFile;
	DWORD dwSizeLoad;
	DWORD dwSizeRead;
	BYTE* pLoadCode; 
	char  pszMessage[1024];
	WORD  uSizePortion;
	WORD  uSizePortionCur;
	DWORD dwNumPortion;
	DWORD dwTail;
	DWORD dwPortion;
	WORD* pWordPortion;
	BYTE* pPortion;
	BYTE  ubByteLoad;
	WORD  uWordLoad;
	WORD  uCmdCode;
	int   i;
	char  pszErrorList[1024];
	union
	{
		BYTE  ub[4];
		WORD  uw[2];
		DWORD dw;
	} SignA;
	union
	{
		BYTE  ub[4];
		WORD  uw[2];
		DWORD dw;
	} SignB;

	uSizePortion= 2*1024;

	bRet= hf.Open( pFileInfo->pszName,CFile::modeRead|CFile::typeBinary ); if( !bRet )goto error_10;
	dwSizeFile= static_cast<DWORD>( hf.GetLength() );
	hf.Read( &SignA,4 );


	SignB.ub[3]= SignA.ub[0];
	SignB.ub[2]= SignA.ub[1];
	SignB.ub[1]= SignA.ub[2];
	SignB.ub[0]= SignA.ub[3];
	dwSizeLoad = SignB.dw;
	if( dwSizeLoad!=(dwSizeFile-4) ) goto error_09;
	hf.Seek(4,CFile::begin);

	pLoadCode   = new BYTE [dwSizeLoad  ]; if( pLoadCode   ==NULL ) goto error_09;
	pWordPortion= new WORD [uSizePortion]; if( pWordPortion==NULL ) goto error_08;

	dwSizeRead  = hf.ReadHuge( pLoadCode,dwSizeLoad );
	if( dwSizeRead!=dwSizeLoad ) goto error_07;
	if( pProgress!=NULL )
	{
		pProgress->SetRange32( 0,dwSizeLoad );
		pProgress->SetPos(0);
	};

	// 1. Выбор процессора

	uCmdCode     = pFileInfo->uProcessor&0x0007;
	bRet         = MkioUsb_Bc_SendCommand( uAddress, 30, 1, &uCmdCode );	if( !bRet ) goto error_06;
	Sleep(10);

// 2. Сброс процессора
	uCmdCode     = 0x0007;
	bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_06;
	Sleep(100);


// 3. Взвод процессора
	uCmdCode     = 0x0006;
	bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_06;
	Sleep(10);


// 4. Загрузка процессора
	dwNumPortion = dwSizeLoad/((DWORD)uSizePortion);
	if( dwSizeLoad%((DWORD)uSizePortion) ) dwNumPortion++;
	dwTail       = dwSizeLoad;
	pPortion     = pLoadCode;

	for( dwPortion=0; dwPortion<dwNumPortion; dwPortion++ )
	{
		if( dwTail>((DWORD)uSizePortion)) uSizePortionCur=uSizePortion;
		else                              uSizePortionCur=(WORD)dwTail;
		for( i=0; i<uSizePortionCur; i++ )
		{
			ubByteLoad = pPortion[i];
			uWordLoad  = MAKEWORD( ubByteLoad,0 ); 
			pWordPortion[i]= uWordLoad;
		};
		bRet      = MkioUsb_Bc_SendCommand( uAddress, 28, uSizePortionCur, pWordPortion );	if( !bRet ) goto error_06;
		dwTail   -=((DWORD)uSizePortionCur);     
		pPortion += uSizePortionCur;
		if( pProgress!=NULL ) pProgress->SetPos( dwSizeLoad-dwTail );
	};

// 5. Запуск процессора
	if( pFileInfo->bRunProcessor )
	{
		uCmdCode     = 0x0000;
		bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_06;
	};
	delete pWordPortion;
	delete pLoadCode;
	hf.Close();
	return TRUE;
error_06: 
	delete pLoadCode;
	delete pWordPortion;
	hf.Close();
	MkioUsb_Bc_CreateErrorListString( iBcIdent,pszErrorList );
	MkioUsb_MessageBox( NULL,pszErrorList,"ОШИБКА СВЯЗИ",MB_OK|MB_ICONERROR );
	return FALSE;
error_07: 
	delete pWordPortion;
	delete pLoadCode;
	hf.Close();
	sprintf( pszMessage,"Ошибка чтения файла загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
	return FALSE;
error_08: 
	delete pLoadCode;
	hf.Close();
	sprintf( pszMessage,"Недостаточно памяти для загрузки файла\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
	return FALSE;
error_09: 
	hf.Close();
	sprintf( pszMessage,"Несоответствующий формат файла загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
	return FALSE;
error_10: 
	sprintf( pszMessage,"Отсутствует файл загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
	return FALSE;
};

BOOL MkioUsb_Bc_SendBaseChain( int iBcIdent, WORD uBaseStart, WORD numBases )
{
	WORD uMode,i,uBase;
	int  iRet;
	BOOL bRet;

	if( MkioUsbCtrl.bc[iBcIdent].uNumLinkedBases[uBaseStart]>1 ) goto SendByChain;

	uMode   = DATA_BC_RT|CX_STOP|CX_SIG;
	uMode  |=(MkioUsbCtrl.bc[iBcIdent].uLine);
	for( i=0; i<numBases; i++ )
	{
		uBase= uBaseStart + i;
		bcdefbase( uBase );
		iRet = bcstartx( uBase,uMode );
		switch( iRet )
		{
			case     0: break;
			default   : return FALSE;
		};
		bRet= MkioUsb_Bc_WaitEvent(iBcIdent,DATA_BC_RT);
		if( !bRet ) return FALSE;
	};
	return TRUE;
SendByChain:  	uMode = DATA_BC_RT|CX_CONT|CX_SIG;
               uMode|=(MkioUsbCtrl.bc[iBcIdent].uLine);
               iRet  = bcstartx( uBaseStart,uMode );              if( iRet!=0 ) return FALSE;
               bRet  = MkioUsb_Bc_WaitEvent(iBcIdent,DATA_BC_RT); if( !bRet   ) return FALSE;
               return TRUE;
};

BOOL MkioUsb_Bc_SetRequestMode( WORD uAddress, WORD uSubAddress, WORD uSizeBlock )
{
  int  iBcIdent;
  BOOL bRet;
  WORD uCmdWord, nBase, uBase, uChainBase, uTail, uSizePortion;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  iBcIdent = MkioUsbCtrl.iBcSelected;

  if( uSizeBlock<=32 )
  {
    uCmdWord                         = CW( uAddress, RT_TRANSMIT, uSubAddress, uSizeBlock );
    bcdefbase( REQUEST_BASE );
    bcputw(  0, uCmdWord );
    MkioUsbCtrl.bc[iBcIdent].uReqSize= uSizeBlock;
    MkioUsbCtrl.bc[iBcIdent].bChain  = FALSE;
  }
  else
  {
     if( uSizeBlock==MkioUsbCtrl.bc[iBcIdent].uReqChainSize ) 
     {
       MkioUsbCtrl.bc[iBcIdent].bChain= TRUE;
       return TRUE;
     }
     else
     {
       bRet = MkioUsb_Bc_RequestUnlinkBases(); if( !bRet ) return FALSE;
       MkioUsbCtrl.bc[iBcIdent].uReqChainSize = uSizeBlock;
       nBase = uSizeBlock / 32; if( uSizeBlock % 32) nBase++;
       uTail = uSizeBlock;
       for( uBase=0; uBase<nBase; uBase++ )
       {
         uSizePortion = uTail; if( uSizePortion>32 ) uSizePortion = 32;
         uChainBase   = REQUEST_CHAIN_START_BASE + uBase;
         uCmdWord     = CW( uAddress, RT_TRANSMIT, uSubAddress, uSizePortion );
         bcdefbase( uChainBase );
         bcputw(  0,uCmdWord   );
         uTail -= uSizePortion;
       };
       bRet = MkioUsb_Bc_RequestLinkBases(); if( !bRet ) return FALSE;
       MkioUsbCtrl.bc[iBcIdent].bChain= TRUE;
     };
  };
  return TRUE;
};

BOOL MkioUsb_Bc_SendRequest( WORD uBase, WORD uRtAddress, WORD uRtSubAddress, int nData, WORD* pData )
{
  WORD uSizeBlock;
  BOOL bRet;
//Parameter uBase now is obsolete, i.e. is unused
  uSizeBlock= (WORD)nData;
  bRet      = MkioUsb_Bc_SetRequestMode( uRtAddress, uRtSubAddress, uSizeBlock ); if( !bRet ) return FALSE;
  bRet      = MkioUsb_Bc_SendRequest( pData );                                    if( !bRet ) return FALSE; 
  return TRUE;
};



/*
	Существует две функции загрузки :
		1.BOOL MkioUsb_RunLoad_Adsp2181_File(...)
		2.BOOL MkioUsb_RunLoad_Adsp2181_File_Ex(...) - т.е. эта
	Эти функции отличаются только дисциплиной загрузки, но результаты загрузки одинаковы!
		-Функция (1) все грузит мелкими порциями с одной базы ДОЗУ КК- с базы №0.
		-Функция (2) все грузит крупными порциями по 2К байт, предварительно записав необходимое
		количество баз в ДОЗУ КК; затем она "пачкой" выдает эти базы на передачу.

	Вроде бы функция (2) работает побыстрее
*/
BOOL MkioUsb_RunLoad_Adsp2181_File_Ex( int iBcIdent, WORD uAddress, MKIO_LOADER_FILE* pFileInfo, CProgressCtrl* pProgress )
{
  union
		{
			 BYTE  ub[4];
				WORD  uw[2];
				DWORD dw;
		} SignA;
  union
		{
			 BYTE  ub[4];
				WORD  uw[2];
				DWORD dw;
		} SignB;
  DWORD dwSegmentMainNum     ;
  DWORD dwSegmentMainSize    ;
  DWORD dwSegmentTailSize    ;
		DWORD dwSegmentMainNumBase ;
  DWORD dwSegmentTailNumBase ;
		DWORD dwiSegm              ;
		WORD* puSegmentData        ;
		DWORD dwiByte,dwiBase      ;
		DWORD dwjByte,dwiSegmWord  ;
  WORD  uCmdWord, uCmdCode   ;
		WORD  uBasePortion[33]     ;
		WORD  nSizeBasePortion     ; 
		DWORD dwLoaded             ;
  CFile hf;
 	BOOL  bRet;
		DWORD dwSizeFile;
		DWORD dwSizeLoad;
		DWORD dwSizeRead;
		DWORD dwTail;
		BYTE* pLoadCode; 
		char  pszMessage[1024];
		int   i;

  dwSegmentMainSize    =                   2*1024;//  должно быть кратно 32-м

		bRet= hf.Open( pFileInfo->pszName,CFile::modeRead|CFile::typeBinary ); if( !bRet )goto error_14;
		dwSizeFile= static_cast<DWORD>( hf.GetLength() );
		hf.Read( &SignA,4 );

		SignB.ub[3]= SignA.ub[0];
		SignB.ub[2]= SignA.ub[1];
		SignB.ub[1]= SignA.ub[2];
		SignB.ub[0]= SignA.ub[3];
		dwSizeLoad = SignB.dw   ;                        if( dwSizeLoad!=(dwSizeFile-4) ) goto error_13;

  pLoadCode     = new BYTE [ dwSizeLoad        ]; if( pLoadCode    ==NULL ) goto error_12;
  puSegmentData = new WORD [ dwSegmentMainSize ]; if( puSegmentData==NULL ) goto error_11;


		hf.Seek(4,CFile::begin);
  dwSizeRead  = hf.ReadHuge( pLoadCode,dwSizeLoad );
		                     if( dwSizeRead!=dwSizeLoad ) goto error_10;

  if( pProgress!=NULL )
		{
			 pProgress->SetRange32( 0,dwSizeLoad );
				pProgress->SetPos(0);
		};
// 1. Выбор процессора

		uCmdCode     = pFileInfo->uProcessor&0x0007;
  bRet         = MkioUsb_Bc_SendCommand( uAddress, 30, 1, &uCmdCode );	if( !bRet ) goto error_09;
		Sleep(10);

// 2. Сброс процессора
		uCmdCode     = 0x0007;
  bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_09;
		Sleep(100);


// 3. Взвод процессора
		uCmdCode     = 0x0006;
  bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_09;
		Sleep(10);


// 4. Загрузка процессора
		dwjByte              =                        0;
		dwLoaded             =                        0;
		dwSegmentMainNum     = dwSizeLoad/dwSegmentMainSize;
  dwSegmentTailSize    = dwSizeLoad%dwSegmentMainSize;
		dwSegmentMainNumBase = dwSegmentMainSize / 32UL;
  dwSegmentTailNumBase = dwSegmentTailSize / 32UL; if( (dwSegmentTailSize%32UL)>0 ) dwSegmentTailNumBase++;


		if( dwSegmentMainNum==0 ) goto Load_Tail;

  for( dwiSegm =0; dwiSegm<dwSegmentMainNum; dwiSegm++ )
		{
			 for( dwiByte=0; dwiByte<dwSegmentMainSize; dwiByte++ )
				{
      puSegmentData[dwiByte]= MAKEWORD( pLoadCode[dwjByte],0 );
      dwjByte++;
				};

   	uCmdWord   = CW( uAddress, RT_RECEIVE, 28, 32 );
				dwiSegmWord= 0;

				for( dwiBase=0; dwiBase<dwSegmentMainNumBase; dwiBase++ )
				{
      for( i=0; i<33; i++ )
						{
							 if( i==0 ) uBasePortion[i]= uCmdWord;
								else
								{
									 uBasePortion[i]= puSegmentData[ dwiSegmWord ];
          dwiSegmWord++;
								};
						};
		    bcdefbase( (WORD)dwiBase );
      bcputblk( 0, uBasePortion, 33 );
				};
    bRet = MkioUsb_Bc_SendBaseChain( iBcIdent, 0,(WORD)dwSegmentMainNumBase );
                                                            if( !bRet ) goto error_09;				  
				dwLoaded+=dwSegmentMainSize;
    if( pProgress!=NULL ) pProgress->SetPos( dwLoaded );
		};

Load_Tail: 
  if( dwSegmentTailSize==0 ) goto Load_End;

	 for( dwiByte=0; dwiByte<dwSegmentTailSize; dwiByte++ )
		{
    puSegmentData[dwiByte]= MAKEWORD( pLoadCode[dwjByte],0 );
    dwjByte++;
		};

		dwTail     = dwSegmentTailSize;
		dwiSegmWord= 0;
		for( dwiBase=0; dwiBase<dwSegmentTailNumBase; dwiBase++ )
		{
    nSizeBasePortion=(WORD)dwTail; if( nSizeBasePortion>32 ) nSizeBasePortion=32;
   	uCmdWord        = CW( uAddress, RT_RECEIVE, 28, nSizeBasePortion );

    for( i=0; i<nSizeBasePortion+1; i++ )
				{
					 if( i==0 ) uBasePortion[i]= uCmdWord;
						else
						{
							 uBasePortion[i]= puSegmentData[ dwiSegmWord ];
        dwiSegmWord++;
						};
				};
		  bcdefbase( (WORD)dwiBase );
    bcputblk( 0, uBasePortion, nSizeBasePortion+1 );
  		dwTail -= nSizeBasePortion;
		};
  bRet= MkioUsb_Bc_SendBaseChain( iBcIdent, 0,(WORD)dwSegmentTailNumBase );
                                               if( !bRet ) goto error_09;				  
		dwLoaded+=dwSegmentTailSize;
  if( pProgress!=NULL ) pProgress->SetPos( dwLoaded );


Load_End:
// 5. Запуск процессора
		if( pFileInfo->bRunProcessor )
		{
 		uCmdCode     = 0x0000;
   bRet         = MkioUsb_Bc_SendCommand( uAddress, 29, 1, &uCmdCode );	if( !bRet ) goto error_09;
		};
		delete puSegmentData;
  delete pLoadCode;
	 hf.Close();
		return TRUE;
error_09: 
		delete pLoadCode;
		delete puSegmentData;
	 hf.Close();
//  sprintf( pszMessage,"Ошибка канала при загрузке файла\n%s",pFileInfo->pszName );
  MkioUsb_Bc_ErrorMessageBox( iBcIdent, NULL );
  return FALSE;
error_10: 
		delete pLoadCode;
		delete puSegmentData;
	 hf.Close();
  sprintf( pszMessage,"Неправильная длина файла загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
  return FALSE;
error_11: 
		delete pLoadCode;
	 hf.Close();
  sprintf( pszMessage,"Недостаточно оперативной памяти ПК для загрузки");
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
  return FALSE;
error_12: 
	 hf.Close();
  sprintf( pszMessage,"Недостаточно оперативной памяти ПК для загрузки" );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
  return FALSE;
error_13: 
	 hf.Close();
  sprintf( pszMessage,"Несоответствующий формат файла загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
  return FALSE;
error_14: 
  sprintf( pszMessage,"Отсутствует файл загрузки\n%s",pFileInfo->pszName );
	MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
  return FALSE;
};


/*
  Эта п.п предназначена для загрузки группы процессоров, заданной
		данныйми в pLoadInfo
*/
BOOL MkioUsb_RunLoad_Adsp2181( MKIO_LOADER_INFO* pLoadInfo )
{
	char  pszMessage[1024];
	int   iBcIdent;
	int   iFile;
	BOOL  bRet;
	WORD  uRestIgnoreMask;

	pLoadInfo->Service_bDriverOpened=FALSE;
	pLoadInfo->Service_bBcCreated   =FALSE;


	if(	!MkioUsbCtrl.bOpen )
	{
   if( !MkioUsb_Open() )   goto error_11;
  	pLoadInfo->Service_bDriverOpened=TRUE;
	};

	if(MkioUsb_IsDeviceFree(pLoadInfo->uTmk)==TRUE)
	{
   iBcIdent= MkioUsb_Bc_CreateDevice(pLoadInfo->uTmk);	if( iBcIdent<0 )  goto error_10;
			pLoadInfo->Service_bBcCreated   =TRUE;
	}
	else
	{
   iBcIdent= MkioUsb_Bc_GetIdent(pLoadInfo->uTmk);  			if( iBcIdent<0 )  goto error_12;
	};

	if( pLoadInfo->uNumFiles<=0 ) goto error_09;


// При загрузке игнорируется ошибка ОС "Неисправность ОУ";
// запомним для последующего восстановления маску игнорирования битов ОС
 uRestIgnoreMask = MkioUsb_Bc_SetAwIgnoreErrorMask(iBcIdent,~RTFL_MASK );

 for( iFile=0; iFile<pLoadInfo->uNumFiles; iFile++ )
	{
   bRet= MkioUsb_RunLoad_Adsp2181_File_Ex( 
				            iBcIdent                ,
															 pLoadInfo->uAddress     ,
															&pLoadInfo->uFile[iFile] ,
															 pLoadInfo->pProgress     );  if(!bRet) goto error_08;
	};

// Восстановим нормальную реакцию на ошибки
 MkioUsb_Bc_SetAwIgnoreErrorMask(iBcIdent,uRestIgnoreMask);
 if(pLoadInfo->Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
 if(pLoadInfo->Service_bDriverOpened) MkioUsb_Close();
	return TRUE;

error_08: if(pLoadInfo->Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
          if(pLoadInfo->Service_bDriverOpened) MkioUsb_Close();
          MkioUsb_Bc_SetAwIgnoreErrorMask(iBcIdent,uRestIgnoreMask);
								  return FALSE;
error_09: if(pLoadInfo->Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
          if(pLoadInfo->Service_bDriverOpened) MkioUsb_Close();
	         sprintf( pszMessage,"Количество файлов загрузки некорректно: %d",pLoadInfo->uNumFiles );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
								  return FALSE;
error_10: sprintf( pszMessage,"Не найдено устройство TA1-USB-01: #%u",pLoadInfo->uTmk );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
          if(pLoadInfo->Service_bDriverOpened) MkioUsb_Close();
								  return FALSE;
error_11: sprintf( pszMessage,"Не открывается драйвер устройства TA1-USB-01" );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
								  return FALSE;
error_12: sprintf( pszMessage,"Устройство TA1-USB-01 #%u\nуже занято под ОУ!",pLoadInfo->uTmk );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
          if(pLoadInfo->Service_bDriverOpened) MkioUsb_Close();
								  return FALSE;
};

/*
*/
//////////////////////////////////////////////////////////////////////////////////////////
int MkioUsb_Mt_GetLastError()
{
  return 0;
};

BOOL MkioUsb_Mt_Select( int iMtIdent )
{
  int iRet,iTmkDevice;
  if( iMtIdent<0 || iMtIdent>=MKIO_MAX_MT ) return FALSE;
  iTmkDevice = MkioUsbCtrl.mt[iMtIdent].iTmkDevice; 
  iRet       = MkioUsb_TmkSelect( iTmkDevice );
  switch( iRet )
  {
    case  0: break;
    case  1: tmkdefevent( MkioUsbCtrl.mt[iMtIdent].hEvent,TRUE); break;
    case -1: 
    default: MkioUsbCtrl.iMtSelected =-1; return FALSE;
  };
  MkioUsbCtrl.iMtSelected = iMtIdent;
  return TRUE;
};



int MkioUsb_Mt_CreateDevice( int iTmkDevice )
{
  WORD i;
  int  iRet,iMtIdent;

  if( MkioUsbCtrl.numActivatedTmkDevices>=MkioUsbCtrl.numTmkDevices )   goto error_0;
  if( iTmkDevice<0 || iTmkDevice>=MkioUsbCtrl.numTmkDevices         )   goto error_0;
  if( MkioUsb_IsDeviceFree(iTmkDevice )== FALSE                     )   goto error_0;

  iMtIdent = -1;
  for( i=0; i<MKIO_MAX_MT; i++ )
  {
    if( MkioUsbCtrl.iMtTmk[i]==-1 )
    {
      iMtIdent = i;
      break;
    };
  };  if( iMtIdent<0 ) goto error_0;

      MkioUsbCtrl.mt[iMtIdent].hEvent= CreateEvent(NULL, TRUE, FALSE, NULL);
  if( MkioUsbCtrl.mt[iMtIdent].hEvent== NULL )                          goto error_1;

  iRet = tmkconfig( iTmkDevice );                         if( iRet!=0 ) goto error_2;
  if( !MkioUsb_TmkSelect(iTmkDevice) )                                  goto error_3;
  mtreset();
  mtdefirqmode(MT_GENER1_BL|MT_GENER2_BL);

  MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx = mtgetmaxbase();
  MkioUsbCtrl.mt[iMtIdent].iTmkDevice   = MkioUsbCtrl.iBcTmk[iMtIdent]= iTmkDevice;
  MkioUsbCtrl.mt[iMtIdent].bOpen        = TRUE;
  tmkdefevent( MkioUsbCtrl.mt[iMtIdent].hEvent,TRUE);
  MkioUsbCtrl.iMtSelected = iMtIdent;
  MkioUsbCtrl.numActivatedMt++;
  MkioUsbCtrl.numActivatedTmkDevices++;

  return iMtIdent;
error_3:  tmkdone( iTmkDevice );
error_2:  CloseHandle(MkioUsbCtrl.mt[iMtIdent].hEvent);
error_1:
error_0:  MkioUsbCtrl.iMtSelected = -1;
  return -1;
};



void MkioUsb_Mt_Free( int iMtIdent )
{
  int iTmkDevice;
  if( iMtIdent<0 || iMtIdent>=MKIO_MAX_MT ) return;
  
  iTmkDevice = MkioUsbCtrl.mt[iMtIdent].iTmkDevice; 

  if( MkioUsb_IsDeviceFree( iTmkDevice )  ) return;

  MkioUsb_Mt_Select(iMtIdent);
  tmkdefevent( NULL,NULL);//TRUE );
  ResetEvent(MkioUsbCtrl.mt[iMtIdent].hEvent);
  CloseHandle( MkioUsbCtrl.mt[iMtIdent].hEvent );
//+++  tmkdone( iTmkDevice );

  MkioUsbCtrl.iMtTmk[iMtIdent]= MkioUsbCtrl.mt[iMtIdent].iTmkDevice  =-1;
  if( iTmkDevice==MkioUsbCtrl.iTmkSelected ) MkioUsbCtrl.iTmkSelected=-1;

  if( MkioUsbCtrl.mt[iMtIdent].pMcoBuffer!=NULL )
  {
    delete MkioUsbCtrl.mt[iMtIdent].pMcoBuffer;
    MkioUsbCtrl.mt[iMtIdent].pMcoBuffer   =NULL;
  };
  MkioUsbCtrl.mt[iMtIdent].uMcoBufSize  =NULL;
  MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames=NULL;
//  MkioUsbCtrl.mt[iMtIdent].pMcoNextFrame=NULL;

  MkioUsbCtrl.numActivatedTmkDevices--;
  MkioUsbCtrl.numActivatedBc--;

  MkioUsbCtrl.mt[iMtIdent].bOpen=FALSE;
  MkioUsb_Bc_ResetInfo( iMtIdent );
};


BOOL MkioUsb_Mt_LinkBases( WORD uBaseStart, WORD uBaseEnd )
{
 int  iRet;
	WORD uBase, uMode;

 if( !MkioUsb_Mt_Select( MkioUsbCtrl.iMtSelected ) ) return FALSE;
	int iMtIdent = MkioUsbCtrl.iMtSelected;

 if( uBaseStart > MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseEnd   > MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx )  return FALSE;
 if( uBaseStart>=uBaseEnd ) return FALSE;

 for( uBase=uBaseStart+1; uBase<=uBaseEnd; uBase++ )
	{
			if( uBase==uBaseEnd )
			{
   			uMode = CX_STOP|CX_NOINT|CX_SIG;
			}
			else
			{
   			uMode = CX_CONT|CX_NOINT|CX_SIG;
			};
   bcdefbase( uBase-1 );	iRet = bcdeflink( uBase,uMode );
			switch( iRet )
			{
					case            0: break;
					case  BC_BAD_BASE: return FALSE;
					case  BC_BAD_FUNC: return FALSE;
					default          : return FALSE;
			};
	};
 MkioUsbCtrl.mt[iMtIdent].uNumLinkedBases[uBaseStart]= uBaseEnd - uBaseStart + 1;
	return TRUE;
};


BOOL MkioUsb_Mt_UnlinkBases( WORD uBaseStart )
{
 int  iRet;
	WORD uBase, uNumBases, uBaseEnd, uMode;

 if( !MkioUsb_Mt_Select( MkioUsbCtrl.iMtSelected ) ) return FALSE;
	int iMtIdent = MkioUsbCtrl.iMtSelected;

 if( uBaseStart > MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx )  return FALSE;

 uNumBases = MkioUsbCtrl.mt[iMtIdent].uNumLinkedBases[uBaseStart]; if( uNumBases==0 ) return TRUE;
 uBaseEnd  = uBaseStart + uNumBases - 1; 
 if( uBaseEnd>MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx ) uBaseEnd=MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx;
 
      uMode=CX_STOP|CX_NOINT|CX_NOSIG ;
 for( uBase=uBaseStart; uBase<=uBaseEnd; uBase++ )
	{
         mtdefbase(uBase);
   iRet= mtdeflink(uBase,uMode);  if( iRet!=0 ) return FALSE;
	};
 MkioUsbCtrl.mt[iMtIdent].uNumLinkedBases[uBaseStart]= 0;
	return TRUE;
};




BOOL MkioUsb_Mt_SetMode( int uNumBases )
{
  BOOL bRet;
  int iMtIdent;
  WORD uBaseEnd;
  int  iNumBases;
  if( !MkioUsb_Mt_Select( MkioUsbCtrl.iMtSelected ) ) return FALSE;
  iMtIdent = MkioUsbCtrl.iMtSelected;
  MkioUsb_Mt_UnlinkBases( MT_START_BASE );
  iNumBases = uNumBases;
  if( iNumBases<0 ) iNumBases = MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx+1;
  uBaseEnd = MT_START_BASE + iNumBases - 1;
  if( uBaseEnd>MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx ) uBaseEnd=MkioUsbCtrl.mt[iMtIdent].uMaxBaseIndx;
  bRet     = MkioUsb_Mt_LinkBases( MT_START_BASE, uBaseEnd );
  return bRet;
};



BOOL MkioUsb_Mt_Run()
{
  int iMtIdent;
  int iRet;

  if( !MkioUsb_Mt_Select( MkioUsbCtrl.iMtSelected ) ) return FALSE;

  iMtIdent = MkioUsbCtrl.iMtSelected;
  MkioUsbCtrl.mt[iMtIdent].nIrqCntr    =     NULL;
  mtdefbase( MT_START_BASE );
  iRet = mtstartx(  MT_START_BASE, CX_CONT|CX_NOINT|CX_SIG ); if( iRet!=0 ) return FALSE;
  return TRUE;
};

BOOL MkioUsb_Mt_Stop()
{
  if( !MkioUsb_Mt_Select( MkioUsbCtrl.iMtSelected ) ) return FALSE;
  mtstop();
  return TRUE;
};



BOOL MkioUsb_Mt_WaitEvent( int iMtIdent )
{
  if( !MkioUsb_Mt_Select( iMtIdent ) ) return FALSE;

  MkioUsbCtrl.mt[iMtIdent].bState   = FALSE;

  switch ( WaitForSingleObject(MkioUsbCtrl.mt[iMtIdent].hEvent,1000))
  {
    default           :
    {
      return FALSE;
    };
    case WAIT_TIMEOUT :
    {
      return FALSE;
    };
    case WAIT_OBJECT_0: ResetEvent(MkioUsbCtrl.mt[iMtIdent].hEvent); break;
  };
  tmkgetevd(&MkioUsbCtrl.mt[iMtIdent].Evd);

  switch( MkioUsbCtrl.mt[iMtIdent].Evd.nInt )
  {
    default: return FALSE;
    case  3: // mtIntX - завершение с исключит.ситуацией
     {
       MkioUsbCtrl.mt[iMtIdent].bState         = TRUE;
       MkioUsbCtrl.mt[iMtIdent].bfState.uInt   = MkioUsbCtrl.mt[iMtIdent].Evd.nInt;
       MkioUsbCtrl.mt[iMtIdent].bfState.uWord  = MkioUsbCtrl.mt[iMtIdent].Evd.mt.wResultX;
       MkioUsbCtrl.mt[iMtIdent].uBase          = MkioUsbCtrl.mt[iMtIdent].Evd.mt.wBase;
       MkioUsbCtrl.mt[iMtIdent].nIrqCntr++;
       return TRUE;
     };break;
    case  4: // bcIntSig  - сигнальное при смене баз
     {
       MkioUsbCtrl.mt[iMtIdent].bState         = FALSE;
       MkioUsbCtrl.mt[iMtIdent].bfState.uInt   = MkioUsbCtrl.mt[iMtIdent].Evd.nInt;
       MkioUsbCtrl.mt[iMtIdent].uBase          = MkioUsbCtrl.mt[iMtIdent].Evd.mt.wBase;
       MkioUsbCtrl.mt[iMtIdent].nIrqCntr++;
       return TRUE;
     };break;
  };
  return FALSE;
};



BOOL MkioUsb_Mt_GetEventInfo( int iMtIdent, WORD* pIntType, WORD* pStatus, WORD* pBase )
{
  switch(MkioUsbCtrl.mt[iMtIdent].bfState.uInt)
  {
  default: 
   {
     *pIntType = NULL;
     *pStatus  = NULL;
     *pBase    = NULL;
      return FALSE;
   };break;
  case  3:
   {
     *pIntType = MkioUsbCtrl.mt[iMtIdent].bfState.uInt;
     *pStatus  = MkioUsbCtrl.mt[iMtIdent].bfState.uWord;
     *pBase    = MkioUsbCtrl.mt[iMtIdent].uBase;
     return TRUE;
   };break;
  case  4:
   {
     *pIntType = MkioUsbCtrl.mt[iMtIdent].bfState.uInt;
     *pStatus  = NULL;
     *pBase    = MkioUsbCtrl.mt[iMtIdent].uBase;
     return TRUE;
   };break;
  };
  return FALSE;
};

/*
   Смещение в  базе:	00	01	02		…		32	33	34	35		…		63
1. KK-ОУ	            KC	ИС	ИС	…	…	ИС	ИС	ОС						
2. OУ-KK	            KC	ОС	ИС	ИС	…	…	ИС	ИС						
3. OУ-OУ	            KC1	KC2	OC2	ИС	ИС	…	…	ИС	ИС	ОС1				
4. КС-ОС	            KC	ОС												
5. КС-ОС+ИС	         KC	ОС	ИС											
6. КС+ИС-ОС	         KC	ИС	ОС											
7. КК-ОУгр.	         KC	ИС	ИС	…	…	ИС	ИС							
8. ОУ-ОУгр.	         KC1	KC2	OC2	ИС	ИС	…	…	ИС	ИС					
9. КСгр.	            KC													
10.КС+ИСгр. 	        KC	ИС												
*/

BOOL MkioUsb_Mt_DetectFrame(  WORD* pSrcData, MT_MSGINFO* pMs )
{
  int         i ;
  BOOL        bGroup;
  BC_CMDWORD  Cw;
  WORD        nData ;


  pMs->iFmt    =    -1;
  pMs->nData   =     0;   
  pMs->bAw     = FALSE;
  bGroup       = FALSE;

  pMs->Cw.uWord = Cw.uWord = pSrcData[0];

  if( Cw.Bf.uAddress== 0x1f                         ) bGroup= TRUE;

  if( Cw.Bf.uSubAddr== 0x00 || Cw.Bf.uSubAddr==0x1F )
  { // Обработка Команд Управления
     if( Cw.Bf.uNumData>=0x00 && Cw.Bf.uNumData<=0x0F )
     {
       if( Cw.Bf.uDirect!=1 ) 
       {
         pMs->Aw.uWord  = pSrcData[1];
         pMs->bAw       =    FALSE;
         pMs->nData     =       0 ;
         pMs->iFmt      =      -1 ;
       }
       else
       {
         pMs->Aw.uWord  = pSrcData[1];
         pMs->bAw       =    TRUE ;
         pMs->nData     =       0 ;
                       pMs->iFmt=    4; // 4. КС-ОС
         if( bGroup )
         {
                       pMs->iFmt=    9; // 9. КСгр
                       pMs->bAw =FALSE;
         };
       };
     }
     else
     {
       if( Cw.Bf.uNumData>=0x10 && Cw.Bf.uNumData<=0x1F )
       {
         if( Cw.Bf.uDirect==0 )
         {
           pMs->iFmt      =       6 ; // 6. КС+ИС-ОС	         KC	ИС	ОС
           pMs->Aw.uWord  = pSrcData[2];
           pMs->bAw       =    TRUE ;
           pMs->nData     =       1 ;
           pMs->uData[0]  = pSrcData[1];
           if( bGroup )
           {
                    pMs->iFmt=   10; // 10.КС+ИСгр.  	        KC	ИС
                    pMs->bAw =FALSE;
           };
         }
         else
         {
           pMs->iFmt      =       5 ; // 5. КС-ОС+ИС	         KC	ОС	ИС
           pMs->Aw.uWord  = pSrcData[1];
           pMs->bAw       =    TRUE ;
           pMs->nData     =       1 ;
           pMs->uData[0]  = pSrcData[2];
           if( bGroup )
           {
                    pMs->iFmt=   -1; // 10.КС+ИСгр.  	        KC	ИС
                    pMs->bAw =FALSE;
           };
         };
       }
       else
       {
         pMs->iFmt      =      -1 ;
       };
     };
     return TRUE;
// Команды упраления разработаны
  };

  nData = Cw.Bf.uNumData; if( nData==0 ) nData=32;

// Обработка кадров обмена информацией
  if( Cw.Bf.uDirect == 0 )
  {
    pMs->nData= nData;//Cw.Bf.uNumData;
    if( bGroup )
    {
      pMs->iFmt= 7; //7. КК-ОУгр.	         KC	ИС	ИС	…	…	ИС	ИС							
      for( i= 0; i< pMs->nData;  i++ ) 
      {
         pMs->uData[i]= pSrcData[i+1];
      };
      pMs->bAw      = FALSE;
    }
    else
    {
      pMs->iFmt= 1; //1. KK-ОУ	            KC	ИС	ИС	…	…	ИС	ИС	ОС						
      for( i= 0; i< pMs->nData; i++ ) 
      {
         pMs->uData[i]= pSrcData[ i+1 ];
      };
      pMs->Aw.uWord = pSrcData[pMs->nData+1];
      pMs->bAw      = TRUE;
    };
  }
  else
  {
    if( bGroup )
    {
      pMs->iFmt =             -1;
    }
    else
    {
      pMs->iFmt =              2; //2. OУ-KK	            KC	ОС	ИС	ИС	…	…	ИС	ИС
      pMs->nData= Cw.Bf.uNumData;
      pMs->Aw.uWord = pSrcData[ 1 ];
      pMs->bAw      =      TRUE ;
      for( i=0; i<= pMs->nData ; i++ )
      {
        pMs->uData[i]=  pSrcData[i+2];
      };
    };
  };
  return TRUE;
};




BOOL MkioUsb_Mt_GetMessageData( int iMtIdent, WORD uBase, MT_MSGINFO* pMs )
{
  int         iRet  ;
  WORD        uData[64];

  pMs->Sw.uWord= mtgetsw();
  pMs->iType   = 0;
  iRet= mtdefbase( uBase ); if( iRet!=0 ) return FALSE;
  mtgetblk( 0,uData,64 );

  MkioUsb_Mt_DetectFrame( uData,pMs );
  return TRUE;
};


//%%%


BOOL MkioUsb_Mt_McoOpen( int iMtIdent, WORD nSize )
{
  DWORD dwRet;
  if( !MkioUsb_Mt_Select(iMtIdent) ) return FALSE;

  MkioUsbCtrl.mt[iMtIdent].pMcoBuffer   = new WORD[nSize];
  MkioUsbCtrl.mt[iMtIdent].uMcoBufSize  = nSize;
  MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames= NULL ;
//  MkioUsbCtrl.mt[iMtIdent].pMcoNextFrame= MkioUsbCtrl.mt[iMtIdent].pMcoBuffer;

  ResetEvent( MkioUsbCtrl.mt[iMtIdent].hEvent );
  dwRet= MT_Start( nSize,MkioUsbCtrl.mt[iMtIdent].hEvent );
                                            if( dwRet!=0 ) return FALSE;
  return TRUE;
};



WORD MkioUsb_Mt_McoGetNumFrames( int iMtIdent )
{
  return MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames;
};


typedef struct tagMCOSTATE
{
  union
  {
    struct
    {
      unsigned short uUnuse       : 10;
      unsigned short uSize        :  6;
    } Bf;
    WORD uWord;
  };
} MT_MCOSTATE;

typedef struct tagMKIO_MT_MCO_FRAME
{
  MT_MCOSTATE  Sw;
  WORD         nData;
  DWORD        dwTime;
  WORD         uData[64];
} MT_MCOFRAME;


int MkioUsb_Mt_McoReadFrames( int iMtIdent, MT_MSGINFO* pFrameArr[], int nMaxFrames )
{
  DWORD         dwRet   ;
  DWORD         nFrames ;
  MT_MSGINFO*   pResFrame;
  MT_MCOFRAME   Mco     ;
  DWORD         iFrame  ;
  WORD*         pData   ;
  WORD*         pMcoNext;
  WORD          nMcoData,i;
  WORD          uLoTime,uHiTime;

  if( !MkioUsb_Mt_Select(iMtIdent) ) return 0;


  dwRet= MT_GetMessage( MkioUsbCtrl.mt[iMtIdent].pMcoBuffer, MkioUsbCtrl.mt[iMtIdent].uMcoBufSize, FALSE, &nFrames );
                                                                                          if( dwRet  !=0 ) return -1;
                                                                                          if( nFrames==0 ) return  0;
                                                                                  if( nFrames>(DWORD)nMaxFrames ) return -2;
  MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames= (WORD)nFrames;

  pMcoNext= MkioUsbCtrl.mt[iMtIdent].pMcoBuffer;
  iFrame= 0;

  for( iFrame=0; iFrame<nFrames; iFrame++ )
  {
    pData       =  pMcoNext;

    Mco.Sw.uWord= *pData; pData++;
    uHiTime     = *pData; pData++;
    uLoTime     = *pData; pData++;
    Mco.dwTime  =  MAKELONG( uLoTime,uHiTime );

    nMcoData    =  Mco.Sw.Bf.uSize;

    for( i=0; i< nMcoData; i++ )
    {
      Mco.uData[i]= *pData; pData++;
    };
    pMcoNext = pData;

    pResFrame = pFrameArr[iFrame];
    pResFrame->iType   = 1;
    pResFrame->Sw.uWord= Mco.Sw.uWord;
    pResFrame->dwTime  = Mco.dwTime;

    MkioUsb_Mt_DetectFrame( Mco.uData, pResFrame );

    pResFrame->bMcoError= FALSE;

    switch( pResFrame->iFmt )
    {
    default:
     {
      pResFrame->bMcoError= TRUE ; 
      pResFrame->bAw      = FALSE;
     }; break;
    case  1:
     {
       if( nMcoData != ( pResFrame->nData+2 ) )
       {
                                            pResFrame->bMcoError= TRUE ; 
         if(nMcoData<=(pResFrame->nData-1)) pResFrame->bAw      =FALSE;
       };
     };break;
    case  2:
     {
       if( nMcoData != ( pResFrame->nData+2 ) )
       {
                                            pResFrame->bMcoError= TRUE ; 
         if( nMcoData< 2                  ) pResFrame->bAw      =FALSE;
       };
     };break;
    case  4:
     {
       if( nMcoData <  2                      )
       {
                                            pResFrame->bMcoError= TRUE ; 
                                            pResFrame->bAw      =FALSE;
       };
     };break;
    case  5:
     {
       if( nMcoData <  3                      )
       {
                                            pResFrame->bMcoError= TRUE ; 
         if( nMcoData< 2                  ) pResFrame->bAw      =FALSE;
       };
     };break;
    case  6:
     {
       if( nMcoData <  3                      )
       {
                                            pResFrame->bMcoError= TRUE ; 
                                            pResFrame->bAw      =FALSE;
       };
     };break;
    case  7:
     {
       if( nMcoData != ( pResFrame->nData+1 ) )
       {
                                            pResFrame->bMcoError= TRUE ; 
       };
     };break;

    case  9:
     {
       if( nMcoData != 1                      )
       {
                                            pResFrame->bMcoError= TRUE ; 
       }; 
     }; break;

    case 10:
     {
       if( nMcoData != 2                      )
       {
                                            pResFrame->bMcoError= TRUE ; 
       }; 
     };break;
    };
  };
  return (int)nFrames;
};

/*
   Смещение в  базе:	00	01	02		…		32	33	34	35		…		63
1+ KK-ОУ	            KC	ИС	ИС	…	…	ИС	ИС	ОС						
2+ OУ-KK	            KC	ОС	ИС	ИС	…	…	ИС	ИС						
3. OУ-OУ	            KC1	KC2	OC2	ИС	ИС	…	…	ИС	ИС	ОС1				
4+ КС-ОС	            KC	ОС												
5+ КС-ОС+ИС	         KC	ОС	ИС											
6+ КС+ИС-ОС	         KC	ИС	ОС											
7+ КК-ОУгр.	         KC	ИС	ИС	…	…	ИС	ИС							
8. ОУ-ОУгр.	         KC1	KC2	OC2	ИС	ИС	…	…	ИС	ИС					
9+ КСгр.	            KC													
10+КС+ИСгр. 	        KC	ИС												
*/



BOOL MkioUsb_Mt_McoStop( int iMtIdent)
{
  if( !MkioUsb_Mt_Select(iMtIdent) ) return FALSE;

  delete MkioUsbCtrl.mt[iMtIdent].pMcoBuffer;
  MkioUsbCtrl.mt[iMtIdent].pMcoBuffer = NULL;
  MkioUsbCtrl.mt[iMtIdent].uMcoBufSize= NULL;
  MkioUsbCtrl.mt[iMtIdent].uMcoNumFrames=  0;
  return TRUE;
};

//>>><<<
/*
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
*/











//////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagMKIOREDANADAPTERCTRL
{
//  HANDLE  hThread;
//  DWORD   dwIdThread;
//  WORD uMode     ;
  int  iPriority ;
  int  iBlock    ;
  int  iPortion  ;
  int  nPortion  ;
  WORD uTail     ;
  WORD numReceivedWord;
  WORD uRecvData[32000];
} REDANADAPTERCTRL;

typedef struct tagMKIOREDANCTRL
{
  BOOL bOpen     ;
  WORD uTmk      ;
  WORD iBcIdent  ;
  WORD uAddress  ;
  WORD uSubAddress;
  WORD uProcessor;
  WORD setReceiveNumBlock ;//setReceiveNumBlock ;
  WORD setReceiveSizeBlock;//setReceiveSizeBlock;
  WORD nRecvNumWord  ;
  REDANADAPTERCTRL adp;
  WORD numReceivedBlock;
  WORD numReceivedWord ;
//  WORD uRecvData[8192]; 
} MKIOREDANCTRL;

MKIOREDANCTRL MkioUsbRedan= { FALSE, -1, -1, 0x11, -1 };



BOOL MkioUsb_Redan_Reset()
{
  MkioUsbRedan.bOpen               = FALSE;
  MkioUsbRedan.iBcIdent            = -1;
  MkioUsbRedan.uAddress            = 0x11;
  MkioUsbRedan.uSubAddress         = 28;
  MkioUsbRedan.uTmk                = -1;
  MkioUsbRedan.uProcessor          = -1;
  MkioUsbRedan.setReceiveNumBlock  = -1;
  MkioUsbRedan.setReceiveSizeBlock = -1;
  MkioUsbRedan.nRecvNumWord        = -1;
  MkioUsbRedan.numReceivedBlock    =  0;
  MkioUsbRedan.numReceivedWord     =  0;
  MkioUsbRedan.adp.iPortion        =  0;
  MkioUsbRedan.adp.iBlock          =  0;
  MkioUsbRedan.adp.numReceivedWord =  0;
  return TRUE;
};

/*
  Протокол обмена по РЕДАНу использует 3 подадреса для собственных целей:
  Подадрес:  28 - подадрес для обмена данными
             29 - сброс выбранного процессора
             30 - выбор процессора
*/

BOOL MkioUsb_Redan_IsRedanIoSubaddress( WORD uSubaddress )
{
  if( uSubaddress == 28 ) return TRUE;
  return FALSE;
};

BOOL MkioUsb_Redan_IsRedanSubaddress( WORD uSubaddress )
{
  if( uSubaddress == 28 || uSubaddress == 29 || uSubaddress == 30 ) return TRUE;
  return FALSE;
};



BOOL MkioUsb_Redan_IsDataReady( BOOL bMode )
{
  BOOL bRet;
  WORD uCheckWord;
  WORD uMode;
  WORD uCmdWord;
  WORD uAdr;
  WORD uCommand;
  int  iBcIdent;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  iBcIdent = MkioUsbCtrl.iBcSelected;

// Контролируем готовность канала к передаче запрошенных данных, готовность
//        канала индицируется снятием бита "Неисправность ОУ" в ОС.
//
  uCommand =   0x0008; // Команда "Установить ОУ в исх.состояние"
  uAdr     =  (MkioUsbRedan.uAddress&0x1F)<<11; 
  uCmdWord =   uAdr  |   0x07e0;
  uCmdWord|=   uCommand;

  uMode    = CTRL_C_A | CX_STOP | CX_SIG | MkioUsbCtrl.bc[iBcIdent].uLine;

  bcdefbase( COMMAND_BASE );
  if( uCmdWord!=MkioUsbCtrl.bc[iBcIdent].uLastCommandBaseWord )
  {
      bcputblk(  0, &uCmdWord, 1 );
      MkioUsbCtrl.bc[iBcIdent].uLastCommandBaseWord= uCmdWord;
  };
  bcdefbase( COMMAND_BASE );
  MkioUsb_Bc_SelectLineForStart( iBcIdent );
  bcstart( COMMAND_BASE,uMode );
  switch ( WaitForSingleObject(MkioUsbCtrl.bc[iBcIdent].hEvent,1000))
  {
   default           :
     {
       return FALSE;
     };
   case WAIT_TIMEOUT :
     {
       return FALSE;
     };
   case WAIT_OBJECT_0: ResetEvent(MkioUsbCtrl.bc[iBcIdent].hEvent);break;
  };
  tmkgetevd(&MkioUsbCtrl.bc[iBcIdent].Evd);
  switch( MkioUsbCtrl.bc[iBcIdent].Evd.nInt )
  {
  default: return FALSE;
  case  1: // bcIntNorm - Нормальное завершение одиночного обмена, этот ти прерывания возникает
   {       //             при наличии готовности данных на изделии( reset FL2 )
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
   	 MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = LOWORD(bcgetansw(CTRL_C_A));
// - При варианте "Сокращенный контроль" выходим
  	 if( bMode==FALSE )	return TRUE;
// - При варианте "Полный контроль" для подтверждения готовности данных пошлем
// 	 запрос слова данных с подадреса 29, в его мл.бите должно быть значение 0.
     bRet = MkioUsb_Bc_SetRequestMode( MkioUsbRedan.uAddress, 29, 1 ); if( !bRet ) return FALSE; 
     bRet = MkioUsb_Bc_SendRequest( &uCheckWord );                     if( !bRet ) return FALSE;
     if( uCheckWord&0x0001 )                                                       return FALSE;
     return TRUE;
   };break;
  case  2: // bcIntExec - завершение с исключит.ситуацией, это прерывание возникает при выставленном
   {       //             бите <Неисправность ОУ> в ОС, который сигнализирует о неготовности данных
           //             на изделии( set FL2 )
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wResult;
     MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = MkioUsbCtrl.bc[iBcIdent].Evd.bc.wAW1;
     return FALSE;
   };break;
  case  3: // bcIntX    - завершение(останов) цепочки обмена
   {// При запросе Готовности этоn тип прерывания возникнуть не может, поэтому ошибка
     MkioUsbCtrl.bc[iBcIdent].bState           = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfState.uInt     = MkioUsbCtrl.bc[iBcIdent].Evd.nInt;
     MkioUsbCtrl.bc[iBcIdent].bfState.uWord    = MkioUsbCtrl.bc[iBcIdent].Evd.bcx.wResultX;
     MkioUsbCtrl.bc[iBcIdent].bAnsWord         = TRUE;
     MkioUsbCtrl.bc[iBcIdent].bfAnsWord.uWord  = LOWORD(bcgetansw(CTRL_C_A));
     return FALSE;
   };break;
  };

  return TRUE;
};



BOOL MkioUsb_Redan_Open( int iTmkDevice )
{
  int iBcIdent;
  if( !MkioUsb_Open() )                                             return FALSE;
  MkioUsb_Redan_Reset();
  iBcIdent= MkioUsb_Bc_CreateDevice( iTmkDevice ); if( iBcIdent<0 ) return FALSE;
  MkioUsbRedan.uTmk    = iTmkDevice;
  MkioUsbRedan.iBcIdent= iBcIdent;
  MkioUsbRedan.bOpen   = TRUE;
  // установим отсутствие реакции на бит "Неисправность ОУ"
  MkioUsb_Bc_SetAwIgnoreErrorMask(MkioUsbRedan.iBcIdent,~(0x0001));
  return TRUE;
};


BOOL MkioUsb_Redan_Close()
{
  if(!MkioUsbRedan.bOpen) return FALSE;
  MkioUsb_Bc_SetAwIgnoreErrorMask(MkioUsbRedan.iBcIdent,0xFFFF);
  MkioUsb_Bc_Free(MkioUsbRedan.iBcIdent);
  MkioUsb_Close();
  MkioUsb_Redan_Reset();
  return TRUE;
};

BOOL MkioUsb_Redan_SetAddress( WORD uBoardAddress  )
{
  MkioUsbRedan.uAddress=uBoardAddress;
  return TRUE;
};

// Назначить процессор-абонент обмена по МКИО
// Вход:    nProc - номер ADSP-процессора
// Возврат: ==0    - Ок
//          !=0    - Error
__int16 MkioUsb_Redan_SetProcessor( WORD uProcessor  )
{
  WORD uCmdCode;
  BOOL bRet    ;
  char pszErrMsg[1024];
  if(!MkioUsbRedan.bOpen) return 1;

  uCmdCode     = uProcessor & 0x0007;
  bRet         = MkioUsb_Bc_SendCommand( MkioUsbRedan.uAddress, 30, 1, &uCmdCode );
  if( !bRet )
  {
    sprintf( pszErrMsg,"Операция: Выбор процессора СКУ #%u\n",uProcessor );
    MkioUsb_Bc_ErrorMessageBox( MkioUsbRedan.iBcIdent,pszErrMsg );
    return 1;
  };
  MkioUsbRedan.uProcessor = uCmdCode;
  return 0;
};




// Выдать командную последовательность  в канал МКИО для процессора-абонента
// Вход:    uCmdID   - идентификатор команды
//          uCmdLen  - количество слов , в командной последовательности
//          pCmdData - указатель на массив командных слов
// Возврат: ==0      - Ок
//          !=0      - Ошибка канала
// Пример:
//           WORD  command_array[23]; 
//           for(i=0;i<23;i++) command_array[i]=(WORD)i;
//           MAN95_SendCommandSequence( 0x81,23,command_array);
//
__int16 MkioUsb_Redan_SendCommandSequence( WORD uCmdID, WORD uCmdLen, WORD* pCmdData )
{
     	WORD   uSend[256];
      WORD   uCmd,uLen,uLen256;
						WORD*  pSrc;
						BOOL   bRet;
						char   pszErrMsg[1024];
						int    i;
    	 if(!MkioUsbRedan.bOpen) return 1;

						if( uCmdLen>256 )
						{
							 sprintf( pszErrMsg,"Для команды 0x%04X, Прц.#%u, ОУ#%u\n"
									                  "задана слишком большая длина данных %u\n,"
									                  "но она не должна превышать 256 слов!!!",
                            uCmdID,MkioUsbRedan.uProcessor,MkioUsbRedan.uAddress,
																												uCmdLen );
 							MkioUsb_MessageBox( NULL,pszErrMsg,"Ошибка протокола МКИО(Redan)",MB_OK|MB_ICONERROR);
									return 1;
						};

// 1. Подключим РЕДАНовский ТМК
     	bRet = MkioUsb_Bc_Select(MkioUsbRedan.iBcIdent);

// 2. Устраним реакцию на бит ОС "Неисправность ОУ"
      MkioUsb_Bc_SetAwIgnoreErrorMask(MkioUsbRedan.iBcIdent,~(0x0001));

// 3. Пошлем команду на выбранный процессор
      uCmd     =  uCmdID<<8;
						uLen256  =  uCmdLen;
						uLen     =  uCmdLen; if( uLen>=256 ) uLen=0;
      uSend[0] =  uCmd|uLen;
						pSrc     =  pCmdData;
						for( i=1; i<=uCmdLen; i++ )
						{
        uSend[i]= *pSrc;
								pSrc++;
						};
      bRet = MkioUsb_Bc_SendCommand( MkioUsbRedan.uAddress,MkioUsbRedan.uSubAddress, uLen256+1,uSend );
     	if( !bRet )
						{ 
    		  sprintf( pszErrMsg,"Операция: Команда:0x%04X, Проц.#%u, ОУ#%u\n",
        									uSend[0],MkioUsbRedan.uProcessor,MkioUsbRedan.uAddress);
        MkioUsb_Bc_ErrorMessageBox( MkioUsbRedan.iBcIdent,pszErrMsg );
								return 2;
						};
      return 0;
};





// Получить количество IRQ-прерываний,уже принятых приемным блоком драйвера
// Возврат: Количество уже принятых прерываний
// Рекомендации: Обращаться к этой функции при мониторинге состояния приемного
//               блока драйвера,например в методе APP.OnIdle();

BOOL MkioUsb_Bc_RequestLinkBases()
{
 int  iRet , iBcIdent;
	WORD uBase, uBaseBeg, uBaseEnd, uMode, nBase;

	iBcIdent = MkioUsbCtrl.iBcSelected;

 if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
 if(  MkioUsbCtrl.bc[iBcIdent].uReqChainSize== 0   ) return FALSE;

 nBase     = MkioUsbCtrl.bc[iBcIdent].uReqChainSize/32;
             if(MkioUsbCtrl.bc[iBcIdent].uReqChainSize%32) nBase++;

 MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase = nBase;

 uBaseBeg  = REQUEST_CHAIN_START_BASE;
 uBaseEnd  = REQUEST_CHAIN_START_BASE + MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase-1;

 for( uBase=uBaseBeg+1; uBase<=uBaseEnd; uBase++ )
	{
   uMode   = DATA_RT_BC | CX_NOSIG | (MkioUsbCtrl.bc[iBcIdent].uLine) ;// CX_SIG Нельзя !!!
			if( uBase<uBaseEnd )		uMode |= CX_CONT;
			else               			uMode |= CX_STOP;
   bcdefbase( uBase-1 );
   iRet = bcdeflink( uBase,uMode );
			switch( iRet )
			{
					case            0: break;
					case  BC_BAD_BASE: return FALSE;
					case  BC_BAD_FUNC: return FALSE;
					default          : return FALSE;
			};
	};
	return TRUE;
};



BOOL MkioUsb_Bc_RequestUnlinkBases()
{
  int  iRet;
  int  iBcIdent;
  WORD uBase, uMode, uBaseEnd;

  iBcIdent = MkioUsbCtrl.iBcSelected;

  if( !MkioUsb_Bc_Select( MkioUsbCtrl.iBcSelected ) ) return FALSE;
  if(  MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase==0   ) return TRUE;

  uBaseEnd  = REQUEST_CHAIN_START_BASE + MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase-1;
  for( uBase= REQUEST_CHAIN_START_BASE; uBase<=uBaseEnd; uBase++ )
  {
    uMode  = DATA_RT_BC | CX_STOP | CX_SIG | (MkioUsbCtrl.bc[iBcIdent].uLine);
    bcdefbase( uBase-1 );
    iRet   = bcdeflink( uBase,uMode );
    switch( iRet )
    {
      case            0: break;
      case  BC_BAD_BASE: return FALSE;
      case  BC_BAD_FUNC: return FALSE;
      default          : return FALSE;
    };
  };
  MkioUsbCtrl.bc[iBcIdent].uReqLinkedBase=0;
  MkioUsbCtrl.bc[iBcIdent].uReqChainSize =0;
  MkioUsbCtrl.bc[iBcIdent].bChain        =FALSE;
  return TRUE;
};


// Задать формат приема по МКИО.
// Вход:    uNumBlocks - количество ожидаемых(принимаемых) IRQ-прерываний
//          uSizeBlock - количество слов,читаемых в каждом IRQ-прерываний
// Возврат: ==0    - Ок
//          !=0    - Ошибка канала
// Рекомендации: Обратится к этой функции перед началом приема по МКИО
// вместе с обращением к _ReceiverClean() и _IrqEnable();

__int16 MkioUsb_Redan_SetReceiveMode( WORD uNumBlocks, WORD uSizeBlock )
{
  WORD   nWords, nWordsMax;
  char   pszErrMsg[1024];
  BOOL   bRet;
  if(!MkioUsbRedan.bOpen) return 1;

  nWordsMax = 32 * REQUEST_CHAIN_NUM_BASES;
  if( uSizeBlock>nWordsMax )
  {
    sprintf( pszErrMsg,"Для запроса Прц.#%u, ОУ#%u\n"
                       "задана слишком большая длина данных %u\n,"
                       "но она не должна превышать %u слов!!!",
             MkioUsbRedan.uProcessor,MkioUsbRedan.uAddress,
             uSizeBlock,nWordsMax );
    MkioUsb_MessageBox( NULL,pszErrMsg,"Ошибка протокола МКИО(Redan)",MB_OK|MB_ICONERROR);
    return 1;
  };
//  nWordsMax = sizeof(MkioUsbRedan.uRecvData)/sizeof(WORD);
  nWordsMax = sizeof(MkioUsbRedan.adp.uRecvData)/sizeof(WORD);
  nWords    = uNumBlocks * uSizeBlock;
  if( nWords>nWordsMax )
  {
    sprintf( pszErrMsg,"Для запроса Прц.#%u, ОУ#%u, блоков %u по %u слов\n"
                       "задана слишком большая общая длина данных %u\n,"
                       "но она не должна превышать %u слов!!!",
             MkioUsbRedan.uProcessor,MkioUsbRedan.uAddress,
             uNumBlocks,uSizeBlock,nWords,nWordsMax );
    MkioUsb_MessageBox( NULL,pszErrMsg,"Ошибка протокола МКИО(Redan)",MB_OK|MB_ICONERROR);
    return 1;
  };
  MkioUsbRedan.setReceiveNumBlock = uNumBlocks;
  MkioUsbRedan.setReceiveSizeBlock= uSizeBlock;
  MkioUsbRedan.nRecvNumWord       =	nWords;

  bRet = MkioUsb_Bc_SetRequestMode( MkioUsbRedan.uAddress, MkioUsbRedan.uSubAddress, uSizeBlock ); if( !bRet) return 1; 
  return 0;
};

int MkioUsb_Redan_SetReceiveModeEx( WORD uNumBlocks, WORD uSizeBlock )
{
  char   pszErrMsg[1024];
  DWORD  uSizeMem;
  DWORD  uSizeMax;
  BOOL   bRet;
  if(!MkioUsbRedan.bOpen) return 1;

  uSizeMax =  sizeof(MkioUsbRedan.adp.uRecvData)/sizeof(WORD);
  uSizeMem = (DWORD)uNumBlocks*(DWORD)uSizeBlock;
  if( uSizeMem>uSizeMax )
  {
    sprintf( pszErrMsg,"Для запроса Прц.#%u, ОУ#%u\n"
                       "задана слишком большая длина данных %u\n,"
                       "cуммарно она не должна превышать %u слов!!!",
             MkioUsbRedan.uProcessor,MkioUsbRedan.uAddress,
             uSizeMem, uSizeMax );
    MkioUsb_MessageBox( NULL,pszErrMsg,"Ошибка протокола МКИО(Redan)",MB_OK|MB_ICONERROR);
    return 1;
  };
  MkioUsbRedan.setReceiveNumBlock = uNumBlocks;
  MkioUsbRedan.setReceiveSizeBlock= uSizeBlock;
  MkioUsbRedan.nRecvNumWord       =	(WORD)uSizeMem;

  bRet = MkioUsb_Bc_SetRequestMode(  MkioUsbRedan.uAddress, MkioUsbRedan.uSubAddress, uSizeBlock  ); if( !bRet) return 1; 
  return 0;
};



int MkioUsb_Redan_GetNumReceivedIrq()
{
   int    iBlock  , iStartBlock, nBlock;
   char   pszErrMsg[1024];
   BOOL   bRet;
   BOOL   bIsRedanSA;
   WORD   uAddress,uSubAddress;
   DWORD  dwTimeReady;

   if( !MkioUsbRedan.bOpen              ) return 0;

   bIsRedanSA = MkioUsb_Redan_IsRedanIoSubaddress( MkioUsbRedan.uSubAddress );

   if( bIsRedanSA )// Аппарат получения готовности через бит НЕИСПРАВНОСТЬ ОУ 
   {               // действует только для РЕДАН-подадресов
     if( !MkioUsb_Redan_IsDataReady(TRUE) )
     {
	    return MkioUsbRedan.adp.iBlock;
     };
   };

   uAddress   = MkioUsbRedan.uAddress          ;
   uSubAddress= MkioUsbRedan.uSubAddress       ;
   nBlock     = MkioUsbRedan.setReceiveNumBlock;
   iStartBlock= MkioUsbRedan.adp.iBlock        ;

   bRet       = MkioUsb_Bc_SetRequestMode( uAddress, uSubAddress, MkioUsbRedan.setReceiveSizeBlock );
                                                                                         if( !bRet )  return MkioUsbRedan.adp.iBlock;

   for( iBlock= iStartBlock; iBlock<nBlock; iBlock++ )
   {
      bRet = MkioUsb_Bc_SendRequest( &MkioUsbRedan.adp.uRecvData[MkioUsbRedan.adp.numReceivedWord] ); // Адрес Приемника данных
                                                                	                        if( !bRet ) goto error;
      MkioUsbRedan.adp.numReceivedWord +=MkioUsbRedan.setReceiveSizeBlock;
      MkioUsbRedan.adp.iBlock++;
      if( iBlock!=(nBlock-1) )
      {
        if(  bIsRedanSA )
        {
            dwTimeReady=::GetTickCount();
            while( (::GetTickCount()-dwTimeReady)<2000 )
            {
              if( MkioUsb_Redan_IsDataReady(FALSE)==TRUE ) continue;
            };
            return MkioUsbRedan.adp.iBlock;
        };
      };
   };
   return MkioUsbRedan.adp.iBlock;
error:
   sprintf( pszErrMsg,"Операция: Запрос Прц.#%u, ОУ#%u Блоков %u по %u слов\n",
                       MkioUsbRedan.uProcessor        , MkioUsbRedan.uAddress,
					   MkioUsbRedan.setReceiveNumBlock, MkioUsbRedan.setReceiveSizeBlock );
   MkioUsb_Bc_ErrorMessageBox( MkioUsbRedan.iBcIdent,pszErrMsg );
   return MkioUsbRedan.adp.iBlock;
};





BOOL MkioUsb_Redan_IrqEnable()
{
  if(!MkioUsbRedan.bOpen) return FALSE;
  MkioUsb_Bc_IrqEnanle( MkioUsbRedan.iBcIdent, TRUE );
  return TRUE;
};

BOOL MkioUsb_Redan_IrqDisable()
{
  if(!MkioUsbRedan.bOpen) return FALSE;
  MkioUsb_Bc_IrqEnanle( MkioUsbRedan.iBcIdent, FALSE );
  return TRUE;
};

// Прочитать информацию,принятую драйвером по каналу МКИО
// Вход:    Len      - количество слов , считываемых из приемного блока драйвера
//                     как правило,равно произведению параметров обращения к функции
//                     MAN95_SetReceiveMode( WORD numBlock, WORD sizeBlock ); 
//          pInfo    - указатель на принимающий массив 

void MkioUsb_Redan_GetReceivedInfo( WORD Len, WORD volatile *pInfo )
{
  if(!MkioUsbRedan.bOpen) return;
//  memcpy( MkioUsbRedan.uRecvData, MkioUsbRedan.adp.uRecvData, sizeof(WORD)*Len ); 
  memcpy( (void*)pInfo, MkioUsbRedan.adp.uRecvData, sizeof(WORD)*Len ); 
};

void MkioUsb_Redan_GetReceivedInfoEx( DWORD nStartWord, DWORD nWords, PWORD pDest )
{
  if(!MkioUsbRedan.bOpen) return;
//  memcpy( &MkioUsbRedan.uRecvData[nStartWord],&MkioUsbRedan.adp.uRecvData[nStartWord], sizeof(WORD)*nWords ); 
  memcpy( (void*)pDest, &MkioUsbRedan.adp.uRecvData[nStartWord], sizeof(WORD)*nWords ); 
};


// Получить слово состогяния канала МКИО
// Вход:    pStatusWord  - указатель на DWORD-слово,принимающее статус канала
//                         LOWORD(pStatusWord)- ответное слово(ОС)  
//                         HIWORD(pStatusWord)- слово расширенного результата обмена
// Возврат: ==0    - Ок
//          !=0    - Ошибка канала

__int16 MkioUsb_Redan_GetChannelStatus( DWORD *pStatusWord )
{
  int   j;
  WORD  uAnsWord,uStateWord; 
  DWORD dwRet;
  if(!MkioUsbRedan.bOpen) return 1;

  j = MkioUsbRedan.iBcIdent;
  uAnsWord    = MkioUsbCtrl.bc[j].bfAnsWord.uWord;
  uStateWord= MkioUsbCtrl.bc[j].bfState.uWord;
  dwRet       = MAKELONG( uAnsWord,uStateWord );
  *pStatusWord= dwRet;
  return 0;
};



// Очистить приемный блок драйвера,перевести его в исходное состояние.
// Bозврат: Отсутствует
// Рекомендации: Обратится к этой функции перед началом приема по МКИО
void MkioUsb_Redan_ReceiverClean()
{
  MkioUsbRedan.adp.iBlock         = 0;
  MkioUsbRedan.adp.numReceivedWord= 0;
  MkioUsbRedan.numReceivedBlock   = 0;
  MkioUsbRedan.numReceivedWord    = 0;
};

// Немедленно прекратить все операции ввода-вывода на канале МКИО
// Возврат: ==0    - Ок
//          !=0    - Ошибка канала
__int16 MkioUsb_Redan_AbortReceive()
{
// Эта функция теперь не нужна, т.к. обмен по МКИО через USB-адаптер
// делает ее не нужной
  return 0;
};


// Установить режим анализа готовности любого процессора
// Возврат: ==0    - Ок
//          !=0    - Ошибка канала
//
__int16 MkioUsb_Redan_SetAskOneProcessor()
{
// Эта функция теперь не нужна, т.к. обмен по МКИО через USB-адаптер
// уже работает по режиму, ранее активируемому этой функцией.
  return 0;
};

// Установить режим анализа готовности любого процессора
// Возврат: ==0    - Ок
//          !=0    - Ошибка канала
__int16 MkioUsb_Redan_SetAskAnyProcessor()
{
// Эта функция теперь не нужна!
  return 0;
};

__int16 MkioUsb_VJ2_SetAddress( WORD uAddress )
{
  MkioUsbRedan.uAddress=uAddress;
  return 0;
}
__int16 MkioUsb_VJ2_SetSubAddress( WORD uSubAddress )
{
  MkioUsbRedan.uSubAddress=uSubAddress;
  return 0;
}


BOOL MkioUsb_VJ2_SendCommand( WORD wLenSeq, WORD* pInfo )
{
  return MkioUsb_Bc_SendCommand( MkioUsbRedan.uAddress, MkioUsbRedan.uSubAddress,wLenSeq,pInfo );
};


__int16 MkioUsb_Manch_WriteWord( WORD uData )
{
  int   j,iRet;
  WORD  uCmd,uLen; 
  WORD* pFrame;

  j = MkioWriteWordFrame.iIndx;
  if( j==0 )
  {
    MkioWriteWordFrame.uCommand   =uData>>8;
    MkioWriteWordFrame.uSizeFrame =uData&0x00ff;
          if( MkioWriteWordFrame.uSizeFrame==0 ) MkioWriteWordFrame.uSizeFrame=256;
    MkioWriteWordFrame.iCountDown =     (__int16)MkioWriteWordFrame.uSizeFrame;
    MkioWriteWordFrame.uFrame[j]  =uData;
    j++;
  }
  else
  {
    MkioWriteWordFrame.uFrame[j]=uData;
    MkioWriteWordFrame.iCountDown--;
    j++;
  };
  MkioWriteWordFrame.iIndx = j;

  if( MkioWriteWordFrame.iCountDown<=0 )
  {
    if( MkioWriteWordFrame.uCommand&0x0080 )
    {
      uCmd   =  MkioWriteWordFrame.uCommand;
      uLen   =  MkioWriteWordFrame.uSizeFrame;
      pFrame = &MkioWriteWordFrame.uFrame[1];
      iRet   =  MkioUsb_Redan_SendCommandSequence( uCmd, uLen, pFrame );
      MkioWriteWordFrame.iIndx             =        0;
      MkioWriteWordFrame.uCommand          =        0;
      MkioWriteWordFrame.uSizeFrame        =        0;
      MkioWriteWordFrame.iCountDown        =        0;
      return iRet;
    }
    else
    {
      MkioWriteWordFrame.iIndx             =        0;
      MkioWriteWordFrame.uCommand          =        0;
      MkioWriteWordFrame.uSizeFrame        =        0;
      MkioWriteWordFrame.iCountDown        =        0;
      return 0;
    };
  };
  return 0;
};


BOOL MkioUsb_ResetAbonent( WORD uRTCurrentAddress, WORD uRTAfterResetAddress )
{
 BOOL bRet;
 WORD uProc;
 char s[1024];

 uProc = 0; bRet = MkioUsb_ResetProcessor( uProc, uRTCurrentAddress   , uRTAfterResetAddress ); if( !bRet ) goto error;
// Теперь игнорируем возникновение ошибки обмена, т.к. будет выставлен бит "Неисправность ОУ"
 uProc = 1; bRet = MkioUsb_ResetProcessor( uProc, uRTAfterResetAddress, uRTAfterResetAddress );// n
 uProc = 2; bRet = MkioUsb_ResetProcessor( uProc, uRTAfterResetAddress, uRTAfterResetAddress );
 return TRUE;
error:
   sprintf( s,"Возникла ошибка при попытке сброса процессора платы СКУ.\n"
              "Адрес ОУ : 0x%02X (%u)\n"
              "Процессор: %u           ",uRTCurrentAddress,uRTCurrentAddress, uProc );
	MkioUsb_MessageBox(NULL,s,"Ошибка аппаратуры",MB_OK|MB_ICONERROR);
   return FALSE;
};



BOOL MkioUsb_ResetProcessor( WORD uProcessor, WORD uRTCurrentAddress, WORD uRTAfterResetAddress  )
{
	char              pszMessage[1024];
	int              iBcIdent;
	BOOL             bRet;
 MKIO_LOADER_INFO li;
 WORD             uCmdCode;

	li.Service_bDriverOpened=FALSE;
	li.Service_bBcCreated   =FALSE;
 li.pProgress            =NULL ;
 li.uAddress             =uRTCurrentAddress;
 li.uTmk                 =0;
 li.uNumFiles            =0;
 li.uFile[0].bRunProcessor=FALSE;
 li.uFile[0].uProcessor   =0;
 lstrcpy( li.uFile[0].pszName,"\0");


	if(	!MkioUsbCtrl.bOpen )
	{
   if( !MkioUsb_Open() )   goto error_11;
  	li.Service_bDriverOpened=TRUE;
	};

	if(MkioUsb_IsDeviceFree(li.uTmk)==TRUE)
	{
   iBcIdent= MkioUsb_Bc_CreateDevice(li.uTmk);	if( iBcIdent<0 )  goto error_10;
			li.Service_bBcCreated   =TRUE;
	}
	else
	{
   iBcIdent= MkioUsb_Bc_GetIdent(li.uTmk);  			if( iBcIdent<0 )  goto error_12;
	};


// 1. Выбор процессора
		uCmdCode     = uProcessor&0x0007;
  bRet         = MkioUsb_Bc_SendCommand( li.uAddress, 30, 1, &uCmdCode );	if( !bRet ) goto error_06;
		Sleep(10);

// 2. Сброс процессора
		uCmdCode     = 0x0007;
  bRet         = MkioUsb_Bc_SendCommand( li.uAddress, 29, 1, &uCmdCode );
// Проигнорируем возможную ошибку, т.к. после сброса процессора, адрес ОУ поменяется с uRTCurrentAddress на uRTAfterResetAddress
// и неизбежно появится ошибка в Расширенном слове результата обмена(wResultX), равная SX_ERAO=5( ошибка адреса ОУ )
//  if( !bRet ) goto error_06;
    Sleep(100);



 if(li.Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
 if(li.Service_bDriverOpened) MkioUsb_Close();
	return TRUE;
/*
error_1:  if(li.Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
          if(li.Service_bDriverOpened) MkioUsb_Close();
          MkioUsb_MessageBox(NULL,"Ошибка при сбросе борта!","Сообщение загрузчика",MB_OK|MB_ICONERROR);
								  return FALSE;
error_2:  if(li.Service_bBcCreated   ) MkioUsb_Bc_Free( iBcIdent );
          if(li.Service_bDriverOpened) MkioUsb_Close();
          MkioUsb_MessageBox(NULL,"Ошибка при сбросе борта!","Сообщение загрузчика",MB_OK|MB_ICONERROR);
								  return FALSE;
*/
error_06: return FALSE;
error_10: sprintf( pszMessage,"Не найдено устройство TA1-USB-01: #%u",li.uTmk );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
          if(li.Service_bDriverOpened) MkioUsb_Close();
								  return FALSE;
error_11: sprintf( pszMessage,"Не открывается драйвер устройства TA1-USB-01" );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
								  return FALSE;
error_12: sprintf( pszMessage,"Устройство TA1-USB-01 #%u\nуже занято под ОУ!",li.uTmk );
					MkioUsb_MessageBox(NULL,pszMessage,"Ошибка комплектации",MB_OK|MB_ICONERROR);
          if(li.Service_bDriverOpened) MkioUsb_Close();
								  return FALSE;
};

// Grek >> (31.05.2011)
#ifdef GREK_INC
BOOL MkioUsb_Redan_SetSubAddress(WORD uBoardSubAddress)
{
	MkioUsbRedan.uSubAddress = uBoardSubAddress;
	return TRUE;
};

BOOL MkioUsb_LoadProc_Ex(WORD wAddr, const BYTE* pbtBuf, long lszBuf)
{
	// Страницы
	const long cPage     = 2*1024;              // Максимальный размер страницы (должно быть кратно 32)
	      long lPageCnt  = lszBuf/cPage;        // Количество страниц
	      long lPageRest = lszBuf%cPage;        // Остаток страницы
	      WORD wPage[cPage];                    // Буфер для хранения страницы
	      memset(static_cast<void*>(wPage), 0, sizeof(wPage));

	// Базовый буфер
	const long cBase     = 32;                  // Максимальный размер базового буфера
	      long lBaseCnt  = cPage/cBase;         // Количество базовых порций
	      long lBaseRest = lPageRest/cBase;     // Остаток базовой порции
	                       if((lPageRest%cBase)>0) ++lBaseRest;
	      WORD wBase[cBase+1];                  // Базовый буфер для обмена

	//------------------
	// Передача страниц
	//------------------

	// Запись командного слова
	wBase[0] = CW( wAddr, RT_RECEIVE, 28, cBase );

	// Буфер для передачи
	const BYTE* pbtCurBuf = pbtBuf;

	// Указатель на буфер
	for(long i=0; i<lPageCnt; ++i)
	{
		// Заполнение буфера страницы
		WORD* pwPage = wPage;

		// ВНИМАНИЕ! cPage должен быть кратен 4
		for(long j=0; j<cPage; j+=4)
		{
			*pwPage++ = *pbtCurBuf++;
			*pwPage++ = *pbtCurBuf++;
			*pwPage++ = *pbtCurBuf++;
			*pwPage++ = *pbtCurBuf++;
		}

		// Заполнение базы
		for(long lBase=0, k=0; lBase<lBaseCnt; k+=cBase, ++lBase)
		{
			// Заполнение базового буфера
			memcpy(static_cast<void*>(&wBase[1]), static_cast<const void*>(&wPage[k]), cBase*sizeof(WORD));

			// Записать в базу
			bcdefbase(static_cast<WORD>(lBase));
			bcputblk(0, wBase, cBase+1);
		}

		// Обмен
		if(!MkioUsb_Bc_SendBaseChain(MkioUsbCtrl.iBcSelected, 0, static_cast<WORD>(lBaseCnt)))
		{
			MkioUsb_MessageBox(NULL,"Ошибка загрузки файла","Ошибка МКИО-канала",MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	//------------------
	// Передача остатка
	//------------------
	if(lPageRest>0)
	{
		// Заполнение буфера страницы
		WORD* pwPage = wPage;
		for(long j=0; j<lPageRest; ++j)
		{
			*pwPage++ = *pbtCurBuf++;
		}

		// Остаток
		WORD  wszBase;
		DWORD dwRest = lPageRest;

		// Заполнение базы
		for(long lBase=0, k=0; lBase<lBaseRest; k+=(wszBase), dwRest-=wszBase, ++lBase)
		{
			// Текущий размер базы
			wszBase = static_cast<WORD>(dwRest>cBase ? cBase:dwRest);

			// Запись командного слова
			wBase[0] = CW(wAddr, RT_RECEIVE, 0x1C, wszBase);

			// Заполнение базового буфера
			memcpy(static_cast<void*>(&wBase[1]), static_cast<const void*>(&wPage[k]), wszBase*sizeof(WORD));

			// Записать в базу
			bcdefbase(static_cast<WORD>(lBase));
			bcputblk(0, wBase, wszBase+1);
		}

		// Обмен
		if(!MkioUsb_Bc_SendBaseChain(MkioUsbCtrl.iBcSelected, 0, static_cast<WORD>(lBaseRest)))
		{
			MkioUsb_MessageBox(NULL,"Ошибка загрузки файла","Ошибка МКИО-канала",MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	// Результат
	return TRUE;
}

BOOL MkioUsb_LoadProcTransit_Ex(WORD wAddr, WORD wProc, const BYTE* pbtBuf, long lszBuf)
{
	// Загрузка программы блоками по nLen байт плюс остаток
	const long lLen  = 16;
	      long lCnt  = lszBuf/lLen; if(lszBuf%lLen) ++lCnt;
	      WORD wSend[lLen+2];
	      memset(static_cast<void*>(wSend), 0, sizeof(wSend));

	// Буфер для передачи
	const BYTE* pbtCurBuf = pbtBuf;

	for(long i=0, lRest=lszBuf, lCur=0; i<lCnt; ++i, lRest-=lCur)
	{
		lCur = (lRest>lLen) ? lLen : lRest;

		// Заголовок массива
		wSend[0] = (0xCB00|static_cast<WORD>(lCur+1));
		wSend[1] = wProc;

		// Данные
		WORD* pwSend = wSend+2;
		for(long j=0; j<lCur; ++j)
		{
			*pwSend++ = *pbtCurBuf++;
		}

		// Обмен
		if(!MkioUsb_Bc_SendCommand(wAddr, 0x1C, static_cast<WORD>(lCur+2), wSend))
		{
			MkioUsb_MessageBox(NULL,"Ошибка загрузки файла (периферия)","Ошибка МКИО-канала",MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	// Результат
	return TRUE;
}
#endif
// Grek <<
