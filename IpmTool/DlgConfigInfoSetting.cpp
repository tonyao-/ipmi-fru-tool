// DlgConfigInfoSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IpmTool.h"
#include "DlgConfigInfoSetting.h"
#include "afxdialogex.h"


// CDlgConfigInfoSetting �Ի���
#define MAX_CHECK_ID IDC_CHECK14

IMPLEMENT_DYNAMIC(CDlgConfigInfoSetting, CDlgFruRepoSetting)

CDlgConfigInfoSetting::CDlgConfigInfoSetting(UINT id, CWnd* pParent /*=NULL*/)
	: CDlgFruRepoSetting(id, pParent)
{

}

CDlgConfigInfoSetting::~CDlgConfigInfoSetting()
{
}

void CDlgConfigInfoSetting::DoDataExchange(CDataExchange* pDX)
{
	CDlgFruRepoSetting::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgConfigInfoSetting, CDlgFruRepoSetting)
	ON_COMMAND_RANGE(IDC_CHECK1, MAX_CHECK_ID, CDlgFruRepoSetting::OnCheckBox)
END_MESSAGE_MAP()


// CDlgConfigInfoSetting ��Ϣ�������
BOOL CDlgConfigInfoSetting::OnInitDialog()
{
	CString str;
	CDlgFruRepoSetting::OnInitDialog();

	for(DWORD i = IDC_CHECK1; i <= IDC_CHECK4; i++)
	{
		((CButton*)GetDlgItem(i))->GetWindowTextW(str);
		m_NameArray.Add(str);
	}

	for(DWORD i = IDC_CHECK5; i <= MAX_CHECK_ID; i++)
	{
		((CButton*)GetDlgItem(i))->SetCheck(TRUE);
		 m_isCheckedArray[i - IDC_CHECK1] = TRUE;

		((CButton*)GetDlgItem(i))->GetWindowTextW(str);
		 m_NameArray.Add(str);
	}

	return TRUE;
}
