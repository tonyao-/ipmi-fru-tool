#ifndef  _SENSOR_EVENT_TYPE_H_
#define  _SENSOR_EVENT_TYPE_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "Typedef.h"

#define INVALID_EVENT 		0xFFFF


enum
{
	ES_EXCEPTION	//EXCEPTION
	, ES_NORMAL		//NORMAL
};

//Threshold Assertion Event Mask
enum
{
	  GL_LNC_MASK 	= BIT_0
	, GH_LNC_MASK 	= BIT_1
	, GL_LC_MASK 	= BIT_2
	, GH_LC_MASK 	= BIT_3
	, GL_LNR_MASK 	= BIT_4
	, GH_LNR_MASK 	= BIT_5
	
	, GL_UNC_MASK 	= BIT_6
	, GH_UNC_MASK 	= BIT_7
	, GL_UC_MASK 	= BIT_8
	, GH_UC_MASK 	= BIT_9
	, GL_UNR_MASK 	= BIT_10
	, GH_UNR_MASK 	= BIT_11
};

//Present threshold comparison status
enum
{
	  AT_NORMAL 	= BIT_7|BIT_6
	, AT_BELOW_LNC 	= BIT_7|BIT_6|BIT_0
	, AT_BELOW_LC 	= BIT_7|BIT_6|BIT_1
	, AT_BELOW_LNR 	= BIT_7|BIT_6|BIT_2
	, AT_ABOVE_UNC 	= BIT_7|BIT_6|BIT_3
	, AT_ABOVE_UC 	= BIT_7|BIT_6|BIT_4
	, AT_ABOVE_UNR 	= BIT_7|BIT_6|BIT_5
};


#define INVALID_EVENT_STATES 0xff
//RESHOLD BASED STATES 
enum{
	  LOWER_NO_CRITICAL_L	= 0		//Lower Non-critical - going low
	, LOWER_NO_CRITICAL_H 	= 1		//Lower Non-critical - going high
	, LOWER_CRITICAL_L 		= 2		//Lower Critical - going low 
	, LOWER_CRITICAL_H 		= 3		//Lower Critical - going high 
	, LOWER_NON_RECOVERABLE_L 	= 4		//Lower Non-recoverable - going low 
	, LOWER_NON_RECOVERABLE_H 	= 5		//Lower Non-recoverable - going high
	
	, UPPER_NO_CRITICAL_L 	= 6		//Upper Non-critical - going low
	, UPPER_NO_CRITICAL_H 	= 7		//Upper Non-critical - going high
	, UPPER_CRITICAL_L 		= 8		//Upper Critical - going low 
	, UPPER_CRITICAL_H 		= 9		//Upper Critical - going high 
	, UPPER_NON_RECOVERABLE_L 	= 10	//Upper Non-recoverable - going low 
	, UPPER_NON_RECOVERABLE_H 	= 11	//Upper Non-recoverable - going high
};

enum
{
	  DGH_UNR 	= BIT_7 | UPPER_NON_RECOVERABLE_H
	, DGH_UC 	= BIT_7 | UPPER_CRITICAL_H
	, DGH_UNC 	= BIT_7 | UPPER_NO_CRITICAL_H
	, AGH_UNC 	= UPPER_NO_CRITICAL_H
	, AGH_UC 	= UPPER_CRITICAL_H
	, AGH_UNR 	= UPPER_NON_RECOVERABLE_H
	
	, DGL_LNR 	= BIT_7 | LOWER_NON_RECOVERABLE_L
	, DGL_LC 	= BIT_7 | LOWER_CRITICAL_L
	, DGL_LNC 	= BIT_7 | LOWER_NO_CRITICAL_L
	, AGL_LNC 	= LOWER_NO_CRITICAL_L
	, AGL_LC 	= LOWER_CRITICAL_L
	, AGL_LNR 	= LOWER_NON_RECOVERABLE_L
};


//SEVERITY EVENT STATES 
enum
{
	  TO_OK = 0								//transition to OK
	, TO_NON_CRITICAL_FROM_OK = 1			//transition to Non-Critical from OK 
	, TO_CRITICAL_FROM_LESS = 2				//transition to Critical from less severe 
	, TO_NON_RECOVERABLE_FROM_OK = 3		//transition to Non-recoverable from less severe 
	, TO_NON_CRITICAL_FROM_MORE	= 4			//transition to Non-Critical from more severe 
	, TO_CRITICAL_FROM_NON_RECOVERABLE = 5	//transition to Critical from Non-recoverable
	, TO_NON_RECOVERABLE = 6				//transition to Non-recoverable
	, MONITOR = 7							//Monitor
	, INFORMATION = 8						//Informational 
};

//Table 42-1, Event/Reading Type Code Ranges
typedef enum _EventType
{
	ET_UNSPECIFIED 		= 0		//Event/Reading Type unspecified.
	, ET_THRESHOLD 		= 1		//Threshold-based
	, ET_GENERIC_MIN 	= 0x02	//Generic Discrete Min
	, ET_SEVERITY 		= 0x07	//SEVERITY EVENT STATES 
	, ET_GENERIC_MAX 	= 0x0C	//Generic Discrete Max
	, ET_SPECIFIC		= 0x6F	//Sensor-specific Discrete
	, ET_OEM_MIN 		= 0x70	//OEM Discrete Min
	, ET_OEM_MAX 		= 0x7F	//OEM Discrete Max
}EventType;

typedef enum _EventDir
{
	EVENT_ASSERTION = 0
	, EVENT_DEASSERTION = 1
}EventDir;

typedef struct _Threshold
{
	uint8 m_UpperNonRecoverable;
	uint8 m_UpperCiritcal;
	uint8 m_UpperNonCiritcal;
	uint8 m_LowerNonCiritcal;
	uint8 m_LowerCiritcal;
	uint8 m_LowerNonRecoverable;
 	uint8	m_PoHysteresis;		//Positive-going Threshold Hysteresis value
 	uint8	m_NeHysteresis;		//Negative-going Threshold Hysteresis value 
}Threshold;

typedef struct _DiscreteState
{
	uint8 m_Value;
	uint8 m_EventOffset;
}DiscreteState;

#define MAX_THRESHOLD_COUNT 12
extern char* g_pThreadholdString[MAX_THRESHOLD_COUNT];
char* SensorEventType_GetString(uint16 reading);

#ifdef _cplusplus
}
#endif

#endif 
