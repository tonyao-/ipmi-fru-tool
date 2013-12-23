#include "StdAfx.h"
#include "PropCarrierPtPConn.h"

LPCTSTR g_P2pAmcResourceDescStr = L"Point-to-Point AdvancedMC Resource Descriptor";
LPCTSTR g_P2pPortDescStr = L"Point-to-Point Port Descriptor";
LPCTSTR g_P2pAmcResourceDescCountStr = {L"P2pP AMC Resource Desc Count"};

const static int g_nDefaultPortCount = 1;
CPropCarrierPtPConn::CPropCarrierPtPConn(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Carrier Point-to-Point Connectivity record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_nP2pAmcResDescCount = 0;
	m_pCarrierP2pConRecord = (CarrierP2pConRecord*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"P2pP AMC Resource Desc Count", 0x01, 0x01, 0x02, L"%d", L"Point-to-Point AMC Resource Descriptor List Count"));

	m_pP2pAmcResDescField[0] = new CFieldCombo(L"ResourceType"	, _T("AMC"), L"1=AMC|0=on-Carrier device ID");
	m_pP2pAmcResDescField[1] = new CFieldNum(L"ResourceID"	, 0x01, 0x00, 0x0F, L"%d", L"On-Carrier device ID or AMC Site Number");
	m_pP2pAmcResDescField[2] = new CFieldNum(L"PortCount"	, g_nDefaultPortCount, 0x01, MAX_PORT_DESC_PER_AMCRES_DESC, L"%d", L"Indicates the number of point-to-point Ports associated with this Resource.");

	m_pP2pPortDescField[0] = new CFieldNum(L"Local Port"	, 0, 0, 0x1F, L"%d", L"Indicates the Port number within the local AMC Slot or on-Carrier device.");
	m_pP2pPortDescField[1] = new CFieldNum(L"Remote Port"	, 0, 0, 0x1F, L"%d", L"Indicates the Port number within the remote AMC Slot or on-Carrier device ID to which this point-to-point connection is routed.");
	m_pP2pPortDescField[2] = new CFieldCombo(L"Remote ResourceType"	, _T("AMC"), L"1=AMC|0=on-Carrier device ID");
	m_pP2pPortDescField[3] = new CFieldNum(L"Remote ResourceID"	, 0x01, 0x00, 0x0F, L"%d", L"On-Carrier device ID or AMC Site Number");

	UpdateData(FALSE);
}


CPropCarrierPtPConn::~CPropCarrierPtPConn(void)
{
	for(int i = 0; i < m_MaxP2pAmcResDescField; i++)
	{
		delete m_pP2pAmcResDescField[i];
	}

	for(int i = 0; i < m_MaxP2pPortDescField; i++)
	{
		delete m_pP2pPortDescField[i];
	}
}

BOOL CPropCarrierPtPConn::UpdateData(BOOL isUpdateData)
{
	int nIndex = 0;
	const static int nIndexBase = 9;
	int nRecordLen = 0;
	P2pAmcResDesc* pDesc = NULL;
	BYTE* pByte = (BYTE*)m_pCarrierP2pConRecord;

	if(isUpdateData)
	{
		m_nP2pAmcResDescCount = GetByteValue(8);

		nIndex = sizeof(RecordHeader);
		for(int i = 0; i < m_nP2pAmcResDescCount; i++)
		{
			pDesc = (P2pAmcResDesc*)&pByte[nIndex];
			CMFCPropertyGridPropertyEx* p =  (CMFCPropertyGridPropertyEx*)GetSubItem(nIndexBase);
			pDesc->m_ResourceId.m_ResourceType	= GetByteValue(nIndexBase + i, 0);
			pDesc->m_ResourceId.m_ResourceId	= GetByteValue(nIndexBase + i, 1);
			pDesc->m_P2pCount					= GetByteValue(nIndexBase + i, 2);

			nIndex += pDesc->m_P2pCount * sizeof(P2pPortDesc) + 2;
			P2pPortDesc* pPort = pDesc->m_P2pPortDesc;
			for(int n = 0; n < pDesc->m_P2pCount; n++, pPort++)
			{
				DWORD word = 0;

				word = GetByteValue(nIndexBase + i, 3 + n, 0);	//Local Port
				word <<= 5;
				word |= GetByteValue(nIndexBase + i, 3 + n, 1);	//Remote Port
				memcpy(pPort->m_Bytes, &word, 2);

				pPort->m_RemoteResourceId.m_ResourceType	= GetByteValue(nIndexBase + i, 3 + n, 2);
				pPort->m_RemoteResourceId.m_ResourceId		= GetByteValue(nIndexBase + i, 3 + n, 3);
			}
		}

		nRecordLen = nIndex - RECORD_HEAD_LEN ;
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		m_nP2pAmcResDescCount = 0;
		CPropFruReco::UpdateData(isUpdateData);
		SetDwValue(8, m_nP2pAmcResDescCount);

		nIndex = sizeof(RecordHeader);
		for(int i = 0; nIndex < m_pCarrierP2pConRecord->m_header.m_RecordLength + RECORD_HEAD_LEN; i++)
		{
			m_nP2pAmcResDescCount++;
			pDesc = (P2pAmcResDesc*)&pByte[nIndex];

			AddP2pAmcResDesc(i, 1, pDesc->m_P2pCount);

			SetDwValue(nIndexBase + i, 0, pDesc->m_ResourceId.m_ResourceType);
			SetDwValue(nIndexBase + i, 1, pDesc->m_ResourceId.m_ResourceId);
			SetDwValue(nIndexBase + i, 2, pDesc->m_P2pCount);

			nIndex += pDesc->m_P2pCount * sizeof(P2pPortDesc) + 2;
			P2pPortDesc* pPort = pDesc->m_P2pPortDesc;
			for(int n = 0; n < pDesc->m_P2pCount; n++, pPort++)
			{
				DWORD word = AS_UINT16(pPort->m_Bytes[1], pPort->m_Bytes[0]);

				SetDwValue(nIndexBase + i, 3 + n, 0, (word>>5)&0x1f);	//Local Port
				SetDwValue(nIndexBase + i, 3 + n, 1, (word)&0x1f);	//Remote Port
				SetDwValue(nIndexBase + i, 3 + n, 2, pPort->m_RemoteResourceId.m_ResourceType);
				SetDwValue(nIndexBase + i, 3 + n, 3, pPort->m_RemoteResourceId.m_ResourceId);
			}
		}
		SetDwValue(8, m_nP2pAmcResDescCount);
	}

	m_FruDataLen = nIndex;

	return TRUE;
}

void CPropCarrierPtPConn::AddPortDesc(CMFCPropertyGridPropertyEx* pParent, int nStart, int nCount)
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_P2pPortDescStr, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str, 0, TRUE);
		pParent->AddSubItem(pDescGroup);

		for(int j = 0; j < m_MaxP2pPortDescField; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pP2pPortDescField[j], pDescGroup);
		}
	}
}

void CPropCarrierPtPConn::AddP2pAmcResDesc(int nStart, int nCount, int nPortDescCount)
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_P2pAmcResourceDescStr, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str);
		AddSubItem(pDescGroup);

		for(int j = 0; j < m_MaxP2pAmcResDescField; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pP2pAmcResDescField[j], pDescGroup);
		}

		if(nPortDescCount)
		{
			AddPortDesc(pDescGroup, 0, nPortDescCount);
		}
	}
}

void CPropCarrierPtPConn::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{

	if(wcscmp(pChangedProp->GetName(),L"P2pP AMC Resource Desc Count") == 0)	//P2pP AMC Resource Desc Count
	{
		DWORD dwNew = pChangedProp->GetByteValue();
		DWORD dwOlder = olderValue.ulVal;
		if(dwNew > dwOlder)
		{
			AddP2pAmcResDesc(dwOlder, dwNew - dwOlder, g_nDefaultPortCount);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_P2pAmcResourceDescStr, i+1);
				RemoveSubItem(str);
			}
		}
	}
	else if(wcscmp(pChangedProp->GetName(), L"PortCount") == 0)	//PortCount
	{
		CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)pChangedProp->GetParent();
		DWORD dwNew = pChangedProp->GetByteValue();
		DWORD dwOlder = olderValue.ulVal;
		if(dwNew > dwOlder)
		{
			AddPortDesc(pParent, dwOlder, dwNew - dwOlder);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_P2pPortDescStr, i+1);
				pParent ->RemoveSubItem(str);
			}
		}
	}

	CMFCPropertyGridPropertyEx::OnPropertyChanged(pParentProp, pChangedProp, olderValue);
}