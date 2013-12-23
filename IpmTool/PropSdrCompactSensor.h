#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"
#include "PropSdr.h"

class CPropSdrCompactSensor : public CPropSdr
{
	CompactSensorRecord* m_pCompactSensorRecord;
	const static DWORD m_dwMaxSize =  sizeof(CompactSensorRecord);
	int m_nStrFieldIndex;
	CompactSensorRecord* m_pDefaultData;
public:
	CPropSdrCompactSensor(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropSdrCompactSensor(void);

protected:
	BYTE* GetDefaultData();
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	void AddSubObj(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

