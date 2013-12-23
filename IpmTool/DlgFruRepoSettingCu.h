#pragma once
#include "DlgFruRepoSetting.h"


// CDlgFruRepoSettingCu 对话框

class CDlgFruRepoSettingCu : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingCu)

public:
	CDlgFruRepoSettingCu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFruRepoSettingCu();

// 对话框数据
	enum { IDD = IDD_DLG_FRUREPO_CU };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
