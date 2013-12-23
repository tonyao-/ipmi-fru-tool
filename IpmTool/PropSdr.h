#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

class CPropSdr : public CPropFruReco
{
	const static int m_nSdrDataOffset = 10;
public:
	CPropSdr(CMFCPropertyGridCtrlEx* pGrid, LPCTSTR lpName, DWORD dwMaxSize, BYTE* pRecord, int nRecordLen);
	~CPropSdr(void);

protected:
	SdrHeader* m_pSdrHeader;
protected:
	virtual BOOL UpdateData(DWORD dwRecordLen, BOOL isUpdateData = TRUE);
};

