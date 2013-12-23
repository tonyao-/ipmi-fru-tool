#include "StdAfx.h"
#include "PropFruInfoPart.h"

const static LPCTSTR g_lpDescTitle = L"Partition Descriptor";

CPropFruInfoPart::CPropFruInfoPart(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"FRU Information Partition record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pFruInfoPart = (FruInfoParti*)m_pFruData;
	CFieldNum* pFieldNum = new CFieldNum(L"Partition Descriptor Count"	, 0x01, 0x01, 0x02, L"%d", L"This contains a count of the number of entries (M) in the FRU Information Partition record.");
	m_FieldArray.Add(pFieldNum);

	m_pFieldNum[0] = new CFieldNum(L"Partition Offset", 0x1000, 0x0000, 0xFFFF, L"%d", L"Partition Offset. LS byte first. This contains the offset within the physical FRU Information device of the partition divided by 10h.");
	m_pFieldNum[1] = new CFieldNum(L"Partition Length", 0x1000, 0x0000, 0xFFFF, L"%d", L"LS byte first. This contains the length of the partition.");

	UpdateData(FALSE);
}


CPropFruInfoPart::~CPropFruInfoPart(void)
{
	delete m_pFieldNum[0];
	delete m_pFieldNum[1];
}


BOOL CPropFruInfoPart::UpdateData(BOOL isUpdateData)
{
	int nRecordLen = 0;
	if(isUpdateData)
	{
		m_pFruInfoPart->m_PartDescCount	= GetByteValue(8);
		for(int i = 0; i < m_pFruInfoPart->m_PartDescCount; i++)
		{
			GetDwValue(i+9, 0, m_pFruInfoPart->m_PartDesc[i].m_Offset, 2);
			GetDwValue(i+9, 1, m_pFruInfoPart->m_PartDesc[i].m_Length, 2);
		}
		nRecordLen = sizeof(FruInfoParti) + sizeof(PartDesc) * (m_pFruInfoPart->m_PartDescCount - 1) - RECORD_HEAD_LEN;
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);
		SetDwValue(8, m_pFruInfoPart->m_PartDescCount);
		AddPartitionDesc(0, m_pFruInfoPart->m_PartDescCount);
		for(int i = 0; i < m_pFruInfoPart->m_PartDescCount; i++)
		{
			SetDwValue(i+9, 0, AS_UINT16(m_pFruInfoPart->m_PartDesc[i].m_Offset[1], m_pFruInfoPart->m_PartDesc[i].m_Offset[0]));
			SetDwValue(i+9, 1, AS_UINT16(m_pFruInfoPart->m_PartDesc[i].m_Length[1], m_pFruInfoPart->m_PartDesc[i].m_Length[0]));
		}
	}

	m_FruDataLen = sizeof(FruInfoParti) + sizeof(PartDesc) * (m_pFruInfoPart->m_PartDescCount - 1);
	return TRUE;
}

void CPropFruInfoPart::AddPartitionDesc(int nStart, int nCount)
{
	CMFCPropertyGridPropertyNum* pProp = NULL;
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_lpDescTitle, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str, 0, TRUE,  L"This contains a count of the number of entries (M) in the FRU Information Partition record.");
		AddSubItem(pDescGroup);

		pProp = new CMFCPropertyGridPropertyNum(m_pFieldNum[0], pDescGroup);
		pProp = new CMFCPropertyGridPropertyNum(m_pFieldNum[1], pDescGroup);
	}
}

void CPropFruInfoPart:: OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	DWORD dwNew = pChangedProp->GetDwValue();
	DWORD dwOlder = olderValue.ulVal;

	if(wcscmp(pChangedProp->GetName(), L"Partition Descriptor Count") == 0)
	{
		if(dwNew > dwOlder)
		{
			AddPartitionDesc(dwOlder, dwNew - dwOlder);

			DWORD dwOffset = GetDwValue(9, 0);
			DWORD dwLen = GetDwValue(9, 1);
			SetDwValue(10, 0, (dwOffset + dwLen + 10 - 1) / 10 * 10);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_lpDescTitle, i+1);
				RemoveSubItem(str);
			}
		}
	}
	
	CString strDescTitle; strDescTitle.Format(L"%s %d", g_lpDescTitle, 1);

	if(wcscmp(pChangedProp->GetName(), L"Partition Offset") == 0)
	{
		pChangedProp->SetDwValue((dwNew + 10 - 1) / 10 * 10);
		if(pChangedProp->GetParent()->GetName() == strDescTitle 
			&& m_pFruInfoPart->m_PartDescCount == 2)
		{
			DWORD dwOffset = GetDwValue(9, 0);
			DWORD dwLen = GetDwValue(9, 1);

			dwOffset = (dwOffset + dwLen + 10 - 1) / 10 * 10;
			if(GetDwValue(10, 0) < dwOffset)
			{
				SetDwValue(10, 0, dwOffset);
			}
		}
	}
	else if(wcscmp(pChangedProp->GetName(), L"Partition Length") == 0)
	{
		if(pChangedProp->GetParent()->GetName() == strDescTitle && m_pFruInfoPart->m_PartDescCount == 2)
		{
			DWORD dwOffset = GetDwValue(9, 0);
			DWORD dwLen = GetDwValue(9, 1);

			dwOffset = (dwOffset + dwLen + 10 - 1) / 10 * 10;
			if(GetDwValue(10, 0) <= dwOffset)
			{
				SetDwValue(10, 0, dwOffset);
			}
		}
	}
	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}
