#pragma once

#include "PropCommonHeader.h"
#include "MFCPropertyGridCtrlEx.h"

class CPropFruReco : public CPropFruInfo
{
public:
	CPropFruReco(CMFCPropertyGridCtrlEx* pGrid, LPCTSTR lpName, DWORD dwMaxLen, BYTE* pRecord, int RecordLen);
	~CPropFruReco(void);

	void SetEndofList(BOOL isEndofList);
protected:
	virtual BOOL UpdateData(BOOL isUpdateData);
	virtual BOOL UpdateData(int nRecordLen, BOOL isUpdateData);
};

