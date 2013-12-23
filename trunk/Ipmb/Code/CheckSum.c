
#include "CheckSum.h"
unsigned char CheckSum_Get(const unsigned char * pData, int Length) 
{
    unsigned char Arr_Sum = 0x0;
    int iIndex=0;

    if ( 0 == pData) 
    {
        return 0xff;
    }

    for(iIndex = 0; iIndex < Length; iIndex++)
    {
        Arr_Sum+= pData[iIndex];    
    }

    Arr_Sum = ~Arr_Sum;
    Arr_Sum = Arr_Sum+1;

    return Arr_Sum;
}

unsigned char CheckSum_Check(const unsigned char *pData, int Length)
{
    unsigned char TempArr = 0x0;
    int iIndex = 0;

    for(iIndex = 0; iIndex < Length; iIndex++)
    {
        TempArr = TempArr + pData[iIndex]; 
    }
    if (0 != TempArr)
    {
        return 0;
    }
    return 1;
}

