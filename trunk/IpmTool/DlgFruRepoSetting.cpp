// CDlgFruRepoSettingCm.cpp : 实现文件
//

#include "stdafx.h"
#include "IpmTool.h"
#include "DlgFruRepoSetting.h"
#include "afxdialogex.h"

////////////////////////////////////////////
#define MAX_CHECK_ID IDC_CHECK14

IMPLEMENT_DYNAMIC(CDlgFruRepoSetting, CDialogEx)
CDlgFruRepoSetting::CDlgFruRepoSetting(UINT idd, CWnd* pParent /*=NULL*/)
	: CDialogEx(idd, pParent)
{
	for(int i = IDC_CHECK1; i <= MAX_CHECK_ID; i++)
	{
		m_isCheckedArray.Add(0);
	}
}

CDlgFruRepoSetting::~CDlgFruRepoSetting()
{
}


void CDlgFruRepoSetting::OnCheckBox(UINT id)
{
	BOOL isCheck = ((CButton*)GetDlgItem(id))->GetCheck();
	m_isCheckedArray[id-IDC_CHECK1] = ((CButton*)GetDlgItem(id))->GetCheck();
}

CString CDlgFruRepoSetting::GetName(UINT id)
{
	ASSERT(id < m_NameArray.GetCount());
	return m_NameArray[id];
}
