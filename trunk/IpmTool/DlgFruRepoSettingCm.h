#pragma once

#include "DlgFruRepoSetting.h"


class CDlgFruRepoSettingCm : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgFruRepoSettingCm)

public:
	CDlgFruRepoSettingCm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFruRepoSettingCm();

// �Ի�������
	enum { IDD = IDD_DLG_FRUREPO };

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
