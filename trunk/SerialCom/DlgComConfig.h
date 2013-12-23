#pragma once

#include "resource.h"

// CDlgComConfig 对话框

class CDlgComConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgComConfig)

public:
	CDlgComConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgComConfig();

// 对话框数据
	enum { IDD = IDD_DLG_SETTING };

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
};
