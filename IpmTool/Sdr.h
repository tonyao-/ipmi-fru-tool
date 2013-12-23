/*************************************************/


#ifndef  __NMCPEMcmc_SDRH_
#define  __NMCPEMcmc_SDRH_

#ifdef _cplusplus
extern "c"{
#endif

#include "typedef.h"
#include "FruInfo.h"
#include "SensorEventType.h"

#define MAX_STR_LEN 		16
#define SDR_TOTAL_COUNT 	6

#define MGM_CTRL_DEV_LOC 	0xC2
#define SDR_VERSION 		0x51
#define INVALID_SENSORID  	0xFF
#define DEFAULT_RESERIVE_SDR_ID	1
#define FRU_REPO_TYPE 		0x10	//logic FRU device

typedef enum _UnitCode
{
	UNIT_NONE = 0
	, UNIT_C = 1
	, UNIT_V = 4
	, UNIT_A = 5
}UnitCode;

typedef enum _SensorDirection
{
	DIR_UNSPECIFIED = 0
	, DIR_INPUT = 1
	, DIR_OUTPUT = 2
}SensorDirection;

typedef enum _StringType
{
	NUMERIC = 0
	, ALPHA = 1
}StringType;

typedef enum _SharingType
{
	ST_SAME = 0
	, ST_INCREMENTS = 1
}SharingType;

typedef enum _SdrRecordId
{
      SDR_DevLoc = 0
    , SDR_Handle
    , SDR_PChanl
    , SDR_Tempra
    , SDR_Mpower
    , SDR_PPower
}SdrRecordId;

typedef enum _SdrType
{
	  SDR_FULL_SENSOR = 0x1
	, SDR_COMPACT_SENSOR = 0x2
	, SDR_EVENT_ONLY = 0x3
	, SDR_FRUDEV_LOCATOR = 0x11
	, SDR_CTLDEV_LOCATOR = 0x12
}SdrType;

typedef enum _EntityId
{
	  ENTITYID_PM  	= 0x0A
	, ENTITYID_CU	= 0x1E
	, ENTITYID_AMC 	= 0xC1
	, ENTITYID_MCH 	= 0xC2
}EntityId;

typedef struct _SensorItem
{
	uint8 m_EventType;
	//uint8 m_SensorLun;
	uint8 m_EntityID;
	uint8 m_SensorNumber;
	uint8 m_SensorType;
	uint8 m_AssertEventMask_M;
	uint8 m_AssertEventMask_L;
	uint8 m_DeassertEventMask_M;
	uint8 m_DeassertEventMask_L;
	
	uint8 m_Unit1;
	uint8 m_Unit2;
	uint8 m_Unit3;
	
	uint8 m_Name[MAX_STR_LEN];
}SensorItem;

#define SDR_HEADER_LEN 5
typedef struct _SdrHeader
{
	uint16	m_Id;
	uint8	m_Version;
	uint8	m_Type;
	uint8	m_Len;
}SdrHeader;
#define GET_SDR_LEN(pSdr) (((SdrHeader*)(pSdr))->m_Len)
void SdrHeader_Init(void);

//SDR Type 11h-FRU Device Locator Record
typedef struct _FruDevRecod
{
	SdrHeader m_SdrHeader;

	//RECORD KEY BYTES 
	uint8	m_IpmbAddress;
	uint8	m_DeviceId;

	#ifdef BIG_EDIAN
	uint8	m_LogicFruDev:1;
	uint8	m_Resevered:2;
	uint8	m_Lun:2;
	uint8	m_BusId:2;
	#else
	uint8	m_BusId:2;
	uint8	m_Lun:2;
	uint8	m_Resevered:2;
	uint8	m_LogicFruDev:1;
	#endif
	
	uint8	m_ChannelNum;
		
	//RECORD BODY BYTES 
	uint8	m_Resevered1;
	uint8	m_DeviceType;
	uint8	m_DeviceTypeEx;
	uint8	m_EntityId;
	uint8	m_InstanceNum;
	uint8	m_Oem;

	#ifdef BIG_EDIAN
	uint8	m_StrType:3;	//Bit[5-7]
	uint8	m_StrLen:5;		//Bit[0-4]
	#else
	uint8	m_StrLen:5;		//Bit[0-4]
	uint8	m_StrType:3;	//Bit[5-7]
	#endif
	uint8	m_String[MAX_STR_LEN];
}FruDevRecod;

//Management controller Device Locator Record,SDR type = 0x12;
//Device Capabilities
typedef struct _tagDevLocatorRecord
{
	SdrHeader m_SdrHeader;

	//RECORD KEY BYTES 
	uint8	m_SlaveAddr;	
	
	#ifdef BIG_EDIAN
	uint8	m_Reserved0:4;		//Bit[2-7]
	uint8	m_ChannelNum:4;		//Bit[0-3]: Channel number for the channel that the management controller is on
	#else
	uint8	m_ChannelNum:4;		//Bit[0-3]: Channel number for the channel that the management controller is on
	uint8	m_Reserved0:4;		//Bit[2-7]
	#endif

	//RECORD BODY BYTES 
	//Power State Notification Global Initialization
	#ifdef BIG_EDIAN
	uint8	m_Reserved1:6;			//Bit[2-7]
	uint8	m_EventGenerator:2;		//Bit[0-1]
	#else
	uint8	m_EventGenerator:2;		//Bit[0-1]
	uint8	m_Reserved1:6;			//Bit[2-7]
	#endif

	//Device Capabilities
	#ifdef BIG_EDIAN
	uint8	m_ChassisDevice:1;			//Bit[7]
	uint8	m_Bridge:1;					//Bit[6]
	uint8	m_IpmbEventGenerator:1;		//Bit[5]
	uint8	m_IpmbEventReceiver:1;		//Bit[4]
	uint8	m_FruInventoryDevice:1;		//Bit[3]
	uint8	m_SelDevice:1;				//Bit[2]
	uint8	m_SdrRepositoryDevice:1;	//Bit[1]
	uint8	m_SensorDevice:1;			//Bit[0]
	#else
	uint8	m_SensorDevice:1;			//Bit[0]
	uint8	m_SdrRepositoryDevice:1;	//Bit[1]
	uint8	m_SelDevice:1;				//Bit[2]
	uint8	m_FruInventoryDevice:1;		//Bit[3]
	uint8	m_IpmbEventReceiver:1;		//Bit[4]
	uint8	m_IpmbEventGenerator:1;		//Bit[5]
	uint8	m_Bridge:1;					//Bit[6]
	uint8	m_ChassisDevice:1;			//Bit[7]
	#endif
	
	uint8	Reserved[3];
	uint8	m_EntityId;
	uint8	m_InstanceNum;
	uint8	m_Oem;
	
	#ifdef BIG_EDIAN
	uint8	m_StrType:3;	//Bit[5-7]
	uint8	m_StrLen:5;		//Bit[0-4]
	#else
	uint8	m_StrLen:5;		//Bit[0-4]
	uint8	m_StrType:3;	//Bit[5-7]
	#endif
	
	uint8	m_IdString[MAX_STR_LEN];
}DevLocatorRecord;
#if 0
typedef struct _DevCapabilities
{
	#ifdef BIG_EDIAN
	uint8	m_ChassisDevice:1;			//Bit[7]
	uint8	m_Bridge:1;					//Bit[6]
	uint8	m_IpmbEventGenerator:1;		//Bit[5]
	uint8	m_IpmbEventReceiver:1;		//Bit[4]
	uint8	m_FruInventoryDevice:1;		//Bit[3]
	uint8	m_SelDevice:1;				//Bit[2]
	uint8	m_SdrRepositoryDevice:1;	//Bit[1]
	uint8	m_SensorDevice:1;			//Bit[0]
	#else
	uint8	m_SensorDevice:1;			//Bit[0]
	uint8	m_SdrRepositoryDevice:1;	//Bit[1]
	uint8	m_SelDevice:1;				//Bit[2]
	uint8	m_FruInventoryDevice:1;		//Bit[3]
	uint8	m_IpmbEventReceiver:1;		//Bit[4]
	uint8	m_IpmbEventGenerator:1;		//Bit[5]
	uint8	m_Bridge:1;					//Bit[6]
	uint8	m_ChassisDevice:1;			//Bit[7]
	#endif
}DevCapabilities;

typedef struct _tagDevLocatorRecord
{
	//Sensor Record Header,5 bytes.
	SdrHeader m_SdrHeader;
	//Record Key Bytes,2 bytes.
	uint8	m_SlaveAddr;	
	uint8	m_ChannlNum;	
	
	//Record Body Bytes,19 bytes.	
	//Power State Notification Global Initialization
	#ifdef BIG_EDIAN
	uint8	m_Reserved1:6;			//Bit[2-7]
	uint8	m_EventGenerator:2;		//Bit[0-1]
	#else
	uint8	m_EventGenerator:2;		//Bit[0-1]
	uint8	m_Reserved1:6;			//Bit[2-7]
	#endif
	uint8	m_DevCapabi;	//DevCapabilities		
	
	uint8	m_Reserveds[3];
	uint8	m_EntityIds;
	uint8	m_InstanNum;
	uint8	m_OemReserv;

	#ifdef BIG_EDIAN
	uint8	m_StrType:3;	//Bit[5-7]
	uint8	m_StrLen:5;		//Bit[0-4]
	#else
	uint8	m_StrLen:5;		//Bit[0-4]
	uint8	m_StrType:3;	//Bit[5-7]
	#endif
	uint8	m_IdString[MAX_STR_LEN];
}DevLocatorRecord;
#endif

void DevLocatorRecord_Init(DevLocatorRecord* pDevLocatorRecord
	, uint16 recordId
	, uint8 ipmbAddress
	, uint8 siteNumber
	, const int8*  pName);

typedef struct _Unit1
{
	#ifdef BIG_EDIAN
	uint8	m_isSigned:2;		//Bit[6-7]:reserved. Write as 11b.
	uint8	m_RateUnit:3;		//Bit[3-5]:Rate unit 
	uint8	m_ModifyUnitFlag:2;	//Bit[1-2]:Modify unit; 0b = none ;01b = Basic Unit / Modifier Unit ;10b = Basic Unit * Modifier Unit 
	uint8	m_Percentage:1;		//Bit[0]:Percentage,  0b = no, 1b = yes 
	#else
	uint8	m_Percentage:1;		//Bit[0]:Percentage,  0b = no, 1b = yes 
	uint8	m_ModifyUnitFlag:2;	//Bit[1-2]:Modify unit; 0b = none ;01b = Basic Unit / Modifier Unit ;10b = Basic Unit * Modifier Unit 
	uint8	m_RateUnit:3;		//Bit[3-5]:Rate unit 
	uint8	m_isSigned:2;		//Bit[6-7]:reserved. Write as 11b.
	#endif
}Unit1;

typedef struct _SensorShare
{
	#ifdef BIG_EDIAN
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	#else
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	#endif
}SensorShare;

//SDR Type 01h-Full Sensor Record
typedef struct _FullSensorRecord
{
	SdrHeader m_SdrHeader;

	//Key byte
	#ifdef BIG_EDIAN
	uint8	m_IpmbAddress:7;	//Bit[1-7]: I2C Slave Addres Or System Software ID
	uint8	m_IdType:1;			//Bit[0]: 0-ID is I2C Slave Addres.
	#else
	uint8	m_IdType:1;			//Bit[0]: 0-ID is I2C Slave Addres.
	uint8	m_IpmbAddress:7;	//Bit[1-7]: I2C Slave Addres Or System Software ID
	#endif
	
	#ifdef BIG_EDIAN
	uint8	m_ChannelNum:4;		//Bit[4-7]:Channel Number
	uint8	m_Reserved:2;		//Bit[2-3]:LUN for Write/Read FRU commands to access FRU information
	uint8	m_SensorOwnerLun:2;	//Bit[0-1]:LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor
	#else
	uint8	m_SensorOwnerLun:2;	//Bit[0-1]:LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor
	uint8	m_Reserved:2;		//Bit[2-3]:LUN for Write/Read FRU commands to access FRU information
	uint8	m_ChannelNum:4;		//Bit[4-7]:Channel Number
	#endif
	
	uint8	m_SensorNumber;	//Unique number identifying the sensor behind the given slave address and LUN, Code FFh reserved.
			
	//RECORD BODY BYTES 
	//Entity ID 
	uint8	m_EntityId;	//0xC1 = Mcmc; 0xC2 = MCMC; 0x0A: EMcmc_PM ,  See Table 43-13, Entity ID Codes.
	
	#ifdef BIG_EDIAN
	//Entity Instance 
	uint8	m_islogicalEntity:1;//Bit[7]: 0b-Physical entity, 1b-Logical entity
	uint8	m_InstanceNumber:7;	//Bit[0-6]: SiteNum + 60h
	#else
	uint8	m_InstanceNumber:7;	//Bit[0-6]: SiteNum + 60h
	uint8	m_islogicalEntity:1;//Bit[7]: 0b-Physical entity, 1b-Logical entity
	#endif
	
	#ifdef BIG_EDIAN
	//Sensor Initialization 
	uint8	m_Settable:1;		//Bit[7]: 0b = Sensor is not settable; 1b = Sensor is settable 
	uint8	m_InitScanning:1;	//Bit[6]: 1b = enable scanning, accepts the"enable/disable scanning" bit in the Set Sensor Event Enable command
	uint8	m_InitEvent:1;		//Bit[5]: 1b = enable events
	uint8	m_InitThresholds:1;	//Bit[4]: 1b = initialize sens or thresholds (per settable threshold mask below)
	uint8	m_InitHysteresis:1;	//Bit[3]: 1b = initialize sensor hysteresis
	uint8	m_InitSensorType:1;	//Bit[2]: 1b = initialize Sensor Type and Event / Reading Type code
	uint8	m_EventEnabled:1;	//Bit[0]: Sensor Default (power up) State, 0b = event generation disabled, 1b = event generation enabled 
	uint8	m_ScanningEnabled:1;//Bit[0]: Sensor Default (power up) State, 0b = sensor scanning disabled, 1b = sensor scanning enabled 
	#else
	uint8	m_ScanningEnabled:1;//Bit[0]: Sensor Default (power up) State 
	uint8	m_EventEnabled:1;	//Bit[0]: Sensor Default (power up) State 
	uint8	m_InitSensorType:1;	//Bit[2]: 1b = initialize Sensor Type and Event / Reading Type code
	uint8	m_InitHysteresis:1;	//Bit[3]: 1b = initialize sensor hysteresis
	uint8	m_InitThresholds:1;	//Bit[4]: 1b = initialize sens or thresholds (per settable threshold mask below)
	uint8	m_InitEvent:1;		//Bit[5]: 1b = enable events
	uint8	m_InitScanning:1;	//Bit[6]: 1b = enable scanning, accepts the"enable/disable scanning" bit in the Set Sensor Event Enable command
	uint8	m_Settable:1;		//Bit[7]: 0b = Sensor is not settable; 1b = Sensor is settable 
	#endif

	#ifdef BIG_EDIAN
	//Sensor Capabilities 
	uint8	m_IgnoreSensor:1;	//Bit[7]: 1b = ignore sensor if Entity is not present or disabled. 
	uint8	m_AutoRearm:1;		//Bit[6]: 0b = no (manual), 1b = yes (auto) 
	uint8	m_Hysteresis:2;		//Bit[4-5]: Sensor Hysteresis Support, 00b =  No hysteresis
	uint8	m_ThresholdFlag:2;	//Bit[2-3]: 00b =  no thresholds. 01b = thresholds are readable; 11b = Fixed,
	uint8	m_EventMessage:2;	//Bit[0-1]: 00b =  per threshold/discrete-state event enable/disable control
										//	01b =  entire sensor only
										//	10b =  global disable only
										//	11b =  no events from sensor 
	#else
	uint8	m_EventMessage:2;	
	uint8	m_ThresholdFlag:2;	
	uint8	m_Hysteresis:2;		
	uint8	m_AutoRearm:1;		
	uint8	m_IgnoreSensor:1;	
	#endif
	
	uint8	m_SensorType;		//IPMI Table 42-3, Sensor Type Codes
	uint8	m_EventType;		//IPMI Table 42-1, Event/Reading Type Code Range
	uint16	m_LowerThresholdMask;		//for non- threshold based sensors,for threshold-based sensors
	uint16	m_UpperThresholdMask;		//for non- threshold based sensors, for threshold-based sensors
	uint16	m_ReadingMask;				//for non- threshold based sensors, 
	//uint8	m_ReadableThresholdMask;	//for threshold-based sensors
	//uint8	m_SettableThresholdMask;	//for threshold-based sensors

	//Sensor Units
	uint8 	m_Uint1;
	uint8	m_BaseUnit;			//Units Type code: See Units Type Codes table. 
	uint8	m_ModifyUnit;		//Units Type code, 00h if unused. 

	#ifdef BIG_EDIAN
	uint8	m_reserved1:1;		//Bit[7]: reserved
	uint8	m_Linearization:7;	//Bit[0-6]:enum (linear, ln, log10, log2, e, ex p10, exp2, 1/x, sqr(x), cube(x), sqrt(x), cube-1(x) ) 
								// - 70h = non-linear. 71h-7Fh = non-linear, OEM defined.
	#else
	uint8	m_Linearization:7;
	uint8	m_reserved1:1;
	#endif

	uint8	m_MLs;					//M: LS 8 bits  [2��s complement, signed, 10 bit ��M�� value.] . 
	#ifdef BIG_EDIAN
	uint8	m_MMs:2;				//Bit[7:6]: M: MS 2 bits 
	uint8	m_Tolerance:6;			//Bit[0:5]: Tolerance: 6 bits, unsigned  (Tolerance in +/- ? raw counts)
	#else
	uint8	m_Tolerance:6;			//Bit[0:5]: Tolerance: 6 bits, unsigned  (Tolerance in +/- ? raw counts)
	uint8	m_MMs:2;				//Bit[7:6]: M: MS 2 bits 
	#endif
	
	uint8	m_BLs;					//B: LS 8 bits  [2��s complement, signed, 10-bit ��B�� value.] -
	#ifdef BIG_EDIAN
	uint8	m_BMs:2;				//Bit[6:7]: B: MS 2 bits 
	uint8	m_AccuracyLs:6;			//Bit[0:5]: Accuracy: LS 6 bits
	#else
	uint8	m_AccuracyLs:6;			//Bit[0:5]: Accuracy: LS 6 bits
	uint8	m_BMs:2;				//Bit[7:6]: B: MS 2 bits 
	#endif

	#ifdef BIG_EDIAN
	uint8	m_AccuracyMs:4;			//Bit[4:7]: Accuracy: MS 4 bits
	uint8	m_AccuracyExp:2;		//Bit[2:3]: Accuracy exp: 2 bits, unsigned 
	uint8	m_SensorDirection:2;	//Bit[0:1]: Sensor Directio
	#else
	uint8	m_SensorDirection:2;	//Bit[0:1]: Sensor Directio
	uint8	m_AccuracyExp:2;		//Bit[2:3]: Accuracy exp: 2 bits, unsigned 
	uint8	m_AccuracyMs:4;			//Bit[4:7]: Accuracy: MS 4 bits
	#endif
	
	#ifdef BIG_EDIAN
	uint8	m_Rexp:4;				//Bit[4:7]: R (result) exponent 4 bits, 2��s complement, signed
	uint8	m_Bexp:4;				//Bit[0:3]: B exponent 4 bits, 2��s complement, signed
	#else
	uint8	m_Bexp:4;				//Bit[0:3]: B exponent 4 bits, 2��s complement, signed
	uint8	m_Rexp:4;				//Bit[4:7]: R (result) exponent 4 bits, 2��s complement, signed
	#endif

	#ifdef BIG_EDIAN
	uint8	m_reserved3:5;			//Bit[3:7]: R (result) exponent 4 bits, 2��s complement, signed
	uint8	m_IsMinSpecified:1;		//Bit[2]: normal min specified  1b = yes,  0b = normal min field unspecified
	uint8	m_IsMaxSpecified:1;		//Bit[1]: normal max specified  1b = yes,  0b = normal max field unspecified
	uint8	m_IsReadingSpecified:1;	//Bit[0]: nominal reading specified  1b  = yes, 0b = nominal reading field unspecified
	#else
	uint8	m_IsReadingSpecified:1;	//Bit[0]: nominal reading specified  1b  = yes, 0b = nominal reading field unspecified
	uint8	m_IsMaxSpecified:1;		//Bit[1]: normal max specified  1b = yes,  0b = normal max field unspecified
	uint8	m_IsMinSpecified:1;		//Bit[2]: normal min specified  1b = yes,  0b = normal min field unspecified
	uint8	m_reserved3:5;			//Bit[3:7]: R (result) exponent 4 bits, 2��s complement, signed
	#endif

	uint8 	m_NormalReading;		//Given as a raw value.
	uint8 	m_NormalMax;		//Given as a raw value.
	uint8 	m_NormalMin;		//Given as a raw value.
	uint8 	m_MaxReading;		//Given as a raw value.
	uint8 	m_MinReading;		//Given as a raw value.
	
	uint8 	m_UpperNonRecoverable;
	uint8 	m_UpperCiritcal;
	uint8 	m_UpperNonCiritcal;
	uint8 	m_LowerNonRecoverable;
	uint8 	m_LowerCiritcal;
	uint8 	m_LowerNonCiritcal;
	
 	uint8	m_PoHysteresis;		//Positive-going Threshold Hysteresis value
 	uint8	m_NeHysteresis;		//Negative-going Threshold Hysteresis value 
	uint8	m_Reserved4[2];
	uint8	m_Oem;
	
	#ifdef BIG_EDIAN
	uint8	m_IdStrType:2;
	uint8	m_Reserved3:1;
	uint8	m_IdStrLen:5;	
	#else
	uint8	m_IdStrLen:5;	
	uint8	m_Reserved3:1;
	uint8	m_IdStrType:2;
	#endif
	uint8	m_IdString[MAX_STR_LEN];
}FullSensorRecord;

void FullSensorRecord_Init(FullSensorRecord* pFullSensorRecord
	, uint16 recordId
	, const SensorItem* pSensorItem
	, const Threshold* pThreshold
	, uint8 ipmbAddress
	, uint8 siteNum
	);

//SDR Type 02h-Compact Sensor Record
typedef struct _CompactSensorRecord
{
	SdrHeader m_SdrHeader;

	//Key byte
	#ifdef BIG_EDIAN
	uint8	m_IpmbAddress:7;	//Bit[1-7]: I2C Slave Addres Or System Software ID
	uint8	m_IdType:1;			//Bit[0]: 0-ID is I2C Slave Addres.
	#else
	uint8	m_IdType:1;			//Bit[0]: 0-ID is I2C Slave Addres.
	uint8	m_IpmbAddress:7;	//Bit[1-7]: I2C Slave Addres Or System Software ID
	#endif
	
	#ifdef BIG_EDIAN
	uint8	m_ChannelNum:4;		//Bit[4-7]:Channel Number
	uint8	m_FruOwnerLun:2;	//Bit[2-3]:LUN for Write/Read FRU commands to access FRU information
	uint8	m_SensorOwnerLun:2;	//Bit[0-1]:LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor
	#else
	uint8	m_SensorOwnerLun:2;	//Bit[0-1]:LUN in the Sensor Owner that is used to send/receive IPMB messages to access the sensor
	uint8	m_FruOwnerLun:2;	//Bit[2-3]:LUN for Write/Read FRU commands to access FRU information
	uint8	m_ChannelNum:4;		//Bit[4-7]:Channel Number
	#endif
	
	uint8	m_SensorNumber;	//Unique number identifying the sensor behind the given slave address and LUN, Code FFh reserved.
			
	//RECORD BODY BYTES 
	//Entity ID 
	uint8	m_EntityId;	//0xC1 = Mcmc; 0xC2 = MCMC; 0x0A: EMcmc_PM ,  See Table 43-13, Entity ID Codes.
	
	#ifdef BIG_EDIAN
	//Entity Instance 
	uint8	m_islogicalEntity:1;//Bit[7]: 0b-Physical entity, 1b-Logical entity
	uint8	m_InstanceNumber:7;	//Bit[0-6]: SiteNum + 60h
	#else
	uint8	m_InstanceNumber:7;	//Bit[0-6]: SiteNum + 60h
	uint8	m_islogicalEntity:1;//Bit[7]: 0b-Physical entity, 1b-Logical entity
	#endif
	
	#ifdef BIG_EDIAN
	//Sensor Initialization 
	uint8	m_Settable:1;		//Bit[7]: 0b = Sensor is not settable; 1b = Sensor is settable 
	uint8	m_InitScanning:1;	//Bit[6]: 1b = enable scanning, accepts the"enable/disable scanning" bit in the Set Sensor Event Enable command
	uint8	m_InitEvent:1;		//Bit[5]: 1b = enable events
	uint8	m_Reserved:1;		//Bit[4]
	uint8	m_InitHysteresis:1;	//Bit[3]: 1b = initialize sensor hysteresis
	uint8	m_InitSensorType:1;	//Bit[2]: 1b = initialize Sensor Type and Event / Reading Type code
	uint8	m_EventEnabled:1;	//Bit[0]: Sensor Default (power up) State 
	uint8	m_ScanningEnabled:1;//Bit[0]: Sensor Default (power up) State 
	#else
	uint8	m_ScanningEnabled:1;//Bit[0]: Sensor Default (power up) State 
	uint8	m_EventEnabled:1;	//Bit[0]: Sensor Default (power up) State 
	uint8	m_InitSensorType:1;	//Bit[2]: 1b = initialize Sensor Type and Event / Reading Type code
	uint8	m_InitHysteresis:1;	//Bit[3]: 1b = initialize sensor hysteresis
	uint8	m_Reserved:1;		//Bit[4]
	uint8	m_InitEvent:1;		//Bit[5]: 1b = enable events
	uint8	m_InitScanning:1;	//Bit[6]: 1b = enable scanning, accepts the"enable/disable scanning" bit in the Set Sensor Event Enable command
	uint8	m_Settable:1;		//Bit[7]: 0b = Sensor is not settable; 1b = Sensor is settable 
	#endif

	#ifdef BIG_EDIAN
	//Sensor Capabilities 
	uint8	m_IgnoreSensor:1;	//Bit[7]: 1b = ignore sensor if Entity is not present or disabled. 
	uint8	m_AutoRearm:1;		//Bit[6]: 0b = no (manual), 1b = yes (auto) 
	uint8	m_Hysteresis:2;		//Bit[4-5]: Sensor Hysteresis Support, 00b =  No hysteresis
	uint8	m_ThresholdFlag:2;	//Bit[2-3]: 00b =  no thresholds. 01b = thresholds are readable; 11b = Fixed,
	uint8	m_EventMessage:2;	//Bit[0-1]: 00b =  per threshold/discrete-state event enable/disable control
										//	01b =  entire sensor only
										//	10b =  global disable only
										//	11b =  no events from sensor 
	#else
	uint8	m_EventMessage:2;	//Bit[0-1]: 00b =  per threshold/discrete-state event enable/disable control
										//	01b =  entire sensor only
										//	10b =  global disable only
										//	11b =  no events from sensor 
	uint8	m_ThresholdFlag:2;	//Bit[2-3]: 00b =  no thresholds. 01b = thresholds are readable; 11b = Fixed,
	uint8	m_Hysteresis:2;		//Bit[4-5]: Sensor Hysteresis Support, 00b =  No hysteresis
	uint8	m_AutoRearm:1;		//Bit[6]: 0b = no (manual), 1b = yes (auto) 
	uint8	m_IgnoreSensor:1;	//Bit[7]: 1b = ignore sensor if Entity is not present or disabled. 
	#endif
	
	uint8	m_SensorType;		//IPMI Table 42-3, Sensor Type Codes
	uint8	m_EventType;		//IPMI Table 42-1, Event/Reading Type Code Range

	uint16	m_LowerThresholdMask;		//for non- threshold based sensors, for threshold-based sensors
	uint16	m_UpperThresholdMask;		//for non- threshold based sensors, for threshold-based sensors
	uint16	m_ReadingMask;				//for non- threshold based sensors
	//uint8	m_ReadableThresholdMask;	//for threshold-based sensors
	//uint8	m_SettableThresholdMask;	//for threshold-based sensors

	//Sensor Units
	uint8 	m_Uint1;
	uint8	m_BaseUnit;			//Units Type code: See Units Type Codes table. 
	uint8	m_ModifyUnit;		//Units Type code, 00h if unused. 

	//Sensor Record Sharing, Sensor Direction 
	//SensorShare m_SensorShare;
	#ifdef BIG_EDIAN
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	#else
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	#endif
	#ifdef BIG_EDIAN
	uint8	m_EntitySharing:1;		//Bit[7]:
	uint8	m_IdStringOffset:7;		//Bit[0-6]:ID String Instance Modifier Offset 
	#else
	uint8	m_IdStringOffset:7;		//Bit[0-6]:ID String Instance Modifier Offset 
	uint8	m_EntitySharing:1;		//Bit[7]:
	#endif
	
	uint8	m_PoHysteresis;		//Positive-going Threshold Hysteresis value
	uint8	m_NeHysteresis;		//Negative-going Threshold Hysteresis value 
	
	uint8	m_Reserved2[3];
	uint8	m_Oem;

	#ifdef BIG_EDIAN
	uint8	m_IdStrType:2;
	uint8	m_Reserved3:1;
	uint8	m_IdStrLen:5;	
	#else
	uint8	m_IdStrLen:5;	
	uint8	m_Reserved3:1;
	uint8	m_IdStrType:2;
	#endif
	uint8	m_IdString[MAX_STR_LEN];
}CompactSensorRecord;

void CompactSensorRecord_Init(CompactSensorRecord* pCompactSensorRecord
	, uint16 recordId
	, const SensorItem* pSensorItem
	, uint8 ipmbAddress
	, uint8 siteNum
	);

//SDR Type 02h-Compact Sensor Record
typedef struct _EventOnlyRecod
{
	SdrHeader m_SdrHeader;

	//RECORD KEY BYTES
	uint8 	m_SensorOwnerId;		//Sensor Owner ID
	uint8 	m_SensorOwnerLun;		//Sensor Owner LUN
	uint8 	m_SensorNumber;		//Sensor Owner Number
	//RECORD BODY BYTES
	uint8 	m_EntityId;			//Entity ID
	uint8 	m_EntityInstance;		//Entity Instance
	uint8 	m_SensorType;			//Sensor Type
	uint8 	m_EventType;			//Event / Reading Type Code
	#ifdef BIG_EDIAN
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	#else
	uint8	m_ShareCount:4;			//Bit[0-3]: number of sensors sharing this record
	uint8	m_IdStringType:2;		//Bit[4-5]: 00b = numeric; 01b = alpha, ID String Instance Modifier Type
	uint8	m_SensorDirection:2;	//Bit[6-7]:Sensor Direction, 00b =  unspecified / not applicable ;01b =  input ;10b =  output 
	#endif
	#ifdef BIG_EDIAN
	uint8	m_EntitySharing:1;		//Bit[7]:
	uint8	m_IdStringOffset:7;		//Bit[0-6]:ID String Instance Modifier Offset 
	#else
	uint8	m_IdStringOffset:7;		//Bit[0-6]:ID String Instance Modifier Offset 
	uint8	m_EntitySharing:1;		//Bit[7]:
	#endif
	uint8	m_Reserved;
	uint8	m_Oem;
	
	#ifdef BIG_EDIAN
	uint8	m_IdStrType:2;
	uint8	m_Reserved3:1;
	uint8	m_IdStrLen:5;	
	#else
	uint8	m_IdStrLen:5;	
	uint8	m_Reserved3:1;
	uint8	m_IdStrType:2;
	#endif
	uint8	m_IdString[MAX_STR_LEN];
}EventOnlyRecod;

void EventOnlyRecod_Init(EventOnlyRecod* pEventOnlyRecod
	, uint16 recordId
	, const SensorItem* pSensorItem
	, uint8 ipmbAddress
	, uint8 siteNum
	);

//Operation Support detail
typedef struct _SdrOperationSupport
{
	#ifdef BIG_EDIAN
	uint8	m_OverflowFlag:1;		//Bit[7]:1=SDR could not  be written due to lack of space in the SDR Repository.  
	uint8	m_modalSDR:2;			//Bit[5-6]: 00b = modal/non-modal SDR Repository Update operation unspecified
	uint8	m_Reserved:1;			//Bit[4]
	uint8	m_DeleteSupported:1;	//Bit[3]: Delete SDR command supported 
	uint8	m_AddSupported:1;		//Bit[2]: Partial Add SDR command supported
	uint8	m_ReserveSupported :1;	//Bit[1]: Reserve SDR Repository command supported
	uint8	m_GetAllocInfoSupported :1;	//Bit[0]: Get SDR Repository Allocation Information command supported 
	#else
	uint8	m_GetAllocInfoSupported :1;	//Bit[0]: Get SDR Repository Allocation Information command supported 
	uint8	m_ReserveSupported :1;	//Bit[1]: Reserve SDR Repository command supported
	uint8	m_AddSupported:1;		//Bit[2]: Partial Add SDR command supported
	uint8	m_DeleteSupported:1;	//Bit[3]: Delete SDR command supported 
	uint8	m_Reserved:1;			//Bit[4]
	uint8	m_modalSDR:2;			//Bit[5-6]: 00b = modal/non-modal SDR Repository Update operation unspecified
	uint8	m_OverflowFlag:1;		//Bit[7]:1=SDR could not  be written due to lack of space in the SDR Repository.  
	#endif
}SdrOperationSupport;


typedef struct _tagSdr
{
	uint8		m_Reserved:7;
	uint8		m_isUnableAceSdrRepo:1;
	uint8		m_isValid:1;

	uint16		m_ReservationId;	
	uint8 		m_Version;
	uint16		m_RecordCount;
	uint16		m_SensorRecordCount;

	SdrOperationSupport	m_OperationSupport;
	
	uint16		m_MaxSpace;
	uint16		m_UsedSpace;
	uint8* 		m_pBuff;
	
	SdrHeader**	m_pRecordArray;
	uint8		m_RecordArrayCount;
}Sdr;

int McDevLocatorRecord_CovertToFruDevRecord(uint8 deviceId, DevLocatorRecord* pDevLocatorRecord, FruDevRecod* pFruDevRecod);

void Sdr_Init(Sdr* pSdr, SdrHeader* pSdrHeader[], uint8 count);
void Sdr_InitEx(Sdr* pSdr, SdrHeader* pSdrHeader[], uint8 count, uint8* pBuff, uint16 bufLen);
uint16 Sdr_GetSdrCount(Sdr* pSdr, uint8 isSensorCount);
uint8 Sdr_GetRecord(Sdr* pSdr, uint16 recordId, uint16* pNextRecordId, uint8 offset, uint8* pBuff, uint8 len);
Bool Sdr_Verify(Sdr* pSdr);
Bool Sdr_Add(Sdr* pSdr, SdrHeader* pSdrHeader);
void Sdr_GetRepositoryInfo(Sdr* pSdr, uint8* pBuff, uint8 len);
SdrHeader* Sdr_Find(Sdr* pSdr, uint16 recordId);
SdrHeader* Sdr_NewRecord(Sdr* pSdr, SdrHeader* pSdrHeader);
void Sdr_Reset(Sdr* pSdr);
void Sdr_Release(Sdr* pSdr);
void Sdr_VerifyReset(Sdr* pSdr);
void Sdr_Dump(Sdr* pSdr);

#ifdef _cplusplus
}
#endif

#endif 	/*_NMCPEMcmc_SDRH_*/



