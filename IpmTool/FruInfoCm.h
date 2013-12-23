
#ifndef  __FRUINFO_CM_H__
#define  __FRUINFO_CM_H__

#ifdef _cplusplus
extern "c"{
#endif

#include "FruInfo.h"

typedef enum _tagPicmgRecordId
{
	  PRID_UserInfo				= 0x1
	, PRID_CurrentReq			= 0x16
	, PRID_CarrierP2pConnect	= 0x18	//Carrier Point-to-Point Connectivity
	, PRID_AmcP2pConnect		= 0x19	//Amc Point-to-Point Connectivity
	, PRID_FruInfoPartition		= 0x20
	, PRID_CarrierManagerIpLink	= 0x21
	, PRID_CarrierInfo			= 0x22
	, PRID_PowerPolicy			= 0x25
	, PRID_ActAndPm				= 0x26
	, PRID_PowerCapability		= 0x27
	, PRID_OemModuleDesc		= 0x29
}PicmgRecordId;

typedef struct _tagPartDesc
{
	uint8 m_Offset[2];	// LS byte first. This contains the offset within the physical FRU
						//Information device of the partition divided by 10h.
	uint8 m_Length[2];	//LS byte first. This contains the length of the partition.
}PartDesc;

//Table 3-10, FRU Information Partition record 20h
typedef struct _tagFruInfoPart
{
	RecordHeader m_header;
	uint8 m_PartDescCount;
	PartDesc m_PartDesc[1];
}FruInfoParti;


//Table 3-12 Carrier Manager IP Link record 21h
typedef struct _tagCarrierIpLink
{
	RecordHeader m_header;
	uint8 m_HelfManagerIp[4];
	uint8 m_CarrierManageIp[4];
	uint8 m_MCH1Ip[4];
	uint8 m_MCH2Ip[4];
	uint8 m_SubnetMaskIp[4];
	uint8 m_Gateway0Ip[4];
	uint8 m_Gateway1Ip[4];
	uint8 m_Username[17];
	uint8 m_Password[21];
}CarrierIpLink;

//Table 3-24 MicroTCA Carrier Power Policy descriptor
enum
{
	  PM_PRIM 		= 0x00
	, PM_REDU 		= 0x01
	, PM_NOT_DEFINE	= 0xFF
};
typedef struct _tagPowerPolicyDesc
{
	uint8 m_PmSiteNumber;	//Identifies the PM described in this descriptor.
	uint8 m_MaxCurrent[2];		//Maximum Current Override, LS Byte first. (1/10A)
	uint8 m_PmRole;			//00h  = Primary PM
							//01h = Redundant PM
							//FFh = PM Role is not specified
							//All other values are reserved
	uint8 m_PowerChannelCount;	//Power Channel Count
}PowerPolicyDesc;

//Table 3-23, "MicroTCA Carrier Power Policy record" 25h
#define MAX_NUM_OF_PMS 2
typedef struct _tagCarriePowerPolicy
{
	RecordHeader m_header;
	
	uint8 m_NumberOfPMs;
	PowerPolicyDesc m_Desc[1];	// An array of descriptors with one for each PM
}CarriePowerPolicy;

typedef struct _tagSlotEntry
{
	uint8 m_SiteNum;
	uint8 m_SiteType;
	uint8 m_SlotNum;
	uint8 m_TierNum;
	uint8 m_SlotOriginY[2];
	uint8 m_SlotOriginX[2];
}SlotEntry;
//Table 3-16 Carrier FRU Information 22h
#define INVALID_CARRIER_NUM 0xFF
typedef struct _tagCarrierInfo
{
	RecordHeader m_header;
	uint8 m_Number;				//Carrier number
	
	#ifdef BIG_EDIAN
	uint8 m_Orientation:1;		//Bit[7], Carrier Orientation
	uint8 m_SlotEntryCount:7;	//Bit[0:6] Slot Entry Count
	#else
	uint8 m_SlotEntryCount:7;	//Bit[0:6] Slot Entry Count
	uint8 m_Orientation:1;		//Bit[7], Carrier Orientation
	#endif

	SlotEntry m_SlotEntry[1];
}CarrierInfo;

//Activation Control
enum
{
	  SYSTEM_MANAGER_ACT= 0	//The System Manager activates this FRU
	, CM_ACT	 		= 1	//The Carrier Manager activates this FRU, and it proceeds to M3
	, SHELF_MANAGER_ACT	= 2	//The Shelf Manager activates this FRU automatically
};
//Table 3-26 MicroTCA Carrier Activation and Current Descriptor
typedef struct _tagCarrierActAndPmDesc
{
	uint8 m_SiteType;		//Site Type
	uint8 m_SiteNum;		//Site Number
	uint8 m_PowerChanNum;	//Power Channel Number
	uint8 m_MaxCurrent;		//Maximum Channel Current(1/10A)
	
	#ifdef BIG_EDIAN
	uint8 m_ActCtrl:2;		//Bit[7:6] - Activation Control
								//11b = Reserved
								//10b = The Shelf Manager activates this FRU automatically
								//01b = The Carrier Manager activates this FRU, and it proceeds to M3
								//00b = The System Manager activates this FRU
	uint8 m_Delay:6;		//Bit[5:0] - Delay Before Next Power On
	
	uint8 m_DeactCtrl:2;	//Bit[7:6] -  Deactivation Control
	uint8 m_Reserve:6;		//[5:0] - Reserved.
	#else
	uint8 m_Delay:6;		//Bit[5:0] - Delay Before Next Power On
	uint8 m_ActCtrl:2;		//Bit[7:6] - Activation Control
								//11b = Reserved
								//10b = The Shelf Manager activates this FRU automatically
								//01b = The Carrier Manager activates this FRU, and it proceeds to M3
								//00b = The Cml Manager activates this FRU
								
	uint8 m_Reserve:6;		//[5:0] - Reserved.
	uint8 m_DeactCtrl:2;	//Bit[7:6] -  Deactivation Control
	#endif	
}CarrierActAndPmDesc;

//Table 3-25, "MicroTCA Carrier Activation and Power Management record" 26h
typedef struct _tagCarrierActAndPm
{
	RecordHeader m_header;
	uint8 m_allowActTime;	//Allowance for Module Activation Readiness
	uint8 m_DescCount;		//Module Activation and Current Descriptor Count
	CarrierActAndPmDesc m_Desc[1];
}CarrierActAndPm;

//Table 3-8, "OEM Module description record" 29h
typedef struct _tagOemModuleDesc
{
	RecordHeader m_header;
	uint8 m_OemId[3];	//OEM Manufacturer ID. LS byte first
	uint8 m_SiteTyp;	//Site Type
	uint8 m_SiteNumber;	//Site Number
	uint8 m_DeviceID;	//Device ID
}OemModuleDesc;

//Table 3-10, "Module Current requirement" 16h
typedef struct _tagCurrentReq
{
	RecordHeader m_header;
	uint8 m_Current;	//0.1A
}CurrentReq;

//Table 3-10, "User	Define	Info" 01h
typedef struct _tagUserDefineInfo
{
	RecordHeader m_header;
	uint8 m_RecordCount;	//
	union
	{
		struct
		{
			uint8 m_type:2;	//Bit 7-6
			uint8 m_reserved:1; //Bit 5
			uint8 m_len:5;	//Bit 4-0;
		};
		uint8 m_BState;
	};
	uint8 m_Macaddr1[6];	//Mac addr
	uint8 m_Macaddr2[6];	//Mac addr
}UserDefineInfo;

//Table 3-27 Power Module Capability record
typedef struct _tagPowerCapability
{
	RecordHeader m_header;
	uint8 m_MaxCurrentOutPut[2];	//Maximum Current Output, 0.1A
}PowerCapability;

//Table 3-18 AMC Link Descriptor Asymmetric Match field values
typedef enum _MatchField
{
	EXACT_MATCH = 0
	, MATCH_10
	, MATCH_01
	, RESERVED
}MatchField;

//Table 3-17 AMC Channel Descriptor
typedef struct _AmcChnlDesc
{
	/*
    uint32 reserve:4;	//Reserved. Must be 1111b.
    uint32 m_Lane3:5;	//19:15 Lane 3 Port Number.
    uint32 m_Lane2:5;	//14:10 Lane 2 Port Number.
    uint32 m_Lane1:5;	//9:5 Lane 1 Port Number.
    uint32 m_Lane0:5;	//4:0 Lane 0 Port Number. 
    */
	uint8 m_Bytes[3];
}AmcChnlDesc;

typedef struct _Guid
{
	uint8	m_Guid[16];
}Guid;

//Table 3-21 AMC Link Type
typedef enum _AmcLinkType
{
	  ALT_PCIE 			= 0x2
	, ALT_PCIE_SWITCH 	= 0x3
	, ALT_PCIE_SWITCH1 	= 0x4
	, ALT_ETHERNET 		= 0x5
	, ALT_SERIAL 		= 0x6
	, ALT_STORAGE 		= 0x7
	, ALT_GUID_MIN 		= 0xF0
	, ALT_GUID_MAX 		= 0xFE
}AmcLinkType;

//Table 3-20 AMC Link Designator
typedef struct _AmcLinkDesignator
{
	uint8 m_ChannelId;
#ifdef BIG_EDIAN
	uint8 m_Reserved:4;
	uint8 m_Lane3Bit:1;
	uint8 m_Lane2Bit:1;
	uint8 m_Lane1Bit:1;
	uint8 m_Lane0Bit:1;
#else
	uint8 m_Lane0Bit:1;
	uint8 m_Lane1Bit:1;
	uint8 m_Lane2Bit:1;
	uint8 m_Lane3Bit:1;
	uint8 m_Reserved:4;
#endif
}AmcLinkDesignator;

//Table 3-19 AMC Link Descriptor
typedef struct _AmcLinkDesc
{
	//39:34 Reserved. Must be 111111b.
	//33:32 AMC Asymmetric Match. 
	//31:24 Link Grouping ID.
	//23:20 AMC Link Type Extension. 
	//19:12	AMC Link Type.
	//11:0  AMC Link Designator. 
	uint8 m_Bytes[3];
	uint8 m_GroupId;
	uint8 m_AsyMatch;
}AmcLinkDesc;

//Table 3-16 AdvancedMC Point-to-Point Connectivity record
typedef struct _RecordType
{
#ifdef BIG_EDIAN
	uint8   m_ResourceType:1;//[7] Record Type ?1 AMC Module, 0 On-Carrier device				
    uint8   m_Reserved:3; 	//[6:4] Reserved; write as Oh
    uint8   m_ResourceId:4;	//[3:0] Connected-device ID if Record Type = 0, Reserved, otherwise
#else
    uint8   m_ResourceId:4;	//[3:0] Connected-device ID if Record Type = 0, Reserved, otherwise
    uint8   m_Reserved:3; 	//[6:4] Reserved; write as Oh
	uint8   m_ResourceType:1;//[7] Record Type ?1 AMC Module, 0 On-Carrier device				
#endif
}RecordType;


//Table 3-15 Point-to-Point Port Descriptor
typedef struct _tagP2pPortDesc
{
#ifdef BIG_EDIAN
    uint8 		m_Bytes[2];	    //4:0   m_RemotePort:5;
    							//9:5   m_LocalPort:5; 
								//15:10   m_Reserved1:6;	
    RecordType  m_RemoteResourceId;
#else
    RecordType  m_RemoteResourceId;
    uint8 		m_Bytes[2];	    //4:0   m_RemotePort:5;
    							//9:5   m_LocalPort:5; 
								//15:10   m_Reserved1:6;	
#endif
}P2pPortDesc;

//Table 3-14 Point-to-Point AdvancedMC Resource Descriptor
typedef struct _tagP2pAmcResDesc
{
    RecordType  m_ResourceId;

    uint8 		m_P2pCount;
    P2pPortDesc m_P2pPortDesc[1];
}P2pAmcResDesc;

//Table 3-13 Carrier Point-to-Point Connectivity record
typedef struct _CarrierP2pConRecord
{
	RecordHeader 	m_header;	
	P2pAmcResDesc 	m_pP2pAmcResDesc;
}CarrierP2pConRecord;


void FruInfoMmc_SetDefault(FruInfo* pFruInfo);
void FruInfoMcmc_SetDefault(FruInfo* pFruInfo);
void FruInfoEmmc_SetDefault(FruInfo* pFruInfo);
void FruInfoCm_SetDefault(FruInfo* pCmFruDb);

extern uint8 g_CurrentReqDefault[];
extern const uint8 g_CurrentReqDefaultLen;

extern uint8 g_EKeyDefineInfoDefault[];
extern const uint8 g_EKeyDefineInfoDefaultLen;

extern uint8 g_PowerCapabilityDefault[];
extern const uint8 g_PowerCapabilityDefaultLen;

extern uint8 g_FruInfoPartiDefault[];
extern const uint8 g_FruInfoPartiDefaultLen;

extern uint8 g_CarrierInfoDefault[];
extern const uint8 g_CarrierInfoDefaultLen;

extern uint8 g_CarrierIpLinkDefault[];
extern const uint8 g_CarrierIpLinkDefaultLen;

extern uint8 g_CarrierInfoDefault[];
extern const uint8 g_CarrierInfoDefaultLen;

extern uint8 g_CarriePowerPolicyDefault[];
extern const uint8 g_CarriePowerPolicyDefaultLen;

extern uint8 g_CarrierActAndPmDescDefault[];
extern const uint8 g_CarrierActAndPmDescDefaultLen;

extern uint8 g_CarrierP2pConnectDefault[];
extern const uint8 g_CarrierP2pConnectDefaultLen;

#ifdef _cplusplus
}
#endif

#endif 


