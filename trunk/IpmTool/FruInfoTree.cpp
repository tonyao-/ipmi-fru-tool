// FruInfoTree.cpp : 实现文件
//

#include "stdafx.h"
#include "IpmTool.h"
#include "FruInfoTree.h"


// CFruInfoTree

IMPLEMENT_DYNAMIC(CFruInfoTree, CMyTreeCtrl)

CFruInfoTree::CFruInfoTree()
{

}

CFruInfoTree::~CFruInfoTree()
{
}


BEGIN_MESSAGE_MAP(CFruInfoTree, CMyTreeCtrl)
END_MESSAGE_MAP()



// CFruInfoTree 消息处理程序

BOOL CFruInfoTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CMyTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


