#pragma once
#include "DlgFruRepoSetting.h"


// CDlgFruRepoSettingCu �Ի���

class CDlgFruRepoSettingCu : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingCu)

public:
	CDlgFruRepoSettingCu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFruRepoSettingCu();

// �Ի�������
	enum { IDD = IDD_DLG_FRUREPO_CU };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBox(UINT id);
};
