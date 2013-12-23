
#include "SensorEventType.h"

char* g_pThreadholdString[MAX_THRESHOLD_COUNT] = 
{
	 "LOWER_NO_CRITICAL_L"
	,"LOWER_NO_CRITICAL_H"
	,"LOWER_CRITICAL_L"
	,"LOWER_CRITICAL_H"
	,"LOWER_NON_RECOVERABLE_L"
	,"LOWER_NON_RECOVERABLE_H"
	
	,"UPPER_NO_CRITICAL_L"
	,"UPPER_NO_CRITICAL_H"
	,"UPPER_CRITICAL_L"
	,"UPPER_CRITICAL_H"
	,"UPPER_NON_RECOVERABLE_L"
	,"UPPER_NON_RECOVERABLE_H"
	
};

char* g_pComparisonString[7] = 
{
	 "AT_NORMAL"
	,"AT_BELOW_LNC"
	,"AT_BELOW_LC"
	,"AT_BELOW_LNR"
	,"AT_ABOVE_UNC"
	,"AT_ABOVE_UC"
	,"AT_ABOVE_UNR"
};

char* SensorEventType_GetString(uint16 reading)
{
	int i = 0;
	//int temp = 1;
	uint8 value = (uint8)(reading & 0x003F);

	if(value == 0) return g_pComparisonString[0];
	
	for(i = 0; i < 7; i++)
	{
		if(value == (1<<i))
		{
			return g_pComparisonString[i+1];
		}
	}
	return "Null";	
}


