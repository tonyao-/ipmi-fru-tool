#include "StdAfx.h"
#include "PropFruCtrl.h"

CPropFruCtrl::CPropFruCtrl(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"FRU Control Capabilities Record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pFruCtrlCap = (FruCtrlCap*)&m_pFruData[10];

	m_FieldArray.Add(new CFieldCombo(L"Issue Diagnostic Interrupt", _T("FALSE"), L"0=FALSE|1=TRUE", L""));
	m_FieldArray.Add(new CFieldCombo(L"Graceful Reboot", _T("FALSE"), L"0=FALSE|1=TRUE", L""));
	m_FieldArray.Add(new CFieldCombo(L"Warm Reset", _T("FALSE"), L"0=FALSE|1=TRUE", L""));

	UpdateData(FALSE);
}


CPropFruCtrl::~CPropFruCtrl(void)
{
}

BOOL CPropFruCtrl::UpdateData(BOOL isUpdateData)
{
	const static int BASE_INDEX = 8;
	int nRecordLen = 0;
	int i = 8;
	if(isUpdateData)
	{
		m_pFruCtrlCap->m_DiagnosticInterrupt = GetByteValue(i++);
		m_pFruCtrlCap->m_GracefulReboot = GetByteValue(i++);
		m_pFruCtrlCap->m_WarmReset = GetByteValue(i++);

		nRecordLen = 5 + 1;
			
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		i = 8;
		CPropFruReco::UpdateData(isUpdateData);

		SetDwValue(i++, m_pFruCtrlCap->m_DiagnosticInterrupt);
		SetDwValue(i++, m_pFruCtrlCap->m_GracefulReboot);
		SetDwValue(i++, m_pFruCtrlCap->m_WarmReset);
	}

	m_FruDataLen = 5 + 1 + RECORD_HEAD_LEN;

	return TRUE;
}

void CPropFruCtrl::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	CMFCPropertyGridPropertyEx::OnPropertyChanged( pParentProp, pChangedProp, olderValue);
}
