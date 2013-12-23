#pragma once

#include "DlgFruRepoSetting.h"


class CDlgFruRepoSettingCm : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingCm)

public:
	CDlgFruRepoSettingCm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFruRepoSettingCm();

// 对话框数据
	enum { IDD = IDD_DLG_FRUREPO };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
