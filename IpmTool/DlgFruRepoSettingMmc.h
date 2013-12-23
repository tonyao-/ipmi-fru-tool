#pragma once

#include "DlgFruRepoSetting.h"

// CDlgFruRepoSettingMmc 对话框

class CDlgFruRepoSettingMmc : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingMmc)

public:
	CDlgFruRepoSettingMmc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFruRepoSettingMmc();

// 对话框数据
	enum { IDD = IDD_DLG_FRUREPO_MMC };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
