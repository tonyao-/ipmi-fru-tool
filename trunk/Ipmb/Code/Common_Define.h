/*H***************************************************************************
**
**  Copyright  (c)  2011,  zhonghangbit, Inc.
**          All Rights Reserved.
**
**  Subsystem:    NMCP_BaseFrame
**  Created By:   lan yanghai
**  Created On:   08/25/2011
**
**  Purpose:
**      Common_Define.h
**
**  History:
**  Programmer      Date       Rev Number Description
**  --------------- ---------- ---------- -----------------------------
**  lan yanghai     08/25/2011 1.0        Created.
***************************************************************************H*/

#ifndef  COMMON_DEFINE_H
#define  COMMON_DEFINE_H

#include "typedef.h"
//#include "sys_header.h"

#define INPUTVOLTAGE_VALUE	48
/***************   Begin   **************/
//Site Type values
#define SITE_TYPE_CM      0x00//
#define SITE_TYPE_CU      0x04//
#define SITE_TYPE_AMC     0x07//
#define SITE_TYPE_MCH     0x0A//
#define SITE_TYPE_PM      0x0B//

#define CMD_EVENT_LUN	0

//Local Address
#define CM_IPMB_ADDR      0x20
#define MCH_1_IPMB_ADDR   0x10
#define MCH_2_IPMB_ADDR   0x12
#define AMC_1_IPMB_ADDR   0x72
#define AMC_2_IPMB_ADDR   0x74
#define AMC_3_IPMB_ADDR   0x76
#define AMC_4_IPMB_ADDR   0x78
#define AMC_5_IPMB_ADDR   0x7A
#define AMC_6_IPMB_ADDR   0x7C
#define AMC_7_IPMB_ADDR   0x7E
#define AMC_8_IPMB_ADDR   0x80
#define AMC_9_IPMB_ADDR   0x82
#define AMC_10_IPMB_ADDR  0x84
#define AMC_11_IPMB_ADDR  0x86
#define AMC_12_IPMB_ADDR  0x88
#define CU_1_IPMB_ADDR    0xA8
#define CU_2_IPMB_ADDR    0xAA
#define PM_1_IPMB_ADDR    0xC2
#define PM_2_IPMB_ADDR    0xC4
#define PM_3_IPMB_ADDR    0xC6
#define PM_4_IPMB_ADDR    0xC8

#define SHMC_IPMB_ADDR    0xCA

#define PM_1_ChanNum 	0xF1
#define PM_2_ChanNum 	0xF2
#define PM_3_ChanNum 	0xF3
#define PM_4_ChanNum 	0xF4

typedef enum _tagPmSiteNum
{
	  PM_1_SITENUM = 0X01
	, PM_2_SITENUM = 0x02
	, PM_3_SITENUM = 0x03
	, PM_4_SITENUM = 0x04
	
	, MAX_PM_SITENUM
}PmSiteNum;

typedef enum _tagCuSiteNum
{
	  CU_1_SITENUM = 0x01
	, CU_2_SITENUM = 0x02
	
	, MAX_CU_SITENUM
}CuSiteNum;

typedef enum _tagMchSiteNum
{
	  MCH_1_SITENUM = 0x01
	, MCH_2_SITENUM = 0x02
	
	, MAX_MCH_SITENUM
}MchSiteNum;

/*0x01 = System reset, 0x02 = Other PM module reset, 0x04 = Watch dog reset*/
typedef enum _tagEMcmcResetType
{
	RESET_SYSTEM	= 0x01,
	RESET_OTHERPM 	= 0x02,
	RESET_WATCHDOG  = 0x04
}EMcmcResetType;


typedef enum _tagAmcSiteNum
{
	  AMC_1_SITENUM = 0X01
	, AMC_2_SITENUM
	, AMC_3_SITENUM
	, AMC_4_SITENUM
	, AMC_5_SITENUM
	, AMC_6_SITENUM
	, AMC_7_SITENUM
	, AMC_8_SITENUM
	, AMC_9_SITENUM
	, AMC_10_SITENUM
	, AMC_11_SITENUM
	, AMC_12_SITENUM
	
	, MAX_AMC_SITENUM
}AmcSiteNum;

typedef struct _tagAddressEntry
{
	uint8 m_PowerChannel;
	uint8 m_SiteNum;
	uint8 m_SiteType;
	uint8 m_IpmbAddress;
	uint8 m_DeviceId;
}AddressEntry;

#define MAX_AMC_BOARD 	12
#define MAX_PM_BOARD	2
#define MAX_CU_BOARD	2
#define MAX_MCH_BOARD	2

#define MAX_BOARD_COUNT (MAX_AMC_BOARD + MAX_PM_BOARD + MAX_CU_BOARD + MAX_MCH_BOARD)

/*  Power Channel Number  */
typedef enum Power_Channel_Number_Enum
{
    MCH_1_ChanNum = 0,
    MCH_2_ChanNum = 1,
    CU_1_ChanNum = 2,
    CU_2_ChanNum = 3,
    AMC_1_ChanNum  = 4,
    AMC_2_ChanNum  = 5,
    AMC_3_ChanNum  = 6,
    AMC_4_ChanNum  = 7,
    AMC_5_ChanNum  = 8,
    AMC_6_ChanNum  = 9,
    AMC_7_ChanNum  = 10,
    AMC_8_ChanNum  = 11,
    AMC_9_ChanNum  = 12,
    AMC_10_ChanNum = 13,
    AMC_11_ChanNum = 14,
    AMC_12_ChanNum = 15,
    
    MAX_ChanNum
    
}POWER_CHANNEL_NUMBER;

//IIC channel number
#define CM_1_IIC_CHAN 0
#define CM_2_IIC_CHAN 1
#define CU_1_IIC_CHAN  0
#define CU_2_IIC_CHAN  1
#define PM_1_IIC_CHAN  0
#define PM_2_IIC_CHAN  1

#define AMC_1_IIC_CHAN  4 
#define AMC_2_IIC_CHAN  5 
#define AMC_3_IIC_CHAN  6 
#define AMC_4_IIC_CHAN  7 
#define AMC_5_IIC_CHAN  8 
#define AMC_6_IIC_CHAN  9 
#define AMC_7_IIC_CHAN  10
#define AMC_8_IIC_CHAN  11
#define AMC_9_IIC_CHAN  12
#define AMC_10_IIC_CHAN 13
#define AMC_11_IIC_CHAN 14
#define AMC_12_IIC_CHAN 15

#define MCH_1_IIC_CHAN  16
#define MCH_2_IIC_CHAN  16

/* Device ID */
typedef enum Device_ID_Enum
{
    CM_Dev_ID = 0,

    SHELF_FRU_1_Dev_ID = 1,	//Physical Shelf FRU Info 1
    SHELF_FRU_2_Dev_ID = 2,	//Physical Shelf FRU Info 2

    MCH_1_Dev_ID = 3,
    MCH_2_Dev_ID = 4,

    AMC_1_Dev_ID = 5,
    AMC_2_Dev_ID = 6,
    AMC_3_Dev_ID = 7,
    AMC_4_Dev_ID = 8,
    AMC_5_Dev_ID = 9,
    AMC_6_Dev_ID = 10,
    AMC_7_Dev_ID = 11,
    AMC_8_Dev_ID = 12,
    AMC_9_Dev_ID = 13,
    AMC_10_Dev_ID= 14,
    AMC_11_Dev_ID= 15,
    AMC_12_Dev_ID= 16,

    CU_1_Dev_ID = 40,
    CU_2_Dev_ID = 41,

    PM_1_Dev_ID = 50,
    PM_2_Dev_ID = 51,
    PM_3_Dev_ID = 52,
    PM_4_Dev_ID = 53,

    RESERVED_DEVID = 0xff
}DEVICE_ID_ENUM;

/**************   End   ****************/

/***************    Begin   **************/
//Current Event Enum
typedef enum Current_Event_Enum
{
    Module_Handle_Close  = 0,
    Module_Handle_Open  = 1,
    Quiesced,
    Backend_Power_Failure,
    Backend_Power_Shut_Down
    
}CURRENT_EVENT_ENUM;

//FRU Control Enum
typedef enum FRU_Control_Enum
{
    FruCtrl_Cold_Reset  = 0,
    FruCtrl_Warm_Reset  = 1,
    FruCtrl_Graceful_Reboot,
    FruCtrl_Issue_Diagnostic_Interrupt,
    FruCtrl_Quiesce
}FRU_CONTROL_ENUM;

/*  PS#  */
#define PS1_ACTIVE    0x01
#define PS1_INACTIVE    0x00

/*   */
#define ENABLE    0x01
#define DISABLE    0x00

/*  Module LEDs  */
#define  LED_BLUE        0x00
#define  LED1            0x01
#define  LED2            0x02
#define  LED3            0x03


#define CYC_READ_TIME	 10000

/*  Led state    */
typedef enum _tagLedState
{
	  LED_ON	=	0x00
	, LED_LONG_BLINK
	, LED_SHORT_BLINK
	, LED_OFF
}LedState;

enum
{
	 OBJECT_CML 		= 0x01
	,OBJECT_EMcmc 		= 0x02
	,OBJECT_IPMB 		= 0x04
	,OBJECT_PM 			= 0x08
	,OBJECT_FRUDEV		= 0x10
	,OBJECT_FRUDRIVER	= 0x20
	,OBJECT_CM			= 0x40
	,OBJECT_CMDLUN		= 0x80
};

typedef enum _Err_Code
{
    ERR_SUCCESS = 0,
	ERR_FAILED,
	ERR_PENDING,
	ERR_NOT_RSP,
	ERR_UNKNOWN,

	/*10~20   DATA Err Begin *************/
	ERR_FRU_DATA_SUCCESS = 10,
	ERR_FRU_DATA_FAILED,
	ERR_SDR_DATA_SUCCESS,
	ERR_SDR_DATA_FAILED,
	/*10~20   DATA Err End **************/
	
    /*21~40   COM Err Begin *************/
    ERR_PARAMETER_NULL=51,
    ERR_CREATE_TASK_FAIL,                    
    ERR_PARAMETER_FAIL,                      
    ERR_POINTER_NULL,                        
    /*21~40   COM Err End *************/


    /*41~60   Tool Err Begin *************/
    ERR_TOOL_CHECKSUM = 41,
    ERR_SETBIT_OVERFLOW,
    /*41~60    Tool Err End *************/


    /*61~80     PM Err Begin *************/
    ERR_PM_FSM_STATE = 61,                   
    ERR_PM_FSM_GREAT_WDOG,                   
    ERR_PM_FSM_WDOG_NULL,                    
    ERR_PM_RECEIVE_MSG,                      
    ERR_PM_FSM_NO_DONE,                      
	ERR_PM_FSM_MASTER_SELECT,
    /*61~70     PM Err End  *************/

    /*71~80     IPMB Err Begin *************/
	ERR_IPMB_TX_QUEUE_FULL,
	ERR_IPMB_CHECKSUM,
	ERR_IPMB_STATE,
    /*71~80     PM Err End  *************/


    /* 81~100    Msg Err Begin *************/
    ERR_MSG_CHECKSUM = 81,
    EER_MSG_FAIL_CREATE_Q_ID,                
    EER_MSG_Q_ID_NULL,                       
    ERR_SEND_MSG_FAIL,                       
    ERR_QUERY_ADDR_MAP_TABLE_FAIL,           
    ERR_BEING_MSG,                           
    ERR_NO_MSG,                              
    ERR_MSG_LEN_LOCK,                        
    ERR_MSG_LEN_EXCEEDED,                    
    ERR_MSG_LOCK,
	ERR_MSGNOTPROC,
    /*81~100   Msg Err End *************/

    /*101  Temputer Err Begin*/
    ERR_TEMPUTER_ABORMAL = 101,             
    /*110 26C64 Err Begin*/
    ERR_WRITE_24C64_ERR = 110,
    ERR_READ_24C64_ERR,

    // 111~120  PS# Error Begin
    ERR_PS_CHANGE,

    //power channel cmd 
    ERR_GIVE_PP_ERROR,
    
    ERR_No
}ERR_CODE;

/* Device_specified (OEM) codes 01h~7Eh */

/* Command_specified codes 80h~BEh      */
#define PM_FIRST_HEART_BEAT     0x80

//fru device number
#define MAX_FRU_DEVICE_NUM_2 2
#define MAX_FRU_DEVICE_NUM_1 1

enum
{
	FRU_CTRL_COLD_RESET  = 0x00
	,FRU_CTRL_WARM_RESET = 0x01
	,FRU_CTRL_GRACEFUL_REBOOT = 0x02
	,FRU_CTRL_DIAGNOSTIC_INTERRUPT = 0x03
	,FRU_CTRL_QUIESCE 	= 0x04
};

#define FRUDB_BUF_SIZE 		512

const AddressEntry* GetAddressEntry(const uint8 siteNum, const uint8 siteType);

#if __MCMC__
#endif

#if __MCMC_CM__
//Api for Ipmb moudle
extern int Cm_PostMsg(uint8 object, uint8 msgID, uint32 param1, uint32 param2);
#define PostMsg 		Cm_PostMsg

//Api for SwTimer
struct _tagTimerManager;
extern struct _tagTimerManager* Cm_GetTimerManager(void);
#define GetTimerManager Cm_GetTimerManager

//Api for TransferDriver
struct _tagIpmb;
extern struct _tagIpmb* Cm_GetIpmb(uint8 iicChannel);
#define GetIpmb 		Cm_GetIpmb

//Api for Sensor
struct _tagCrb;
extern struct _tagCrb* Cm_GetCrb(uint8 targetAddr, uint8 targetLun, uint8 isReq);
#define GetCrb 			Cm_GetCrb

struct _tagLed;
extern struct _tagLed* Cm_GetLed(uint8 ledType);
#define GetLed 			Cm_GetLed


extern Xboolean Get_Swap_IsClose();
#define HandleSensor_isClose Get_Swap_IsClose

//Api for Led moudle
extern XStatus Set_LedBlue_State(Xuint8 state);
extern XStatus Set_LedAlarm_State(Xuint8 state);

#define Led_SetAlarmLedState	Set_LedAlarm_State
#define Led_SetBlueLedState 	Set_LedBlue_State


#define Led_SetRunLedState Set_LedAlarm_State
#define Led_SetLed1State(...)

extern void microblaze_disable_interrupts();
extern void microblaze_enable_interrupts();
#define LOCK()  	{ microblaze_disable_interrupts()
#define UNLOCK()	microblaze_enable_interrupts();}
#endif

#if __EMcmc_PM__
//Api for Ipmb moudle
extern int EMcmc_PostMsg(uint8 object, uint8 msgID, uint32 param1, uint32 param2);
#define PostMsg 		EMcmc_PostMsg

//Api for SwTimer
struct _tagTimerManager;
extern struct _tagTimerManager* EMcmc_TimerManager(void);
#define GetTimerManager EMcmc_TimerManager

//Api for TransferDriver
struct _tagIpmb;
extern struct _tagIpmb* EMcmc_GetIpmb(uint8 iicChannel);
#define GetIpmb 		EMcmc_GetIpmb

//Api for Sensor
struct _tagCrb;
extern struct _tagCrb* EMcmc_GetCrb(uint8 targetAddr, uint8 targetLun, uint8 isReq);
#define GetCrb 			EMcmc_GetCrb

struct _tagLed;
extern struct _tagLed* EMcmc_GetLed(uint8 ledType);
#define GetLed 			EMcmc_GetLed

/*
extern Xboolean Get_Swap_IsClose();
#define HandleSensor_isClose Get_Swap_IsClose

//Api for Led moudle
extern XStatus Set_LedBlue_State(Xuint8 state);
extern XStatus Set_LedAlarm_State(Xuint8 state);
extern XStatus Set_LedRun_State(Xuint8 state);
*/
#define Led_SetLed1State	Set_LedAlarm_State
#define Led_SetBlueLedState 	Set_LedBlue_State
#define Led_SetRunLedState		Set_LedRun_State
#define Led_SetSmpLedState(...)

#ifdef WIN32

#else
extern void microblaze_disable_interrupts();
extern void microblaze_enable_interrupts();
#define LOCK()  	{ microblaze_disable_interrupts()
#define UNLOCK()	microblaze_enable_interrupts();}
#endif
#endif


#if __AMC_MCMC__
//Api for Ipmb moudle
extern int Mcmc_PostMsg(uint8 object, uint8 msgID, uint32 param1, uint32 param2);
#define PostMsg 		Mcmc_PostMsg

//Api for SwTimer
struct _tagTimerManager;
extern struct _tagTimerManager* Mcmc_TimerManager(void);
#define GetTimerManager Mcmc_TimerManager

//Api for TransferDriver
struct _tagIpmb;
extern struct _tagIpmb* Mcmc_GetIpmb(uint8 iicChannel);
#define GetIpmb 		Mcmc_GetIpmb

//Api for Sensor
struct _tagCrb;
extern struct _tagCrb* Mcmc_GetCrb(uint8 targetAddr, uint8 targetLun, uint8 isReq);
#define GetCrb 			Mcmc_GetCrb

struct _tagLed;
extern struct _tagLed* Mcmc_GetLed(uint8 ledType);
#define GetLed 			Mcmc_GetLed


extern uint8 Is_HandSwitch_Close(void);
#define HandleSensor_isClose Is_HandSwitch_Close

//Api for Led moudle
extern void Set_LedBlue_State(Bool state);
extern void Set_LedAlarm_State(Bool state);

#define Led_SetRunLedState(...)
#define Led_SetLed1State(...)

#define LOCK()
#define UNLOCK()
#endif

#if __EMcmc_CU__
#endif

/**************    End   ****************/







#endif


