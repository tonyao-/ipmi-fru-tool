#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

#define MAX_PORT_DESC_PER_AMCRES_DESC	30
#define MAX_P2PAMC_RES_DESC 2

class CPropCarrierPtPConn :	public CPropFruReco
{
	const static int m_MaxP2pAmcResDescField = 3;
	CField*		m_pP2pAmcResDescField[m_MaxP2pAmcResDescField];

	const static int m_MaxP2pPortDescField = 4;
	CField*		m_pP2pPortDescField[m_MaxP2pPortDescField];

	int m_nP2pAmcResDescCount;
	CarrierP2pConRecord* m_pCarrierP2pConRecord;
public:
	const static DWORD m_dwMaxSize =  sizeof(CarrierP2pConRecord) + sizeof(P2pAmcResDesc) * MAX_P2PAMC_RES_DESC  + sizeof(P2pPortDesc) * MAX_PORT_DESC_PER_AMCRES_DESC * MAX_P2PAMC_RES_DESC;

	CPropCarrierPtPConn(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropCarrierPtPConn(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	//void AddSubObj(int nStart, int nCount);
	void AddPortDesc(CMFCPropertyGridPropertyEx* pParent, int nStart, int nCount);
	void AddP2pAmcResDesc(int nStart, int nCount, int nPortDescCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

