#pragma once

#include "DlgFruRepoSetting.h"
// CDlgFruRepoSettingPm �Ի���

class CDlgFruRepoSettingPm : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingPm)

public:
	CDlgFruRepoSettingPm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFruRepoSettingPm();

// �Ի�������
	enum { IDD = IDD_DLG_FRUREPO_PM };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
