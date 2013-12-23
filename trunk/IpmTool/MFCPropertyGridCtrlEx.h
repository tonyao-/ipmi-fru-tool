#pragma once

#include "MFCPropertyGridPropertyEx.h"

#define WM_PROPERTY_CHANGED		WM_USER + 0x100

class CMFCPropertyGridCtrlEx : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CMFCPropertyGridCtrlEx)
protected:
	CMFCPropertyGridPropertyEx* m_pGroup;

public:
	CMFCPropertyGridCtrlEx(void);
	~CMFCPropertyGridCtrlEx(void);

	virtual int InsertProperty(CMFCPropertyGridPropertyEx* pProp, LPCTSTR lpName, BOOL bRedraw = TRUE, BOOL bAdjustLayout = TRUE);

	virtual CMFCPropertyGridPropertyEx* GetPropertyByName(LPCTSTR lpName);
	virtual BOOL DeletePropertyByName(LPCTSTR lpName);

	virtual void Serialize(CArchive& ar);
	virtual void Update(BOOL isFlag = TRUE){};
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue);

protected:

	DECLARE_MESSAGE_MAP()

};

