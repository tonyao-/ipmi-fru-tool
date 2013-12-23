#include "StdAfx.h"
#include "PropFruReco.h"



CPropFruReco::CPropFruReco(CMFCPropertyGridCtrlEx* pGrid, LPCTSTR lpName, DWORD dwMaxLen, BYTE* pRecord, int RecordLen)
	: CPropFruInfo(pGrid, lpName, dwMaxLen, pRecord, RecordLen)
{
	m_FieldArray.Add(new CFieldNum(L"Record Type ID"	, 0xC0, 0xC0, 0xC0, L"0x%02X", L"For all records defined in this specification a value of C0h (OEM) shall be used."));
	m_FieldArray.Add(new CFieldNum(L"Version"		, 0x02, 0x02, 0x02, L"0x%02X", L"End of List/Version."));
	m_FieldArray.Add(new CFieldNum(L"Record Length"	, 0x00, 0x00, 0x00, L"%d"));
	m_FieldArray.Add(new CFieldNum(L"Record Checksum", 0x00, 0x00, 0x00, L"0x%02X"));
	m_FieldArray.Add(new CFieldNum(L"Header Checksum", 0x00, 0x00, 0x00, L"0x%02X"));
	m_FieldArray.Add(new CFieldNum(L"Manufacturer ID", 0x00315A, 0x00315A, 0x00315A, L"0x%06X", L"LS byte first. Write as the three byte ID assigned to PICMG. For this specification, the value 12634 (00315Ah) shall be used."));
	m_FieldArray.Add(new CFieldNum(L"PICMG Record ID", 0x20, 0x20, 0x20, L"0x%02X", L"For the FRU Information Partition record, the value 20h shall be used."));
	m_FieldArray.Add(new CFieldNum(L"Record Format Version"		, 0x00, 0x00, 0x00, L"0x%02X", L"For this specification, the value 00h shall be used."));
}


CPropFruReco::~CPropFruReco(void)
{
}

BOOL CPropFruReco::UpdateData(BOOL isUpdateData)
{
	RecordHeader* pHeader = (RecordHeader*)m_pFruData;
	return UpdateData(pHeader->m_RecordLength, isUpdateData);
}

BOOL CPropFruReco::UpdateData(int nRecordLen, BOOL isUpdateData)
{
	RecordHeader* pHeader = (RecordHeader*)m_pFruData;

	pHeader->m_RecordLength = nRecordLen;

	if(isUpdateData)
	{
		pHeader->m_RecordId	= GetByteValue(0);
		pHeader->m_Version	= GetByteValue(1);
		GetDwValue(5, pHeader->m_ManufacturerID, 3);
	
		pHeader->m_PicmgRecordID		= GetByteValue(6);
		pHeader->m_RecordFormatVersion	= GetByteValue(7);

		pHeader->m_RecordChecksum = CheckSum_Get(&pHeader->m_ManufacturerID[0], pHeader->m_RecordLength);
		pHeader->m_HeaderChecksum = CheckSum_Get((uint8*)pHeader, RECORD_HEAD_LEN - 1);

		SetDwValue(2, pHeader->m_RecordLength);
		SetDwValue(3, pHeader->m_RecordChecksum);
		SetDwValue(4, pHeader->m_HeaderChecksum);
	}
	else
	{
		m_pGrid->AddProperty(this);

		CreateProp(L"");

		SetDwValue(0, pHeader->m_RecordId);
		SetDwValue(1, m_pFruData[1]);
		SetDwValue(2, pHeader->m_RecordLength);
		SetDwValue(3, pHeader->m_RecordChecksum);
		SetDwValue(4, pHeader->m_HeaderChecksum);
		SetDwValue(5, AS_UINT32(0, pHeader->m_ManufacturerID[2], pHeader->m_ManufacturerID[1], pHeader->m_ManufacturerID[0]));
		SetDwValue(6, pHeader->m_PicmgRecordID);
		SetDwValue(7, pHeader->m_RecordFormatVersion);
	}

	return TRUE;
}


void CPropFruReco::SetEndofList(BOOL isEndofList)
{
	RecordHeader* pHeader = (RecordHeader*)m_pFruData;

	pHeader->m_EndOfList = isEndofList;
	pHeader->m_RecordChecksum = CheckSum_Get(&pHeader->m_ManufacturerID[0], pHeader->m_RecordLength);
	pHeader->m_HeaderChecksum = CheckSum_Get((uint8*)pHeader, RECORD_HEAD_LEN - 1);

	SetDwValue(1, m_pFruData[1]);
	SetDwValue(3, pHeader->m_RecordChecksum);
	SetDwValue(4, pHeader->m_HeaderChecksum);
};