#pragma once

#include "PropFruReco.h"
#include "MFCPropertyGridCtrlEx.h"
#include "FruInfoCm.h"

#define MAX_GUID_COUNT		5
#define MAX_LINK_DESC_COUNT	20
#define MAX_CHNL_DESC_COUNT	20
class CPropAmcPtpConn : public CPropFruReco
{
	const static int m_MaxAmcChnl= 4;
	CField*		m_pAmcChnlDescField[m_MaxAmcChnl];

	const static int m_MaxAmcLinkField = 5;
	CField*		m_pAmcLinkField[m_MaxAmcLinkField];

	const static int m_MaxField = 3;
	CField*		m_pField[m_MaxField];

	CField*		m_pGuidField;

	int m_nP2pAmcResDescCount;
	//AmcLinkDesc* m_pAmcLinkDesc;

public:
	const static DWORD m_dwMaxSize =  RECORD_HEAD_LEN + MAX_GUID_COUNT * sizeof(Guid) + sizeof(AmcLinkDesc) * MAX_LINK_DESC_COUNT + sizeof(AmcChnlDesc) * MAX_CHNL_DESC_COUNT + 3;

	CPropAmcPtpConn(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen);
	~CPropAmcPtpConn(void);

protected:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);
	//void AddSubObj(int nStart, int nCount);
	void AddGuidDesc(int nStart, int nCount);
	void AddChnlDesc(int nStart, int nCount);
	void AddLinkDesc(int nStart, int nCount);
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
};

