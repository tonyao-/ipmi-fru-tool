#include "StdAfx.h"
#include "PropCarrierActAndPm.h"

LPCTSTR g_lpActAndCurDesc = L"Module Activation and Current Descriptor";
CPropCarrierActAndPm::CPropCarrierActAndPm(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Carrier Activation and Power Management record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pCarrierActAndPm = (CarrierActAndPm*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Readiness", 01, 01, 0xFF, L"%d", L"Allowance for Module Activation Readiness, This field contains the number of seconds after Carrier Manager start-up that Modules have to transition to state M3 and maintain their power-up sequence position."));
	m_FieldArray.Add(new CFieldNum(L"Descriptor Count", 16, 01, 20, L"%d", L"Module Activation and Current Descriptor Count. This contains a count of the number of entries (M) in this record."));
	

	m_pField[0] = new CFieldCombo(L"Site Type", _T("AdvancedMC"), L"4=Cooling Unit|7=AdvancedMC|10=MCH|11=Power Module", L"Cooling Unit = 04h.\nAdvancedMC = 07h.\nMCH = 0Ah.\nPower Module = 0Bh.\nOEM Module = C0h-CFh.\nUnknown = 0xFFh.\nReserved =Other values.\n");
	m_pField[1] = new CFieldNum(L"Site Number"	, 0x01, 0x01,   12, L"%d", L"This field, when combined with the Site Type, forms a MicroTCA Carrier Local Address of the FRU.");
	m_pField[2] = new CFieldNum(L"Power Channel Number", 01, 01, 0xFF, L"%d", L"This field specifies the Power Channel to which the corresponding FRU is connected.");
	m_pField[3] = new CFieldNum(L"Maximum Channel Current", 01, 01, 0xFF, L"%d", L"This field contains the maximum Payload Power current, in tenths of Amps, that can be routed to the Module identified by MicroTCA Carrier Local Address. The power connector pin as well as Backplane trace implementation determine this maximum current.");
	m_pField[4] = new CFieldCombo(L"Activation Control", _T("Carrier Manager"), L"0=System Manager|1=Carrier Manager|2=Shelf Manager", L"10b = The Shelf Manager activates this FRU automatically, the FRU M-state transitions to M3 when the Carrier Manager receives the \"Set FRU Activation (Activate)\" command from the Shelf Manager \
			\n01b = The Carrier Manager activates this FRU, and it proceeds to M3 00b = The System Manager activates this FRU, the FRU M-state remains in M2	\
			\n00b = The System Manager activates this FRU, the FRU M-state remains in M2 until the Carrier Manager receives the \"Set FRU Activation (Activate)\" command from the System Manager"
			);
	m_pField[5] = new CFieldNum(L"Delay", 10, 01, 0x1F, L"%d", L"Delay Before Next Power On. Tenths of a second to delay before powering up any other FRU after powering this FRU.");
	
	m_pField[6] = new CFieldCombo(L"Deactivation Control", _T("Carrier Manager"), L"0=System Manager|1=Carrier Manager|2=Shelf Manager", L"10b = The Shelf Manager Deactivation this FRU automatically, the FRU M-state transitions to M3 when the Carrier Manager receives the \"Set FRU Activation (Activate)\" command from the Shelf Manager \
			\n01b = The Carrier Manager Deactivation this FRU, and it proceeds to M3 00b = The System Manager activates this FRU, the FRU M-state remains in M2	\
			\n00b = The System Manager Deactivation this FRU, the FRU M-state remains in M2 until the Carrier Manager receives the \"Set FRU Activation (Activate)\" command from the System Manager"
			);

	UpdateData(FALSE);
}


CPropCarrierActAndPm::~CPropCarrierActAndPm(void)
{
	for(int i = 0; i < m_FieldCount; i++)
	{
		delete m_pField[i];
	}
}

void CPropCarrierActAndPm::AddSubObj(int nStart, int nCount)
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_lpActAndCurDesc, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str, 0, 1, L"This is an array of activation and current descriptors for each AdvancedMC, MCH, PM, CU and OEM Module implemented on the MicroTCA Carrier. Each descriptor is 6 bytes in size and follows the format found in Table 3-26.");

		AddSubItem(pDescGroup);

		for(int j = 0; j < m_FieldCount; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pField[j], pDescGroup);
		}
	}
}

BOOL CPropCarrierActAndPm::UpdateData(BOOL isUpdateData)
{
	const static int nIndexBase = 10;
	int nRecordLen = 0;
	if(isUpdateData)
	{
		m_pCarrierActAndPm->m_allowActTime = GetByteValue(8);
		m_pCarrierActAndPm->m_DescCount = GetByteValue(9);

		nRecordLen = sizeof(CarrierActAndPm) - RECORD_HEAD_LEN + (m_pCarrierActAndPm->m_DescCount - 1) * sizeof(CarrierActAndPmDesc);

		CarrierActAndPmDesc* pDesc = m_pCarrierActAndPm->m_Desc;
		for(int i = 0; i < m_pCarrierActAndPm->m_DescCount; i++ , pDesc++)
		{
			pDesc->m_SiteType		= GetByteValue(nIndexBase + i, 0);
			pDesc->m_SiteNum		= GetByteValue(nIndexBase + i, 1);
			pDesc->m_PowerChanNum	= GetByteValue(nIndexBase + i, 2);
			pDesc->m_MaxCurrent		= GetByteValue(nIndexBase + i, 3);
			pDesc->m_ActCtrl		= GetByteValue(nIndexBase + i, 4);
			pDesc->m_Delay			= GetByteValue(nIndexBase + i, 5);
			pDesc->m_DeactCtrl		= GetByteValue(nIndexBase + i, 6);
		}

		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);

		SetDwValue(8, m_pCarrierActAndPm->m_allowActTime);
		SetDwValue(9, m_pCarrierActAndPm->m_DescCount);

		AddSubObj(0, m_pCarrierActAndPm->m_DescCount);

		CarrierActAndPmDesc* pDesc = m_pCarrierActAndPm->m_Desc;
		for(int i = 0; i < m_pCarrierActAndPm->m_DescCount; i++ , pDesc++)
		{
			SetDwValue(i + nIndexBase, 0, pDesc->m_SiteType		);
			SetDwValue(i + nIndexBase, 1, pDesc->m_SiteNum		);
			SetDwValue(i + nIndexBase, 2, pDesc->m_PowerChanNum	);
			SetDwValue(i + nIndexBase, 3, pDesc->m_MaxCurrent	);
			SetDwValue(i + nIndexBase, 4, pDesc->m_ActCtrl		);
			SetDwValue(i + nIndexBase, 5, pDesc->m_Delay		);
			SetDwValue(i + nIndexBase, 6, pDesc->m_DeactCtrl	);
		}
	}

	m_FruDataLen = sizeof(CarrierActAndPm) + (m_pCarrierActAndPm->m_DescCount - 1) * sizeof(CarrierActAndPmDesc);

	return TRUE;
}

void CPropCarrierActAndPm::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	if(wcscmp(pChangedProp->GetName(), L"Descriptor Count") == 0)	//Descriptor Count
	{
		DWORD dwNew = pChangedProp->GetByteValue();
		DWORD dwOlder = olderValue.ulVal;
		if(dwNew > dwOlder)
		{
			AddSubObj(dwOlder, dwNew - dwOlder);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_lpActAndCurDesc, i+1);
				RemoveSubItem(str);
			}
		}
	}

	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}
