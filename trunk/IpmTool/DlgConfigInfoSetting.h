#pragma once

#include "DlgFruRepoSetting.h"
// CDlgConfigInfoSetting �Ի���

class CDlgConfigInfoSetting : public CDlgFruRepoSetting
{
	DECLARE_DYNAMIC(CDlgConfigInfoSetting)

public:
	CDlgConfigInfoSetting(UINT idd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConfigInfoSetting();

// �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
