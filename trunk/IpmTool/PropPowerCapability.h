#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropPowerCapability : public CPropFruReco
{
	const static DWORD m_dwMaxSize =  sizeof(PowerCapability);
	PowerCapability* m_pPowerCapability;
public:
	CPropPowerCapability(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int RecordLen);
	~CPropPowerCapability(void);

	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
};

