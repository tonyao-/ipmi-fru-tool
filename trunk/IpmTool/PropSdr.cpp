#include "StdAfx.h"
#include "PropSdr.h"

CPropSdr::CPropSdr(CMFCPropertyGridCtrlEx* pGrid, LPCTSTR lpName, DWORD dwMaxSize, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, lpName, dwMaxSize, pRecord, nRecordLen)
{
	m_pSdrHeader = (SdrHeader*)&m_pFruData[m_nSdrDataOffset];

	m_FieldArray.Add(new CFieldNum(L"Record ID"		, m_pSdrHeader->m_Id		, m_pSdrHeader->m_Id		, m_pSdrHeader->m_Id		, L"0x%04x"		, L"The Record ID is used by the Sensor Data Repository device for record organization and access. It is not related to the sensor ID. "));
	m_FieldArray.Add(new CFieldNum(L"SDR Version"	, m_pSdrHeader->m_Version	, m_pSdrHeader->m_Version	, m_pSdrHeader->m_Version	, L"0x%x"	, L"Version of the S ensor Model specification that this record is compatible with. 51h  for this specification.  BCD encoded with bits 7:4 holding  the Least Significant digit of the revision and bits 3:0 holding the Most Significant bits. "));
	m_FieldArray.Add(new CFieldNum(L"Record Type"	, m_pSdrHeader->m_Type		, m_pSdrHeader->m_Type		, m_pSdrHeader->m_Type		, L"%d"		, L""));
	m_FieldArray.Add(new CFieldNum(L"Record Length"	, m_pSdrHeader->m_Len		, m_pSdrHeader->m_Len		, m_pSdrHeader->m_Len		, L"%d"		, L"Number of remaining record bytes following. "));

}

CPropSdr::~CPropSdr(void)
{
}

BOOL CPropSdr::UpdateData(DWORD dwRecordLen, BOOL isUpdateData)
{
	int i = 0;
	int nRecordLen = 0;
	if(isUpdateData)
	{
		i = 8;
		SetDwValue(11, dwRecordLen);

		m_pSdrHeader->m_Id		= GetByteValue(i++);
		m_pSdrHeader->m_Version = GetByteValue(i++);
		m_pSdrHeader->m_Type	= GetByteValue(i++);
		m_pSdrHeader->m_Len		= (uint8)dwRecordLen;

		nRecordLen = m_pSdrHeader->m_Len + SDR_HEADER_LEN + sizeof(RecordHeader) - RECORD_HEAD_LEN;

		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);
		i = 8;
		SetDwValue(i++, m_pSdrHeader->m_Id);
		SetDwValue(i++, m_pSdrHeader->m_Version);
		SetDwValue(i++, m_pSdrHeader->m_Type);
		SetDwValue(i++, m_pSdrHeader->m_Len);
	}

	m_FruDataLen = nRecordLen + SDR_HEADER_LEN;

	return TRUE;
}
