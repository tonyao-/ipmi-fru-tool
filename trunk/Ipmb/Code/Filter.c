

#ifdef _cplusplus
extern "c"{
#endif

#include "CheckSum.h"
#include "Filter.h"
#include "Debug.h"

#define PACKET_TAG 	0X7E
#define KEY_BYTE	0X7d
#define CODE_00		0X00
#define CODE_01		0x01

void Filter_Init(Filter* filter
		, FilterTransferFun 	filterOk
		, void*			pObj
		)
{
	memset(filter, 0, sizeof(Filter));
	
	filter->FilterOk = filterOk;
	filter->m_pObj = pObj;
}

unsigned char Filter_Verify(Filter* filter)
{
	if(filter->m_Len <= 2
		|| filter->m_buffer[0] != filter->m_Len - 2
		|| !CheckSum_Check(&filter->m_buffer[0], filter->m_Len))
	{
		return 0;
	}

	return 1;
}

int Filter_UnPacking(Filter* filter, const unsigned char * pSrc, unsigned int srcLen)
{
	int bRet = 1;
	unsigned int i = 0;
	
	for(i = 0; i < srcLen; i++)
	{
		if(*pSrc ==  PACKET_TAG && !filter->m_isFindHead)
		{
			filter->m_isFindHead = 1;
			filter->m_Len = 0;
			filter->m_errCount = 0;
			filter->m_isValid = 0;
		}
		else if(filter->m_isFindHead)
		{
			if(*pSrc == PACKET_TAG)
			{
				filter->m_isFindHead = 0;
				if(Filter_Verify(filter))
				{
					filter->m_isValid = 1;
					if(filter->FilterOk)
					{
						filter->FilterOk(filter->m_pObj, &filter->m_buffer[1], filter->m_Len-2);
					}
				}
				else
				{
					filter->m_isFindHead = 1;
					filter->m_Len = 0;
					filter->m_errCount = 0;
					filter->m_isValid = 0;
				}
			}
			else if(*pSrc == CODE_01 && filter->m_Len >1 && filter->m_buffer[filter->m_Len-1] == KEY_BYTE)
			{
				filter->m_buffer[filter->m_Len-1] = PACKET_TAG;
			}
			else if(*pSrc == CODE_00 && filter->m_Len > 1 && filter->m_buffer[filter->m_Len-1] == KEY_BYTE)
			{
				filter->m_buffer[filter->m_Len-1] = KEY_BYTE;
			}
			else
			{
				filter->m_buffer[filter->m_Len++] = *pSrc;
			}

			if(filter->m_Len >= BUFFER_LEN)
			{
				filter->m_isFindHead = 0;
				filter->m_isValid = 0;
			}
		}
		else
		{
			filter->m_errCount++;
			bRet = 0;
		}
		pSrc++;
	}

	return bRet;
}

int Filter_changeCode(unsigned char* pBuf, unsigned char value)
{
	if(value == PACKET_TAG)
	{
		pBuf[0] = KEY_BYTE;
		pBuf[1] = CODE_01;
		return 2;
	}
	else if(value == KEY_BYTE)
	{
		pBuf[0] = KEY_BYTE;
		pBuf[1] = CODE_00;
		return 2;
	}
	else
	{
		pBuf[0] = value;
		return 1;
	}
}


int Filter_Packing(Filter* filter
	, unsigned char * pDst
	, unsigned char* dstLen
	, const unsigned char * pSrc
	, const unsigned char  srcLen)
{
	int i = 0;
	int index = 0;
	unsigned char checkSum = 0;
	unsigned char buffer[BUFFER_LEN];

	if(srcLen >= BUFFER_LEN) return 0;

	memcpy(&buffer[1], pSrc, srcLen);
	buffer[0] = srcLen;
	checkSum = CheckSum_Get(buffer, srcLen + 1);
	
	pDst[index++] = PACKET_TAG;

	index += Filter_changeCode(&pDst[index], srcLen);	
	for(i = 0; i < srcLen; i++)
	{
		index += Filter_changeCode(&pDst[index], *pSrc);
		pSrc++;
	}	
	index += Filter_changeCode(&pDst[index], checkSum);
	
	pDst[index++] = PACKET_TAG;

	*dstLen = index;

	return  index;
}


#ifdef _cplusplus
}
#endif

