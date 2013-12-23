#pragma once
#include "propfrureco.h"
class CPropCarrierActAndPm : public CPropFruReco
{
	CarrierActAndPm* m_pCarrierActAndPm;
	const static DWORD m_dwMaxSize =  sizeof(CarrierActAndPm) + sizeof(CarrierActAndPmDesc) * 20;

	const static int m_FieldCount = 7;
	CField*		m_pField[m_FieldCount];
public:
	CPropCarrierActAndPm(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropCarrierActAndPm(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	void AddSubObj(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

