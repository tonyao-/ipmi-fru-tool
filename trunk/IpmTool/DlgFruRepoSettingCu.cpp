// CDlgFruRepoSettingCu.cpp : 实现文件
//

#include "stdafx.h"
#include "IpmTool.h"
#include "DlgFruRepoSettingCu.h"
#include "afxdialogex.h"


// CDlgFruRepoSettingCu 对话框
#define MAX_CHECK_ID IDC_CHECK11

IMPLEMENT_DYNAMIC(CDlgFruRepoSettingCu, CDlgFruRepoSetting)

CDlgFruRepoSettingCu::CDlgFruRepoSettingCu(CWnd* pParent /*=NULL*/)
	: CDlgFruRepoSetting(CDlgFruRepoSettingCu::IDD, pParent)
{

}

CDlgFruRepoSettingCu::~CDlgFruRepoSettingCu()
{
}

void CDlgFruRepoSettingCu::DoDataExchange(CDataExchange* pDX)
{
	CDlgFruRepoSetting::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFruRepoSettingCu, CDlgFruRepoSetting)
	ON_COMMAND_RANGE(IDC_CHECK1, MAX_CHECK_ID, OnCheckBox)
END_MESSAGE_MAP()


// CDlgFruRepoSettingCu 消息处理程序
BOOL CDlgFruRepoSettingCu::OnInitDialog()
{
	CDlgFruRepoSetting::OnInitDialog();

	for(DWORD i = IDC_CHECK3; i <= MAX_CHECK_ID - 1; i++)
	{
		((CButton*)GetDlgItem(i))->SetCheck(TRUE);
		 m_isCheckedArray[i - IDC_CHECK1] = TRUE;
	}

	((CButton*)GetDlgItem(MAX_CHECK_ID))->SetCheck(FALSE);
	return TRUE;
}

void CDlgFruRepoSettingCu::OnCheckBox(UINT id)
{
	CDlgFruRepoSetting::OnCheckBox(id);
}
