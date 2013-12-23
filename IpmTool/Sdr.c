/*************************************************/

#include "Sdr.h"
#include "Command_Define.h"
#include "Debug.h"

void DevLocatorRecord_Init(DevLocatorRecord* pDevLocatorRecord, uint16 recordId, uint8 ipmbAddress, uint8 siteNumber, const int8*  pName)
{
	int strLen = strlen(pName);
	
	SdrHeader* pSdrHeader = (SdrHeader*)pDevLocatorRecord;

	memset(pDevLocatorRecord, 0, sizeof(DevLocatorRecord));

	
	Assert(strLen < sizeof(pDevLocatorRecord->m_IdString));
	
	pSdrHeader->m_Id     	= recordId;
	pSdrHeader->m_Version 	= SDR_VERSION;	//V1.5
	pSdrHeader->m_Type   	= SDR_CTLDEV_LOCATOR;
	
	//Key byte
	pDevLocatorRecord->m_SlaveAddr = ipmbAddress;
	pDevLocatorRecord->m_ChannelNum = 0x00;	//Primary IPMB
	
	//RECORD BODY BYTES 
	pDevLocatorRecord->m_EventGenerator = 0;	// 0 = Enable event message generation from controller
	
	pDevLocatorRecord->m_IpmbEventGenerator = 1;		//Event_Gen | FRU_Invectory | Sensor_Dev.
	pDevLocatorRecord->m_FruInventoryDevice = 1;		//Event_Gen | FRU_Invectory | Sensor_Dev.
	pDevLocatorRecord->m_SensorDevice = 1;
	
	pDevLocatorRecord->m_EntityId = ENTITYID_MCH;
	pDevLocatorRecord->m_InstanceNum = siteNumber + 0x60;
	pDevLocatorRecord->m_Oem = 0;	
	
	pDevLocatorRecord->m_StrType 		= 3;	//11b =  8-bit ASCII + Latin 1
	pDevLocatorRecord->m_StrLen 		= strLen+1;
	
	memcpy(pDevLocatorRecord->m_IdString, pName, strLen);

	pSdrHeader->m_Len = sizeof(DevLocatorRecord) 
		- SDR_HEADER_LEN 
		- sizeof(pDevLocatorRecord->m_IdString) 
		+ pDevLocatorRecord->m_StrLen + 1;
}

void CompactSensorRecord_Init(CompactSensorRecord* pCompactSensorRecord
	, uint16 recordId
	, const SensorItem* pSensorItem
	, uint8 ipmbAddress
	, uint8 siteNum
	)
{
	SdrHeader* pSdrHeader = (SdrHeader*)pCompactSensorRecord;
	
	memset(pCompactSensorRecord, 0, sizeof(CompactSensorRecord));
	
	pSdrHeader->m_Id 		= recordId;
	pSdrHeader->m_Version 	= SDR_VERSION;
	pSdrHeader->m_Type 		= SDR_COMPACT_SENSOR;
	//Key byte
	pCompactSensorRecord->m_IpmbAddress 		= ipmbAddress;	
	pCompactSensorRecord->m_IdType 				= 0;	//0-ID is I2C Slave Addres.
	pCompactSensorRecord->m_ChannelNum			= 0;
	pCompactSensorRecord->m_FruOwnerLun 		= 0;
	pCompactSensorRecord->m_SensorOwnerLun 		= 0;
	pCompactSensorRecord->m_SensorNumber		= pSensorItem->m_SensorNumber;	

	//RECORD BODY BYTES 
	pCompactSensorRecord->m_EntityId 			= ENTITYID_PM;	
	
	pCompactSensorRecord->m_islogicalEntity 	= 0;
	pCompactSensorRecord->m_InstanceNumber 		= siteNum + 0x60;	//SiteNum + 60h
	
	pCompactSensorRecord->m_Settable	 		= 0;	//0b = Sensor is not settable
	pCompactSensorRecord->m_InitScanning 		= 0;	//0b = disable scanning
	pCompactSensorRecord->m_InitEvent	 		= 1;	//1b = enable events
	pCompactSensorRecord->m_InitHysteresis 		= 0;
	pCompactSensorRecord->m_InitSensorType 		= 1;	//1b = Threshold-based
	pCompactSensorRecord->m_EventEnabled 		= 0;	//?
	pCompactSensorRecord->m_ScanningEnabled 	= 0;	
	
	pCompactSensorRecord->m_IgnoreSensor 		= 1;	//1b = ignore sensor if Entity is not present or disabled.
	pCompactSensorRecord->m_AutoRearm	 		= 1;	//1b = Auto Re-arm Support 
	pCompactSensorRecord->m_Hysteresis			= 0;
	pCompactSensorRecord->m_ThresholdFlag		= 3;	//11b = The threshold value that are "hard-coded" in the sensor.
	pCompactSensorRecord->m_EventMessage 		= 0;
	
	pCompactSensorRecord->m_SensorType 			= pSensorItem->m_SensorType;	
	pCompactSensorRecord->m_EventType			= pSensorItem->m_EventType;	//1b = Threshold-based.
	
	pCompactSensorRecord->m_LowerThresholdMask		= (pSensorItem->m_AssertEventMask_M << 8) | pSensorItem->m_AssertEventMask_L;
	pCompactSensorRecord->m_UpperThresholdMask	= (pSensorItem->m_DeassertEventMask_M << 8) | pSensorItem->m_DeassertEventMask_L;
	pCompactSensorRecord->m_ReadingMask		= 0;
	
	//pCompactSensorRecord->m_Reserved1 	= 3;	//reserved. Write as 11b.
	//pCompactSensorRecord->m_RateUnit 		= 0;	//000b = none 
	//pCompactSensorRecord->m_ModifyUnitFlag= 1;	//01b = Basic Unit / Modifier Unit
	//pCompactSensorRecord->m_Percentage 	= 0;	//0b = no

	pCompactSensorRecord->m_Uint1 			= pSensorItem->m_Unit1;	//0b = no
	pCompactSensorRecord->m_BaseUnit 		= pSensorItem->m_Unit2;	//
	pCompactSensorRecord->m_ModifyUnit 		= pSensorItem->m_Unit3;	//
	
	pCompactSensorRecord->m_SensorDirection = DIR_UNSPECIFIED;
	pCompactSensorRecord->m_IdStringType 	= NUMERIC;
	pCompactSensorRecord->m_ShareCount	 	= 0;	//0
	
	pCompactSensorRecord->m_EntitySharing 	= ST_INCREMENTS;
	pCompactSensorRecord->m_IdStringOffset 	= 1;
	
	pCompactSensorRecord->m_PoHysteresis 	= 0;
	pCompactSensorRecord->m_NeHysteresis 	= 0;
	
	pCompactSensorRecord->m_Oem 			= 0;
	pCompactSensorRecord->m_IdStrType 		= 3;	//11b =  8-bit ASCII + Latin 1
	pCompactSensorRecord->m_IdStrLen 		= strlen((char*)pSensorItem->m_Name)+1;
	
	memcpy(pCompactSensorRecord->m_IdString, pSensorItem->m_Name, pCompactSensorRecord->m_IdStrLen);

	pSdrHeader->m_Len = sizeof(CompactSensorRecord) 
		- SDR_HEADER_LEN 
		- sizeof(pCompactSensorRecord->m_IdString) 
		+ pCompactSensorRecord->m_IdStrLen;
}

void FullSensorRecord_Init(FullSensorRecord* pFullSensorRecord
	, uint16 recordId
	, const SensorItem* pSensorItem
	, const Threshold* pThresholdItem
	, uint8 ipmbAddress
	, uint8 siteNum
	)
{
	SdrHeader* pSdrHeader = (SdrHeader*)pFullSensorRecord;
	
	memset(pFullSensorRecord, 0, sizeof(FullSensorRecord));
	
	pSdrHeader->m_Id 		= recordId;
	pSdrHeader->m_Version 	= SDR_VERSION;
	pSdrHeader->m_Type 		= SDR_FULL_SENSOR;
	//Key byte
	pFullSensorRecord->m_IpmbAddress 		= ipmbAddress;	
	pFullSensorRecord->m_IdType 				= 0;	//0-ID is I2C Slave Addres.
	pFullSensorRecord->m_ChannelNum			= 0;
	//pFullSensorRecord->m_FruOwnerLun 		= 0;
	pFullSensorRecord->m_SensorOwnerLun 		= 0;
	pFullSensorRecord->m_SensorNumber		= pSensorItem->m_SensorNumber;	

	//RECORD BODY BYTES 
	pFullSensorRecord->m_EntityId 			= ENTITYID_PM;	
	
	pFullSensorRecord->m_islogicalEntity 	= 0;
	pFullSensorRecord->m_InstanceNumber 		= siteNum + 0x60;	//SiteNum + 60h
	
	pFullSensorRecord->m_Settable	 		= 0;	//0b = Sensor is not settable
	pFullSensorRecord->m_InitScanning 		= 0;	//0b = disable scanning
	pFullSensorRecord->m_InitEvent	 		= 1;	//1b = enable events
	pFullSensorRecord->m_InitHysteresis 		= 0;
	pFullSensorRecord->m_InitSensorType 		= 1;	//1b = Threshold-based
	pFullSensorRecord->m_EventEnabled 		= 0;	//?
	pFullSensorRecord->m_ScanningEnabled 	= 0;	
	
	pFullSensorRecord->m_IgnoreSensor 		= 1;	//1b = ignore sensor if Entity is not present or disabled.
	pFullSensorRecord->m_AutoRearm	 		= 1;	//1b = Auto Re-arm Support 
	pFullSensorRecord->m_Hysteresis			= 0;
	pFullSensorRecord->m_ThresholdFlag		= 3;	//11b = The threshold value that are "hard-coded" in the sensor.
	pFullSensorRecord->m_EventMessage 		= 0;
	
	pFullSensorRecord->m_SensorType 			= pSensorItem->m_SensorType;	
	pFullSensorRecord->m_EventType			= pSensorItem->m_EventType;	//1b = Threshold-based.
	
	pFullSensorRecord->m_LowerThresholdMask		= (pSensorItem->m_AssertEventMask_M << 8) | pSensorItem->m_AssertEventMask_L;
	pFullSensorRecord->m_UpperThresholdMask	= (pSensorItem->m_DeassertEventMask_M << 8) | pSensorItem->m_DeassertEventMask_L;
	pFullSensorRecord->m_ReadingMask		= 0;
	
	//pFullSensorRecord->m_Reserved1 	= 3;	//reserved. Write as 11b.
	//pFullSensorRecord->m_RateUnit 		= 0;	//000b = none 
	//pFullSensorRecord->m_ModifyUnitFlag= 1;	//01b = Basic Unit / Modifier Unit
	//pFullSensorRecord->m_Percentage 	= 0;	//0b = no

	pFullSensorRecord->m_Uint1 			= pSensorItem->m_Unit1;	//0b = no
	pFullSensorRecord->m_BaseUnit 		= pSensorItem->m_Unit2;	//
	pFullSensorRecord->m_ModifyUnit 		= pSensorItem->m_Unit3;	//
	
	pFullSensorRecord->m_SensorDirection = DIR_UNSPECIFIED;
	
	pFullSensorRecord->m_UpperNonRecoverable= pThresholdItem->m_UpperNonRecoverable;
	pFullSensorRecord->m_UpperCiritcal 		= pThresholdItem->m_UpperCiritcal;
	pFullSensorRecord->m_UpperNonCiritcal 	= pThresholdItem->m_UpperNonCiritcal;
	pFullSensorRecord->m_LowerNonRecoverable= pThresholdItem->m_LowerNonRecoverable;
	pFullSensorRecord->m_LowerCiritcal 		= pThresholdItem->m_LowerCiritcal;
	pFullSensorRecord->m_LowerNonCiritcal 	= pThresholdItem->m_LowerNonCiritcal;
	
	pFullSensorRecord->m_PoHysteresis 	= pThresholdItem->m_PoHysteresis;
	pFullSensorRecord->m_NeHysteresis 	= pThresholdItem->m_NeHysteresis;
	
	pFullSensorRecord->m_Oem 			= 0;
	pFullSensorRecord->m_IdStrType 		= 3;	//11b =  8-bit ASCII + Latin 1
	pFullSensorRecord->m_IdStrLen 		= strlen((char*)pSensorItem->m_Name) + 1;
	
	memcpy(pFullSensorRecord->m_IdString, pSensorItem->m_Name, pFullSensorRecord->m_IdStrLen);

	pSdrHeader->m_Len = sizeof(FullSensorRecord) 
		- SDR_HEADER_LEN 
		- sizeof(pFullSensorRecord->m_IdString) 
		+ pFullSensorRecord->m_IdStrLen;
}

void EventOnlyRecod_Init(EventOnlyRecod* pEventOnlyRecod
	, uint16 recordId
	, const SensorItem* pSensorItem
	, uint8 ipmbAddress
	, uint8 siteNum
	)
{
	SdrHeader* pSdrHeader = (SdrHeader*)pEventOnlyRecod;
	
	memset(pEventOnlyRecod, 0, sizeof(EventOnlyRecod));
	
	pSdrHeader->m_Id 					= recordId;
	pSdrHeader->m_Version 				= SDR_VERSION;
	pSdrHeader->m_Type 					= SDR_EVENT_ONLY;
	//Key byte
	pEventOnlyRecod->m_SensorOwnerId 	= ipmbAddress;	
	pEventOnlyRecod->m_SensorOwnerLun 	= 0;	//0-ID is I2C Slave Addres.
	pEventOnlyRecod->m_SensorNumber		= pSensorItem->m_SensorNumber;	

	//RECORD BODY BYTES 
	pEventOnlyRecod->m_EntityId 		= pSensorItem->m_EntityID;	
	pEventOnlyRecod->m_EntityInstance 	= siteNum + 0x60;	//SiteNum + 60h
	pEventOnlyRecod->m_SensorType 		= pSensorItem->m_SensorType;
	pEventOnlyRecod->m_EventType		= pSensorItem->m_EventType;	//1b = Threshold-based.
	
	pEventOnlyRecod->m_SensorDirection  = DIR_UNSPECIFIED;
	pEventOnlyRecod->m_IdStringType 	= NUMERIC;
	pEventOnlyRecod->m_ShareCount	 	= 0;	//0
	
	pEventOnlyRecod->m_EntitySharing 	= ST_INCREMENTS;
	pEventOnlyRecod->m_IdStringOffset 	= 1;
		
	pEventOnlyRecod->m_Oem 				= 0;
	pEventOnlyRecod->m_IdStrType 		= 3;	//11b =  8-bit ASCII + Latin 1
	pEventOnlyRecod->m_IdStrLen 		= strlen((char*)pSensorItem->m_Name) + 1;
	
	memcpy(pEventOnlyRecod->m_IdString, pSensorItem->m_Name, pEventOnlyRecod->m_IdStrLen);

	pSdrHeader->m_Len = sizeof(EventOnlyRecod) 
		- SDR_HEADER_LEN 
		- sizeof(pEventOnlyRecod->m_IdString) 
		+ pEventOnlyRecod->m_IdStrLen;
}

int McDevLocatorRecord_CovertToFruDevRecord(uint8 deviceId, DevLocatorRecord* pDevLocatorRecord, FruDevRecod* pFruDevRecod)
{
	SdrHeader* pSdrHeader = (SdrHeader*)pDevLocatorRecord;

	pFruDevRecod->m_SdrHeader.m_Id = pDevLocatorRecord->m_SdrHeader.m_Id;
	pFruDevRecod->m_SdrHeader.m_Version = pDevLocatorRecord->m_SdrHeader.m_Version;
	pFruDevRecod->m_SdrHeader.m_Type = SDR_FRUDEV_LOCATOR;
	
	pFruDevRecod->m_IpmbAddress = pDevLocatorRecord->m_SlaveAddr;
	pFruDevRecod->m_DeviceId = deviceId;
	pFruDevRecod->m_LogicFruDev = pDevLocatorRecord->m_FruInventoryDevice;
	pFruDevRecod->m_Lun = 0;
	pFruDevRecod->m_BusId = 0;
	pFruDevRecod->m_ChannelNum = pDevLocatorRecord->m_ChannelNum;
	
	pFruDevRecod->m_DeviceType = FRU_REPO_TYPE;
	pFruDevRecod->m_DeviceTypeEx = 0;
	pFruDevRecod->m_EntityId = pDevLocatorRecord->m_EntityId;
	pFruDevRecod->m_InstanceNum = pDevLocatorRecord->m_InstanceNum;
	pFruDevRecod->m_Oem = 0;
	
	pFruDevRecod->m_StrType = pDevLocatorRecord->m_StrType;
	//pFruDevRecod->m_StrLen = pDevLocatorRecord->m_StrLen;
	memcpy(pFruDevRecod->m_String, pDevLocatorRecord->m_IdString, pDevLocatorRecord->m_StrLen);
	
	TRACE_LEVEL(TRACE_SDR, ("FruDev Record-%s[%d] Created.\n", pFruDevRecod->m_String, pSdrHeader->m_Id));

	return 0;
}

////////////////////////////////////////////////////////////////////////

uint8 Sdr_GetRecord(Sdr* pSdr, uint16 recordId, uint16* pNextRecordId, uint8 offset, uint8* pBuff, uint8 len)
{
	uint8 isFound = False;
	int j = 0;
	SdrHeader*	pSdrHeader = Null;
	uint8 nReadLen = 0;
	uint8 nRecordLen = 0;

	*pNextRecordId = 0;
	
	if(0 == recordId && pSdr->m_pRecordArray[0])
	{
		recordId = pSdr->m_pRecordArray[0]->m_Id;
	}
	
	for(j = 0; j < pSdr->m_RecordArrayCount; j++)
	{
		pSdrHeader = pSdr->m_pRecordArray[j];

		if(Null == pSdrHeader) break;

		if(!isFound)
		{
			if(pSdrHeader->m_Id == recordId)
			{
				nRecordLen = pSdrHeader->m_Len + SDR_HEADER_LEN;
				if(offset >= nRecordLen)
				{
					return 0;
				}
				if(len == 0xFF)
				{
					len = nRecordLen;
				}
				if(offset + len > nRecordLen)
				{
					len = nRecordLen - offset;
				}
				nReadLen = len;
				memcpy(pBuff, &((uint8*)pSdrHeader)[offset], nReadLen);
				isFound = True;
			}
		}
		else
		{
			*pNextRecordId = pSdrHeader->m_Id;
			break;
		}
	}

	return nReadLen;
}

uint16 Sdr_GetSdrCount(Sdr* pSdr, uint8 isSensorCount)
{
	int j = 0;
	SdrHeader*	pSdrHeader = Null;
	int sensorCount = 0;
	int sdrCount = 0;
	
	for(j = 0; j < pSdr->m_RecordArrayCount; j++)
	{
		pSdrHeader = pSdr->m_pRecordArray[j];
		if(Null == pSdrHeader) break;

		sdrCount++;
		
		if(pSdrHeader->m_Type == SDR_FULL_SENSOR
			|| pSdrHeader->m_Type == SDR_COMPACT_SENSOR
			|| pSdrHeader->m_Type == SDR_EVENT_ONLY
			)
		{
			sensorCount++;
		}
	}
	
	if(isSensorCount)
	{
		return sensorCount;
	}
	else
	{
		return sdrCount;
	}
}

void Sdr_GetRepositoryInfo(Sdr* pSdr, uint8* pBuff, uint8 len)
{
	uint16 sdrCount = Sdr_GetSdrCount(pSdr, False);
	
	Assert(len >= 13);
	memset(pBuff, 0, len);
	
	pBuff[0] = pSdr->m_Version;
	pBuff[1] = (uint8)sdrCount;
	pBuff[2] = (uint8)(sdrCount >> 8);
	
	TRACE_LEVEL(TRACE_SDR, ("%s():\n", _FUNC_));
	DUMP_BYTE_LEVEL(TRACE_SDR, pBuff, len);
}

static uint8 Sdr_GetSensorId(SdrHeader* pSdrHeader, uint8* pShareCount)
{
	uint8 sensorId = 0;
	
	if(pSdrHeader->m_Type == SDR_COMPACT_SENSOR)
	{
		sensorId = ((CompactSensorRecord*)pSdrHeader)->m_SensorNumber;
		*pShareCount = ((CompactSensorRecord*)pSdrHeader)->m_ShareCount;
	}
	else if(pSdrHeader->m_Type == SDR_EVENT_ONLY)
	{
		sensorId = ((EventOnlyRecod*)pSdrHeader)->m_SensorNumber;
		*pShareCount = ((EventOnlyRecod*)pSdrHeader)->m_ShareCount;
	}
	else
	{
		sensorId = INVALID_SENSORID;
	}

	return sensorId;
}

static Bool Sdr_IsExistSensorId(Sdr* pSdr, uint8 sensorId, uint8 index)
{
	int i = 0;
	uint8 id = 0;
	uint8 shareCount = 0;
	
	for(i = index; i< pSdr->m_RecordArrayCount; i++)
	{		
		if(Null == pSdr->m_pRecordArray[i]) break;

		id = Sdr_GetSensorId(pSdr->m_pRecordArray[i], &shareCount);
		if(INVALID_SENSORID != id)
		{
			if(sensorId >= id && id <= sensorId + shareCount)
			{
				TRACE_LEVEL(TRACE_SDR, ("Verify False, same sensorId[%d] is in records!\n", id));
				return False;
			}
		}
	}
	return True;
}

static Bool Sdr_CheckSensorId(const Sdr* pSdr)
{
	int j = 0;
	int k = 0;
	uint16 id = 0;
	uint8 shareCount = 0;
	//Check same sensorId in all sensor records
	
	for(j = 0; j < pSdr->m_RecordArrayCount; j++)
	{
		
		if(Null == pSdr->m_pRecordArray[j]) break;
		
		id = Sdr_GetSensorId(pSdr->m_pRecordArray[j], &shareCount);

		if(INVALID_SENSORID != id)
		{
			for(k = id; k <= id + shareCount; k++)
			{
				if(False == Sdr_IsExistSensorId(pSdr, k, j + 1))
				{
					TRACE_LEVEL(TRACE_SDR, ("Verify False, same sensorId[%d] is in records!\n", id));
					return False;
				}
			}
		}
	}
	return True;
}

static Bool Sdr_CheckRecordId(const Sdr* pSdr)
{
	int i = 0;
	int j = 0;
	uint16 Id = 0;
	
	//Check same recordId in all sdr records
	for(j = 0; j < pSdr->m_RecordArrayCount; j++)
	{
		if(Null == pSdr->m_pRecordArray[j]) break;
		
		Id = pSdr->m_pRecordArray[j]->m_Id;

		//Trace("pSdr->m_pRecordArray[%d].id=%d\n", j, Id);
		for(i = j+1; i< pSdr->m_RecordArrayCount; i++)
		{
			if(Null == pSdr->m_pRecordArray[i]) break;

			//Trace("\tpSdr->m_pRecordArray[%d].id=%d\n", i, pSdr->m_pRecordArray[i]->m_Id);
			
			if(pSdr->m_pRecordArray[i]->m_Id == Id)
			{
				TRACE_LEVEL(TRACE_WARNING, ("Verify False, same recordId[%d] is in records!\n", Id));
				return False;
			}
		}
	}
	return True;
}

Bool Sdr_Verify(Sdr* pSdr)
{
	if(pSdr->m_UsedSpace <= pSdr->m_MaxSpace)
	{
		pSdr->m_isValid = Sdr_CheckRecordId(pSdr) && Sdr_CheckSensorId(pSdr);
	}
	else
	{
		pSdr->m_isValid = False;
	}
	return pSdr->m_isValid;
}

void Sdr_RecordDump(SdrHeader* pSdrHeader)
{
	if(SDR_CTLDEV_LOCATOR == pSdrHeader->m_Type)
	{
		DevLocatorRecord* pRecord = (DevLocatorRecord*)(pSdrHeader);
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, 
			("SDR[%d]: Type=Management controller Device Locator Record[0x%02x]: IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type, pRecord->m_IdString));
	}
	else if(SDR_FRUDEV_LOCATOR == pSdrHeader->m_Type)
	{
		DevLocatorRecord* pRecord = (DevLocatorRecord*)(pSdrHeader);
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, 
			("SDR[%d]: Type=FRU Device Locator Record[0x%02x]: IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type, pRecord->m_IdString));
	}
	else if(SDR_FULL_SENSOR == pSdrHeader->m_Type)
	{
		FullSensorRecord* pRecord = (FullSensorRecord*)(pSdrHeader);
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, 
			("SDR[%d]: Type=Full Sensor Record[0x%02x]: IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type,  pRecord->m_IdString));
	}
	else if(SDR_COMPACT_SENSOR == pSdrHeader->m_Type)
	{
		CompactSensorRecord* pRecord = (CompactSensorRecord*)(pSdrHeader);
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, ("SDR[%d]: Type=Compact Sensor Record[0x%02x]: IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type,  pRecord->m_IdString));
	}
	else if(SDR_EVENT_ONLY == pSdrHeader->m_Type)
	{
		EventOnlyRecod* pRecord = (EventOnlyRecod*)(pSdrHeader);
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, ("SDR[%d]: Type=Event Only Sensor Record[0x%02x]: IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type,  pRecord->m_IdString));
	}
	#if 0
	else if(SDR_FRUDEV_LOCATOR == pSdrHeader->m_Type)
	{
		DevLocatorRecord* pRecord = (DevLocatorRecord*)(pSdrHeader);
		Trace("SDR[%d]: Type=SDR_FRUDEV_LOCATOR[0x%02x]:  IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type,  pRecord->m_IdString);
	}
	else if(SDR_CTLDEV_LOCATOR == pSdrHeader->m_Type)
	{
		DevLocatorRecord* pRecord = (DevLocatorRecord*)(pSdrHeader);
		Trace("SDR[%d]: Type=Management controller Device Locator Record[0x%02x]:  IdString=%s\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type,  pRecord->m_IdString);
	}
	#endif
	else
	{
		TRACE_LEVEL(TRACE_SDR | TRACE_MAIN, ("SDR[%d]: Type=[0x%02x]\n"
			, pSdrHeader->m_Id, pSdrHeader->m_Type));
	}
}

void Sdr_Dump(Sdr* pSdr)
{
	int i = 0; 
	for( i = 0; i < pSdr->m_RecordArrayCount; i++)
	{
		if(pSdr->m_pRecordArray[i])
		{
			Sdr_RecordDump(pSdr->m_pRecordArray[i]);
		}
	}
}

SdrHeader* Sdr_Find(Sdr* pSdr, uint16 recordId)
{
	int i = 0; 
	for( i = 0; i < pSdr->m_RecordArrayCount; i++)
	{
		if(pSdr->m_pRecordArray[i])
		{
			if(pSdr->m_pRecordArray[i]->m_Id == recordId) 
			{
				return pSdr->m_pRecordArray[i];
			}
		}
	}
	
	return Null;
}

Bool Sdr_Add(Sdr* pSdr, SdrHeader* pSdrHeader)
{
	int i = 0; 
	for( i = 0; i < pSdr->m_RecordArrayCount; i++)
	{
		if(pSdr->m_pRecordArray[i] == Null)
		{
			pSdr->m_pRecordArray[i] = pSdrHeader;
			pSdr->m_RecordCount++;
			//Sdr_RecordDump(pSdrHeader);
			return True;
		}
	}
	return False;
}

SdrHeader* Sdr_NewRecord(Sdr* pSdr, SdrHeader* pSdrHeader)
{
	int i = 0;
	SdrHeader* pHeader = Null;
	uint16 freeSpace = pSdr->m_MaxSpace - pSdr->m_UsedSpace;
	
	if(freeSpace < pSdrHeader->m_Len + SDR_HEADER_LEN)
	{
		TRACE_LEVEL(TRACE_WARNING,("WARNING: %s(%d) Free Space(=%d) is not enough\n!", _FUNC_, pSdrHeader->m_Len + SDR_HEADER_LEN, freeSpace));
		return Null;
	}
	if(pSdrHeader->m_Len == 0 || pSdrHeader->m_Version != SDR_VERSION)
	{
		return Null;
	}

	i = 0;
	pHeader = (SdrHeader*)&pSdr->m_pBuff[i];
	while(pHeader->m_Len)
	{
		i += pHeader->m_Len + SDR_HEADER_LEN;
		pHeader = (SdrHeader*)&pSdr->m_pBuff[i];
	}
	
	memcpy(pHeader, pSdrHeader, sizeof(SdrHeader));
	pSdr->m_UsedSpace += pSdrHeader->m_Len + SDR_HEADER_LEN;
	Sdr_Add(pSdr, pHeader);
	
	TRACE_LEVEL(TRACE_SDR,("%s(%d,%d,%d),recordLen=%d, maxSpace=%d, FreeSpace=%d\n"
		, _FUNC_
		, pSdrHeader->m_Id, pSdrHeader->m_Type, pSdrHeader->m_Len
		, pSdrHeader->m_Len + SDR_HEADER_LEN
		, pSdr->m_MaxSpace
		, pSdr->m_MaxSpace - pSdr->m_UsedSpace
		));

	return pHeader;
}
void Sdr_InitEx(Sdr* pSdr, SdrHeader* pSdrHeader[], uint8 count, uint8* pBuff, uint16 bufLen)
{
	Sdr_Init(pSdr, pSdrHeader, count);

	pSdr->m_MaxSpace = bufLen;
	pSdr->m_pBuff = pBuff;
	memset(pBuff, 0, bufLen);
}

void Sdr_Reset(Sdr* pSdr)
{
	Sdr_InitEx(pSdr, pSdr->m_pRecordArray, pSdr->m_RecordArrayCount, pSdr->m_pBuff, pSdr->m_MaxSpace);
}

void Sdr_Release(Sdr* pSdr)
{
	memset(pSdr->m_pRecordArray, 0, pSdr->m_RecordArrayCount * 4);
	memset(pSdr, 0, sizeof(Sdr));
}

void Sdr_Init(Sdr* pSdr, SdrHeader* pSdrHeader[], uint8 count)
{
	memset(pSdr, 0, sizeof(Sdr));
	
	pSdr->m_Version = SDR_VERSION;
	pSdr->m_pRecordArray = pSdrHeader;
	pSdr->m_RecordArrayCount = count;
	pSdr->m_ReservationId = DEFAULT_RESERIVE_SDR_ID;
	
	memset(pSdr->m_pRecordArray, 0, pSdr->m_RecordArrayCount * 4);
}


void Sdr_VerifyReset(Sdr* pSdr)
{
	int i = 0;
	
	AssertTrue(pSdr->m_MaxSpace > 0);
	AssertTrue(pSdr->m_UsedSpace == 0);
	AssertTrue(pSdr->m_pBuff);
	AssertTrue(pSdr->m_pRecordArray);
	AssertTrue(pSdr->m_RecordArrayCount);
	AssertTrue(pSdr->m_pRecordArray[0] == Null);
	AssertTrue(pSdr->m_pRecordArray[pSdr->m_RecordArrayCount-1] == Null);
}


