#pragma once

#include "resource.h"

// CDlgComConfig �Ի���

class CDlgComConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgComConfig)

public:
	CDlgComConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgComConfig();

// �Ի�������
	enum { IDD = IDD_DLG_SETTING };

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
};
