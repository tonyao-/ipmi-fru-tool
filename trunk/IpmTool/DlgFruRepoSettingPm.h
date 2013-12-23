#pragma once

#include "DlgFruRepoSetting.h"
// CDlgFruRepoSettingPm 对话框

class CDlgFruRepoSettingPm : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingPm)

public:
	CDlgFruRepoSettingPm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFruRepoSettingPm();

// 对话框数据
	enum { IDD = IDD_DLG_FRUREPO_PM };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
