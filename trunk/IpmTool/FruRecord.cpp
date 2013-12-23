#include "StdAfx.h"
#include "FruRecord.h"
#include "MFCPropertyGridPropertyEx.h"
#include "MFCPropertyGridCtrlEx.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const NumFieldTemp g_FruInforHeader[] = 
{
	 {L"Record Type ID"	, 0xC0, 0xC0, 0xC0, L"0x%02X", L"For all records defined in this specification a value of C0h (OEM) shall be used."}
	,{L"Version"		, 0x02, 0x02, 0x02, L"0x%02X", L"End of List/Version."}
	,{L"Record Length"	, 0x00, 0x00, 0x00, L"%d"}
	,{L"Record Checksum", 0x00, 0x00, 0x00, L"0x%02X"}
	,{L"Header Checksum", 0x00, 0x00, 0x00, L"0x%02X"}
	,{L"Manufacturer ID", 0x00315A, 0x00315A, 0x00315A, L"0x%06X", L"LS byte first. Write as the three byte ID assigned to PICMG. For this specification, the value 12634 (00315Ah) shall be used."}
	,{L"PICMG Record ID", 0x20, 0x20, 0x20, L"0x%02X", L"For the FRU Information Partition record, the value 20h shall be used."}
	,{L"Record Format Version"		, 0x00, 0x00, 0x00, L"0x%02X", L"For this specification, the value 00h shall be used."}
};

CFruRecord::CFruRecord()
{
}


CFruRecord::~CFruRecord()
{
}

void CFruRecord::InitRecordHeard(CMFCPropertyGridCtrlEx* pGrid, BYTE PicmgID, LPCTSTR lpTitle, LPCTSTR lpPicmgIdDesc)
{
	m_pGrid = pGrid;
	CMFCPropertyGridPropertyNum* pField = NULL;

	m_pGroup = new CMFCPropertyGridPropertyEx(lpTitle);
	m_pGrid->AddProperty(m_pGroup);

	const NumFieldTemp* pReco = g_FruInforHeader;
	for(int i = 0; i < sizeof(g_FruInforHeader) / sizeof(NumFieldTemp); i++, pReco++)
	{
		pField = new CMFCPropertyGridPropertyNum(pReco);

		m_pGroup->AddSubItem(pField);
	}

	SetValue(6, PicmgID, lpPicmgIdDesc);
}

void CFruRecord::OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue)
{
	Update();
	m_pGrid->AdjustLayout();
}

void CFruRecord::Update(BOOL isFlag)
{
	m_pHeader->m_RecordId	= (BYTE)GetDwValue(0);
	m_pHeader->m_Version	= (BYTE)GetDwValue(1);
	

	memcpy(&m_pHeader->m_ManufacturerID, &GET_ROW_DWVALUE(m_pGroup, 5), 3);
	m_pHeader->m_PicmgRecordID			= (BYTE)GetDwValue(6);
	m_pHeader->m_RecordFormatVersion	= (BYTE)GetDwValue(7);

	m_pHeader->m_RecordChecksum = CheckSum_Get(&m_pHeader->m_ManufacturerID[0], m_pHeader->m_RecordLength);
	m_pHeader->m_HeaderChecksum = CheckSum_Get((uint8*)m_pHeader, RECORD_HEAD_LEN - 1);

	SetValue(2, m_pHeader->m_RecordLength);
	SetValue(3, m_pHeader->m_RecordChecksum);
	SetValue(4, m_pHeader->m_HeaderChecksum);
	
}

BOOL CFruRecord::SetValue(int nRow, DWORD dwValue, LPCTSTR lpDesc)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	if(lpDesc)
	{
		m_pGroup->GetSubItem(nRow)->SetDescription(lpDesc);
	}

	((CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow))->SetValue((_variant_t)(dwValue));

	return TRUE;
}

BOOL CFruRecord::SetStrValue(int nRow, const char* lpStr)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	((CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow))->SetValue((_variant_t)(lpStr));
	return TRUE;
}

BOOL CFruRecord::SetStrValue(int nRow, LPCTSTR lpStr)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	((CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow))->SetValue((_variant_t)(lpStr));
	return TRUE;
}


DWORD CFruRecord::GetDwValue(int nRow)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	return ((CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow))->GetDwValue();
};

DWORD CFruRecord::GetDwValue(int nRow, BYTE* pBuff, int len)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	DWORD dw = ((CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow))->GetDwValue();
	ASSERT(len <= 4);

	memcpy(pBuff, &dw, len);

	return TRUE;
};

DWORD CFruRecord::GetSubDwValue(CMFCPropertyGridPropertyEx* pProp, int nRow, int nSubRow)
{
	if(nRow >= pProp->GetSubItemsCount()) return FALSE;

	CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*)pProp->GetSubItem(nRow);
	if(pSubItem == NULL) return 0;

	if(nSubRow >= pSubItem->GetSubItemsCount())
		return FALSE;

	pSubItem = (CMFCPropertyGridPropertyEx*)pSubItem->GetSubItem(nSubRow);
	return pSubItem->GetDwValue();
}

DWORD CFruRecord::GetSubDwValue(int nRow, int nSubRow)
{
	return GetSubDwValue(m_pGroup, nRow, nSubRow);
};

BOOL CFruRecord::SetSubDwValue(int nRow, int nSubRow, DWORD dwValue)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*)(m_pGroup->GetSubItem(nRow)->GetSubItem(nSubRow));

	if(pSubItem)
	{
		pSubItem->SetValue((_variant_t)(dwValue));
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
};

BOOL CFruRecord::GetSubDwValue(int nRow, int nSubRow, BYTE* pBuff, int len)
{
	ASSERT(len <= 4);
	if(nRow >= m_pGroup->GetSubItemsCount()) return FALSE;

	CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow);
	if(pSubItem == NULL) return FALSE;

	if(nSubRow >= pSubItem->GetSubItemsCount())
		return FALSE;

	pSubItem = (CMFCPropertyGridPropertyEx*)pSubItem->GetSubItem(nSubRow);
	//memcpy(pBuff, &pSubItem->GetSubItem(nSubRow)->GetValue().ulVal, len);
	DWORD dw = pSubItem->GetDwValue();
	memcpy(pBuff, &dw, len);

	return TRUE;
};

BOOL CFruRecord::GetStrValue(int nRow, CString& str)
{
	if(nRow >= m_pGroup->GetSubItemsCount())
		return FALSE;

	str = m_pGroup->GetSubItem(nRow)->GetValue().bstrVal;
	return TRUE;
}

BOOL CFruRecord::GetSubStrValue(int nRow, int nSubRow, CString& str)
{
	if(nRow >= m_pGroup->GetSubItemsCount()) return FALSE;

	CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*)m_pGroup->GetSubItem(nRow);
	if(pSubItem == NULL) return 0;

	if(nSubRow >= pSubItem->GetSubItemsCount())
		return FALSE;

	str = pSubItem->GetSubItem(nSubRow)->GetValue().bstrVal;
	return TRUE;
}

int CFruRecord::GetStrValue(int nRow, BYTE* chBuff, int nLen, Bool isUnicode)
{
	int len = 0;
	if(nRow >= m_pGroup->GetSubItemsCount())
		return 0;

	if(isUnicode)
	{
		CString str(m_pGroup->GetSubItem(nRow)->GetValue().bstrVal);

		len = str.GetLength();
		if(len > nLen)
		{
			return 0;
		}

		memcpy(chBuff, str, len);
	}
	else
	{
		CStringA str(m_pGroup->GetSubItem(nRow)->GetValue().bstrVal);

		len = str.GetLength();
		if(len > nLen)
		{
			return 0;
		}

		memcpy(chBuff, str, len);
	}

	return len;
};

BOOL CFruRecord::GetIpAddr(int nRow, BYTE* pAddrBuff)
{
	int i = 0;
	int iStart = 0;
	int iEnd = 0;

	if(nRow >= m_pGroup->GetSubItemsCount()) return FALSE;

	CStringA strIpAddr(m_pGroup->GetSubItem(nRow)->GetValue().bstrVal);

	iEnd = strIpAddr.Find(".", iStart);
	while(iEnd >= 0 && i < 4)
	{
		CStringA str = strIpAddr.Mid(iStart, iEnd - iStart);
		pAddrBuff[i++] = (BYTE)atoi(str);

		iStart = iEnd + 1;
		if(i < 3)
		{
			iEnd = strIpAddr.Find(".", iStart);
		}
		else
		{
			iEnd = strIpAddr.GetLength();
		}
	}

	if(i == 4)
		return TRUE;
	else
		return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
