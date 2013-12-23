#ifndef  _Fru_H_
#define  _Fru_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "Typedef.h"

#define MOULD_8				8
#define FRU_PAGE_SIZE 32
#define MAX_RECORD_COUNT 10

//Fru Record Type Identification
typedef enum _FRU_RECORD_TYPE
{
	  FRT_POWER_SUPPLY_INFO		= 0x00
	, FRT_DC_OUTPUT				= 0x01
	, FRT_DC_LOAD				= 0x02
	, FRT_MANAGEMENT_ACCESS		= 0x03
	, FRT_BASE_COMPATIBILITY	= 0x04
	, FRT_EXTENDED_COMPATIBILITY= 0x05
	//0x06-0xBF	Reserved
	//0xC0-0xFF	OEM Record Types
	, FRT_OEM					= 0xC0
	, COMPACT_SENSOR_RECORD 	= 0xC1
	, FULL_SENSOR_RECORD 		= 0xC2
	, FRUCTRL_RECORD 			= 0xC3
}FRU_RECORD_TYPE;

typedef struct _tagRecordItem
{
	uint8* 	m_pRecord;
	uint8	m_Len;
}RecordItem;

typedef struct _tagTypeLen
{
	#if BIG_EDIAN
	uint8 m_Type:2;	//type code, 
						//00 - binary, 
						//01 - BCD, 
						//10 - 6-bit ASCII, 
						//11 - Interpretation depends on Language Codes.
	uint8 m_Len:6;	//number of data bytes.
						//000000 indicates that the field is empty
						//000001 indicates ��end of fields��
	#else
	uint8 m_Len:6;	//number of data bytes.
	uint8 m_Type:2;	//type code, 
	#endif
}TypeLen;
typedef union _unTypeLen
{	
	TypeLen m_TypeLen;	//C1h indicates ��end of fields��
	uint8 	m_BState;
}unTypeLen;


typedef struct _tagInnerUseArea
{
	#if BIG_EDIAN
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	#else
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	#endif

	uint8 m_Length;	//Board Area Length (in multiples of 8 bytes)
	uint8 m_Buff[6];
}InnerUseArea;


typedef struct _tagChassisInfoArea
{
	#if BIG_EDIAN
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	#else
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	#endif

	uint8 m_Length;	//Board Area Length (in multiples of 8 bytes)
	uint8 m_Buff[1];
}ChassisInfoArea;

typedef struct _tagBoardAreaInfo
{
	#if BIG_EDIAN
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	#else
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	#endif

	uint8 m_Length;	//Board Area Length (in multiples of 8 bytes)
	uint8 m_LanguageCode;
	
	uint8 m_DateTime[3];
	
	unTypeLen m_ManufacturerLen;
	uint8 m_pManufacturer[4];
	
	unTypeLen m_BoardNameLen[12];
	uint8 m_pBoardName;	
}BoardAreaInfo;

typedef struct _tagProductInfo
{
	#if BIG_EDIAN
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	#else
	uint8 m_FormatVersion:4;//Bit[0:3], 1h for this specification.
	uint8 m_Reserved:4;		//Bit[4:7], reserved, write as 0000b
	#endif

	uint8 m_Length;	//Board Area Length (in multiples of 8 bytes)
	uint8 m_Buff[14];
}ProductInfo;

#define RECORD_HEAD_LEN 5
typedef struct _tagRecordHeader
{
	uint8 m_RecordId;
	
	#ifdef BIG_EDIAN
	uint8 m_EndOfList: 1;	//Bit[7]
	uint8 m_Reserved: 3;	//Bit[4:6]
	uint8 m_Version	: 4;	//Bit[0:3]	Record Format version (=2h unless otherwise specified)
	#else
	uint8 m_Version	: 4;	//Bit[0:3]	Record Format version (=2h unless otherwise specified)
	uint8 m_Reserved: 3;	//Bit[4:6], 	write as 000b
	uint8 m_EndOfList: 1;	//Bit[7], 	
	#endif

	uint8 m_RecordLength;
	uint8 m_RecordChecksum;
	uint8 m_HeaderChecksum;
	
	uint8 m_ManufacturerID[3];
	uint8 m_PicmgRecordID;
	uint8 m_RecordFormatVersion;
}RecordHeader;

#define DATA_BASE_HEAD_SIZE 8
typedef struct _tagFruHeader
{
	uint8 m_CommonHead;
	uint8 m_InternalUseArea;
	uint8 m_ChassisInfoArea;
	uint8 m_BoardInfoArea;
	uint8 m_ProductInfoArea;
	uint8 m_MultiRecordArea;
	uint8 m_Pad;
	uint8 m_Checksum;
}FruHeader;

//----------------------------------------------------------------

struct _tagFruInfo;
typedef void (*SetDefaultFun)(struct _tagFruInfo* pFruInfo);
typedef int (*FilterFun)(struct _tagFruInfo* pFruInfo, const RecordHeader* pRecord);
typedef int (*ReleaseFun)(struct _tagFruInfo* pFruInfo);
typedef int (*FruAccessFun)(struct _tagFruInfo* pFruInfo, uint32 offset, uint8* pBuff, uint16 len);

//Fru state
typedef enum _FruInfoState
{
	FRU_INIT = 0
	, FRU_READING
	, FRU_READ_SUCCESS
	, FRU_READ_FAILED
	, FRU_VERIFY_SUCCESS
	, FRU_VERIFY_FAILED
	//Max is 7
}FruInfoState;


typedef struct _tagFruInfo
{	
	uint8	m_ErrCounter:1;		//error count to Access fru
	
	uint8	m_reserved:1;
	uint8	m_accessMode:1;		//0b = Device is accessed by bytes, 1b = Device is accessed by words
	uint8	m_isInnerUseAreaError:1;
	uint8	m_isUnableAccFruDevice:1;
	uint8 	m_State:3;	//Reference FruInfoState
	uint8	m_isUsed:1;	//0-unUsed, 1-used

	FruHeader*		m_pFruHeader;
	InnerUseArea*	m_pInnerUseArea;
	ChassisInfoArea* m_pChassisInfoArea;
	BoardAreaInfo*	m_pBoardAreaInfo;
	ProductInfo*	m_pProductInfoArea;
	
	RecordHeader** 	m_pRecordArray;
	uint8 			m_MaxRecordCount;
	
	uint16 m_FruAreaSize;
	uint16 m_UsedBufLen;	//Fru buffer length
	uint8* m_Buffer;		//Point to FRU buffer

	FruAccessFun		Read;
	FruAccessFun		Write;
	FilterFun		Filter;
}FruInfo;

#define PFRUDB(x) ((FruInfo*)x)

void FruInfo_Init(FruInfo* pFruInfo
	, uint8* pFruInfoBuff
	, uint16 len
	, RecordHeader* pRecordArray[]
	, uint8 recordCount
	, FruAccessFun read
	, FruAccessFun write
	);
void FruInfo_Release(FruInfo* pFruInfo);
void FruInfo_Reset(FruInfo* pFruD);
void FruInfo_SetData(FruInfo* pFruInfo, uint32 offset, const uint8 *pData, uint16 len);
void FruInfo_SetState(FruInfo* pFruInfo, FruInfoState state);
Bool FruInfo_isOk(FruInfo* pFruInfo);
Bool FruInfo_Read(FruInfo* pFruInfo);
Bool FruInfo_Write(FruInfo* pFruInfo);
Bool FruInfo_Parse(FruInfo* pFruInfo);
Bool FruInfo_ReadOffset(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len);
Bool FruInfo_WriteOffset(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len);
void FruInfo_SetDefault(FruInfo* pFruInfo, const RecordItem* recordItemArray, uint8 count);
Bool FruInfo_PagedAccess(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len, FruAccessFun Access);
RecordHeader* FruInfo_FindEx(FruInfo* pFruInfo,uint8 recordTypeId, uint8 PicmgID, uint8* startIndex);
RecordHeader* FruInfo_Find(FruInfo* pFruInfo,uint8 recordTypeId, uint8 PicmgID);
void FruInfo_FormatHeadArea(uint8* pData, uint8 nLen);
void FruInfo_FormatRecord(RecordHeader* pHeader, uint8 nLen);


extern uint8 g_FruInforDefault[];
extern uint8 g_FruInforDefaultLen;
extern uint8 g_BoardAreaInfoDefault[];
extern uint8 g_BoardAreaInfoDefaultLen;
extern uint8 g_ProductAreaInfoDefault[];
extern uint8 g_ProductAreaInfoDefaultLen;

#if XDEBUG
const int8* CmFruInfo_GetRecordName(uint8 recordId);
#else
#define CmFruInfo_GetRecordName(...)
#endif

#ifdef _cplusplus
}
#endif

#endif 


