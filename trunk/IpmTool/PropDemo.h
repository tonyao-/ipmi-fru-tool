#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropCarrierPowerPolicy : public CPropFruReco
{
	CarriePowerPolicy* m_pCarriePowerPolicy;
	const static DWORD m_dwMaxSize =  sizeof(CarrierInfo) + sizeof(SlotEntry) * 20;

	const static int m_FieldCount = 4;
	CField*		m_pField[m_FieldCount];
public:
	CPropCarrierPowerPolicy(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropCarrierPowerPolicy(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	void AddSubObj(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

