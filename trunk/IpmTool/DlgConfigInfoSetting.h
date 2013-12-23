#pragma once

#include "DlgFruRepoSetting.h"
// CDlgConfigInfoSetting 对话框

class CDlgConfigInfoSetting : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgConfigInfoSetting)

public:
	CDlgConfigInfoSetting(UINT idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConfigInfoSetting();

// 对话框数据

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
