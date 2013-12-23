#include "StdAfx.h"
#include "PropModuleCurrentReq.h"


CPropModuleCurrentReq::CPropModuleCurrentReq(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Module Current requirement", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pCurrentReq = (CurrentReq*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Current Draw", 0x01, 0x01, 0xFF, L"%d", L"This field holds the Payload Power (PWR) requirement of the Module given as current requirement in units of 0.1A at 12V. (This equals the value of the power in W divided by 1.2.)"));

	UpdateData(FALSE);
}


CPropModuleCurrentReq::~CPropModuleCurrentReq(void)
{
}

BOOL CPropModuleCurrentReq::UpdateData(BOOL isUpdateData)
{
	int nRecordLen = 0;

	if(isUpdateData)
	{
		m_pCurrentReq->m_Current = GetByteValue(8);
		nRecordLen = sizeof(CurrentReq) - RECORD_HEAD_LEN ;
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);
		SetDwValue(8, m_pCurrentReq->m_Current);
	}

	m_FruDataLen = sizeof(CurrentReq);

	return TRUE;
}
