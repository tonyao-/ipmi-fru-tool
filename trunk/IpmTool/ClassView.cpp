
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "IpmTool.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassView::~CClassView()
{
	for(int i = 0; i < m_NodeArray.GetCount(); i++)
	{
		delete m_NodeArray[i];
	}

	m_NodeArray.RemoveAll();
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_COMMAND(ID_CARRIER_FRUREPO_CREATE, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnAddFruInfo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_COMMAND(ID_CARRIERMANAGERIPLINK, OnAddFruInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

#if VIEW_TREE
	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;      // 未能创建
	}
#else
	if (!m_FruInfoTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;      // 未能创建
	}
#endif
	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
#if VIEW_TREE
	FillClassView();
#else
	FillFruInfo();
#endif
	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

#if VIEW_TREE
void CClassView::FillClassView()
{
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("FakeApp 类"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndClassView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndClassView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndClassView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);
}
#else
void CClassView::FillFruInfo()
{
	int i = 0;
	m_NodeArray.Add(new CTreeNodeData(0, L"FRU设备"));
	m_NodeArray.Add(new CTreeNodeData(1, L"CM"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(CM_Dev_ID), IDR_CARRIER_FRUREPO));
	//m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(MCH_1_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(1, L"MCMC1"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(MCH_1_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(MCH_1_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"MCMC2"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(MCH_2_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(MCH_2_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"PM1"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(PM_1_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(PM_1_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"PM2"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(PM_2_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(PM_2_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC1"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_1_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_1_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC2"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_2_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_2_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC3"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_3_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_3_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC4"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_4_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_4_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC5"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_5_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_5_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC6"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_6_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_6_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC7"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_7_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_7_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC8"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_8_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_8_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC9"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_9_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_9_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC10"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_10_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_10_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC11"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_11_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_11_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"AMC12"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(AMC_12_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo", FruAttr_getById(AMC_12_Dev_ID), IDR_MENU_CONFIGINFO));
	m_NodeArray.Add(new CTreeNodeData(1, L"CU"));
	m_NodeArray.Add(new CTreeNodeData(2, L"FruRepo", FruAttr_getById(CU_1_Dev_ID), IDR_CARRIER_FRUREPO));
	m_NodeArray.Add(new CTreeNodeData(2, L"ConfigInfo"));//, FruAttr_getById(CU_1_Dev_ID), IDR_MENU_CONFIGINFO)
	m_FruInfoTree.FillTreeNodeData(NULL, m_NodeArray, i, NULL);
	m_FruInfoTree.Expend(L"FRU设备", TRUE);
	m_FruInfoTree.Expend(L"CM", TRUE);
}
#endif

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
#if VIEW_TREE
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
#else
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_FruInfoTree;
	m_FruInfoTree.OnContextMenu(point);
#endif
}
void CClassView::OnAddFruInfo()
{
	OnAddFruInfo(0);
}
void CClassView::OnAddFruInfo(UINT id)
{
	int i = 0;
	HTREEITEM hTreeItem = m_FruInfoTree.GetSelectedItem();
	CTreeNodeData* pNodeData = NULL;

	if(hTreeItem)
	{
		pNodeData = (CTreeNodeData*)m_FruInfoTree.GetItemData(hTreeItem);
		if(pNodeData)
		{
			m_NodeArray.Add(new CTreeNodeData(pNodeData->m_Level+1, pNodeData->m_pName));
			m_FruInfoTree.DeleteAllItems();
			m_FruInfoTree.FillTreeNodeData(NULL, m_NodeArray, i, NULL);
			m_FruInfoTree.UpdateData();
			m_FruInfoTree.ExpendAll(m_FruInfoTree.GetRootItem(), TRUE);
		}
	}

}
void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
#if VIEW_TREE
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
#else
	m_FruInfoTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
#endif
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnClassDefinition()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnClassProperties()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
#if VIEW_TREE
	m_wndClassView.GetWindowRect(rectTree);
#else
	m_FruInfoTree.GetWindowRect(rectTree);
#endif
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

#if VIEW_TREE
	m_wndClassView.SetFocus();
#else
	m_FruInfoTree.SetFocus();
#endif
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

#if VIEW_TREE
	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
#else
	m_FruInfoTree.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);
#endif
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}
