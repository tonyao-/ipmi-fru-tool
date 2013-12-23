#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropFruCtrl : public CPropFruReco
{
	FruCtrlCap* m_pFruCtrlCap;
	const static DWORD m_dwMaxSize = 20;

public:
	CPropFruCtrl(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropFruCtrl(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

