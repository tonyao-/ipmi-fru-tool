#pragma once


// CDlgFruRepoSetting �Ի���
class CDlgFruRepoSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFruRepoSetting)
public:
	CStringArray m_NameArray;
	CDlgFruRepoSetting(UINT idd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFruRepoSetting();
public:
	CByteArray m_isCheckedArray;
	afx_msg void OnCheckBox(UINT id);
	CString GetName(UINT id);
};

