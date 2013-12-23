#include "StdAfx.h"
#include "PropCarrierPowerPolicy.h"

LPCTSTR g_PowerPolicyDescriptor = L"Power Policy Descriptor";

CPropCarrierPowerPolicy::CPropCarrierPowerPolicy(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"MicroTCA Carrier Power Policy record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pCarriePowerPolicy = (CarriePowerPolicy*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Number of PMs"			, 0x01, 0x01, 0x02, L"%d", L"This field specifies the number of PMs (N) defined in this record."));
	//m_FieldArray.Add(new CFieldNum(L"Power Policy Descriptors", 0x00, 0x00, 0x00, L"%d", L"An array of descriptors with one for each PM. The size varies depending on the number of PMs (N) and the number of Power Channel feeds (C) to each PM. The format is described in Table 3-24, MicroTCA Carrier Power Policy descriptor."));


	m_pField[0] = new CFieldNum(L"PM Site Number"				, 1, 1, 2, L"%d", L"Identifies the PM described in this descriptor.");
	m_pField[1] = new CFieldNum(L"Maximum Current Override"	, 255, 100, 0xFFFF, L"%d", L"LS Byte first. This field optionally defines maximum available Payload Power. in 1110th Amps, that the PM is restricted to deliver, due to external power source, cabling, power supply efficiency, or other constraints. A value of FFFFh shall indicate that no such restriction has been set.");
	m_pField[2] = new CFieldCombo(L"PM Role", _T("AdvancedMC"), L"0=Primary PM|1=Redundant PM|255=Not specified", L"00h  = Primary PM \n01h = Redundant PM \nFFh = PM Role is not specified \nAll other values are reserved.");
	m_pField[3] = new CFieldNum(L"Power Channel Count"		, 16, 10, 18, L"%d", L"This field contains a count of the number of Power Channels listed in this descriptor (C)");

	UpdateData(FALSE);
}


CPropCarrierPowerPolicy::~CPropCarrierPowerPolicy(void)
{
	for(int i = 0; i < m_FieldCount; i++)
	{
		delete m_pField[i];
	}
}

BOOL CPropCarrierPowerPolicy::UpdateData(BOOL isUpdateData)
{
	const static int BASE_INDEX = 9;
	int nRecordLen = 0;
	if(isUpdateData)
	{
		m_pCarriePowerPolicy->m_NumberOfPMs = GetByteValue(8);

		nRecordLen = sizeof(CarriePowerPolicy) - RECORD_HEAD_LEN + (m_pCarriePowerPolicy->m_NumberOfPMs - 1) * sizeof(PowerPolicyDesc);

		PowerPolicyDesc* pDesc = m_pCarriePowerPolicy->m_Desc;
		for(int i = 0; i < m_pCarriePowerPolicy->m_NumberOfPMs; i++ , pDesc++)
		{
			pDesc->m_PmSiteNumber	= GetByteValue(BASE_INDEX + i, 0);
			GetDwValue(BASE_INDEX + i, 1, pDesc->m_MaxCurrent, sizeof(pDesc->m_MaxCurrent));
			pDesc->m_PmRole = GetByteValue(BASE_INDEX + i, 2);
			pDesc->m_PowerChannelCount = GetByteValue(BASE_INDEX + i, 3);
		}
			
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);

		SetDwValue(8, m_pCarriePowerPolicy->m_NumberOfPMs);

		AddSubObj(0, m_pCarriePowerPolicy->m_NumberOfPMs);

		PowerPolicyDesc* pDesc = m_pCarriePowerPolicy->m_Desc;
		for(int i = 0; i < m_pCarriePowerPolicy->m_NumberOfPMs; i++ , pDesc++)
		{
			SetDwValue(i + BASE_INDEX, 0, pDesc->m_PmSiteNumber);
			SetDwValue(i + BASE_INDEX, 1, AS_UINT16(pDesc->m_MaxCurrent[1], pDesc->m_MaxCurrent[0]));
			SetDwValue(i + BASE_INDEX, 2, pDesc->m_PmRole);
			SetDwValue(i + BASE_INDEX, 3, pDesc->m_PowerChannelCount);
		}
	}

	m_FruDataLen = sizeof(CarriePowerPolicy) + (m_pCarriePowerPolicy->m_NumberOfPMs - 1) * sizeof(PowerPolicyDesc);

	return TRUE;
}

void CPropCarrierPowerPolicy::AddSubObj(int nStart, int nCount)
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_PowerPolicyDescriptor, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str, 0, 1, L"An array of descriptors with one for each PM. The size varies depending on the number of PMs (N) and the number of Power Channel feeds (C) to each PM. The format is described in Table 3-24, MicroTCA Carrier Power Policy descriptor.");

		AddSubItem(pDescGroup);

		for(int j = 0; j < m_FieldCount; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pField[j], pDescGroup);
		}
	}
}


void CPropCarrierPowerPolicy::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	if(wcscmp(pChangedProp->GetName(), L"Number of PMs") == 0)	//Number of PMs
	{
		DWORD dwNew = (BYTE)pChangedProp->GetDwValue();
		DWORD dwOlder = m_pCarriePowerPolicy->m_NumberOfPMs;
		if(dwNew > dwOlder)
		{
			AddSubObj(dwOlder, dwNew - dwOlder);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_PowerPolicyDescriptor, i+1);
				RemoveSubItem(str);
			}
		}
	}
	else if(wcscmp(pChangedProp->GetName(), L"Maximum Current Override") == 0)	//Maximum Current Override
	{

	}

	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}
