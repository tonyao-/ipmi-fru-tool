#include "StdAfx.h"
#include "FruRepo.h"
#include "resource.h"
#include "IpmTool.h"

#include "PropCarrierInformation.h"
#include "PropCarrierPowerPolicy.h"
#include "PropCarrierManagerIpLink.h"
#include "PropCarrierPtPConn.h"
#include "PropCarrierActAndPm.h"
#include "PropModuleCurrentReq.h"
#include "PropAmcPtpConn.h"
#include "PropPowerCapability.h"
#include "PropSdrFullSensor.h"
#include "PropSdrCompactSensor.h"
#include "PropFruCtrl.h"

#if 0
void FruInfo_RemoveInnerUseArea(FruInfo* pFruInfo)
{
	int nLen = 1;
	//uint8* pBuff = malloc(pFruInfo->m);
	FruHeader* pFruHeader = (FruHeader*)pFruInfo->m_Buffer;

	if(pFruHeader->m_InternalUseArea)
	{
		InnerUseArea* pInnerUseArea = (InnerUseArea*)&pFruInfo->m_Buffer[pFruHeader->m_InternalUseArea << 3];

		if(pFruHeader->m_ChassisInfoArea)
		{
			pFruHeader->m_ChassisInfoArea -= pInnerUseArea->m_Length;
		}
		if(pFruHeader->m_BoardInfoArea)
		{
			pFruHeader->m_BoardInfoArea -= pInnerUseArea->m_Length;
		}
		if(pFruHeader->m_ProductInfoArea)
		{
			pFruHeader->m_ProductInfoArea -= pInnerUseArea->m_Length;
		}
		if(pFruHeader->m_MultiRecordArea)
		{
			pFruHeader->m_MultiRecordArea -= pInnerUseArea->m_Length;
		}
		pFruInfo->m_isInnerUseAreaError = 0;
	}
}

void FruInfo_RemoveChassisInfoArea(FruInfo* pFruInfo)
{
}

void FruInfo_RemoveBoardAreaInfo(FruInfo* pFruInfo)
{
}

void FruInfo_RemoveProductAreaInfo(FruInfo* pFruInfo)
{
}

Bool FruInfo_AddRecord(FruInfo* pFruInfo, RecordHeader* pRecordData)
{
	RecordHeader* pRecord = Null;
	FruHeader* pFruHeader = (FruHeader*)pFruInfo->m_Buffer;
	int nIndex = pFruHeader->m_MultiRecordArea << 3;

	for(int i = 0; i < pFruInfo->m_MaxRecordCount; i++)
	{
		pRecord = pFruInfo->m_pRecordArray[i];
		if(NULL == pRecord)
		{
			if(0 < i)
			{
				pRecord = pFruInfo->m_pRecordArray[i-1];
				pRecord->m_EndOfList = 0;
				pRecord->m_RecordChecksum = CheckSum_Get(&pRecord->m_ManufacturerID[0], pRecord->m_RecordLength);
				pRecord->m_HeaderChecksum = CheckSum_Get((uint8*)pRecord, RECORD_HEAD_LEN - 1);
			}
			pRecord = pRecordData;

			pRecord->m_EndOfList = 1;
			pRecord->m_RecordChecksum = CheckSum_Get(&pRecord->m_ManufacturerID[0], pRecord->m_RecordLength);
			pRecord->m_HeaderChecksum = CheckSum_Get((uint8*)pRecord, RECORD_HEAD_LEN - 1);
			memcpy(&pFruInfo->m_Buffer[nIndex], pRecord, pRecord->m_RecordLength + RECORD_HEAD_LEN);
			Assert(nIndex + pRecord->m_RecordLength + RECORD_HEAD_LEN <= pFruInfo->m_FruAreaSize);
			break;
		}
		else
		{
			nIndex += pRecord->m_RecordLength + RECORD_HEAD_LEN;
		}
	}

	return True;
}

Bool FruInfo_RemoveRecord(FruInfo* pFruInfo, uint8 reocrdId, uint8 nPicmgRecordID)
{
	RecordHeader* pRecord = Null;
	RecordHeader* pNextRecord = Null;
	RecordHeader* pPreRecord = Null;
	FruHeader* pFruHeader = (FruHeader*)pFruInfo->m_Buffer;
	int nIndex = pFruHeader->m_MultiRecordArea << 3;

	for(int i = 0; i < pFruInfo->m_MaxRecordCount; i++)
	{
		pRecord = pFruInfo->m_pRecordArray[i];
		pPreRecord = (i > 0) ? pFruInfo->m_pRecordArray[i-1] : NULL;
		pNextRecord = (i+1 < pFruInfo->m_MaxRecordCount) ? pFruInfo->m_pRecordArray[i+1] : NULL;

		if(pRecord)
		{
			if(reocrdId == pRecord->m_RecordId && nPicmgRecordID == pRecord->m_PicmgRecordID)
			{
				if(pNextRecord)
				{
					memcpy(&pFruInfo->m_Buffer[nIndex], pNextRecord, pFruInfo->m_UsedBufLen - nIndex);
					pFruInfo->m_UsedBufLen -= pRecord->m_RecordLength + RECORD_HEAD_LEN;
					memcpy(pFruInfo->m_pRecordArray[i], pFruInfo->m_pRecordArray[i+1], pFruInfo->m_MaxRecordCount - i - 1);
				}
				else if(pPreRecord)
				{
					pPreRecord->m_EndOfList = 1;
					pPreRecord->m_RecordChecksum = CheckSum_Get(&pPreRecord->m_ManufacturerID[0], pPreRecord->m_RecordLength);
					pPreRecord->m_HeaderChecksum = CheckSum_Get((uint8*)pPreRecord, RECORD_HEAD_LEN - 1);
					pFruInfo->m_UsedBufLen -= pRecord->m_RecordLength + RECORD_HEAD_LEN;
				}
				break;
			}
			else
			{
				nIndex += pRecord->m_RecordLength + RECORD_HEAD_LEN;
			}
		}
		else
		{
			break;
		}
	}

	return True;
}
#endif 

IMPLEMENT_DYNAMIC(CFruRepoData, CObject)

CFruRepoData::CFruRepoData(uint8 nDeviceId, int nMaxLen, LPCTSTR lpName)
{
	m_pData = (uint8*)malloc(nMaxLen);
	m_nUsedLen = 0;
	m_nDeviceId = nDeviceId;
	m_nMaxLen = nMaxLen;
	m_strName = lpName;
};

CFruRepoData::~CFruRepoData()
{
	delete m_pData;
};

BOOL CFruRepoData::SetData(uint8* pData, int dataLen)
{
	ASSERT(dataLen <= m_nMaxLen);
	if(dataLen< m_nMaxLen)
	{
		m_nUsedLen = dataLen;
		memcpy(m_pData, pData, dataLen);
		return TRUE;
	}

	return FALSE;
}

void CFruRepoData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nDeviceId;
		ar << m_nUsedLen;
		for(int i = 0; i < m_nUsedLen; i++)
		{
			ar << m_pData[i];
		}
	}
	else
	{
		ar >> m_nDeviceId;
		ar >> m_nUsedLen;
		for(int i = 0; i < m_nUsedLen; i++)
		{
			ar >> m_pData[i];
		}
	}
}

////////////////////////////////////////////////////////////
CFruRepo::CFruRepo()
{
	m_pFruRepoData = NULL;

	m_pCommonHeaderProp = NULL;
	m_pBoardInfoProp	= NULL;
	m_pProductInfoProp	= NULL;

	FruInfo_Init(&m_FruInfo, m_FruRepoBuff, sizeof(m_FruRepoBuff), m_pFruRecordArray, sizeof(m_pFruRecordArray) / 4, NULL, NULL);
	//m_PropArray.SetSize(30);

	g_FruInforDefault[7] = CheckSum_Get(g_FruInforDefault, 7);
	FruInfo_FormatHeadArea(g_BoardAreaInfoDefault, g_BoardAreaInfoDefaultLen);
	FruInfo_FormatHeadArea(g_ProductAreaInfoDefault, g_ProductAreaInfoDefaultLen);
}


CFruRepo::~CFruRepo(void)
{
}

BOOL CFruRepo::CommonHeaderInit()
{
	return TRUE;
}

void CFruRepo::Reset()
{
	RemoveAll();
	
	m_PropArray.RemoveAll();

	m_pCommonHeaderProp = NULL;
	m_pBoardInfoProp = NULL;
	m_pProductInfoProp = NULL;
	m_isIncludeMultiRecord = FALSE;
}

int CFruRepo::GetFruData(BYTE* pFruData, int nBufLen)
{
	int nIndex = 0;

	for(int i = 0; i < m_PropArray.GetCount(); i++)
	{
		memcpy(&pFruData[nIndex], m_PropArray[i]->m_pFruData, m_PropArray[i]->m_FruDataLen);
		nIndex += m_PropArray[i]->m_FruDataLen;
	}
	return nIndex;
}

CPropFruReco* CFruRepo::CreateProp(RecordHeader* pheader)
{
	CPropFruReco* pProp = NULL;

	m_isIncludeMultiRecord = TRUE;

	if(FRT_OEM == pheader->m_RecordId)
	{
		switch(pheader->m_PicmgRecordID)
		{
			case PRID_FruInfoPartition:
				pProp = new CPropFruInfoPart(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_CarrierInfo:
				pProp = new CPropCarrierInformation(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_PowerPolicy:
				pProp = new CPropCarrierPowerPolicy(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_CarrierManagerIpLink:
				pProp = new CPropCarrierManagerIpLink(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_CarrierP2pConnect:
				pProp = new CPropCarrierPtPConn(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_ActAndPm:
				pProp = new CPropCarrierActAndPm(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_CurrentReq:
				pProp = new CPropModuleCurrentReq(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_AmcP2pConnect:
				pProp = new CPropAmcPtpConn(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			case PRID_PowerCapability:
				pProp = new CPropPowerCapability(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
				break;
			default:
				pProp = NULL;
				AfxMessageBox(L"Load fru data failed.");
		}
	}
	else if(FULL_SENSOR_RECORD == pheader->m_RecordId)
	{
		pProp = new CPropSdrFullSensor(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
	}
	else if(COMPACT_SENSOR_RECORD == pheader->m_RecordId)
	{
		pProp = new CPropSdrCompactSensor(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
	}
	else if(FRUCTRL_RECORD == pheader->m_RecordId)
	{
		pProp = new CPropFruCtrl(this, (BYTE*)pheader, pheader->m_RecordLength + RECORD_HEAD_LEN);
	}
	return pProp;
}
BOOL CFruRepo::Init(InnerUseArea* pInnerUseArea
		, ChassisInfoArea* pChassisInfoArea
		, BoardAreaInfo* pBoardAreaInfo
		, ProductInfo* pProductAreaInfo
		)
{
	Reset();

	m_pCommonHeaderProp = new CPropCommonHeader(this, (BYTE*)m_FruInfo.m_pFruHeader, DATA_BASE_HEAD_SIZE, m_pFruRepoData?m_pFruRepoData->GetName():NULL);


	if(pBoardAreaInfo)
	{
		m_pBoardInfoProp	= new CPropBoardInfo(this, (BYTE*)m_FruInfo.m_pBoardAreaInfo, m_FruInfo.m_pBoardAreaInfo->m_Length << 3);
	}

	if(pProductAreaInfo)
	{
		m_pProductInfoProp	= new CPropProductInfo(this, (BYTE*)m_FruInfo.m_pProductInfoArea, m_FruInfo.m_pProductInfoArea->m_Length << 3);
	}

	return TRUE;
}
void CFruRepo::AddCommHeader()
{
	if(m_pCommonHeaderProp)
	{
		MessageBox(L"已存在BoardInfoArea数据， 操作失败！");
		return;
	}

	m_pCommonHeaderProp = new CPropCommonHeader(this, g_FruInforDefault, g_FruInforDefaultLen, m_pFruRepoData?m_pFruRepoData->GetName():NULL);
	m_pCommonHeaderProp->Expand(FALSE);
	m_PropArray.Add(m_pCommonHeaderProp);
	UpdateCommHeader();
}

void CFruRepo::AddBoardInfoArea()
{
	if(m_pBoardInfoProp)
	{
		MessageBox(L"已存在BoardInfoArea数据， 操作失败！");
		return;
	}

	m_pBoardInfoProp	= new CPropBoardInfo(this, g_BoardAreaInfoDefault, g_BoardAreaInfoDefaultLen);
	m_pBoardInfoProp->Expand(FALSE);
	m_PropArray.Add(m_pBoardInfoProp);

	UpdateCommHeader();

}

void CFruRepo::AddProductInfoArea()
{
	if(m_pProductInfoProp)
	{
		MessageBox(L"已存在ProductInfo数据， 操作失败！");
		return;
	}

	m_pProductInfoProp	= new CPropProductInfo(this, (BYTE*)g_ProductAreaInfoDefault, g_ProductAreaInfoDefaultLen);
	m_pProductInfoProp->Expand(FALSE);
	m_PropArray.Add(m_pProductInfoProp);

	UpdateCommHeader();

	if(theApp.m_pMainWnd)
	{
		AfxGetMainWnd()->PostMessage(ID_SHOW_FRUDATA);
	}
}

BOOL CFruRepo::Init(BOOL isIncludeInnerUseArea
		, BOOL isIncludeChassisInfoArea
		, BOOL isIncludeBoardAreaInfo
		, BOOL isIncludeProductAreaInfo
		, CFruRepoData* pRepo
		)
{
	int nOffset = 1;
	
	m_pFruRepoData = pRepo;

	AddCommHeader();
	if(isIncludeInnerUseArea)
	{
	}
	if(isIncludeChassisInfoArea)
	{
	}
	if(isIncludeBoardAreaInfo)
	{
		AddBoardInfoArea();
	}
	if(isIncludeProductAreaInfo)
	{
		AddProductInfoArea();
	}
	return TRUE;
}

CPropFruReco* CFruRepo::AddRecord(RecordHeader* pRecord, uint8 nLen)
{
	CPropFruReco* pProp = NULL;
	pProp = CreateProp(pRecord);

	if(pProp)
	{
		for(int i = 0; i < m_PropArray.GetCount(); i++)
		{
			CPropFruReco* pPropEx = (CPropFruReco*)m_PropArray[i];
			if(pPropEx->m_pFruData[0] != 1)// Multy Record
			{
				pPropEx->Expand(FALSE);
				pPropEx->SetEndofList(FALSE);
			}
		}

		m_PropArray.Add(pProp);
		pProp->Expand(TRUE);
		pProp->SetEndofList(TRUE);
	}
	else
	{
		CString str; str.Format(L"Add Record Failed(recordId=0x%x, nPicmgRecordID=0x%x)", pRecord->m_RecordId, pRecord->m_PicmgRecordID);
		AfxMessageBox(str);
	}
	UpdateCommHeader();

	return pProp;
}

void CFruRepo::RemoveRecord(BYTE reocrdId, uint8 nPicmgRecordID)
{
	for(int i = 0; i < m_PropArray.GetCount(); i++)
	{
	}

	CString str; str.Format(L"Unsupport Record(recordId=0x%x, nPicmgRecordID=0x%x)", reocrdId, nPicmgRecordID);
	AfxMessageBox(str);
}

void CFruRepo::SetFruData(CFruRepoData* pRepo)
{
	m_pFruRepoData = pRepo;
	SetFruData(pRepo->GetData(), pRepo->GetUsedLen());
}
void CFruRepo::SetFruData(BYTE* pFruData, int nLen)
{
	FruInfo_Reset(&m_FruInfo);
	FruInfo_SetData(&m_FruInfo, 0 , pFruData, nLen);
	m_FruInfo.m_State = FRU_READ_SUCCESS;
	if(!FruInfo_Parse(&m_FruInfo))
	{
		ASSERT(FALSE);
		AfxMessageBox(L"Fru Repo Data is invalid");
		return;
	}

	Reset();

	m_pCommonHeaderProp = new CPropCommonHeader(this, (BYTE*)m_FruInfo.m_pFruHeader, DATA_BASE_HEAD_SIZE, m_pFruRepoData?m_pFruRepoData->GetName():NULL);
	m_pCommonHeaderProp->Expand(FALSE);
	m_PropArray.Add(m_pCommonHeaderProp);

	if(m_FruInfo.m_pBoardAreaInfo)
	{
		m_pBoardInfoProp	= new CPropBoardInfo(this, (BYTE*)m_FruInfo.m_pBoardAreaInfo, m_FruInfo.m_pBoardAreaInfo->m_Length << 3);
		m_pBoardInfoProp->Expand(FALSE);
		m_PropArray.Add(m_pBoardInfoProp);
	}

	if(m_FruInfo.m_pProductInfoArea)
	{
		m_pProductInfoProp	= new CPropProductInfo(this, (BYTE*)m_FruInfo.m_pProductInfoArea, m_FruInfo.m_pProductInfoArea->m_Length << 3);
		m_pProductInfoProp->Expand(FALSE);
		m_PropArray.Add(m_pProductInfoProp);
	}

	CPropFruReco* pProp = NULL;
	for(int i = 0; i < m_FruInfo.m_MaxRecordCount; i++)
	{
		RecordHeader* pheader = m_FruInfo.m_pRecordArray[i];

		if(NULL == pheader) break;

		pProp = CreateProp(pheader);

		if(pProp)
		{
			m_PropArray.Add(pProp);
			pProp->Expand(FALSE);
		}
	}

	if(pProp)
	{
		pProp->Expand(TRUE);
		pProp->SetEndofList(TRUE);
	}

	if(theApp.m_pMainWnd)
	{
		AfxGetMainWnd()->PostMessage(ID_SHOW_FRUDATA);
	}
}

void CFruRepo::UpdateCommHeader()
{
	int nOffset = 1;
	if(m_pBoardInfoProp)
	{
		m_pCommonHeaderProp->m_pFruHeader->m_InternalUseArea = 0;
		m_pCommonHeaderProp->m_pFruHeader->m_ChassisInfoArea = 0;
		m_pCommonHeaderProp->m_pFruHeader->m_BoardInfoArea = nOffset;
		nOffset += m_pBoardInfoProp->m_pBoardAreaInfo->m_Length;
	}
	else
	{
		m_pCommonHeaderProp->m_pFruHeader->m_BoardInfoArea = 0;
	}

	if(m_pProductInfoProp)
	{
		m_pCommonHeaderProp->m_pFruHeader->m_ProductInfoArea = nOffset;
		nOffset += m_pProductInfoProp->m_pProductInfo->m_Length;
	}
	else
	{
		m_pCommonHeaderProp->m_pFruHeader->m_ProductInfoArea = 0;
	}

	if(m_isIncludeMultiRecord)
	{
		m_pCommonHeaderProp->m_pFruHeader->m_MultiRecordArea = nOffset;
	}
	else
	{
		m_pCommonHeaderProp->m_pFruHeader->m_MultiRecordArea = 0;
	}

	m_pCommonHeaderProp->m_pFruHeader->m_Checksum = CheckSum_Get((uint8*)m_pCommonHeaderProp->m_pFruHeader, DATA_BASE_HEAD_SIZE - 1);

	m_pCommonHeaderProp->SetDwValue(3, m_pCommonHeaderProp->m_pFruHeader->m_BoardInfoArea);	//Modify Product info offset
	m_pCommonHeaderProp->SetDwValue(4, m_pCommonHeaderProp->m_pFruHeader->m_ProductInfoArea);	//Modify Product info offset
	m_pCommonHeaderProp->SetDwValue(5, m_pCommonHeaderProp->m_pFruHeader->m_MultiRecordArea);	//Mulit record info offset
	m_pCommonHeaderProp->SetDwValue(6, m_pCommonHeaderProp->m_pFruHeader->m_Checksum);	//Mulit Check sum
}

void CFruRepo::OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue)
{
	UpdateCommHeader();

	if(wcscmp(pProp->GetName(), L"Board Info Offset") == 0)	//Board Info Offset
	{
		DWORD dwNew =pProp->GetDwValue();
		DWORD dwOlder = olderValue.ulVal;

		if(dwOlder == 0 && dwNew > 0)
		{
			((CPropFruInfo*)pProp)->UpdateData(FALSE);
		}
		else if(dwNew == 0 && dwOlder > 0)
		{
			DeletePropertyByName(g_BoardInfoAreaStr);
		}
	}
	else if(wcscmp(pProp->GetName(), L"Product Info Offset") == 0)	//Product Info Offset
	{
		DWORD dwNew = pProp->GetDwValue();
		DWORD dwOlder = olderValue.ulVal;

		if(dwOlder == 0 && dwNew > 0)
		{
			if(pProp->GetByteValue(3) > 0)
			{
				((CPropFruInfo*)pProp)->CreateProp(g_BoardInfoAreaStr);
			}
			else
			{
				((CPropFruInfo*)pProp)->CreateProp(g_CommonHeaderStr);
			}
		}
		else if(dwNew == 0 && dwOlder > 0)
		{
			DeletePropertyByName(g_PorductInfoAreaStr);
		}
	}

	m_pCommonHeaderProp->UpdateData();
	CMFCPropertyGridCtrlEx::OnPropertyChanged(pProp, olderValue);

	AfxGetMainWnd()->PostMessage(ID_SHOW_FRUDATA);

	if(m_pFruRepoData)
		m_pFruRepoData->SetUsedLen(GetFruData(m_pFruRepoData->GetData(), m_pFruRepoData->GetMaxLen()));
}

int CFruRepo::CreateCommHeader(uint8* pBuff, int nLen)
{
	FruHeader header = {0};

	header.m_CommonHead = 1;
	header.m_InternalUseArea = 0;
	header.m_ChassisInfoArea = 0;
	header.m_BoardInfoArea = 0;
	header.m_ProductInfoArea = 0;
	header.m_MultiRecordArea = 0;

	header.m_Checksum = CheckSum_Get((uint8*)&header, sizeof(FruHeader) - 1);

	if(nLen >= sizeof(FruHeader))
	{
		memcpy(pBuff, &header, sizeof(FruHeader));
		return sizeof(FruHeader);
	}
	else
	{
		return 0;
	}
}
