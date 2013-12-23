
#pragma once
#include "FruRepo.h"
#include "DlgConfigInfoSetting.h"

typedef CArray<CFruRepoData*, CFruRepoData*> TFruRepoDataArray;
class CFruRepoDataArray : public TFruRepoDataArray
{
	DECLARE_DYNAMIC(CFruRepoDataArray)
public:
	CFruRepoDataArray(){};
	~CFruRepoDataArray();

public:
	CFruRepoData* GetFruRepoData(uint8 devid);
	virtual void Serialize(CArchive& ar);
};


#include "FruRepo.h"
#define CONFIG_OBJCOMBO 0

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

#define m_wndPropList m_GridCtrl
class CPropertiesWnd : public CDockablePane
{
// 构造
public:
	CPropertiesWnd();

	void AdjustLayout();

// 特性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
protected:
	void InitFruRepo();
	void InitConfigInfo();

protected:
	uint8	m_DeviceId;
	CFruRepoDataArray m_FruRepoDataArray;
	CFruRepoDataArray m_ConfigInfoArray;
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	//CMFCPropertyGridCtrl m_wndPropList;
	
	CFruRepo	m_GridCtrl;

	FruInfo 		m_CmFruInfo;
	RecordHeader*	m_pCmFruRecordArray[MAX_FRU_RECORD];
	uint8			m_CmFruRepoBuff[MAX_FRU_SIZE];

// 实现
public:
	virtual ~CPropertiesWnd();
	void SetFruInfo(BYTE* pFruData = NULL, int nLen = 0);
	int GetFruInfo(BYTE* pFruData, int nBufLen);
	virtual void Serialize(CArchive& ar);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnPropertiesCm();
	afx_msg void OnUpdatePropertiesCm(CCmdUI* pCmdUI);
	afx_msg void OnPropertiesOutput();
	afx_msg void OnUpdatePropertiesOutput(CCmdUI* pCmdUI);

	afx_msg void OnAddBoardInfoAera();
	afx_msg void OnUpdateAddBoardInfoAera(CCmdUI* pCmdUI);
	afx_msg void OnAddProductInfoAera();
	afx_msg void OnUpdateAddProductInfoAera(CCmdUI* pCmdUI);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg LRESULT OnCreateFruRepo(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnOpenFruRepo(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnCreateConfig(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnOpenConfig(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
	void OnCreateCmFruRecord(CByteArray& createFlag);
	void OnCreateMmcFruRecord(CByteArray& createFlag);
	void OnCreatePmFruRecord(CByteArray& createFlag);
	void OnCreateCuFruRecord(CByteArray& createFlag);

	void OnCreateMchConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId);
	void OnCreateMmcConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId);
	void OnCreatePmConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId);
	void OnCreateCuConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId);
};

