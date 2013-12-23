
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CListBox
{
// ����
public:
	COutputList();

// ʵ��
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputEdit : public CRichEditCtrl
{
// ����
public:
	COutputEdit();

// ʵ��
public:
	virtual ~COutputEdit();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();
public:
	afx_msg void OnEditCopy();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// ����
	WCHAR m_FruDataStrBuf[4096];
public:
	COutputWnd();

	void UpdateFonts();

	void ShowData(BYTE* pData, int nLen);
// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	//COutputList m_wndOutputBuild;
	COutputEdit m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnEditCopy();

	DECLARE_MESSAGE_MAP()
};

