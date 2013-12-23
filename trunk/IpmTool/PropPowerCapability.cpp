#include "StdAfx.h"
#include "PropPowerCapability.h"


CPropPowerCapability::CPropPowerCapability(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Power Module Capability record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pPowerCapability = (PowerCapability*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Maximum Current Output", 0x01, 0x01, 0xFFFF, L"%d", L" LS-byte first. This field holds the maximum Payload Power current output from the supply, in 1110 of Amps."));

	UpdateData(FALSE);
}


CPropPowerCapability::~CPropPowerCapability(void)
{
}

BOOL CPropPowerCapability::UpdateData(BOOL isUpdateData)
{
	int nRecordLen = 0;
	DWORD dw = 0;

	if(isUpdateData)
	{
		dw = GetDwValue(8);
		m_pPowerCapability->m_MaxCurrentOutPut[0] = dw & 0x000000FF;
		m_pPowerCapability->m_MaxCurrentOutPut[1] = (dw>>8) & 0x000000FF;
		nRecordLen = sizeof(PowerCapability) - RECORD_HEAD_LEN ;
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);

		dw = AS_UINT16(m_pPowerCapability->m_MaxCurrentOutPut[1], m_pPowerCapability->m_MaxCurrentOutPut[0]);
		SetDwValue(8, dw);
	}

	m_FruDataLen = sizeof(PowerCapability);

	return TRUE;
}
