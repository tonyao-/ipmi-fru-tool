
#ifndef  __CONFIG_INIO_TESTER__
#define  __CONFIG_INIO_TESTER__

#ifdef _cplusplus
extern "c"{
#endif

#include "Typedef.h"
#include "FruInfo.h"
#include "Sdr.h"
	
typedef enum _SENSOR_ID
{
	  HOTSWAP_SENSOR_ID = 0
	, TEMPERATURE1_SENSOR_ID
	, TEMPERATURE2_SENSOR_ID
    , TEMPERATURE3_SENSOR_ID
    , TEMPERATURE4_SENSOR_ID
    , TEMPERATURE5_SENSOR_ID

    , Voltage33_SENSOR_ID1 = 6
    , Voltage33_SENSOR_ID2
    , Voltage25_SENSOR_ID
    ,Voltage120_SENSOR_ID
      
}SENSOR_ID;

typedef enum _SENSOR_RECORD_ID
{
	  DEV_LOCATOR_RECORD_ID = 0
	, HOTSWAP_RECORD_ID
	, TEMPERATURE1_RECORD_ID
	, TEMPERATURE2_RECORD_ID
	
    , TEMPERATURE3_RECORD_ID

    , TEMPERATURE4_RECORD_ID

    , TEMPERATURE5_RECORD_ID

}SENSOR_RECORD_ID;

void CfgTester_Init(uint8* pBuffer, int nLen, uint8 ipmbAddr, uint8 siteNumber);
void CfgTester_SetDefault(FruInfo* pFruInfo);
uint8* FruInfo_FromatFullSensorrecord(uint8* pFullSensorRecord, uint8* nSdrLen);
uint8* FruInfo_FromatCompactSensorrecord(uint8* pCompactSensorRecord, uint8* nSdrLen);

extern FullSensorRecord	g_Temp1FullSensorRecord;
extern FullSensorRecord	g_Temp2FullSensorRecord;
extern FullSensorRecord	g_Temp3FullSensorRecord;
extern FullSensorRecord	g_Temp4FullSensorRecord;
extern FullSensorRecord	g_Voltage33FullSensorRecord1;
extern FullSensorRecord g_Voltage33FullSensorRecord2;
extern FullSensorRecord g_Voltage25FullSensorRecord;
extern FullSensorRecord g_Voltage120FullSensorRecord;

extern FruInfo m_CfgTesterFruInfo;
extern const SensorItem g_SensorItem[8];
extern const Threshold g_ThresholdItem[8];
extern FullSensorRecord* g_pMchFullSensorRecord[8];

//Pm Sdr////////////////////////////////////////////////////////////////
#define PMSTATUS_SENSOR_ID		2
#define PM_STATE_RECORD_ID		3

extern CompactSensorRecord	g_PmMsCompactSensorRecord;
extern FullSensorRecord	g_VInputFullSensorRecord;
extern const SensorItem g_PmSensorItem[];
extern const Threshold g_PmThresholdItem[];
extern FullSensorRecord*	g_pPmFullSensorRecord[];

//AMC Sdr////////////////////////////////////////////////////////////////
#define AMC_RECORDID_OFFSET		2
#define AMC_SENSOR_ID_OFFSET	0
extern const Threshold		g_AmcThresholdItem[];
extern const SensorItem		g_AmcSensorItem[];
extern FullSensorRecord*	g_pAmcFullSensorRecord[];

#ifdef _cplusplus
}
#endif

#endif 


