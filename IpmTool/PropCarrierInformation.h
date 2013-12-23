#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropCarrierInformation : public CPropFruReco
{
	const static int m_FieldCount = 6;
	CField*		m_pField[m_FieldCount];
	CarrierInfo* m_pCarrierInfo;
public:
	const static DWORD m_dwMaxSize =  sizeof(CarrierInfo) + sizeof(SlotEntry) * 20;
	CPropCarrierInformation(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int RecordLen);
	~CPropCarrierInformation(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	void AddSubObj(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

