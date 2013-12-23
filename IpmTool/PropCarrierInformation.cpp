#include "StdAfx.h"
#include "PropCarrierInformation.h"
#include "Common_Define.h"

LPCTSTR g_SlotEntryName = L"SlotEntry";
LPCTSTR g_SlotEntryCount = L"Carrier Orientation/Slot Entry Count";

CPropCarrierInformation::CPropCarrierInformation(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Carrier FRU Information Record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pCarrierInfo = (CarrierInfo*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Carrier Number", 01, 01, 0xFF, L"%d", L"Contains a unique value from 1 to 16 used to identify this MicroTCA Carrier. A value of FFh indicates the Carrier Number is not specified."));
	m_FieldArray.Add(new CField(g_SlotEntryCount, L"Contains a unique value from 1 to 16 used to identify this MicroTCA Carrier. A value of FFh indicates the Carrier Number is not specified."
		, 2
		, new CFieldCombo(L"[7] Carrier Orientation"	, L"Left To Right", L"0=Left To Right|1=Bottom To Top", L"[7] - Carrier Orientation.\n0b = Slot Numbers increment from left to right and Tier Numbers bottom to top. \nlb = Slot Numbers increment from bottom to top and her Numbers increment left to right")
		, new CFieldNum(L"[6:0] Slot Entry Count"	, 2, 02,   18, L"%d", L"[6:0]Indicates the number of entries (M) in the Carrier Information record for the MicroTCA Carrier. There is one Slot Entry for each Slot in a MicroTCA Carrier including PM,  CU, MCH, OEM and AdvancedMC Slots.")
		));

	m_pField[0] = new CFieldNum(L"Site Number"	, 0x01, 0x01,   12, L"%d", L"This field, when combined with the Site Type, forms a MicroTCA Carrier Local Address of the FRU.");
	m_pField[1] = new CFieldCombo(L"Site Type", _T("AdvancedMC"), L"4=Cooling Unit|7=AdvancedMC|10=MCH|11=Power Module", L"Cooling Unit = 04h.\nAdvancedMC = 07h.\nMCH = 0Ah.\nPower Module = 0Bh.\nOEM Module = C0h-CFh.\nUnknown = 0xFFh.\nReserved =Other values.\n");
	m_pField[2] = new CFieldNum(L"Slot Number"	, 0x01, 0x00, 0x20, L"0x%02X", L"This is the MicroTCA Carrier relative physical Slot Number for the Slot.");
	m_pField[3] = new CFieldNum(L"Tier Number"	, 0x01, 0x00, 0xFF, L"0x%02X", L"This is the MicroTCA Carrier relative Tier Number for the Slot.");
	m_pField[4] = new CFieldNum(L"Slot Origin Y", 0x00, 0x00, 0xFFFF, L"%d", L"Indicate the distance from ejector handle corner of the Slot to the bottom edge of the MicroTCA Carrier in a standard vertical orientation or left edge of the MicroTCA Carrier in a horizontal orientation, least significant byte first. It be in 1 millimeter increments, rounded to the nearest millimeter.");
	m_pField[5] = new CFieldNum(L"Slot Origin X", 0x00, 0x00, 0xFFFF, L"%d", L"Indicate the distance from ejector handle corner of the Slot to the left edge of the MicroTCA Carrier in a standard vertical orientation or bottom edge of the MicroTCA Carrier in a horizontal orientation, least significant byte first. It be in 1 millimeter increments, rounded to the nearest millimeter.");

	UpdateData(FALSE);
}


CPropCarrierInformation::~CPropCarrierInformation(void)
{
	for(int i = 0; i < m_FieldCount; i++)
	{
		delete m_pField[i];
	}
}

BOOL CPropCarrierInformation::UpdateData(BOOL isUpdateData)
{
	int nRecordLen = 0;
	if(isUpdateData)
	{
		m_pCarrierInfo->m_Number = GetByteValue(8);
		m_pCarrierInfo->m_Orientation = GetByteValue(9, 0);
		m_pCarrierInfo->m_SlotEntryCount = GetByteValue(9, 1);

		nRecordLen = sizeof(CarrierInfo) - RECORD_HEAD_LEN + (m_pCarrierInfo->m_SlotEntryCount - 1) * sizeof(SlotEntry);

		SlotEntry* pSlotEntry = m_pCarrierInfo->m_SlotEntry;
		for(int i = 0; i < m_pCarrierInfo->m_SlotEntryCount; i++ , pSlotEntry++)
		{
			pSlotEntry->m_SiteNum	= GetByteValue(10 + i, 0);
			pSlotEntry->m_SiteType	= GetByteValue(10 + i, 1);
			pSlotEntry->m_SlotNum	= GetByteValue(10 + i, 2);
			pSlotEntry->m_TierNum	= GetByteValue(10 + i, 3);

			GetDwValue(10 + i, 4, pSlotEntry->m_SlotOriginY, sizeof(pSlotEntry->m_SlotOriginY));
			GetDwValue(10 + i, 5, pSlotEntry->m_SlotOriginX, sizeof(pSlotEntry->m_SlotOriginX));
		}

		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);

		SetDwValue(8, m_pCarrierInfo->m_Number);
		SetDwValue(9, 0, m_pCarrierInfo->m_Orientation);
		SetDwValue(9, 1, m_pCarrierInfo->m_SlotEntryCount);

		AddSubObj(0, m_pCarrierInfo->m_SlotEntryCount);

		SlotEntry* pSlotEntry = m_pCarrierInfo->m_SlotEntry;
		for(int i = 0; i < m_pCarrierInfo->m_SlotEntryCount; i++, pSlotEntry++)
		{
			SetDwValue(i + 10, 0, pSlotEntry->m_SiteNum);
			SetDwValue(i + 10, 1, pSlotEntry->m_SiteType);
			SetDwValue(i + 10, 2, pSlotEntry->m_SlotNum);
			SetDwValue(i + 10, 3, pSlotEntry->m_TierNum);

			SetDwValue(i + 10, 4, AS_UINT16(pSlotEntry->m_SlotOriginY[1],pSlotEntry->m_SlotOriginY[0]));
			SetDwValue(i + 10, 5, AS_UINT16(pSlotEntry->m_SlotOriginX[1],pSlotEntry->m_SlotOriginX[0]));
		}
	}

	m_FruDataLen = sizeof(CarrierInfo) + (m_pCarrierInfo->m_SlotEntryCount - 1) * sizeof(SlotEntry);

	return TRUE;
}

void CPropCarrierInformation::AddSubObj(int nStart, int nCount)
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_SlotEntryName, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str, 0, TRUE);

		AddSubItem(pDescGroup);

		for(int j = 0; j < m_FieldCount; j++)
		{
			CMFCPropertyGridPropertyEx::CreateProp(m_pField[j], pDescGroup);
		}
	}
}

void CPropCarrierInformation::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{

	if(wcscmp(pChangedProp->GetName(), L"Carrier Number") == 0)	//Carrier Number
	{
		DWORD dw = pChangedProp->GetDwValue();
		if(dw > 16 && dw != 0xFF)
		{
			dw = 0xFF;
			SetDwValue(8, dw);
		}
	}
	else if(wcscmp(pChangedProp->GetName(), g_SlotEntryCount) == 0
		|| wcscmp(pChangedProp->GetName(), L"[6:0] Slot Entry Count") == 0)	//[6:0] Slot Entry Count
	{
		DWORD dwNew = GetByteValue(9, 1);
		DWORD dwOlder = m_pCarrierInfo->m_SlotEntryCount;
		if(dwNew > dwOlder)
		{
			AddSubObj(dwOlder, dwNew - dwOlder);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_SlotEntryName, i+1);
				RemoveSubItem(str);
			}
		}
	}
	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}

#if 0
void CPropCarrierInformation::InitGrid(CMFCPropertyGridCtrlEx* pGrid)
{
	CreatePropHeard(pGrid, PRID_CarrierInfo, L"Carrier FRU Information Record",  L"For the Carrier FRU Information record, the value 22h shall be used.");

	const NumFieldTemp* pStrReco = g_CarrierInformationRecord;
	CMFCPropertyGridPropertyNum* pField = NULL;

	for(int i = 0; i < 1; i++, pStrReco++)
	{
		pField = new CMFCPropertyGridPropertyNum(pStrReco);

		m_pGroup->AddSubItem(pField);
	}

	CMFCPropertyGridPropertyEx* pSubItem = new CMFCPropertyGridPropertyEx(L"Carrier Orientation/Slot Entry Count", 0, TRUE);
	m_pGroup->AddSubItem(pSubItem);
	pStrReco = &g_CarrierInformationRecord[1];
	for(int i = 1; i < 3; i++, pStrReco++)
	{
		pField = new CMFCPropertyGridPropertyNum(pStrReco);

		pSubItem->AddSubItem(pField);
	}

	AddSubItem(0, m_pCarrierInfo->m_SlotEntryCount);

	Update();
	pGrid->Update();
}

void CPropCarrierInformation::AddSubItem(int nStart, int nCount) const
{
	for(int i = nStart; i < nStart + nCount; i++)
	{
		CString str;
		str.Format(L"%s %d", g_SlotEntryName, i+1);
		CMFCPropertyGridPropertyEx* pDescGroup = new CMFCPropertyGridPropertyEx(str);
		m_pGroup->AddSubItem(pDescGroup);

		const NumFieldTemp* pReco = g_SlotEntries;
		for(int j = 0; j < sizeof(g_SlotEntries) / sizeof(NumFieldTemp); j++, pReco++)
		{
			CMFCPropertyGridPropertyEx* pField = NULL;
			if(j == 1)
			{
				pField = new CMFCPropertyGridPropertyCombo(g_SiteTypeField.m_strName
					, g_SiteTypeField.m_strDefault
					, g_SiteTypeField.m_strDwString
					, g_SiteTypeField.m_strDesc
					);
			}
			else
			{
				pField = new CMFCPropertyGridPropertyNum(pReco);
			}
			
			pDescGroup->AddSubItem(pField);
		}
	}
}

void CPropCarrierInformation::Update(BOOL isFlag)
{
	m_pCarrierInfo->m_Number =(BYTE) GetDwValue(8);
	m_pCarrierInfo->m_Orientation = GetByteValue(9, 0);
	m_pCarrierInfo->m_SlotEntryCount = GetByteValue(9, 1);

	m_pHeader->m_RecordLength = sizeof(CarrierInfo) - RECORD_HEAD_LEN + (m_pCarrierInfo->m_SlotEntryCount - 1) * sizeof(SlotEntry);

	SlotEntry* pSlotEntry = m_pCarrierInfo->m_SlotEntry;
	for(int i = 0; i < m_pCarrierInfo->m_SlotEntryCount; i++ , pSlotEntry++)
	{
		pSlotEntry->m_SiteNum	= GetByteValue(10 + i, 0);
		pSlotEntry->m_SiteType	= GetByteValue(10 + i, 1);
		pSlotEntry->m_SlotNum	= GetByteValue(10 + i, 2);
		pSlotEntry->m_TierNum	= GetByteValue(10 + i, 3);

		GetSubDwValue(10 + i, 4, pSlotEntry->m_SlotOriginY, sizeof(pSlotEntry->m_SlotOriginY));
		GetSubDwValue(10 + i, 5, pSlotEntry->m_SlotOriginX, sizeof(pSlotEntry->m_SlotOriginX));
	}
	
	CFruRecord::Update(isFlag);	
}

void CPropCarrierInformation::OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue)
{

	if(wcscmp(pProp->GetName(), g_CarrierInformationRecord[0].m_strName) == 0)	//Carrier Number
	{
		DWORD dw = pProp->GetDwValue();
		if(dw > 16 && dw != 0xFF)
		{
			dw = 0xFF;
			SetValue(8, dw);
		}
	}
	else if(wcscmp(pProp->GetName(), g_CarrierInformationRecord[2].m_strName) == 0
		|| wcscmp(pProp->GetName(), L"Carrier Orientation/Slot Entry Count") == 0)	//[6:0] Slot Entry Count
	{
		DWORD dwNew = GetByteValue(9, 1);
		DWORD dwOlder = m_pCarrierInfo->m_SlotEntryCount;
		if(dwNew > dwOlder)
		{
			AddSubItem(dwOlder, dwNew - dwOlder);
		}
		else
		{
			for(DWORD i = dwNew; i < dwOlder; i++)
			{
				CString str;
				str.Format(L"%s %d", g_SlotEntryName, i+1);
				m_pGroup->RemoveSubItem(str);
			}
		}
	}

	Update();
	m_pGrid->AdjustLayout();
}

#endif