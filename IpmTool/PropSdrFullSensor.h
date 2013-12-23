#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"
#include "PropSdr.h"

class CPropSdrFullSensor : public CPropSdr
{
	FullSensorRecord* m_pFullSensorRecord;
	const static DWORD m_dwMaxSize =  sizeof(FullSensorRecord) + 10;
	int m_nStrFieldIndex;
public:
	CPropSdrFullSensor(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropSdrFullSensor(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	void AddSubObj(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

