
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "IpmTool.h"
#include "DlgFruRepoSettingCm.h"
#include "DlgFruRepoSettingMmc.h"
#include "DlgFruRepoSettingPm.h"
#include "DlgFruRepoSettingCu.h"
#include "DlgConfigInfoSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CFruRepoDataArray, TFruRepoDataArray)
CFruRepoData* CFruRepoDataArray::GetFruRepoData(uint8 devid)
{
	for(int i = 0; i < GetCount(); i++)
	{
		if(GetAt(i)->m_nDeviceId == devid)
		{
			return GetAt(i);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
CFruRepoDataArray::~CFruRepoDataArray()
{
	for(int i = 0; i < GetCount(); i++)
	{
		delete GetAt(i);
	}
}

void CFruRepoDataArray::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//ar << nCount;
	}
	else
	{
		//ar >> nCount;
	}

	for(int i = 0; i < GetCount(); i++)
	{
		GetAt(i)->Serialize(ar);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar


CPropertiesWnd::CPropertiesWnd()
{
	m_DeviceId = RESERVED_DEVID;
	InitFruRepo();
	InitConfigInfo();
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
/*
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_COMMAND(ID_BUTTON_CM, OnPropertiesCm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CM, OnUpdatePropertiesCm)
	ON_COMMAND(ID_PROPERTIES_OUTPUT, OnPropertiesOutput)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_OUTPUT, OnUpdatePropertiesOutput)
	ON_COMMAND(ID_BTN_BOARDINFO, OnAddBoardInfoAera)
	ON_UPDATE_COMMAND_UI(ID_BTN_BOARDINFO, OnUpdateAddBoardInfoAera)
	ON_COMMAND(ID_BTN_PRODUCTINFO, OnAddProductInfoAera)
	ON_UPDATE_COMMAND_UI(ID_BTN_PRODUCTINFO, OnUpdateAddProductInfoAera)
*/
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(ID_CREATE_FRUREPO, OnCreateFruRepo)
	ON_MESSAGE(ID_OPEN_FRUREPO, OnOpenFruRepo)
	ON_MESSAGE(ID_CREATE_CONFIGINFO, OnCreateConfig)
	ON_MESSAGE(ID_OPEN_CONFIGINFO, OnOpenConfig)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序
void CPropertiesWnd::InitFruRepo()
{
	int nIndex = 0;
	CString strName;
	const static int m_nMaxCmSize = 1024;
	const static int m_nMaxMcmcSize = 256;
	const static int m_nMaxAmcSize = 256;
	const static int m_nMaxPmSize = 256;
	const static int m_nMaxCuSize = 256;

	m_FruRepoDataArray.Add(new CFruRepoData(CM_Dev_ID, m_nMaxCmSize, L"CM"));

	for(int i = PM_1_Dev_ID; i <= PM_4_Dev_ID; i++)
	{
		strName.Format(L"PM%d", ++nIndex);
		m_FruRepoDataArray.Add(new CFruRepoData(i, m_nMaxPmSize, strName));
	}

	m_FruRepoDataArray.Add(new CFruRepoData(MCH_1_Dev_ID, m_nMaxMcmcSize, L"MCMC1"));
	m_FruRepoDataArray.Add(new CFruRepoData(MCH_2_Dev_ID, m_nMaxMcmcSize, L"MCMC2"));

	nIndex = 0;
	for(int i = AMC_1_Dev_ID; i <= AMC_12_Dev_ID; i++)
	{
		strName.Format(L"AMC%d", ++nIndex);
		m_FruRepoDataArray.Add(new CFruRepoData(i, m_nMaxAmcSize, strName));
	}
	m_FruRepoDataArray.Add(new CFruRepoData(CU_1_Dev_ID, m_nMaxCuSize, L"CU"));
}

void CPropertiesWnd::InitConfigInfo()
{
	int nIndex = 0;
	CString strName;
	const static int m_nMaxCmSize = 1024;
	const static int m_nMaxMcmcSize = 1024;
	const static int m_nMaxAmcSize = 1024;
	const static int m_nMaxPmSize = 1024;
	const static int m_nMaxCuSize = 1024;

	m_ConfigInfoArray.Add(new CFruRepoData(CM_Dev_ID, m_nMaxCmSize, L"CM"));

	for(int i = PM_1_Dev_ID; i <= PM_4_Dev_ID; i++)
	{
		strName.Format(L"PM%d", ++nIndex);
		m_ConfigInfoArray.Add(new CFruRepoData(i, m_nMaxPmSize, strName));
	}

	m_ConfigInfoArray.Add(new CFruRepoData(MCH_1_Dev_ID, m_nMaxMcmcSize, L"MCMC1"));
	m_ConfigInfoArray.Add(new CFruRepoData(MCH_2_Dev_ID, m_nMaxMcmcSize, L"MCMC2"));

	nIndex = 0;
	for(int i = AMC_1_Dev_ID; i <= AMC_12_Dev_ID; i++)
	{
		strName.Format(L"AMC%d", ++nIndex);
		m_ConfigInfoArray.Add(new CFruRepoData(i, m_nMaxAmcSize, strName));
	}
	m_ConfigInfoArray.Add(new CFruRepoData(CU_1_Dev_ID, m_nMaxCuSize, L"CU"));
}

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

#if CONFIG_OBJCOMBO
	m_wndObjectCombo.GetWindowRect(&rectCombo);

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
#endif
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

#if CONFIG_OBJCOMBO
	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}

	m_wndObjectCombo.AddString(_T("应用程序"));
	m_wndObjectCombo.AddString(_T("属性窗口"));
	m_wndObjectCombo.SetCurSel(0);
#endif

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	SetFruInfo();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	static BOOL bFlag = True;
	
	if(bFlag)
	{
		m_wndPropList.ExpandAll();
	}
	else
	{
		m_wndPropList.ExpandAll(FALSE);
	}
	bFlag = !bFlag;
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	//m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
	FruInfo_Reset(&m_CmFruInfo);
	FruInfoMcmc_SetDefault(&m_CmFruInfo);

	SetFruInfo(m_CmFruInfo.m_Buffer, m_CmFruInfo.m_UsedBufLen);
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
	FruInfo_Reset(&m_CmFruInfo);
	FruInfoEmmc_SetDefault(&m_CmFruInfo);

	SetFruInfo(m_CmFruInfo.m_Buffer, m_CmFruInfo.m_UsedBufLen);
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
	FruInfo_Reset(&m_CmFruInfo);
	FruInfoMmc_SetDefault(&m_CmFruInfo);

	SetFruInfo(m_CmFruInfo.m_Buffer, m_CmFruInfo.m_UsedBufLen);
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnPropertiesCm()
{
	// TODO: 在此处添加命令处理程序代码
	FruInfo_Reset(&m_CmFruInfo);
	FruInfoCm_SetDefault(&m_CmFruInfo);

	SetFruInfo(m_CmFruInfo.m_Buffer, m_CmFruInfo.m_UsedBufLen);
}

void CPropertiesWnd::OnUpdatePropertiesCm(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnPropertiesOutput()
{
	// TODO: 在此处添加命令处理程序代码
	//MessageBox(L"Test");
	FruHeader header = {0};
	CFruRepo::CreateCommHeader((uint8*)&header, sizeof(FruHeader));
	SetFruInfo((uint8*)&header, sizeof(FruHeader));

}
void CPropertiesWnd::OnUpdatePropertiesOutput(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnAddBoardInfoAera()
{
	// TODO: 在此处添加命令处理程序代码
	m_GridCtrl.AddBoardInfoArea();

}

void CPropertiesWnd::OnUpdateAddBoardInfoAera(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnAddProductInfoAera()
{
	// TODO: 在此处添加命令处理程序代码
	m_GridCtrl.AddProductInfoArea();

}

void CPropertiesWnd::OnUpdateAddProductInfoAera(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::SetFruInfo(BYTE* pFruData, int nLen)
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	if(pFruData == NULL)
	{
		return;
		//FruInfo_Init(&m_CmFruInfo, m_CmFruRepoBuff, sizeof(m_CmFruRepoBuff), m_pCmFruRecordArray, sizeof(m_pCmFruRecordArray)/4, NULL, NULL);
		//FruInfoEmmc_SetDefault(&m_CmFruInfo);
		//static BYTE buff[1024] = {0};
		//CfgTester_Init(buff, 1024, 0x72, 1);


		//pFruData = m_CfgTesterFruInfo.m_Buffer;
		//nLen = m_CfgTesterFruInfo.m_UsedBufLen;
	}

	TraceEx = (TraceFun)MyTrace;

	m_GridCtrl.SetFruData(pFruData, nLen);

	//InitPropList();
}

int CPropertiesWnd::GetFruInfo(BYTE* pFruData, int nBufLen)
{
	return m_GridCtrl.GetFruData(pFruData, nBufLen);
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("外观"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t) false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一:“无”、“细”、“可调整大小”或“对话框外框”"));
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("细"));
	pProp->AddOption(_T("可调整大小"));
	pProp->AddOption(_T("对话框外框"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("宋体, Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
	pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
#if CONFIG_OBJCOMBO
	m_wndObjectCombo.SetFont(&m_fntPropList);
#endif
}

void CPropertiesWnd::OnCreateMmcFruRecord(CByteArray& createFlag)
{
	int i = 4;
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CurrentReqDefault, g_CurrentReqDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CurrentReqDefault, g_CurrentReqDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_EKeyDefineInfoDefault, g_EKeyDefineInfoDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_EKeyDefineInfoDefault, g_EKeyDefineInfoDefaultLen);
	}
}
void CPropertiesWnd::OnCreatePmFruRecord(CByteArray& createFlag)
{
	int i = 4;
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_PowerCapabilityDefault, g_PowerCapabilityDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_PowerCapabilityDefault, g_PowerCapabilityDefaultLen);
	}
}
void CPropertiesWnd::OnCreateCuFruRecord(CByteArray& createFlag)
{
	int i = 4;
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CurrentReqDefault, g_CurrentReqDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CurrentReqDefault, g_CurrentReqDefaultLen);
	}
}
void CPropertiesWnd::OnCreateCmFruRecord(CByteArray& createFlag)
{
	int i = 4;
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_FruInfoPartiDefault, g_FruInfoPartiDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_FruInfoPartiDefault, g_FruInfoPartiDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CarrierIpLinkDefault, g_CarrierIpLinkDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CarrierIpLinkDefault, g_CarrierIpLinkDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CarrierInfoDefault, g_CarrierInfoDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CarrierInfoDefault, g_CarrierInfoDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CarriePowerPolicyDefault, g_CarriePowerPolicyDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CarriePowerPolicyDefault, g_CarriePowerPolicyDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CarrierActAndPmDescDefault, g_CarrierActAndPmDescDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CarrierActAndPmDescDefault, g_CarrierActAndPmDescDefaultLen);
	}
	if(createFlag[i++])
	{
		FruInfo_FormatRecord((RecordHeader *)g_CarrierP2pConnectDefault, g_CarrierP2pConnectDefaultLen);
		m_GridCtrl.AddRecord((RecordHeader *)g_CarrierP2pConnectDefault, g_CarrierP2pConnectDefaultLen);
	}

}

LRESULT CPropertiesWnd::OnCreateFruRepo(WPARAM wparam, LPARAM lparam)
{
	CDlgFruRepoSetting* pDlg = NULL;
	uint8 deviceId = (uint8)wparam;

	if(deviceId == CM_Dev_ID)
	{
		pDlg = new CDlgFruRepoSettingCm();
	}
	else if(deviceId >= PM_1_Dev_ID && deviceId <= PM_4_Dev_ID)
	{
		pDlg = new CDlgFruRepoSettingPm();
	}
	else if(deviceId >= MCH_1_Dev_ID && deviceId <= MCH_2_Dev_ID)
	{
		pDlg = new CDlgFruRepoSettingMmc();
	}
	else if(deviceId >= AMC_1_Dev_ID && deviceId <= AMC_12_Dev_ID)
	{
		pDlg = new CDlgFruRepoSettingMmc();
	}
	else if(deviceId >= CU_1_Dev_ID && deviceId <= CU_2_Dev_ID)
	{
		pDlg = new CDlgFruRepoSettingCu();
	}

	if(IDOK != pDlg->DoModal())
	{
		delete pDlg;
		return 0;
	}

	CFruRepoData* pRepo = m_FruRepoDataArray.GetFruRepoData(deviceId);
	if(NULL == pRepo) return 0;

	m_DeviceId = deviceId;

	pRepo->Reset();

	m_GridCtrl.Reset();
	m_GridCtrl.Init(pDlg->m_isCheckedArray[0]
		, pDlg->m_isCheckedArray[1]
		, pDlg->m_isCheckedArray[2]
		, pDlg->m_isCheckedArray[3]
		, pRepo
		);

	if(m_DeviceId == CM_Dev_ID)
	{
		OnCreateCmFruRecord(pDlg->m_isCheckedArray);
	}
	else if(m_DeviceId >= PM_1_Dev_ID && m_DeviceId <= PM_4_Dev_ID)
	{
		OnCreatePmFruRecord(pDlg->m_isCheckedArray);
	}
	else if(m_DeviceId >= MCH_1_Dev_ID && m_DeviceId <= MCH_2_Dev_ID)
	{
		OnCreateMmcFruRecord(pDlg->m_isCheckedArray);
	}
	else if(m_DeviceId >= AMC_1_Dev_ID && m_DeviceId <= AMC_12_Dev_ID)
	{
		OnCreateMmcFruRecord(pDlg->m_isCheckedArray);
	}
	else if(m_DeviceId >= CU_1_Dev_ID && m_DeviceId <= CU_2_Dev_ID)
	{
		OnCreateCuFruRecord(pDlg->m_isCheckedArray);
	}

	if(theApp.m_pMainWnd)
	{
		AfxGetMainWnd()->PostMessage(ID_SHOW_FRUDATA);
	}
	pRepo->SetUsedLen(m_GridCtrl.GetFruData(pRepo->GetData(), pRepo->GetMaxLen()));

	delete pDlg;
	return 0;
}

LRESULT CPropertiesWnd::OnOpenFruRepo(WPARAM wparam, LPARAM lparam)
{
	if(m_DeviceId == (uint8)wparam) return 0;

	CFruRepoData* pRepo = m_FruRepoDataArray.GetFruRepoData((uint8)wparam);
	if(NULL == pRepo) return 0;

	if(pRepo->GetUsedLen() == 0)
	{
		if(IDYES == MessageBox(L"该设备的Fru库还没有被创建，是否现在创建？", L"打开Fru库", MB_YESNO|MB_ICONQUESTION))
		{
			return OnCreateFruRepo(wparam, lparam);
		}
		else
		{
			return 0;
		}
	}

	m_GridCtrl.Reset();

	m_GridCtrl.SetFruData(pRepo);

	m_DeviceId = (uint8)wparam;

	return 0;
}

void CPropertiesWnd::OnCreateMchConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId)
{
	int nIndex = 0;
	uint8 nLen = 0;
	uint8* pData = 0;
	CByteArray& createFlag = pDlg->m_isCheckedArray;
	const FruAttr* pFruAttr = FruAttr_getById(deviceId); 
	ASSERT(pFruAttr);

	if(createFlag[4])
	{
		uint8 g_FullSensorRecord[] =
		{
			 FRUCTRL_RECORD, 0x00, 0x00 ,0x00, 0x00
			,0x00, 0x31, 0x5A	
			,0x00, 0x00, 0x00	
		};
		FruInfo_FormatRecord((RecordHeader *)g_FullSensorRecord, sizeof(g_FullSensorRecord));
		CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)g_FullSensorRecord, sizeof(g_FullSensorRecord));
	}

	for(int i = 5; i < 13; i++)
	{
		nIndex = i-5;
		if(createFlag[i])
		{
			FullSensorRecord_Init(g_pMchFullSensorRecord[nIndex],TEMPERATURE1_RECORD_ID + nIndex, &g_SensorItem[nIndex], &g_ThresholdItem[nIndex], pFruAttr->m_IpmbAddress, pFruAttr->m_SiteNum);
			pData = FruInfo_FromatFullSensorrecord((uint8*)g_pMchFullSensorRecord[nIndex], &nLen);
			FruInfo_FormatRecord((RecordHeader *)pData, nLen);
			CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)pData, nLen);
		}
	}

}

void CPropertiesWnd::OnCreateMmcConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId)
{
	int nIndex = 0;
	uint8 nLen = 0;
	uint8* pData = 0;
	CByteArray& createFlag = pDlg->m_isCheckedArray;
	const FruAttr* pFruAttr = FruAttr_getById(deviceId); 
	ASSERT(pFruAttr);

	if(createFlag[4])
	{
		uint8 g_FullSensorRecord[] =
		{
			 FRUCTRL_RECORD, 0x00, 0x00 ,0x00, 0x00
			,0x00, 0x31, 0x5A	
			,0x00, 0x00, 0x00	
		};
		FruInfo_FormatRecord((RecordHeader *)g_FullSensorRecord, sizeof(g_FullSensorRecord));
		CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)g_FullSensorRecord, sizeof(g_FullSensorRecord));
	}

	int nRecordId = AMC_RECORDID_OFFSET;
	for(int i = 5; i <= 8; i++)
	{
		nIndex = i-5;
		if(createFlag[i])
		{
			FullSensorRecord_Init(g_pAmcFullSensorRecord[nIndex],nRecordId++, &g_AmcSensorItem[nIndex], &g_AmcThresholdItem[nIndex], MCH_1_IPMB_ADDR, MCH_1_SITENUM);
			pData = FruInfo_FromatFullSensorrecord((uint8*)g_pAmcFullSensorRecord[nIndex], &nLen);
			FruInfo_FormatRecord((RecordHeader *)pData, nLen);
			CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)pData, nLen);
		}
	}
}

void CPropertiesWnd::OnCreatePmConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId)
{
	int nIndex = 0;
	uint8 nLen = 0;
	uint8* pData = 0;
	CByteArray& createFlag = pDlg->m_isCheckedArray;
	const FruAttr* pFruAttr = FruAttr_getById(deviceId); 
	ASSERT(pFruAttr);

	if(createFlag[4])
	{
		CompactSensorRecord_Init(&g_PmMsCompactSensorRecord, PM_STATE_RECORD_ID , &g_PmSensorItem[0], pFruAttr->m_IpmbAddress, pFruAttr->m_SiteNum);
		pData = FruInfo_FromatCompactSensorrecord((uint8*)&g_PmMsCompactSensorRecord, &nLen);
		FruInfo_FormatRecord((RecordHeader *)pData, nLen);
		CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)pData, nLen);
	}

	int nRecordId = PM_STATE_RECORD_ID + 1;
	for(int i = 5; i <= 9; i++)
	{
		nIndex = i-5;
		if(createFlag[i])
		{
			FullSensorRecord_Init(g_pPmFullSensorRecord[nIndex],nRecordId++, &g_PmSensorItem[nIndex+1], &g_PmThresholdItem[nIndex], MCH_1_IPMB_ADDR, MCH_1_SITENUM);
			pData = FruInfo_FromatFullSensorrecord((uint8*)g_pPmFullSensorRecord[nIndex], &nLen);
			FruInfo_FormatRecord((RecordHeader *)pData, nLen);
			CPropFruReco* pProp = m_GridCtrl.AddRecord((RecordHeader*)pData, nLen);
		}
	}
}

void CPropertiesWnd::OnCreateCuConfigInfo(CDlgConfigInfoSetting* pDlg, uint8 deviceId)
{
}


LRESULT CPropertiesWnd::OnCreateConfig(WPARAM wparam, LPARAM lparam)
{
	CDlgConfigInfoSetting* pDlg = NULL;
	uint8 deviceId = (uint8)wparam;

	if(deviceId == CM_Dev_ID)
	{
		//pDlg = new CDlgFruRepoSetting(IDD_DLG_CONFIGINFO_MCH);
	}
	else if(deviceId >= MCH_1_Dev_ID && deviceId <= MCH_2_Dev_ID)
	{
		pDlg = new CDlgConfigInfoSetting(IDD_DLG_CONFIGINFO_MCH);
	}
	else if(deviceId >= PM_1_Dev_ID && deviceId <= PM_4_Dev_ID)
	{
		pDlg = new CDlgConfigInfoSetting(IDD_DLG_CONFIGINFO_PM);
	}
	else if(deviceId >= AMC_1_Dev_ID && deviceId <= AMC_12_Dev_ID)
	{
		pDlg = new CDlgConfigInfoSetting(IDD_DLG_CONFIGINFO_AMC);
	}
	else if(deviceId >= CU_1_Dev_ID && deviceId <= CU_2_Dev_ID)
	{
		//pDlg = new CDlgConfigInfoSetting(IDD_DLG_CONFIGINFO_CU);
		return 0;
	}

	if(IDOK != pDlg->DoModal())
	{
		delete pDlg;
		return 0;
	}

	CFruRepoData* pRepo = m_ConfigInfoArray.GetFruRepoData(deviceId);
	if(NULL == pRepo) return 0;

	m_DeviceId = deviceId;

	pRepo->Reset();

	m_GridCtrl.Reset();
	m_GridCtrl.Init(pDlg->m_isCheckedArray[0]
		, pDlg->m_isCheckedArray[1]
		, pDlg->m_isCheckedArray[2]
		, pDlg->m_isCheckedArray[3]
		, pRepo
		);

	
	if(deviceId >= MCH_1_Dev_ID && deviceId <= MCH_2_Dev_ID)
	{
		OnCreateMchConfigInfo(pDlg, deviceId);
	}
	else if(deviceId >= PM_1_Dev_ID && deviceId <= PM_4_Dev_ID)
	{
		OnCreatePmConfigInfo(pDlg, deviceId);
	}
	else if(deviceId >= AMC_1_Dev_ID && deviceId <= AMC_12_Dev_ID)
	{
		OnCreateMmcConfigInfo(pDlg, deviceId);
	}
	else if(deviceId >= CU_1_Dev_ID && deviceId <= CU_2_Dev_ID)
	{
	}

	if(theApp.m_pMainWnd)
	{
		AfxGetMainWnd()->PostMessage(ID_SHOW_FRUDATA);
	}
	pRepo->SetUsedLen(m_GridCtrl.GetFruData(pRepo->GetData(), pRepo->GetMaxLen()));

	delete pDlg;
	return 0;
}

LRESULT CPropertiesWnd::OnOpenConfig(WPARAM wparam, LPARAM lparam)
{
	if(m_DeviceId == (uint8)wparam) return 0;

	CFruRepoData* pRepo = m_ConfigInfoArray.GetFruRepoData((uint8)wparam);
	if(NULL == pRepo) return 0;

	if(pRepo->GetUsedLen() == 0)
	{
		if(IDYES == MessageBox(L"该设备的Fru库还没有被创建，是否现在创建？", L"打开Fru库", MB_YESNO|MB_ICONQUESTION))
		{
			return OnCreateConfig(wparam, lparam);
		}
		else
		{
			return 0;
		}
	}

	m_GridCtrl.Reset();

	m_GridCtrl.SetFruData(pRepo);

	m_DeviceId = (uint8)wparam;

	return 0;
}


void CPropertiesWnd::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		//ar << nCount;
	}
	else
	{
		//ar >> nCount;
	}
	m_FruRepoDataArray.Serialize(ar);
	m_ConfigInfoArray.Serialize(ar);
}
