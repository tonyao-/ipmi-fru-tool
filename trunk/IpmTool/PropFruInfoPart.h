#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"


class CPropFruInfoPart : public CPropFruReco
{
	FruInfoParti* m_pFruInfoPart;
	CFieldNum*	m_pFieldNum[2];

public:
	const static DWORD m_dwMaxSize = sizeof(FruInfoParti) + sizeof(PartDesc);

	CPropFruInfoPart(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int RecordLen);
	~CPropFruInfoPart(void);

public:
	void AddPartitionDesc(int nStart, int nCount);
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
	 
};

