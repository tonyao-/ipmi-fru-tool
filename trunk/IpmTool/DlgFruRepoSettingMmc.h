#pragma once

#include "DlgFruRepoSetting.h"

// CDlgFruRepoSettingMmc �Ի���

class CDlgFruRepoSettingMmc : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingMmc)

public:
	CDlgFruRepoSettingMmc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFruRepoSettingMmc();

// �Ի�������
	enum { IDD = IDD_DLG_FRUREPO_MMC };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
