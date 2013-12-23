#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropCarrierManagerIpLink : public CPropFruReco
{
	CarrierIpLink* m_pCarrierIpLink;
	const static DWORD m_dwMaxSize =  sizeof(CarrierIpLink);
public:
	CPropCarrierManagerIpLink(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropCarrierManagerIpLink(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	//virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

