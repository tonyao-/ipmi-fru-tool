
#include "CheckSum.h"
#include "FruInfo.h"
#include "Debug.h"


////////////////////////////////////////////////////////////////////////////
//0			page1			  32			page2					page3
//[****************************|****************************|***************
//		|-----nBoundaryLen---- |							|
//	  	|-------------------nRemainder--------------------------|
//	  m_nLow												 m_nHigh
////////////////////////////////////////////////////////////////////////////
Bool FruInfo_PagedAccess(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len, FruAccessFun Access)
{
	static uint16 nAccessedLen = 0;
	static uint16 nAccessLen = 0;
	static uint16 nBoundaryLen = 0;
	static uint16 nPages = 0;

	nAccessedLen = 0;
	nAccessLen = 0;
	nBoundaryLen = 0;
	nPages = 0;

	if(len == 0) return True;

	do
	{
		nPages = offset / FRU_PAGE_SIZE;
		nBoundaryLen = (nPages + 1) * FRU_PAGE_SIZE - offset;

		if(len <= nBoundaryLen)
		{
			nAccessLen = len;
		}
		else if(len > nBoundaryLen)
		{
			nAccessLen = nBoundaryLen;
		}

		Assert(Access);
		nAccessedLen = Access(pFruInfo, offset, pData, nAccessLen);
		if(nAccessLen != nAccessedLen)
		{
			return False;
		}

		offset += nAccessLen;
		len -= nAccessLen;
		pData += nAccessLen;
	}while(0 != len);
	
	return True;	
}

Bool FruInfo_ReadOffset(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len)
{
	return FruInfo_PagedAccess(pFruInfo, offset, pData, len, pFruInfo->Read);
}

Bool FruInfo_Read(FruInfo* pFruInfo)
{
	Bool nRet = 0;

	nRet = FruInfo_ReadOffset(pFruInfo, 0, pFruInfo->m_Buffer, pFruInfo->m_FruAreaSize);

	if(nRet)
	{
		pFruInfo->m_State = FRU_READ_SUCCESS;
	}
	else
	{
		pFruInfo->m_State = FRU_READ_FAILED;
		Warning("%s(len=%d) return FAILED.\n", _FUNC_, pFruInfo->m_FruAreaSize);
	}

	return nRet;
}

Bool FruInfo_WriteOffset(FruInfo* pFruInfo, uint32 offset, uint8* pData, uint16 len)
{
	return FruInfo_PagedAccess(pFruInfo, offset, pData, len, pFruInfo->Write);
}

Bool FruInfo_Write(FruInfo* pFruInfo)
{
	return FruInfo_WriteOffset(pFruInfo, 0, pFruInfo->m_Buffer, pFruInfo->m_UsedBufLen);
}
void FruInfo_SetState(FruInfo* pFruInfo, FruInfoState state)
{
	pFruInfo->m_State = state;
}

Bool FruInfo_isOk(FruInfo* pFruInfo)
{
	return pFruInfo->m_State == FRU_VERIFY_SUCCESS;
}

void FruInfo_SetData(FruInfo* pFruInfo, uint32 offset, const uint8 *pData, uint16 len)
{
	Assert(pFruInfo->m_FruAreaSize >= offset + len);

	memcpy(&pFruInfo->m_Buffer[offset], pData, len);
}

Bool FruInfo_CheckSum(const unsigned char *pData, uint16 Length, const uint8 checkSum)
{
    unsigned char TempArr = checkSum;
    int iIndex = 0;

    if ( Null == pData) 
    {
        return False;
    }

    for(iIndex = 0; iIndex < Length; iIndex++)
    {
        TempArr = TempArr + pData[iIndex]; 
    }

    if (0 != TempArr)
    {
        return False;
    }
    return True;
}

Bool FruInfo_isValidRecord(FruInfo* pFruInfo, const uint8* pRecordData)
{
	const RecordHeader* pHeader = (RecordHeader*)pRecordData;

	if(!FruInfo_CheckSum(&pRecordData[5], pHeader->m_RecordLength, pHeader->m_RecordChecksum))
	{
		TRACE_LEVEL(TRACE_FRU|TRACE_WARNING, ("Record[0x%02x, 0x%02x] data checksum error, Dump data:\n"
 			, pHeader->m_RecordId, pHeader->m_PicmgRecordID
			));
		DUMP_BYTE_LEVEL(TRACE_FRU, (uint8*)&pRecordData[5], pHeader->m_RecordLength);
		return False;
	}

	if(!FruInfo_CheckSum(pRecordData, 4, pHeader->m_HeaderChecksum))
	{
		TRACE_LEVEL(TRACE_FRU|TRACE_WARNING, ("Record Header[0x%02x, 0x%02x] checksum error. Dump data:\n"
 			, pHeader->m_RecordId, pHeader->m_PicmgRecordID
			));
		
		DUMP_BYTE_LEVEL(TRACE_FRU, (uint8*)pRecordData, pHeader->m_RecordLength + 5);
		return False;
	}
	
	TRACE_LEVEL(TRACE_FRU, ("Record[0x%02x] verify success. len=%d\n"
		, pHeader->m_PicmgRecordID
		, pHeader->m_RecordLength));
	
	return True;
}

Bool FruInfo_AreaVerify(FruInfo* pFruInfo, const uint8* pByte)
{	
	int nLen = pByte[1] * MOULD_8;
	
	if(nLen == 0)
	{
		Warning("Area len=[%d] error.\n", nLen);
		return False;
	}
	
	if(!FruInfo_CheckSum(pByte, nLen-1, pByte[nLen-1]))
	{
		Warning("Area checksum error. Dump area data.\n", pFruInfo->m_UsedBufLen);
		DUMP_BYTE_LEVEL(TRACE_FRU, (uint8*)pByte, nLen);
		return False;
	}
	
	TRACE_LEVEL(TRACE_FRU, ("Area verify success. len=%d\n", nLen));
	
	return True;
}

uint8 FruInfo_ParseMultiRecordArea(FruInfo* pFruInfo, const uint8* pByte)
{
	int i = 0;
	RecordHeader* pHeader = Null;
	uint8 recordCount = 0;
		
	do
	{
		pHeader = (RecordHeader*)pByte;
		if(pHeader->m_RecordLength == 0)
		{
			pHeader->m_RecordLength = 0;
		}
		
		if(FruInfo_isValidRecord(pFruInfo, pByte))
		{						
			if(pFruInfo->Filter)
			{
				pFruInfo->Filter(pFruInfo, pHeader);
			}
			for(i = 0; i < pFruInfo->m_MaxRecordCount; i++)
			{
				if(Null == pFruInfo->m_pRecordArray[i])
				{
					pFruInfo->m_pRecordArray[i] = pHeader;
					recordCount++;
					goto Next;
				}				
			}
			Trace("The space of m_pRecordArray is not enough!");
			Assert(False);
			return 0;
		}
		else
		{
			return 0;
		}
Next:
		pByte += pHeader->m_RecordLength + RECORD_HEAD_LEN;
		pFruInfo->m_UsedBufLen += pHeader->m_RecordLength + RECORD_HEAD_LEN;
		
	}while(0 == pHeader->m_EndOfList);

	return recordCount;
}

Bool FruInfo_Parse(FruInfo* pFruInfo)
{
	uint8 recordCount = 0;
	Bool nRet = True;
	uint8 index = 0;	
	FruHeader* pFruHeader = (FruHeader*)pFruInfo->m_Buffer;

	if(pFruInfo->m_State == FRU_VERIFY_FAILED)
	{
		goto END; 
	}

	if(pFruInfo->m_FruAreaSize < DATA_BASE_HEAD_SIZE || pFruInfo->m_State != FRU_READ_SUCCESS)
	{
		Warning("FruInfo length is invalid or FruInfo read failed, len=%d\n", pFruInfo->m_UsedBufLen);
		nRet = False;
		pFruInfo->m_State = FRU_VERIFY_FAILED;
		goto END;
	}
	TRACE_LEVEL(TRACE_FRU, ("FruInfo database len = %d\n", pFruInfo->m_UsedBufLen));
	
	//Verify Database
	if(!FruInfo_CheckSum((uint8*)pFruHeader, DATA_BASE_HEAD_SIZE - 1, pFruHeader->m_Checksum))
	{
		Warning("FruInfo comm head checksum error, dump data:\n", pFruInfo->m_UsedBufLen);
		DUMP_BYTE_LEVEL(TRACE_FRU, (uint8*)pFruHeader, DATA_BASE_HEAD_SIZE - 1);
		nRet = False;
		pFruInfo->m_State = FRU_VERIFY_FAILED;
		goto END;
	}
	pFruInfo->m_pFruHeader = pFruHeader;

	//InternalUse Area Info
	index = pFruHeader->m_InternalUseArea * MOULD_8;
	if(index)
	{
		nRet = FruInfo_AreaVerify(pFruInfo, &pFruInfo->m_Buffer[index]);
		if(!nRet)
		{
			pFruInfo->m_State = FRU_VERIFY_FAILED;
			goto END;
		}
		pFruInfo->m_pInnerUseArea = (InnerUseArea*)&pFruInfo->m_Buffer[index];
	}
	
	//ChassisInfo Area Info
	index = pFruHeader->m_ChassisInfoArea * MOULD_8;
	if(index)
	{
		nRet = FruInfo_AreaVerify(pFruInfo, &pFruInfo->m_Buffer[index]);
		if(!nRet)
		{
			pFruInfo->m_State = FRU_VERIFY_FAILED;
			goto END;
		}
		pFruInfo->m_pChassisInfoArea = (ChassisInfoArea*)&pFruInfo->m_Buffer[index];
	}

	//Board Area Info
	index = pFruHeader->m_BoardInfoArea * MOULD_8;
	if(index)
	{
		nRet = FruInfo_AreaVerify(pFruInfo, &pFruInfo->m_Buffer[index]);
		if(!nRet)
		{
			pFruInfo->m_State = FRU_VERIFY_FAILED;
			goto END;
		}
		pFruInfo->m_pBoardAreaInfo = (BoardAreaInfo*)&pFruInfo->m_Buffer[index];
	}

	//Product Area Info
	index = pFruHeader->m_ProductInfoArea * MOULD_8;
	if(index)
	{
		nRet = FruInfo_AreaVerify(pFruInfo, &pFruInfo->m_Buffer[index]);
		if(!nRet)
		{
			pFruInfo->m_State = FRU_VERIFY_FAILED;
			goto END;
		}
		pFruInfo->m_pProductInfoArea = (ProductInfo*)&pFruInfo->m_Buffer[index];
	}
	
	//Multi record area
	index = pFruHeader->m_MultiRecordArea * MOULD_8;
	if(index)
	{
		pFruInfo->m_UsedBufLen = index;
		recordCount = FruInfo_ParseMultiRecordArea(pFruInfo, &pFruInfo->m_Buffer[index]);
	}
    /*else
    {
//        nRet = False;
    }
	
	if(nRet && recordCount)
	{
		pFruInfo->m_State = FRU_VERIFY_SUCCESS;
	}
	else
	{
		pFruInfo->m_State = FRU_VERIFY_FAILED;
	}
	*/
	pFruInfo->m_State = FRU_VERIFY_SUCCESS;

END:	
	TRACE_LEVEL(TRACE_MAIN, ("%s() Fru data verify [%s], FruInfo BufSize|UsedSize=%d|%d, recordCount=%d\n"
		, _FUNC_
		, pFruInfo->m_State == FRU_VERIFY_SUCCESS ? "SUCCESS" : "FAILED"
		, pFruInfo->m_FruAreaSize
		, pFruInfo->m_UsedBufLen
		, recordCount
		));
	//if(FRU_VERIFY_FAILED == pFruInfo->m_State)
	{
		TRACE_LEVEL(TRACE_FRU, ("Dump Fru Data:\n"));
		DUMP_BYTE_LEVEL_EX(TRACE_FRU, (uint8*)pFruInfo->m_Buffer, pFruInfo->m_FruAreaSize, 10);
	}
	return nRet;
}

uint8 g_FruInforDefault[] =
{
	0x01, 0x00, 0x00, 0x01, 0x05, 0x09, 0x00, 0xFA
};
uint8 g_FruInforDefaultLen = sizeof(g_FruInforDefault);

uint8 g_BoardAreaInfoDefault[] =
{
		0x01, 0x04, 0x00
	,0x00, 0x00, 0x00
	,0xC4	//7	Board Manufacturer
	,'Z', 'H', 'B', 'T'
	,0xCB	//12	Board Product Name
	,'N', 'M', 'C', 'P', '-', '1', '0', '-', '0', '-', '0'	//23
	,0xC0	//24 Board Serial Number 
	,0xC0	//25 Board Part Number
	,0xC0	//26 FRU File ID
	,0xC1	//27 Board Serial Number 
	,0x00, 0x00, 0x00, 0x00
	,0xE1	//CheckSum
};
uint8 g_BoardAreaInfoDefaultLen = sizeof(g_BoardAreaInfoDefault);

uint8 g_ProductAreaInfoDefault[] =
{
		0x01, 0x04, 0x00
	,0xC4	//3	Product Manufacturer
	,'Z', 'H', 'B', 'T'
	,0xCB	//8	Product Name
	,'N', 'M', 'C', 'P', '-', '1', '0', '-', '0', '-', '0'	
	,0xC0	//20 Product Part/Model Number
	,0xC0	//21 Product Version
	,0xC0	//22 Product Serial Number 
	,0xC0	//23 Asset Tag 
	,0xC0	//24 FRU File ID
	,0xC1	//25 Custom product info area fields
	,0x00, 0x00, 0x00, 0x00, 0x00
	,0xE1	//31 CheckSum
};
uint8 g_ProductAreaInfoDefaultLen = sizeof(g_ProductAreaInfoDefault);

void FruInfo_FormatRecord(RecordHeader* pHeader, uint8 nLen)
{
	pHeader->m_Version = 0x02;
	pHeader->m_RecordLength = nLen - RECORD_HEAD_LEN;
	pHeader->m_RecordChecksum = CheckSum_Get(&pHeader->m_ManufacturerID[0], pHeader->m_RecordLength);
	pHeader->m_HeaderChecksum = CheckSum_Get((uint8*)pHeader, RECORD_HEAD_LEN - 1);
}

void FruInfo_FormatHeadArea(uint8* pData, uint8 nLen)
{
	pData[1] = nLen / MOULD_8;
	pData[nLen - 1] = CheckSum_Get(pData, nLen-1);
}

void FruInfo_SetDefault(FruInfo* pFruInfo, const RecordItem* recordItemArray, uint8 count)
{
	int i = 0;
	int nLen = 0;
	int nIndex = 0;
	uint8* pBuff = pFruInfo->m_Buffer;
	RecordHeader* pHeader = Null;
	const RecordItem* pRecordItem = recordItemArray;
	FruHeader* pFruHeader = (FruHeader*)g_FruInforDefault;
		
	pFruHeader->m_Checksum = CheckSum_Get(&pFruHeader->m_CommonHead, 7);
	
	//InitFru Header
	nIndex = 0;
	memcpy(&pBuff[nIndex], g_FruInforDefault, sizeof(g_FruInforDefault));
	
	//Init board record area
	nLen = sizeof(g_BoardAreaInfoDefault);
	FruInfo_FormatHeadArea(g_BoardAreaInfoDefault, nLen);
	nIndex = pFruHeader->m_BoardInfoArea * MOULD_8;
	memcpy(&pBuff[nIndex], g_BoardAreaInfoDefault, nLen);

	//Init product record area
	nLen = sizeof(g_ProductAreaInfoDefault);
	FruInfo_FormatHeadArea(g_ProductAreaInfoDefault, nLen);
	nIndex = pFruHeader->m_ProductInfoArea * MOULD_8;
	memcpy(&pBuff[nIndex], g_ProductAreaInfoDefault, nLen);

	//Initialize Multi record
	nIndex = pFruHeader->m_MultiRecordArea * MOULD_8;
	for(i = 0; i < count; i++, pRecordItem++)
	{
		pHeader = (RecordHeader*)pRecordItem->m_pRecord;
		pHeader->m_EndOfList = (i == (count - 1)) ? 1 : 0;

		FruInfo_FormatRecord(pHeader, pRecordItem->m_Len);
		
		Assert(nIndex + pRecordItem->m_Len <= pFruInfo->m_FruAreaSize);
		memcpy(&pBuff[nIndex], pRecordItem->m_pRecord, pRecordItem->m_Len);
		nIndex += pRecordItem->m_Len;
	}
	
	pFruInfo->m_UsedBufLen = nIndex;
	pFruInfo->m_State = FRU_READ_SUCCESS;
}

RecordHeader* FruInfo_Find(FruInfo* pFruInfo,uint8 recordTypeId, uint8 PicmgID)
{
	uint8 index = 0;
	return FruInfo_FindEx(pFruInfo, recordTypeId, PicmgID, &index);
}

RecordHeader* FruInfo_FindEx(FruInfo* pFruInfo,uint8 recordTypeId, uint8 PicmgID, uint8* startIndex)
{
	int i = 0;
	RecordHeader* pheader = Null;
	
	for(i = *startIndex; i < pFruInfo->m_MaxRecordCount; i++)
	{
		pheader = pFruInfo->m_pRecordArray[i];
		if(pheader)
		{
			if(pheader->m_RecordId == recordTypeId	&& pheader->m_PicmgRecordID== PicmgID)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	*startIndex = i;
	return pheader;
}

void FruInfo_Reset(FruInfo* pFruInfo)
{
	pFruInfo->m_State = FRU_INIT;
	
	pFruInfo->m_pFruHeader = Null;
	pFruInfo->m_pInnerUseArea = Null;
	pFruInfo->m_pChassisInfoArea = Null;
	pFruInfo->m_pBoardAreaInfo = Null;
	pFruInfo->m_pProductInfoArea = Null;

	memset(pFruInfo->m_pRecordArray, 0, sizeof(RecordHeader*) * pFruInfo->m_MaxRecordCount);
	
	pFruInfo->m_UsedBufLen = 0;
}

void FruInfo_Release(FruInfo* pFruInfo)
{
	FruInfo_Reset(pFruInfo);
	
	memset(pFruInfo->m_Buffer, 0, pFruInfo->m_FruAreaSize);
	memset(pFruInfo, 0, sizeof(FruInfo));
}

void FruInfo_Init(FruInfo* pFruInfo
	, uint8* pFruInfoBuff
	, uint16 len
	, RecordHeader* pRecordArray[]
	, uint8 recordCount
	, FruAccessFun read
	, FruAccessFun write
	)
{
	Assert(pFruInfoBuff && len);
	
	memset(pFruInfo, 0, sizeof(FruInfo));
	memset(pFruInfoBuff, 0, len);
	memset(pRecordArray, 0, sizeof(RecordHeader*) * recordCount);

	pFruInfo->m_isUsed = True;
	
	pFruInfo->m_pRecordArray = pRecordArray;
	pFruInfo->m_MaxRecordCount = recordCount;

	pFruInfo->m_Buffer = pFruInfoBuff;
	pFruInfo->m_FruAreaSize = len;

	pFruInfo->Read = read;
	pFruInfo->Write = write;
}

