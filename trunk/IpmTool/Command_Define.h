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
**      command_define.h
**
**  History:
**  Programmer      Date       Rev Number Description
**  --------------- ---------- ---------- -----------------------------
**  lan yanghai     08/25/2011 1.0        Created.
***************************************************************************H*/

#ifndef    COMMAND_DEFINE_H
#define    COMMAND_DEFINE_H


#define PICMG_IDENTIFIER 0x00

/************************   Completion_code_define Begin  ***************************/
/* Generic Completion_codes 00h,C0h~FFh */
#define COMMAND_COMPLETE        0x00
#define NODE_BUSY               0xc0
#define INVALID_COMMAND         0xc1
#define INVALID_LUN             0xc2
#define TIMEOUT_PROC_CMD        0xc3
#define OUT_OF_SPACE            0xc4
#define INVALID_RESERVED_ID     0xc5
#define REQUEST_DATA_TRUNCATED  0xc6
#define INVALID_DATA_LENGTH     0xc7
#define DATA_LENGTH_EXCEEDED    0xc8
#define PARAMETER_OUT_RANGE     0xc9
#define CANNOT_RETURN_NUMBRE_OF_DATA      0xca	//Cannot return number of requested data bytes.
#define REQUESTED_NOT_PRESENT   0xcb	//Requested Sensor, data, or record not present. 
#define INVALID_DATA_FILED      0xcc	//Invalid data field in Request
#define ILLEGAL_SPECIFIED_TYPE  0xcd	//Command illegal for specified sensor or record type.
#define RESPONSE_NOT_PROVIDE    0xce
#define CANNOT_EXECUTE_DUP      0xcf
#define SDR_RESPOSITORY_UPDATE  0xd0
#define DEVICE_FIRMWARE_UPDATE  0xd1
#define INITIA_AGENT_PROGRESS   0xd2
#define DESTINATION_UNAVAILABLE 0xd3
#define SECURITY_RESTRICTION    0xd4
#define CANNOT_EXECUTE_COMMAND  0xd5	//Cannot execute command. Command, or request parameter(s), not supported in present state. 
#define SUB_FUNCTION_INVALID    0xd6	//Cannot execute command, Parameter is illegal because command sub-function has been disabled or is unavailable

#define FRU_DEVICE_BUSY    		0x81	//FRU device busy
#define UNSPECIFIED_ERROR       0xff

/* Device_specified (OEM) codes 01h~7Eh */

/* Command_specified codes 80h~BEh      */

/* All others reserved. */

/**********************   Completion_code_define END   *************************/
////////////////////////////////////////////////////////////////////////////////////
/***********************   Sensor_Type_Define Begin   *************************/
#define TEMPERATURE_SENSOR                             0x01
#define VOLTAGE_SENSOR                                 0x02
#define CURRENT_SENSOR                                 0x03
#define FAN_SENSOR                                     0x04
#define PHYSICAL_SECURITY_SENSOR                       0x05
#define PLATFORM_SECURITY_VIOLATION_ATTEMPT_SENSOR     0x06
#define PROCESSOR_SENSOR                               0x07
#define POWER_SUPPLY_SENSOR                            0x08
#define POWER_UNIT_SENSOR                              0x09
#define COOLING_DEVICE_SENSOR                          0x0A
#define POWER_REDUNDANCY_SENSOR_SENSOR                 0x0B
#define MENORY_SENSOR                                  0x0C
#define DRIVE_SLOT_SENSOR                              0x0D
#define TPOST_MENORY_RESIZE_SENSOR                     0x0E
#define SYSTEM_FIRMWARE_SENSOR                         0x0F
#define EVENT_LOGGING_DISABLED_SENSOR                  0x10
#define WATCHDOG_1_SENSOR                              0x11
#define SYSTEM_EVENT_SENSOR                            0x12
#define CRITICAL_INTERRUPT_SENSOR                      0x13
#define BUTTON_SWITCH_SENSOR                           0x14
#define MODULE_BOARD_SENSOR                            0x15
#define MICROCONTROLLER_COPROCESSOR_SENSOR             0x16
#define ADD_IN_CARD_SENSOR                             0x17
#define CHASSIS_SENSOR                                 0x18
#define CHIP_SET_SENSOR                                0x19
#define OTHER_FRU_SENSOR                               0x1A
#define CABLE_INTERCONNECT_SENSOR                      0x1B
#define TERMINATOR_SENSOR                              0x1C
#define SYSTEM_BOOT_RESTART_INITIATED_SENSOR           0x1D
#define BOOT_ERROR_SENSOR                              0x1E
#define OS_BOOT_SENSOR                                 0x1F
#define OS_CRITICAL_STOP_SHUTDOWN_SENSOR               0x20
#define SLOTCONNECTOR_SENSOR                           0x21
#define SYSTEM_ACPI_POWER_STATE_SENSOR                 0x22
#define WATCHDOG_2_SENSOR                              0x23
#define PLATFORM_ALERT_SENSOR                          0x24
#define ENTITY_PRESENCE_SENSOR                         0x25
#define MONITOR_ASIC_IC_SENSOR                         0x26
#define LAN_SENSOR                                     0x27
#define MANAGEMENT_SUBSYSTEM_HEALTH_SENSOR             0x28
#define BATTERY__SENSOR                                0x29
#define SESSION_AUDIT_SENSOR                           0x2A
#define VERSION_CHANGE_SENSOR                          0x2B
#define FRU_STATE_SENSOR                               0x2C

#define FRU_HOT_SWAP        				0xF0
#define IPMB_PHYSICAL_LINK                  0xF1
#define MODULE_HOT_SWAP                     0xF2
#define PWRCHANL_NOTIFI          			0xF3
#define TELCO_ALARM_INPUT                   0xF4
/************************   Sensor Type Define End  ***************************/


/************************ Other  Sensor Number Define End  ***************************/
/////////////////////////////////////////////////////////////////////////////////////////
/*************************NetFunction   Begin***************************/
/* NetFunction Define */
#define NETFUN_CHASSIS_REQ      0x00
#define NETFUN_CHASSIS_RSP      0x01
#define NETFUN_BRIDGE_REQ       0x02
#define NETFUN_BRIDGE_RSP      0x03
#define NETFUN_SSEVENT_REQ      0x04
#define NETFUN_SSEVENT_RSP      0x05
#define NETFUN_APPLICA_REQ      0x06
#define NETFUN_APPLICA_RSP      0x07
#define NETFUN_FIRMWAR_REQ      0x08
#define NETFUN_FIRMWAR_RSP      0x09
#define NETFUN_STORAGE_REQ      0x0a
#define NETFUN_STORAGE_RSP      0x0b
#define NETFUN_TRANSPORT_REQ    0x0c
#define NETFUN_TRANSPORT_RSP    0x0d
/*0x0e~0x2b : reserved */

#define NETFUN_GROUPEX_REQ      0x2c
#define NETFUN_PICMG_REQ		0x2c
#define NETFUN_PICMG_RSP		0x2D
#define NETFUN_GROUPEX_RSP      0x2d
#define NETFUN_OEMGROUP_REQ     0x2e
#define NETFUN_OEMGROUP_RSP     0x2f
/* 0x30~0x3f : Controller-specific OEM/Group  */
#define NETFUN_UARTTEST_REQ     0x30
#define NETFUN_UARTTEST_RSP     0x31

#define NETFUN_FSM_REQ     0x32
#define NETFUN_FSM_RSP     0x33
/////////////////////////////////////////////////////////////////////

/* Logic_Unit_Number Define */
#define LUN_LOCAL_MESSAGE           0x00
#define LUN_BRIDGE_MESSAGE         0x02
#define LUN_UART_PAYLOAD            0x01
#define LUN_UART_TESTTOOL           0x03

/* Channel Number Define */
#define CHAN_NUMBER_IPMB            0x00
#define CHAN_NUMBER_SYSI            0x0f

/* Channel Protocol Type Define */
#define CHAN_PROTOC_IPMB            0x01
#define CHAN_PROTOC_ICMB            0x02
#define CHAN_PROTOC_SMB             0x04
#define CHAN_PROTOC_KCS             0x05
#define CHAN_PROTOC_SMIC            0x06
#define CHAN_PROTOC_BT10            0x07
#define CHAN_PROTOC_BT15            0x08
#define CHAN_PROTOC_TMOD            0x09

/* Channel Medium Type Define */
#define CHAN_MEDIUM_IPMB            0x01
#define CHAN_MEDIUM_ICMB10          0x02
#define CHAN_MEDIUM_ICMB09          0x03
#define CHAN_MEDIUM_ELAN            0x04
#define CHAN_MEDIUM_UART            0x05
#define CHAN_MEDIUM_OLAN            0x06
#define CHAN_MEDIUM_SMBPCI          0x07
#define CHAN_MEDIUM_SMB10           0x08
#define CHAN_MEDIUM_SMB20           0x09
#define CHAN_MEDIUM_USB1            0x0a
#define CHAN_MEDIUM_USB2            0x0b
#define CHAN_MEDIUM_SYSI            0x0c

/********************* NetFunction  End  **********************/
////////////////////////////////////////////////////////////////////////////////
/************************ Command Define  Begin  ************************/

/************************   NetFn = APP   ***************************/
/* IPM Device "Global" Commands */
#define    GET_DEVICE_ID            0x01        /* M for CM,MCMC,EMcmc */
#define    COLD_RESET               0x02
#define    WARM_RESET               0x03
#define    GET_SELF_TEST_RESULT     0x04        /* M for CM,MCMC,EMcmc */
#define    MANUFACTURING_TEST_ON    0x05
#define    GET_DEVICE_GUID          0x08
#define    GET_DEVICE_ID_BROST      0x01        /* M for CM,MCMC,EMcmc */

/* BMC Watchdog Timer Commands  */
#define    RESET_WATCHDOG_TIMER     0x22
#define    SET_WATCHDOG_TIMER       0x24
#define    GET_WATCHDOG_TIMER       0x25

/* BMC Device and Messaging Commands */
#define    SET_GLOBAL_ENABLES       0x2e
#define    GET_GLOBAL_ENABLES       0x2f
#define    CLEAR_MESSAGE_FLAGS      0x30
#define    GET_MESSAGE_FLAGS        0x31
#define    ENABLE_MESSAGE_RECEIVE   0x32
#define    GET_MESSAGE              0x33
#define    SEND_MESSAGE             0x34
#define    READ_EVENT_BUFFER        0x35
#define    GET_BT_INTF_CAPABILITY   0x36
#define    GET_SYSTEM_GUID          0x37
#define    GET_CHANNEL_AUTHEN       0x38
#define    GET_SESSION_CHALLENGE    0x39
#define    ACTIVATE_SESSION         0x3a
#define    SET_SESSION_PRIVILEGE    0x3b
#define    CLOSE_SESSION            0x3c
#define    GET_SESSION_INFO         0x3d
#define    GET_AUTH_CODE            0x3f
#define    SET_CHANNEL_ACCESS       0x40
#define    GET_CHANNEL_ACCESS       0x41
#define    GET_CHANNEL_INFO         0x42
#define    SET_USER_ACCESS          0x43
#define    GET_USER_ACCESS          0x44
#define    SET_USER_NAME            0x45
#define    GET_USER_NAME            0x46
#define    SET_USER_PASSWORD        0x47
#define    MASTER_WRITE_READ        0x52

/************************   NetFn = S/E   ***************************/
/* Event Commands */
#define    SET_EVENT_RECEIVER       0x00
#define    GET_EVENT_RECEIVER       0x01
#define    PLATFORM_EVENT_MESSAGE   0x02

/* PEF and Alerting Commands */
#define    GET_PEF_CAPABILITY       0x10
#define    ARM_POSTPONE_TIMER       0x11
#define    SET_PEF_PARAMETER        0x12
#define    GET_PEF_PARAMETER        0x13
#define    SET_LAST_EVENT_ID        0x14
#define    GET_LAST_EVENT_ID        0x15
#define    ALERT_IMMEDIATE          0x16
#define    PET_ACKNOWLEDGE          0x17

/* Sensor Device Commands */
#define    GET_SDR_REPO_INFO      	0x20
#define    GET_DEVICE_SDR           0x21
#define    RESERVE_DEVICE_SDR_REPOSITORY   0x22
#define    GET_READING_FACTOR       0x23
#define    SET_SENSOR_HYSTERESIS    0x24
#define    GET_SENSOR_HYSTERESIS    0x25
#define    SET_SENSOR_THRESHOLD     0x26
#define    GET_SENSOR_THRESHOLD     0x27
#define    SET_SENSOR_EVENT_ENA		0x28
#define    GET_SENSOR_EVENT_ENA	  	0x29
#define    REARM_SENSOR_EVENT       0x2a
#define    GET_EVENT_STATUS         0x2b
#define    GET_SENSOR_READING       0x2d
#define    SET_SENSOR_TYPE          0x2e
#define    GET_SENSOR_TYPE          0x2f

/************************   NetFn = Storage   ***************************/
/* FRU Device Commands */
#define    GET_FRU_INVENTORY_INFO   0x10
#define    READ_FRU_DATA            0x11
#define    WRITE_FRU_DATA           0x12

/* SDR Device Commands */
#define    GET_SDR_REPOSITORY_INFO  0x20
#define    GET_SDR_ALLOCA_INFO      0x21
#define    RESERVE_SDR_REPOSITORY   0x22
#define    GET_SDR                  0x23
#define    ADD_SDR                  0x24
#define    PARTIAL_ADD_SDR          0x25
#define    DELETE_SDR               0x26
#define    CLEAR_SDR_REPOSITORY     0x27
#define    GET_REPOSITORY_TIME      0x28
#define    SET_REPOSITORY_TIME      0x29
#define    ENTER_UPDATE_MODE        0x2a
#define    EXIT_UPDATE_MODE         0x2b
#define    RUN_INITIALIZE_AGENT     0x2c

/* SEL Device Commands */
#define    GET_SEL_INFO             0x40
#define    GET_SEL_ALLOCA_INFO      0x41
#define    RESERVE_SEL              0x42
#define    GET_SEL_ENTRY            0x43
#define    ADD_SEL_ENTRY            0x44
#define    PARTIAL_ADD_SEL_ENTRY    0x45
#define    DELETE_SEL_ENTRY         0x46
#define    CLEAR_SEL                0x47
#define    GET_SEL_TIME             0x48
#define    SET_SEL_TIME             0x49
#define    GET_AUX_LOG_STATUS       0x5a
#define    SET_AUX_LOG_STATUS       0x5b

/************************   NetFn = Transport   ***************************/
/* LAN Device Commands */
#define    SET_LAN_CONFG_PARAME     0x01
#define    GET_LAN_CONFG_PARAME     0x02
#define    SUSPEND_BMC_ARPS         0x03
#define    GET_IP_STATISTICS        0x04

/* Serial/Modem Device Commands */

/************************   NetFn = Bridge   ***************************/

/* Bridge Management Commands(ICMB) */
/* Discovery Commands */
/* Bridging Commands */
/* Event Commands */
/* OEM Commands for Bridge */
/* Other Bridge Commands */

/************************   NetFn = PICMG   ***************************/
/* AdvancedTCA Commands */
#define    GET_PICMG_PROPERTY       0x00
#define    GET_ADDRESS_INFO         0x01
#define    GET_SHELF_ADD_INFO       0x02
#define    SET_SHELF_ADD_INFO       0x03
#define    FRU_CONTROL              0x04
#define    GET_FRU_LED_PROPERTY     0x05
#define    GET_LED_COLOR_CAPABT     0x06
#define    SET_FRU_LED_STATE        0x07
#define    GET_FRU_LED_STATE        0x08
#define    SET_IPMB_STATE           0x09
#define    SET_FRU_ACTIV_POLICY     0x0a
#define    GET_FRU_ACTIV_POLICY     0x0b
#define    SET_FRU_ACTIVATION       0x0c
#define    GET_LOCATOR_RECORD_ID    0x0d
#define    SET_PORT_STATE           0x0e
#define    GET_PORT_STATE           0x0f
#define    COMPUTE_POWER_PROPERTY   0x10
#define    SET_POWER_LEVEL          0x11
#define    GET_POWER_LEVEL          0x12
#define    RENEGOTIATE_POWER        0x13
#define    GET_FAN_SPEED_PROPERTY   0x14
#define    SET_FAN_LEVEL            0x15
#define    GET_FAN_LEVEL            0x16
#define    BUSED_RESOURCE           0x17
#define    GET_IPMB_LINK_INFO       0x18
#define    GET_SHELF_MAN_IPMB_ADD   0x1b
#define    SET_FAN_POLICY           0x1c
#define    GET_FAN_POLICY           0x1d
#define    FRU_CONTROL_CAPABILTY    0x1e
#define    FRU_INVENTORY_LOCK       0x1f
#define    FRU_INVECTORY_WRITE      0x20
#define    GET_SHELF_MAN_IP         0x21
#define    GET_SHELF_POWER_ALLOCA   0x22

/* AdvancedMC Commands */
#define    SET_AMC_PORT_STATE       0x19
#define    GET_AMC_PORT_STATE       0x1a

/* MicroTCA Commands */
#define    GET_LOCATION_INFO        0x23
#define    POWER_CHANNEL_CONTROL    0x24
#define    GET_POWER_CHANNEL_STATE  0x25
#define    PM_RESET                 0x26
#define    GET_PM_STATUS            0x27
#define    PM_HEARTBEAT             0x28
#define    GET_TELCO_ALARM_CAPABT   0x29
#define    SET_TELCO_ALARM_STATE    0x2a
#define    GET_TELCO_ALARM_STATE    0x2b

/*****************************Command Define  End*******************************/

/* Power Channel Control  Enum */
typedef enum _PWR_CTRL
{
    DISABLE_MP = 0,
    ENABLE_MP,
    DISABLE_BOARD,
    ENABLE_BOARD,
    DISABLE_PP,
    ENABLE_PP,
    DISABLE_PS,
    ENABLE_PS,
}PWR_CTRL;


typedef enum _tagPmRole
{
	PRIMARY_PM =0
 	,REDUNDANT_PM = 1
}PmRole;

typedef enum _tagCmType
{
 	REDUNDANT_CM = 0
 	,PRIMARY_CM = 1
}CmType;


//SELF_TEST_RESULT
enum
{
	 STR_NO_ERROR 					= 0x55	//No error. All Self Tests Passed.
	,STR_NOT_IMPLEMENTED 			= 0x56	//Self Test function not implemented in this controller.
	,STR_DATA_OR_DEVICE_CORRUPTED 	= 0x57	//Corrupted or inaccessible data or devices 
	,STR_FATAL_HW_ERROR 			= 0x58	//Fatal hardware error
	,STR_FATAL_RESERVEC 			= 0xFF	//
};

typedef struct _SelfTestBitfield
{
#ifdef BIG_EDIAN
	uint8 m_CanNotAccessSelDevice: 1;			//BIT[7] Cannot access SEL device
	uint8 m_CanNotAccessSdrRepository: 1;		//BIT[6] Cannot access SDR Repository
	uint8 m_CanNotAccessBmcFruDevice: 1;		//BIT[5] Cannot access BMC FRU device
	uint8 m_IpmbSignalLineDoNotResponse: 1;	//BIT[4] IPMB signal lines do not respond 
	uint8 m_SdrRepositoryEmpty: 1;			//BIT[3] SDR Repository empty
	uint8 m_BmcInteUseAreaCorrupted: 1;		//BIT[2] Internal Use Area of BMC FRUcorrupted
	uint8 m_FirmwareBootBlockCorrupted: 1;	//BIT[1] controller update ¡®boot block¡¯ firmware corrupted
	uint8 m_FirmwareCorrupted: 1;				//BIT[0] controller operational firmware corrupted
#else
	uint8 m_FirmwareCorrupted: 1;				//BIT[0] controller operational firmware corrupted
	uint8 m_FirmwareBootBlockCorrupted: 1;	//BIT[1] controller update ¡®boot block¡¯ firmware corrupted
	uint8 m_BmcInteUseAreaCorrupted: 1;		//BIT[2] Internal Use Area of BMC FRU corrupted
	uint8 m_SdrRepositoryEmpty: 1;			//BIT[3] SDR Repository empty
	uint8 m_IpmbSignalLineDoNotResponse: 1;	//BIT[4] IPMB signal lines do not respond 
	uint8 m_CanNotAccessBmcFruDevice: 1;		//BIT[5] Cannot access BMC FRU device
	uint8 m_CanNotAccessSdrRepository: 1;		//BIT[6] Cannot access SDR Repository
	uint8 m_CanNotAccessSelDevice: 1;			//BIT[7] Cannot access SEL device
#endif
}SelfTestBitfield;

typedef struct  _GetDeviceIDRsp
{
    unsigned char    CompCode;       
    unsigned char    Device_ID;       
    unsigned char    Device_Revision;           /*[7]:1 device provides device SERs 0 device does not provide Device SDRs
                                                 [6:4] reserved Teturn as 0
                                                 [3:0] Device Revision*/ 
    unsigned char    Firmware_Revision_1;       /*[7] Device available 0=normaloperation
                                                 [6:0] Major Firmware Revision*/   
    unsigned char    Firmware_Revision_2;       /*Minor Firmware Revision.BCDencoded*/   
    unsigned char    IPMI_Version;
	
#if BIG_EDIAN	
    unsigned char    ChassisDevice:1;  
    unsigned char    Bridge:1;  
    unsigned char    EventGenerator:1;  
    unsigned char    EventReceiver:1;  
    unsigned char    FRUInventoryDevice:1;  
    unsigned char    SELDevice:1;  
    unsigned char    SDRRepositoryDevice:1;  
    unsigned char    SensorDevice:1; 
#else
    unsigned char    SensorDevice:1; 
    unsigned char    SDRRepositoryDevice:1;  
    unsigned char    SELDevice:1;  
    unsigned char    FRUInventoryDevice:1;  
    unsigned char    EventReceiver:1;  
    unsigned char    EventGenerator:1;  
    unsigned char    Bridge:1;  
    unsigned char    ChassisDevice:1;  
#endif
	
    unsigned char    ManufacturerID[3];       /*eg 7154=1BF2h Manufacturer_ID_0=F2h Manufacturer_ID_1=1BH Manufacturer_ID_2=00H*/	
    unsigned char    ProductID[2];  
    unsigned char    AuxiliaryFWRevi[4];  

}GetDeviceIDRsp;

//GET DEVICE SDR REQ
typedef struct _GetDeviceSdrReq
{
    uint8    m_ReservationIdLs;
    uint8    m_ReservationIdMs;
    uint8    m_RecordIdLs;
    uint8    m_RecordIdMs;
    uint8    m_Offset;
    uint8    m_Len;             /* FFh means read entire record. */       

}GetDeviceSdrReq;

//GET DEVICE SDR RSP
typedef struct _GetDeviceSdrRsq
{
    uint8    m_CompCode;  
    uint8    m_NextIdLs;
    uint8    m_NextIdMs;
    uint8    m_Buff[1]; 
}GetDeviceSdrRsq;

//GET SDR REPO INFO RSP
typedef struct _GetSdrRepoInfoRsq
{
    uint8    m_CompCode;  
    uint8    m_SdrVersion;
    uint8    m_RecordCountLs;
    uint8    m_RecordCountMs; 
    uint8    m_FreeSpaceLs; 
    uint8    m_FreeSpaceMs; 
    uint8    m_MostRecentAddTimestamp[4]; 	//Ls first
    uint8    m_MostRecentEraseTimestamp[4]; //Ls first
    uint8    m_OperationSupport; 
}GetSdrRepoInfoRsq;


//Reserve Device SDR Repository Command
typedef struct  _ReserveSdrRepoRsp
{
    unsigned char    m_CompCode;  
    unsigned char    m_ReservationIdLs;
    unsigned char    m_ReservationIdMs;

}ReserveSdrRepoRsp;

//Get Sensor Reading Command 
typedef struct _GetSensorReadingRsp
{
    unsigned char    m_CompCode;  
    unsigned char    m_SensorReading;    
    unsigned char    m_Status;
    unsigned char    m_ComparisonStatus1;
    unsigned char    m_ComparisonStatus2;

}GetSensorReadingRsp;

//Get FRU Inventory Area Info Command
typedef struct _GetFruInfoRsp
{
    unsigned char    m_CompCode;  
    unsigned char    m_FruAreaSizeLs;
    unsigned char    m_FruAreaSizeMs;
    unsigned char    m_AccessMode;                 /*[0] 0b=device is accessed by bytes, 1b=device is accessed by words*/
}GetFruInfoRsp;

//Read FRU Data Command 
typedef struct  _ReadFruDataReq
{
    unsigned char    m_DeviceId;
    unsigned char    m_OffsetLs;
    unsigned char    m_OffsetMs;
    unsigned char    m_Count;           /*count is '1' based*/

}ReadFruDataReq;

typedef struct _ReadFruDataRsp
{
    unsigned char    m_CompCode;  
    unsigned char    m_Count;
    unsigned char    m_Data[1];

}ReadFruDataRsp;

//Write FRU Data Command
typedef struct _WriteFruDataReq
{
    unsigned char    m_DeviceId;
    unsigned char    m_OffsetLs;
    unsigned char    m_OffsetMs;
    unsigned char    m_Data[1];
}WriteFruDataReq;

typedef struct _WriteFruDataRsp
{
    unsigned char    m_CompCode;  
    unsigned char    m_Count;    

}WriteFruDataRsp;

//Get SDR Repository Info Command
typedef struct _OperationSupport
{
#ifdef BIG_EDIAN
	uint8 overflow_Flag:1;
	uint8 update_Operation:2;
	uint8 reserved:1;
	uint8 delete_SDR:1;
	uint8 partial_Add_SDR:1;
	uint8 reserve_SDR_Repository:1;
	uint8 Get_SDR_Rep_Alloc_Info:1;
#else
	uint8 Get_SDR_Rep_Alloc_Info:1;
	uint8 reserve_SDR_Repository:1;
	uint8 partial_Add_SDR:1;
	uint8 delete_SDR:1;
	uint8 reserved:1;
	uint8 update_Operation:2;
	uint8 overflow_Flag:1;
#endif

}OperationSupport;

typedef struct _GetSdrInfoRsp
{
    uint8	m_CompCode;
    uint8   m_SdrVer;         		/*51h*/
    uint8   m_RecordCountLs;
    uint8   m_RecordCountMs;
	uint8 	m_FreeSpaceLS;
	uint8 	m_FreeSpaceMS;
	uint32	m_AdditionTimestamp;
	uint32	m_EraseTimestamp;
	OperationSupport	m_OperationSupport;
}GetSdrInfoRsp;

typedef struct _ReserveDevSDRReposRsp
{
    uint8	m_CompCode;
    uint8   m_ReservaIdLs;  
    uint8   m_ReservaIdMs;
}ReserveDevSDRReposRsp;

//GET DEVICE SDR REQ
typedef struct _GetSdrReq
{
    uint8    m_ReservationIdLs;
    uint8    m_ReservationIdMs;
    uint8    m_RecordIdLs;
    uint8    m_RecordIdMs;
    uint8    m_Offset;
    uint8    m_Len;             /* FFh means read entire record. */       

}GetSdrReq;

//GET DEVICE SDR RSP
typedef struct _GetSdrRsq
{
    uint8    m_CompCode;  
    uint8    m_NextIdLs;
    uint8    m_NextIdMs;
    uint8    m_Buff[1]; 
}GetSdrRsq;

//GET PICMg Properties --<PICMG 3.0 P146>
typedef struct _GetPicmgRep
{
    uint8    m_PICMGIden;
}GetPicmgRep;

typedef struct _GetPicmgRsq
{
    uint8    m_CompCode;  
    uint8    m_PICMGIden;
    uint8    m_PICMGExtension;
    uint8    m_MaxFruDeviceId; 
    uint8    m_FruDeviceId; 
}GetPicmgRsq;

//Get Address Info --<PICMG 3.0 P145>
typedef struct _GetAddressInfoRep
{
    uint8    m_PICMGIden;
    uint8    m_FruDevId;
    uint8    m_AddressKeyType;
    uint8    m_AddressKey;
    uint8    m_SiteKey;
}GetAddressInfoRep;

typedef struct _GetAddressInfoRsp
{
    uint8    m_CompCode;  
    uint8    m_PICMGIden;
    uint8    m_HardWareAddress;
    uint8    m_IPMB_0Address; 
    uint8    m_Reserve;   
    uint8    m_FruDevId; 
    uint8    m_SiteId; 
    uint8    m_SiteType; 

}GetAddressInfoRsp;

//Get Fru Led Properties --<PICMG 3.0 P174>
typedef struct _GetFruLedPropertiesReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    m_DeviceId;

}GetFruLedPropertiesReq;

typedef struct  _GetFruLedPropertiesRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char	 General_Status_LEDs;
	unsigned char	 Application_Status_LEDs;

}GetFruLedPropertiesRsp;

//Get Led Color Capabilities --<PICMG 3.0 P176>
typedef struct _GetLedColorCapReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    m_DeviceId;
	unsigned char    m_LedId;

}GetLedColorCapReq;

typedef struct  _GetLedColorCapRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char	 LedColorCap;
	unsigned char	 DefaultLedColorCtrlSte;
	unsigned char	 DefaultLedColorOverrideSte;

}GetLedColorCapRsp;

//Set Fru Led State --<PICMG 3.0 P178>
typedef struct _SetFruLedStateReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevID;        
    unsigned char    LedID;   
    unsigned char    LedFun;   
    unsigned char    OnDruation;   
    unsigned char    ColorIlluminated;   
}SetFruLedStateReq;

typedef struct  _SetFruLedStateRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;

}SetFruLedStateRsp;

//Get Fru Led State --<PICMG 3.0 P178>
typedef struct _GetFruLedStateReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevID;        
    unsigned char    LedID;   

}GetFruLedStateReq;

typedef struct  _GetFruLedStateRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char    LedSte;
	unsigned char    LocalCtrlLedFun;
	unsigned char    LocalCtrlOnDutation;
	unsigned char    LocalCtrlColor;
	unsigned char    OverrideSte;

}GetFruLedStateRsp;

//Set IPMB State --<PICMG 3.0 P239>
typedef struct _SetIpmbStateReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    IPMB_A_Ste;        
    unsigned char    IPMB_B_Ste;   

}SetIpmbStateReq;

typedef struct  _SetIpmbStateRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;

}SetIpmbStateRsp;


//Get Dev Locator Record Id --<PICMG 3.0 P239>
typedef struct _GetDevLocatorReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;        
}GetDevLocatorReq;

typedef struct  _GetDevLocatorRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char    RecordIdLs;
	unsigned char    RecordIdMs;

}GetDevLocatorRsp;

//Set Fru Activation Policy --<PICMG 3.0 P158>
typedef struct _SetFruActPolicyReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;        
	unsigned char    FruActPolicyMask;        
    unsigned char    FruActPolicySet;        
}SetFruActPolicyReq;

typedef struct  _SetFruActPolicyRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;

}SetFruActPolicyRsp;

//Get Fru Activation Policy --<PICMG 3.0 P159>
typedef struct _GetFruActPolicyReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;            
}GetFruActPolicyReq;

typedef struct  _GetFruActPolicyRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char    FruActPolicy;
}GetFruActPolicyRsp;

//Set Fru Activation--<PICMG 3.0 P158>
//Set FRU Activation command
#define DEACTIVATE_FRU 	0
#define ACTIVATE_FRU 	1
typedef struct  _SetFruActReq
{
    unsigned char    m_PicmgId;
    unsigned char    m_DeviceId;      
    unsigned char    m_FruActCtrl; //00h = Deactivate FRU; 01h = Activate FRU; 02h-FFh = Reserved.
}SetFruActReq;
typedef struct  _SetFruActRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
}SetFruActRsp;

//Get Fan Speed Properties--<PICMG 3.0 P158>
typedef struct _GetFanSpeedProReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;                   
}GetFanSpeedProReq;

typedef struct  _GetFanSpeedProRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char    MinSpeedLevel;
    unsigned char    MaxSpeedLevel;
    unsigned char    NormalOperLevel;
    unsigned char    FanTrayPro;
}GetFanSpeedProRsp;

//Set Fan Level--<PICMG 3.0 P260>
typedef struct _SetFanLevelReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;          
    unsigned char    FanLevel;                   
}SetFanLevelReq;

typedef struct  _SetFanLevelRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
}SetFanLevelRsp;

//Get Fan Level--<PICMG 3.0 P260>
typedef struct _GetFanLevelReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;                           
}GetFanLevelReq;

typedef struct  _GetFanLevelRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;    
	unsigned char    OverrideFanLevel;
    unsigned char    LocalCtrlFanLevel;
}GetFanLevelRsp;

//Get IPMB Link Info--<PICMG 3.0 P236>
typedef struct _GetIPMBLinkInfoReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    IPMB_0LinkInfoKey1;  
	unsigned char    IPMB_0LinkInfoKey2;  
}GetIPMBLinkInfoReq;

typedef struct  _GetIPMBLinkInfoRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;    
	unsigned char    IPMB_0LinkNumber;
    unsigned char    IPMB_0SensorNumber;
}GetIPMBLinkInfoRsp;

//Set Fan Policy--<ATCA R2.0 P298>
typedef struct _SetFanProReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FanTraySiteNum;          
    unsigned char    FanEnableSte;      
    unsigned char    FanPolicyTimeout; 
    unsigned char    SiteNumber; 
    unsigned char    SiteType; 
}SetFanProReq;

typedef struct  _SetFanProRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
}SetFanProRsp;

//Get Fan Policy--<ATCA R2.0 P299>
typedef struct _GetFanProReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FanTraySiteNum;          
    unsigned char    SiteNumber; 
    unsigned char    SiteType; 
}GetFanProReq;

typedef struct  _GetFanProRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;   
	unsigned char    Policy;
    unsigned char    Coverage;
}GetFanProRsp;

//FRU Control Capabilities--<ATCA R2.0 P183>
typedef struct _FruCtrlCapReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;          
}FruCtrlCapReq;

typedef struct  _FruCtrlCapRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;   
	unsigned char    FruCtrl;
}FruCtrlCapRsp;


//FRU Inventory Device Lock Control--<ATCA R2.0 P229>
typedef struct _FruInventDevLockCtrlReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;    
    unsigned char    Operation;  
    unsigned char    FruInventDevLockId;  
}FruInventDevLockCtrlReq;

typedef struct  _FruInventDevLockCtrlRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;   
	unsigned char    FruInventoryDevLockId;
	unsigned char    FruInventoryDevLastComTime;
}FruInventDevLockCtrlRsp;

//FRU Inventory Device Write--<ATCA R2.0 P229>
typedef struct _FruInventDevWReq
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    FruDevId;    
    unsigned char    FruInventDevLockId;  
    unsigned char    FruInventOffsetW;      
    unsigned char    Data[250];  
}FruInventDevWReq;

typedef struct  _FruInventDevWRsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;   
	unsigned char    CountWrite;
}FruInventDevWRsp;

//Channel Descriptor
typedef struct _tagChannelDescriptor
{/*

    uint32 reserve:4;
    uint32 m_Lane3:5;
    uint32 m_Lane2:5;
    uint32 m_Lane1:5;
    uint32 m_Lane0:5;
*/
    unsigned char  m_Lan[3];

}ChannelDescriptor;


//Link Descriptor
typedef struct _tagAMC_Link_Info
{
    uint8 m_ChannelId;//AMC Channel ID
    
#if BIG_EDIAN
    uint8 m_Lane0Flg:1;//Lane 0 Bit Flag (1 = Lane Included; 0 = Lane Excluded)
    uint8 m_Lane1Flg:1;
    uint8 m_Lane2Flg:1;
    uint8 m_Lane3Flg:1;
    uint8 m_Reserved:4; 
#else
    uint8 m_Reserved:4; 

    uint8 m_Lane3Flg:1;
    uint8 m_Lane2Flg:1;
    uint8 m_Lane1Flg:1;
    uint8 m_Lane0Flg:1;
#endif

    uint8 m_LinkType;//see Table 3-21, "AMC Link Type."
    
#if BIG_EDIAN
    uint8 m_LinkTypeExtension:4;//AMC Link Type Extension
    uint8 m_Reserved1:4; 
#else
    uint8 m_Reserved1:4;//AMC Link Type Extension
    uint8 m_LinkTypeExtension:4; 
#endif

    uint8 m_LinkGroupId;//Link Grouping ID

#if BIG_EDIAN  
    uint8 m_AsymmetricMatch:2;//AMC Asymmetric Match
    uint8 m_Reserve2:6;
#else
    uint8 m_Reserve2:2;//AMC Asymmetric Match
    uint8 m_AsymmetricMatch:6;
#endif

}AMCLinkInfo;
//set port state--<ATCA R2.0 P250>
typedef struct Set_AMC_Port_State_Req
{
	unsigned char    PICMG_ID;          /*0x00*/
    AMCLinkInfo		 m_LinkInfo;
    unsigned char    m_SetState;        /*0x00:disable;0x01:enable*/
    
#if BIG_EDIAN
    unsigned char  m_Reserved:4;
    unsigned char  m_carrierDeviceId:4; 
#else
    unsigned char  m_carrierDeviceId:4;
    unsigned char  m_Reserved:4; 
#endif

}SET_AMC_PORT_STATE_REQ;

typedef struct Set_AMC_Port_State_Rsp
{
 	unsigned char     m_CompCode;
	unsigned char     PICMG_ID;
}SET_AMC_PORT_STATE_RSP;

//get port state --<ATCA R2.0 P250>
typedef struct Get_AMC_Port_State_Req
{
	unsigned char    PICMG_ID;          /*0x00*/
    unsigned char    mChannelId;       
    
#if BIG_EDIAN
    unsigned char  m_Reserved:4;
    unsigned char  m_carrierDeviceId:4; 
#else
    unsigned char  m_carrierDeviceId:4;
    unsigned char  m_Reserved:4; 
#endif

}GET_AMC_PORT_STATE_REQ;


typedef struct Get_AMC_Port_State_Rsp
{
 	uint8		m_CompCode;
	uint8		PICMG_ID;
    
    AMCLinkInfo	 m_LinkInfo1;
    uint8		m_GetState1;/*0x00:disable;0x01:enable*/
	/*
    LinkInfo m_LinkInfo2;
    uint8    m_GetState2;

    LinkInfo m_LinkInfo3;
    uint8    m_GetState3;

    LinkInfo m_LinkInfo4;
    uint8    m_GetState4;*/

}GET_AMC_PORT_STATE_RSP;


typedef struct _tagLINK_INFO
{
#if BIG_EDIAN
	uint8	m_Interface:2;
	uint8	m_ChannelNumber:6;
#else
	uint8	m_ChannelNumber:6;
	uint8	m_Interface:2;
#endif
	
#if BIG_EDIAN
	uint8	m_LinkTypeEx:4;
	uint8	m_Port3Bit:1;
	uint8	m_Port2Bit:1;
	uint8	m_Port1Bit:1;
	uint8	m_Port0Bit:1;
#else
	uint8	m_Port0Bit:1;
	uint8	m_Port1Bit:1;
	uint8	m_Port2Bit:1;
	uint8	m_Port3Bit:1;
	uint8	m_LinkTypeEx:4;
#endif

	uint8	m_LinkType;
	uint8	m_LinkGroupID;
} LinkInfo;


typedef struct Set_Port_State_Req
{
	uint8		m_PicmgID;          /*0x00*/
	LinkInfo	m_LinkInfo;
	uint8		m_State;        /*0x00:disable;0x01:enable*/
}SET_PORT_STATE_REQ;

typedef struct Set_Port_State_Rsp
{
 	uint8     m_CompCode;
	uint8     m_PicmgID;
}SET_PORT_STATE_RSP;

//get port state --<ATCA R2.0 P250>
typedef struct Get_Port_State_Req
{
	uint8    m_PicmgID;          /*0x00*/
    
#if BIG_EDIAN
    uint8	m_Interface:2;
    uint8	m_ChannelNumber:6;
#else
    uint8	m_ChannelNumber:6;
	uint8	m_Interface:2;
#endif

}GET_PORT_STATE_REQ;


typedef struct Get_Port_State_Rsp
{
 	uint8		m_CompCode;
	uint8		m_PicmgID;
	LinkInfo	m_LinkInfo1;
    uint8		m_State1;	/*0x00:disable;0x01:enable*/

	/*
    LinkInfo m_LinkInfo2;
    uint8    m_GetState2;

    LinkInfo m_LinkInfo3;
    uint8    m_GetState3;

    LinkInfo m_LinkInfo4;
    uint8    m_GetState4;*/

}GET_PORT_STATE_RSP;


//Power Channel Control --<MTCA R1.0 P206>
typedef struct _PowerChannelCtrlReq
{
    uint8    PICMG_ID; 
    uint8    PwrChannelNum; 
    uint8    PwrChannelCtrl; 
    uint8    PwrChannelCurrentLimit; 
    uint8    PrimaryPm; 
    uint8    RedundantPm;  
}PowerChannelCtrlReq;

typedef struct _PowerChannelCtrlRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
}PowerChannelCtrlRsp;

//Get Power Channel Status--<MTCA R1.0 P207>
typedef struct _GetPowerChannelSteReq
{
    uint8    PICMG_ID; 
    uint8    StartPwrChannelNum; 
    uint8    PwrChannelCount;  
}GetPowerChannelSteReq;

typedef struct _GetPowerChannelSteRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
    uint8    MaxPowerChannelNum;
    uint8    GlobalStatus;
    uint8    PowerChannelStatus[12]; 
}GetPowerChannelSteRsp;

//PM Reset--<MTCA R1.0 P211>
typedef struct _PmResetReq
{
 	uint8    PICMG_ID;
    uint8    SiteNum; 
}PmResetReq;
typedef struct _PmResetRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
}PmResetRsp;

//Get PM Status--<MTCA R1.0 P211>
typedef struct _GetPmSteReq
{
 	uint8    m_CompCode;
    uint8    SiteNum;    
}GetPmSteReq;

typedef struct _GetPmSteRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;    
    uint8    PmHealthySte; 
}GetPmSteRsp;

//Pm Heartbeat--<MTCA R1.0 P212>
typedef struct _PmHeartbeatReq
{
 	uint8    PICMG_ID;
    uint8    Timeout;        
    uint8    Ps1;    
}PmHeartbeatReq;

typedef struct _PmHeartbeatRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;    
}PmHeartbeatRsp;

//Get Telco Alarm Capability--<MTCA R1.0 P227>
typedef struct _GetTelcoAlarmCapabReq
{
    uint8    PICMG_ID;    
    uint8    FruDevId;    
}GetTelcoAlarmCapabReq;
typedef struct _GetTelcoAlarmCapabRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;    
    uint8    m_AlarmCapab;    
}GetTelcoAlarmCapabRsp;

//Set Telco Alarm --<MTCA R1.0 P228>
typedef struct _SetTelcoAlarmSteReq
{
    uint8    PICMG_ID;    
    uint8    FruDevId;   
    uint8    AlarmId;  
    uint8    AlarmCtrl;  
}SetTelcoAlarmSteReq;

typedef struct _SetTelcoAlarmSteRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;    
}SetTelcoAlarmSteRsp;

//Get Telco Alarm Capability--<MTCA R1.0 P229>
typedef struct _GetTelcoAlarmSteReq
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;        
    uint8    AlarmId;
}GetTelcoAlarmSteReq;

typedef struct _GetTelcoAlarmSteRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID;        
    uint8    m_AlarmSte;    
}GetTelcoAlarmSteRsp;





typedef struct  _GetSelfTestResultRsp
{
    unsigned char    CompCode;       
    unsigned char    SelfTestResult;       
    unsigned char    SelfTestErrorDef;      	
}GetSelfTestResultRsp;

//Event Commands
typedef struct  _SetEventReceiverReq
{
    unsigned char    m_EventRevSlaveAddr;   /*Event Receiver Slave Address. 0FFh disables Event Message Generation, 
                                                Otherwise: 
                                                [7:1] -  IPMB (I2C) Slave Address 
                                                [0] -  always 0b when [7:1] hold I2C slave address*/
    unsigned char    m_EventRevLun;   /*7:2] -  reserved 
                                        1:0] -  Event Receiver LUN */


}SetEventReceiverReq;

typedef struct  _SetEventReceiverRsp
{
    unsigned char    m_CompCode; 
                                    
}SetEventReceiverRsp;

typedef struct  _GetEventReceiverRsp
{
    unsigned char    m_CompCode;   
    unsigned char    Rec_Slave_Add;   /*[7:1] Slave Address  [0] 0b*/
    unsigned char    Rec_LUN;         /*[7:2] reserved [1:0] LUN*/

}GetEventReceiverRsp;
typedef struct  _GetDeviceSDRInfoRsp
{
    unsigned char    m_CompCode;  
    unsigned char    Number;          /*For Operation = ¡°Get Sensor Count¡± (or if byte 1 not present in request): 
										Number of sensors in device for LUN this command was addressed to. 
										For Operation = ¡°Get SDR Count¡±: 
										Total Number of SDRs in the device..*/    
    unsigned char    Flags;           /* [7] 0b = static sensor population;1b = dynamic sensor population
                                         [6:4] - reserved
                                         [3] - 1b = LUN 3 has sensors,[2] - 1b = LUN 2 has sensors,
                                         [1] - 1b = LUN 1 has sensors,[0] - 1b = LUN 0 has sensors    */
    unsigned char    Indicator_1;
    unsigned char    Indicator_2;
    unsigned char    Indicator_3;
    unsigned char    Indicator_4;      /*Sensor Population Change Indicator. LS byte first.
                                         Four byte timestamp, or counter. Updated or incremented each time the
                                         sensor population changes. This field is not provided if the flags indicate a
                                         static sensor population.*/

}GetDeviceSDRInfoRsp;



typedef struct _FruControlReq
{
    unsigned char    PICMG_ID;          /* 0x00 must be used*/
    unsigned char    m_DeviceId;     /* CmdLun = 0x0*/
    unsigned char    FRU_Control;       /* 00h = Cold Reset,01h = Warm Reset02h = Graceful Reboot,03h = Issue Diagnostic Interrupt,04h = Quiesce,05h â€?FFh = Reserved */

}FruControlReq;

typedef struct  _FruControlRsp
{
    unsigned char    m_CompCode;  
    unsigned char    PICMG_ID;          /*0x00*/

}FruControlRsp;







typedef struct Get_LED_Color_Capabilities_Req
{
	unsigned char    PICMG_ID;          /*0x00*/
	unsigned char    m_DeviceId;
	unsigned char	 LED_ID;

}GET_LED_COLOR_CAPABILITIES_REQ;

typedef struct  Get_LED_Color_Capabilities_Rsp
{
	unsigned char    m_CompCode;
	unsigned char    PICMG_ID;
	unsigned char	 LED_Color_Capabilities;
	unsigned char	 Default_color_Local_state;
	unsigned char	 Default_color_Override_state;

}GET_LED_COLOR_CAPABILITIES_RSP;

// FRU Control Capabilities command
typedef struct _FruCtrlCap
{
	#ifdef BIG_EDIAN
	unsigned char    m_Reserved1:4;				//[4-7] - Reserved
	unsigned char    m_DiagnosticInterrupt:1;	//[3] - lb - Capable of issuing a diagnostic interrupt
	unsigned char    m_GracefulReboot:1;		//[2] - lb - Capable of issuing a graceful reboot
	unsigned char    m_WarmReset:1;				//[1] - lb - Capable of issuing a warm reset
	unsigned char    m_Reserved0:1;				//[0] - Reserved
	#else
	unsigned char    m_Reserved0:1;				//[0] - Reserved
	unsigned char    m_WarmReset:1;				//[1] - lb - Capable of issuing a warm reset
	unsigned char    m_GracefulReboot:1;		//[2] - lb - Capable of issuing a graceful reboot
	unsigned char    m_DiagnosticInterrupt:1;	//[3] - lb - Capable of issuing a diagnostic interrupt
	unsigned char    m_Reserved1:4;				//[4-7] - Reserved
	#endif
}FruCtrlCap;

/*******************************E_KEY*****************************************/




typedef struct _SetFRUActPolicyReq
{
    unsigned char    m_PicmgId;
    unsigned char    m_DeviceId;      
    unsigned char    m_MaskBit;      
    unsigned char    m_ActPolicy;      
}SetFRUActPolicyReq;

typedef struct _SetFRUActPolicyRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;

}SetFRUActPolicyRsp;

typedef struct _GetFRUActPolicyRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;
    uint8    m_FruActPolicy;
}GetFRUActPolicyRsp;

typedef struct _SetFRUActRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;
}SetFRUActRsp;

typedef struct _GetFRUActRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;
    
	uint8    MinSpeedLevel;
	uint8    MaxSpeedLevel;
	uint8    NormalOperLevel;
	uint8    FanTrayPro;
}GetFRUActRsp;



typedef struct _GetIpmbLinkRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;
	uint8    Ipmb0LinkNum;
	uint8    Ipmb0SensorNum;    
}GetIpmbLinkRsp;

typedef struct _SetFanPolicyRsp
{
 	uint8    m_CompCode;
	uint8    PICMG_ID;
 
}SetFanPolicyRsp;

typedef struct _GetFanPolicyRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
    uint8    Policy;
    uint8    Coverage;
}GetFanPolicyRsp;

typedef struct _FruInvenDevLockCtrlRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
    uint8    FruInvenDevLockId[2];
    uint8    FruInvenDevLastCommTs[4];
}FruInvenDevLockCtrlRsp;

typedef struct _FruInvenDevWriteRsp
{
 	uint8    m_CompCode;
    uint8    PICMG_ID; 
    uint8    CountWrite;
}FruInvenDevWriteRsp;



#endif



