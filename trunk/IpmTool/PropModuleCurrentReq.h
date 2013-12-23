#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropModuleCurrentReq : public CPropFruReco
{
	const static DWORD m_dwMaxSize =  sizeof(CurrentReq);
	CurrentReq* m_pCurrentReq;
public:
	CPropModuleCurrentReq(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int RecordLen);
	~CPropModuleCurrentReq(void);

	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
};

