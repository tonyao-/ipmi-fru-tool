// DlgComConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgComConfig.h"
#include "afxdialogex.h"


// CDlgComConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgComConfig, CDialogEx)

CDlgComConfig::CDlgComConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgComConfig::IDD, pParent)
{

}

CDlgComConfig::~CDlgComConfig()
{
}

void CDlgComConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgComConfig, CDialogEx)
END_MESSAGE_MAP()


// CDlgComConfig ��Ϣ�������
BOOL CDlgComConfig::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	CComboBox* pCtrl = (CComboBox*)GetDlgItem(IDC_COMBO2);
	pCtrl->AddString(L"4800");
	pCtrl->AddString(L"9600");
	pCtrl->AddString(L"14400");
	pCtrl->AddString(L"19200");
	pCtrl->AddString(L"38400");
	pCtrl->AddString(L"56000");
	pCtrl->AddString(L"57600");
	pCtrl->AddString(L"115200");

	pCtrl->SetCurSel(7);
	return bRet;
}