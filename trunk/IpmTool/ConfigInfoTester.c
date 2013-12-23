
#include "ConfigInfoTester.h"
#include "Sdr.h"
#include "Common_Define.h"
#include "Command_Define.h"
#include "CheckSum.h"
//#include "Mcmc.h"
#include "Debug.h"


FruInfo m_CfgTesterFruInfo;


FullSensorRecord	g_Temp1FullSensorRecord;
FullSensorRecord	g_Temp2FullSensorRecord;
FullSensorRecord	g_Temp3FullSensorRecord;
FullSensorRecord	g_Temp4FullSensorRecord;
FullSensorRecord	g_Voltage33FullSensorRecord1;
FullSensorRecord    g_Voltage33FullSensorRecord2;
FullSensorRecord    g_Voltage25FullSensorRecord;
FullSensorRecord    g_Voltage120FullSensorRecord;

FullSensorRecord*	g_pMchFullSensorRecord[8] = 
{
	&g_Temp1FullSensorRecord
	,&g_Temp2FullSensorRecord
	,&g_Temp3FullSensorRecord
	,&g_Temp4FullSensorRecord

	,&g_Voltage33FullSensorRecord1
	,&g_Voltage33FullSensorRecord2
	,&g_Voltage25FullSensorRecord
	,&g_Voltage120FullSensorRecord
};

const SensorItem g_SensorItem[8] =
{
	 {ET_THRESHOLD, 0, TEMPERATURE1_SENSOR_ID	, TEMPERATURE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD	, 0, 0, "Temp1"}
	,{ET_THRESHOLD, 0, TEMPERATURE2_SENSOR_ID	, TEMPERATURE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD	, 0, 0, "Temp2"}
	,{ET_THRESHOLD, 0, TEMPERATURE3_SENSOR_ID	, TEMPERATURE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD	, 0, 0, "Temp3"}
	,{ET_THRESHOLD, 0, TEMPERATURE4_SENSOR_ID	, TEMPERATURE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD	, 0, 0, "Temp4"}
        
	,{ET_THRESHOLD, 0, Voltage33_SENSOR_ID1	, VOLTAGE_SENSOR    	, 0x7A,	0x95, 0x7A, 0x95, FULL_SENSOR_RECORD	, 0, 0, "3.3Voltage1"}
	,{ET_THRESHOLD, 0, Voltage33_SENSOR_ID2	, VOLTAGE_SENSOR    	, 0x7A, 0x95, 0x7A, 0x95, FULL_SENSOR_RECORD	, 0, 0, "3.3Voltage2"}
	,{ET_THRESHOLD, 0, Voltage25_SENSOR_ID	    , VOLTAGE_SENSOR    	, 0x7A, 0x95, 0x7A, 0x95, FULL_SENSOR_RECORD	, 0, 0, "2.5Voltage"}
	,{ET_THRESHOLD, 0, Voltage120_SENSOR_ID	, VOLTAGE_SENSOR    	, 0x7A,	0x95, 0x7A, 0x95, FULL_SENSOR_RECORD	, 0, 0, "12Voltage"}

};

const Threshold g_ThresholdItem[8] =
{
	 {80, 75, 70}		        // for Temp Sensor
	,{80, 75, 70}		        // for Temp Sensor
	,{80, 75, 70}		        // for Temp Sensor
	,{80, 75, 70}		        // for Temp Sensor
	,{63, 53, 43,23, 13,3}		// for 3.3 Voltage Sensor		
	,{63, 53, 43,23, 13,3}		// for 3.3 Voltage Sensor		
	,{40, 35, 30,20, 15,10}		// for 2.5 Voltage Sensor			
	,{210, 180, 150,90, 60,30}		// for 12 Voltage Sensor	
};

uint8* FruInfo_FromatFullSensorrecord(uint8* pFullSensorRecord, uint8* nSdrLen)
{
	#define nHeaderLen  10
	#define FULL_SDR_SIZE  100
	static uint8 g_FullSensorRecord[FULL_SDR_SIZE] =
	{
		 FULL_SENSOR_RECORD, 0x00, 0x00 ,0x00, 0x00
		,0x00, 0x31, 0x5A	
		,0x27, 0x00	
	};

	int len = ((SdrHeader*)pFullSensorRecord)->m_Len + SDR_HEADER_LEN;
	g_FullSensorRecord[2] = (uint8)(nHeaderLen + len);
	memcpy(&g_FullSensorRecord[nHeaderLen], pFullSensorRecord, len);

	len += nHeaderLen;

	* nSdrLen = len;

	return g_FullSensorRecord;
}

uint8* FruInfo_FromatCompactSensorrecord(uint8* pCompactSensorRecord, uint8* nSdrLen)
{
	#define nHeaderLen  10
	#define FULL_SDR_SIZE  100
	static uint8 g_CompactSensorRecord[FULL_SDR_SIZE] =
	{
		 COMPACT_SENSOR_RECORD, 0x00, 0x00 ,0x00, 0x00
		,0x00, 0x31, 0x5A	
		,0x27, 0x00	
	};

	int len = ((SdrHeader*)pCompactSensorRecord)->m_Len + SDR_HEADER_LEN;
	pCompactSensorRecord[2] = (uint8)(nHeaderLen + len);
	memcpy(&g_CompactSensorRecord[nHeaderLen], pCompactSensorRecord, len);

	len += nHeaderLen;

	* nSdrLen = len;

	return g_CompactSensorRecord;
}

//Pm SDR//////////////////////////////////

static uint8 g_PmMsSdrDefault[FULL_SDR_SIZE] =
{
		COMPACT_SENSOR_RECORD, 0x00, 0x00 ,0x00, 0x00
	,0x00, 0x31, 0x5A	
	,0x27, 0x00	
	,0xFA	
};

static uint8 g_VInputSdrDefault[FULL_SDR_SIZE] =
{
		FULL_SENSOR_RECORD, 0x00, 0x00 ,0x00, 0x00
	,0x00, 0x31, 0x5A	
	,0x27, 0x00	
	,0xFA	
};

CompactSensorRecord	g_PmMsCompactSensorRecord;
FullSensorRecord	g_VInputFullSensorRecord;

FullSensorRecord*	g_pPmFullSensorRecord[] = 
{
	&g_VInputFullSensorRecord
	,&g_Temp1FullSensorRecord
	,&g_Temp2FullSensorRecord
	,&g_Temp3FullSensorRecord
	,&g_Temp4FullSensorRecord
};
const SensorItem g_PmSensorItem[] =
{
	 {ET_SPECIFIC,  0, PMSTATUS_SENSOR_ID		, POWER_SUPPLY_SENSOR	, 0x00, 0x03, 0x00, 0x03, COMPACT_SENSOR_RECORD	, 0, 0, "PmMS"}
	,{ET_THRESHOLD, 0, PMSTATUS_SENSOR_ID + 1	, VOLTAGE_SENSOR		, 0x7A, 0x95, 0x7A, 0x95, FULL_SENSOR_RECORD	, 0, 0, "VInput"}
	,{ET_THRESHOLD, 0, PMSTATUS_SENSOR_ID + 2	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp1"}
	,{ET_THRESHOLD, 0, PMSTATUS_SENSOR_ID + 3	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp2"}
	,{ET_THRESHOLD, 0, PMSTATUS_SENSOR_ID + 4	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp3"}
	,{ET_THRESHOLD, 0, PMSTATUS_SENSOR_ID + 5	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp4"}
};

const Threshold g_PmThresholdItem[] =
{
	 {54, 52, 50, 46, 44, 42}	// for Input-V Sensor
	,{80, 75, 70}		// for Temp Sensor
	,{80, 75, 70}		// for Temp Sensor
	,{80, 75, 70}		// for Temp Sensor
	,{80, 75, 70}		// for Temp Sensor
};

//Amc SDR//////////////////////////////////

FullSensorRecord	g_V33FullSensorRecord;
FullSensorRecord	g_V12FullSensorRecord;

const Threshold g_AmcThresholdItem[] =
{
	 {80, 75, 70, 0, 0, 0}		// for Temp Sensor
	,{80, 75, 70, 0, 0, 0}		// for Temp Sensor
	,{39, 37, 35, 30, 28, 26}		// for 3.3v Sensor
	,{18, 16, 14, 10, 8, 6}		// for 3.3v Sensor
};

 FullSensorRecord*	g_pAmcFullSensorRecord[] = 
 {
 	&g_Temp1FullSensorRecord
	,&g_Temp2FullSensorRecord
	,&g_V33FullSensorRecord
	,&g_V12FullSensorRecord
};
	
const SensorItem g_AmcSensorItem[] =
{
	 {ET_THRESHOLD, 0, AMC_SENSOR_ID_OFFSET+1	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp1"}
	,{ET_THRESHOLD, 0, AMC_SENSOR_ID_OFFSET+2	, TEMPERATURE_SENSOR, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "Temp2"}
	,{ET_THRESHOLD, 0, AMC_SENSOR_ID_OFFSET+3	, VOLTAGE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "3.3v"}
	,{ET_THRESHOLD, 0, AMC_SENSOR_ID_OFFSET+4	, VOLTAGE_SENSOR	, 0x0A, 0x80, 0x0A, 0x80, FULL_SENSOR_RECORD, 0, 0, "12v"}
};
