// CSerialCom.h : CSerialCom DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSerialComApp
// �йش���ʵ�ֵ���Ϣ������� CSerialCom.cpp
//

class CSerialComApp : public CWinApp
{
public:
	CSerialComApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
