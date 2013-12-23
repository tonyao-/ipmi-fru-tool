#include "StdAfx.h"
#include "PropAmcPtpConn.h"

LPCTSTR g_lpGuidTitle = L"GUID";
LPCTSTR g_lpAmcChnlDescTitle = L"AMC Channel Descriptor";
LPCTSTR g_lpAmcLinkDescTitle = L"AMC Link Descriptor";
LPCTSTR g_lpPortDesc = L"A Port Number of 31 (1Fh) indicates that the corresponding Lane is not included in the Channel defined by an AMC Channel Descriptor. ";

CPropAmcPtpConn::CPropAmcPtpConn(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"AMC Point-to-Point Connectivity record", m_dwMaxSize, pRecord, nRecordLen)
{
	int i = 0;
	m_FieldArray.Add(new CFieldNum(L"OEM GUID Count", 0x00, 0x00, MAX_GUID_COUNT, L"%d", L"Point-to-Point AMC Resource Descriptor List Count"));
	m_pGuidField  = new CFieldStr(L"Lane 0 Port Number"	, L"                ", 16, 16, L"AAAAAAAAAAAAAAAA", L"________________", L"1234567890abcdefABCDEF", L"The Port within this AMC resource that functions as Lane 0 of this AMC Channel.");

	i = 0;
	m_pAmcChnlDescField[i++] = new CFieldNum(L"Lane 0 Port Number"	, 0x01, 0x00, 0x1F, L"%d", g_lpPortDesc);
	m_pAmcChnlDescField[i++] = new CFieldNum(L"Lane 1 Port Number"	, 0x01, 0x00, 0x1F, L"%d", g_lpPortDesc);
	m_pAmcChnlDescField[i++] = new CFieldNum(L"Lane 2 Port Number"	, 0x01, 0x00, 0x1F, L"%d", g_lpPortDesc);
	m_pAmcChnlDescField[i++] = new CFieldNum(L"Lane 3 Port Number"	, 0x01, 0x00, 0x1F, L"%d", g_lpPortDesc);

	i = 0;
	m_pAmcLinkField[i++] = new CField(L"AMC Link Designator", L"Identifies the AMC Channel and the Ports within the AMC Channel that are being described"
		, 5
		,  new CFieldNum(L"AMC Channel ID"	, 0x01, 0x00, 0x7F, L"%d", L"Identifies an AMC Channel Descriptor defined in an AMC Point-to-Point Connectivity record.")
		,  new CFieldCombo(L"Lane 0 Bit Flag", _T("Included"), L"1=Included|0=Excluded")
		,  new CFieldCombo(L"Lane 1 Bit Flag", _T("Included"), L"1=Included|0=Excluded")
		,  new CFieldCombo(L"Lane 2 Bit Flag", _T("Included"), L"1=Included|0=Excluded")
		,  new CFieldCombo(L"Lane 3 Bit Flag", _T("Included"), L"1=Included|0=Excluded")
		);
	m_pAmcLinkField[i++] = new CFieldCombo(L"AMC Link Type", _T("AMC.1 PCI Express"), L"2=AMC.1 PCI Express|3=AMC.1 PCI Express Advanced Switching|5=AMC.2 Ethernet|6=AMC.4 Serial RapidlO|7=AMC.3 Storage|240=GUID Definition", L"Identifies the AMC.x subsidiary specification that governs this description or identifies the description as proprietary; see Table 3-21, \"AMC Link Type.\"");
	m_pAmcLinkField[i++] = new CFieldNum(L"AMC Link Type Extension"	, 0x01, 0x00, 0x0F, L"%d", L"Identifies the subset of a subsidiary specification that is implemented and is defined entirely by the subsidiary specification identified in the Link Type field.");
	m_pAmcLinkField[i++] = new CFieldNum(L"Link Grouping ID"	, 0x01, 0x00, 0x0F, L"%d", L"Indicates whether the Ports of this Channel are operated together with Ports in other Channels. A value of 0 always indicates a Single-Channel Link. A common, non-zero Link Grouping ID in multiple Link Descriptors indicates that the Ports covered by those Link Descriptors must be operated together. A unique non-zero Link Grouping ID also indicates Single-Channel Link.");
	m_pAmcLinkField[i++] = new CFieldCombo(L"AMC Asymmetric Match"	, _T("Exact match"), L"0=Exact match|1=Matches with '10b'|2=Matches with '01b'");

	i = 0;
	m_pField[i++] = new CFieldCombo(L"ResourceType"	, _T("AMC"), L"1=AMC|0=on-Carrier device ID");
	m_pField[i++] = new CFieldNum(L"ResourceID"		, 0x01, 0x00, 0x0F, L"%d", L"On-Carrier device ID or AMC Site Number");
	m_pField[i++] = new CFieldNum(L"AMC Channel Descriptor Count"		, 0x01, 0x01, MAX_CHNL_DESC_COUNT, L"%d", L"On-Carrier device ID or AMC Site Number");


	UpdateData(FALSE);
}


CPropAmcPtpConn::~CPropAmcPtpConn(void)
{
	delete m_pGuidField;

	for(int i = 0; i < m_MaxAmcChnl; i++)
	{
		delete m_pAmcChnlDescField[i];
	}

	for(int i = 0; i < m_MaxAmcLinkField; i++)
	{
		delete m_pAmcLinkField[i];
	}

	for(int i = 0; i < m_MaxField; i++)
	{
		delete m_pField[i];
	}
}

void CPropAmcPtpConn::AddGuidDesc(int nStart, int nCount)
{
	CMFCPropertyGridPropertyEx* pDescGroup = GetItemByName(g_lpGuidTitle);
	if(NULL == pDescGroup)
	{
		pDescGroup = new CMFCPropertyGridPropertyEx(g_lpGuidTitle);
		AddSubItem(pDescGroup);
	}

	(nCount == 0)? pDescGroup->Expand(FALSE) : pDescGroup->Expand(TRUE);

	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_lpGuidTitle, i+1);

		CMFCPropertyGridPropertyEx* pProp = CMFCPropertyGridPropertyEx::CreateProp(m_pGuidField, pDescGroup);
		pProp->SetName(str);
	}
}


void CPropAmcPtpConn::AddChnlDesc(int nStart, int nCount)
{
	CMFCPropertyGridPropertyEx* pDescGroup = GetItemByName(g_lpAmcChnlDescTitle);
	if(NULL == pDescGroup)
	{
		pDescGroup = new CMFCPropertyGridPropertyEx(g_lpAmcChnlDescTitle);
		AddSubItem(pDescGroup);
	}

	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_lpAmcChnlDescTitle, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup1 = new CMFCPropertyGridPropertyEx(str, NULL, TRUE);

		pDescGroup->AddSubItem(pDescGroup1);

		for(int j = 0; j < m_MaxAmcChnl; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pAmcChnlDescField[j], pDescGroup1);
		}
	}
}

void CPropAmcPtpConn::AddLinkDesc(int nStart, int nCount)
{
	CMFCPropertyGridPropertyEx* pDescGroup = GetItemByName(g_lpAmcLinkDescTitle);
	if(NULL == pDescGroup)
	{
		pDescGroup = new CMFCPropertyGridPropertyEx(g_lpAmcLinkDescTitle);
		AddSubItem(pDescGroup);
	}

	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_lpAmcLinkDescTitle, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup1 = new CMFCPropertyGridPropertyEx(str, NULL, TRUE);
		pDescGroup1->AllowEdit(FALSE);

		pDescGroup->AddSubItem(pDescGroup1);

		for(int j = 0; j < m_MaxAmcLinkField; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pAmcLinkField[j], pDescGroup1);
		}
	}
}

BOOL CPropAmcPtpConn::UpdateData(BOOL isUpdateData)
{
	int nCount = 0;
	int i = 0;
	m_FruDataLen = 8;
	int nRecordLen = 0;
	BYTE* pByte = (BYTE*)m_pFruData;

	if(isUpdateData)
	{
		m_FruDataLen = 10;
		
		pByte[m_FruDataLen++] = GetByteValue(8);
		nCount = pByte[m_FruDataLen-1];
		for(int j = 0; j < nCount; j++)
		{
			GetByArrayValue(9, j, &pByte[m_FruDataLen], sizeof(Guid));
			m_FruDataLen += sizeof(Guid);
		}

		RecordType* pRecordType = (RecordType*)&pByte[m_FruDataLen++];
		pRecordType->m_ResourceType = GetByteValue(10);
		pRecordType->m_ResourceId	= GetByteValue(11);
		pByte[m_FruDataLen++]		= GetByteValue(12);
		nCount = pByte[m_FruDataLen-1];
		for(int i = 0; i < nCount; i++)
		{
			DWORD value = 0;
			value = GetDwValue(13, i, 3) & 0x0000001F;
			value <<= 5;
			value |= GetDwValue(13, i, 2) & 0x0000001F;
			value <<= 5;
			value |= GetDwValue(13, i, 1) & 0x0000001F;
			value <<= 5;
			value |= GetDwValue(13, i, 0) & 0x0000001F;

			memcpy(&pByte[m_FruDataLen], &value, 3);

			m_FruDataLen += 3;
		}

		AmcLinkDesc* pDesc = (AmcLinkDesc*)&pByte[m_FruDataLen];
		for(int i = 0; i < nCount; i++)
		{
			pDesc = (AmcLinkDesc*)&pByte[m_FruDataLen];

			DWORD* value = (DWORD*)pDesc->m_Bytes;

			//Get 23:20 AMC Link Type Extension. 
			*value = GetDwValue(14, i, 2) & 0x0000000F;
			*value <<= (20 - 12);

			//Get 19:12	AMC Link Type.
			*value |= GetDwValue(14, i, 1) & 0x000000FF;
			*value <<= 12;

			//Get 11:0  AMC Link Designator. 
			AmcLinkDesignator* pLinkDes = (AmcLinkDesignator*)value;
			pLinkDes->m_ChannelId = (BYTE)GetDwValue(14, i, 0, 0);
			pLinkDes->m_Lane0Bit = (BYTE)GetDwValue(14, i, 0, 1);
			pLinkDes->m_Lane1Bit = (BYTE)GetDwValue(14, i, 0, 2);
			pLinkDes->m_Lane2Bit = (BYTE)GetDwValue(14, i, 0, 3);
			pLinkDes->m_Lane3Bit = (BYTE)GetDwValue(14, i, 0, 4);


			//Link Grouping ID.
			pDesc->m_GroupId = GetByteValue(14, i, 3);
			pDesc->m_AsyMatch = GetByteValue(14, i, 4);

			m_FruDataLen += 5;
		}

		CPropFruReco::UpdateData(m_FruDataLen - RECORD_HEAD_LEN, isUpdateData);
	}
	else
	{
		m_FruDataLen = 10;
		int nCount = pByte[m_FruDataLen++];
		CPropFruReco::UpdateData(isUpdateData);

		SetDwValue(8, nCount);

		AddGuidDesc(0, nCount);
		for(int j = 0; j < nCount; j++)
		{
			SetByArrayValue(9, j, &pByte[m_FruDataLen], sizeof(Guid));
			m_FruDataLen += sizeof(Guid);
		}

		i = 0;
		RecordType* pRecordType = (RecordType*)&pByte[m_FruDataLen++];
		CMFCPropertyGridPropertyEx::CreateProp(m_pField[i++], this);
		SetDwValue(10, pRecordType->m_ResourceType);

		CMFCPropertyGridPropertyEx::CreateProp(m_pField[i++], this);
		SetDwValue(11, pRecordType->m_ResourceId);

		CMFCPropertyGridPropertyEx::CreateProp(m_pField[i++], this);
		nCount = pByte[m_FruDataLen++];
		SetDwValue(12, nCount);

		AddChnlDesc(0, nCount);
		for(int i = 0; i < nCount; i++)
		{
			DWORD value = AS_UINT32(0, pByte[m_FruDataLen + 2], pByte[m_FruDataLen + 1], pByte[m_FruDataLen]);
			SetDwValue(13, i, 0, value & 0x0000001F);
			SetDwValue(13, i, 1, (value>>5) & 0x0000001F);
			SetDwValue(13, i, 2, (value>>10) & 0x0000001F);
			SetDwValue(13, i, 3, (value>>15) & 0x0000001F);

			m_FruDataLen += 3;
		}

		AddLinkDesc(0, nCount);
		AmcLinkDesc* pDesc = (AmcLinkDesc*)&pByte[m_FruDataLen];
		for(int i = 0; i < nCount; i++)
		{
			pDesc = (AmcLinkDesc*)&pByte[m_FruDataLen];

			DWORD dw = 0;
			DWORD value = AS_UINT32(0, pDesc->m_Bytes[2], pDesc->m_Bytes[1], pDesc->m_Bytes[0]);

			//Get 11:0  AMC Link Designator. 
			dw = value & 0x00000FFF;
			AmcLinkDesignator* pLinkDes = (AmcLinkDesignator*)&dw;
			SetDwValue(14, i, 0, 0, pLinkDes->m_ChannelId);
			SetDwValue(14, i, 0, 1, pLinkDes->m_Lane0Bit);
			SetDwValue(14, i, 0, 2, pLinkDes->m_Lane1Bit);
			SetDwValue(14, i, 0, 3, pLinkDes->m_Lane2Bit);
			SetDwValue(14, i, 0, 4, pLinkDes->m_Lane3Bit);

			//Get 19:12	AMC Link Type.
			SetDwValue(14, i, 1, (value>>12) & 0x000000FF);

			//Get 23:20 AMC Link Type Extension. 
			SetDwValue(14, i, 2, (value>>20) & 0x0000000F);

			//Link Grouping ID.
			SetDwValue(14, i, 3, pDesc->m_GroupId);
			SetDwValue(14, i, 4, pDesc->m_AsyMatch);

			m_FruDataLen += 5;

		}
	}

	return TRUE;
}

void CPropAmcPtpConn::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	if(wcscmp(pChangedProp->GetName(),L"OEM GUID Count") == 0)	//OEM GUID Count
	{
		DWORD dwNew = pChangedProp->GetByteValue();
		DWORD dwOlder = olderValue.ulVal;
		if(dwNew > dwOlder)
		{
			AddGuidDesc(dwOlder, dwNew - dwOlder);
		}
		else
		{
			CMFCPropertyGridPropertyEx* pProp = GetItemByName(g_lpGuidTitle);
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_lpGuidTitle, i+1);
				pProp->RemoveSubItem(str);
			}
		}
	}
	else if(wcscmp(pChangedProp->GetName(), L"AMC Channel Descriptor Count") == 0)	//PortCount
	{
		CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)pChangedProp->GetParent();
		DWORD dwNew = pChangedProp->GetByteValue();
		DWORD dwOlder = olderValue.ulVal;
		if(dwNew > dwOlder)
		{
			AddChnlDesc(dwOlder, dwNew - dwOlder);
			AddLinkDesc(dwOlder, dwNew - dwOlder);
		}
		else
		{
			CMFCPropertyGridPropertyEx* pChnlDescProp = GetItemByName(g_lpAmcChnlDescTitle);
			CMFCPropertyGridPropertyEx* pLinkDescProp = GetItemByName(g_lpAmcLinkDescTitle);

			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_lpAmcChnlDescTitle, i+1);
				pChnlDescProp->RemoveSubItem(str);

				str.Format(L"%s %d", g_lpAmcLinkDescTitle, i+1);
				pLinkDescProp->RemoveSubItem(str);
			}
		}
	}
	
	CMFCPropertyGridPropertyEx::OnPropertyChanged(pParentProp, pChangedProp, olderValue);
}
