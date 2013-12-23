// MyTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTreeCtrl.h"
#include "resource.h"
#include "MainFrm.h"
// CMyTreeCtrl

IMPLEMENT_DYNAMIC(CMyTreeCtrl, CTreeCtrl)

CMyTreeCtrl::CMyTreeCtrl()
{

}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	ON_COMMAND(ID_CARRIER_FRUREPO_CREATE, OnCarrierFruRepoCreate)
	ON_COMMAND(ID_CARRIER_FRUREPO_OPEN, OnCarrierFruRepoOpen)
	ON_COMMAND(ID_CREATE_CONFIGINFO, OnConfigInfoCreate)
	ON_COMMAND(ID_OPEN_CONFIGINFO, OnConfigInfoOpen)
END_MESSAGE_MAP()

void CMyTreeCtrl::OnCarrierFruRepoCreate()
{
	HTREEITEM hItem = GetSelectedItem();
	CTreeNodeData* pNodeData = (CTreeNodeData*)GetItemData(hItem);
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	::PostMessage(pWnd->m_wndProperties.m_hWnd,ID_CREATE_FRUREPO, pNodeData->m_FruAttr->m_DeviceId, 0);
}
void CMyTreeCtrl::OnCarrierFruRepoOpen()
{
	HTREEITEM hItem = GetSelectedItem();
	CTreeNodeData* pNodeData = (CTreeNodeData*)GetItemData(hItem);
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	::PostMessage(pWnd->m_wndProperties.m_hWnd,ID_OPEN_FRUREPO, pNodeData->m_FruAttr->m_DeviceId, 0);
}

void CMyTreeCtrl::OnConfigInfoCreate()
{
	HTREEITEM hItem = GetSelectedItem();
	CTreeNodeData* pNodeData = (CTreeNodeData*)GetItemData(hItem);
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	::PostMessage(pWnd->m_wndProperties.m_hWnd,ID_CREATE_CONFIGINFO, pNodeData->m_FruAttr->m_DeviceId, 0);
}

void CMyTreeCtrl::OnConfigInfoOpen()
{
	HTREEITEM hItem = GetSelectedItem();
	CTreeNodeData* pNodeData = (CTreeNodeData*)GetItemData(hItem);
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	::PostMessage(pWnd->m_wndProperties.m_hWnd,ID_OPEN_CONFIGINFO, pNodeData->m_FruAttr->m_DeviceId, 0);
}

SEARCH_FALG CMyTreeCtrl::SeatchItem(HTREEITEM hItem, OperateFun Operate, DWORD param)
{
	HTREEITEM hNextItem = hItem;
	if(hItem)
	{
		hNextItem = GetNextItem(hItem,TVGN_CHILD);
		if(FLAG_BREAK == Operate(this, hItem, hNextItem != NULL, param)) return FLAG_BREAK;

		if(hNextItem)
		{
			if(FLAG_BREAK == SeatchItem(hNextItem, Operate, param)) return FLAG_BREAK;
		}
	}

	hItem = GetNextItem(hItem,TVGN_NEXT);
	while(hItem)
	{
		hNextItem = GetNextItem(hItem,TVGN_CHILD);

		if(FLAG_BREAK == Operate(this, hItem, hNextItem != NULL, param)) return FLAG_BREAK;

		if(hNextItem)
		{
			if(FLAG_BREAK == SeatchItem(hNextItem, Operate, param)) return FLAG_BREAK;
		}

		hItem = GetNextItem(hItem,TVGN_NEXT);
	}
	return FLAG_CONTINUE;
}

HTREEITEM CMyTreeCtrl::FindItem(HTREEITEM hItem, LPCTSTR lpName)
{
	HTREEITEM hNextItem = hItem;
	if(hItem)
	{
		if(GetItemText(hItem) == lpName) return hItem; 

		hNextItem = GetNextItem(hItem,TVGN_CHILD);

		if(hNextItem)
		{
			HTREEITEM retTreeItem = FindItem(hNextItem, lpName);
			if(retTreeItem) return retTreeItem;
		}
	}
	else
	{
		return NULL;
	}

	hItem = GetNextItem(hItem,TVGN_NEXT);
	while(hItem)
	{
		if(GetItemText(hItem) == lpName) return hItem;

		hNextItem = GetNextItem(hItem,TVGN_CHILD);
		if(hNextItem)
		{
			HTREEITEM retTreeItem = FindItem(hNextItem, lpName);
			if(retTreeItem) return retTreeItem;
		}

		hItem = GetNextItem(hItem,TVGN_NEXT);
	}
	return NULL;
}

HTREEITEM CMyTreeCtrl::FindItem(HTREEITEM hItem, void* pItemData)
{
	HTREEITEM hNextItem = hItem;
	if(hItem)
	{
		if((void*)GetItemData(hItem) == pItemData) return hItem;

		hNextItem = GetNextItem(hItem,TVGN_CHILD);

		if(hNextItem)
		{
			HTREEITEM retTreeItem = FindItem(hNextItem, pItemData);
			if(retTreeItem) return retTreeItem;
		}
	}
	else
	{
		return NULL;
	}

	hItem = GetNextItem(hItem,TVGN_NEXT);
	while(hItem)
	{
		if((void*)GetItemData(hItem) == pItemData) return hItem;

		hNextItem = GetNextItem(hItem,TVGN_CHILD);
		if(hNextItem)
		{
			HTREEITEM retTreeItem = FindItem(hNextItem, pItemData);
			if(retTreeItem) return retTreeItem;
		}

		hItem = GetNextItem(hItem,TVGN_NEXT);
	}
	return NULL;
}

SEARCH_FALG MyExpend(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, BOOL isParent, DWORD param)
{
	//TreeNodeData* pTreeNodeData = (TreeNodeData*)pTreeCtrl->GetItemData(hItem);
	//if(hItem && isParent && pTreeNodeData && pTreeNodeData->m_Level < 2)
	if(hItem && isParent)
	{
		pTreeCtrl->Expand(hItem, param);
	}
	return FLAG_CONTINUE;
}

void CMyTreeCtrl::Expend(LPCTSTR lpName, BOOL isExpend)
{
	HTREEITEM hItem = FindItem(GetRootItem(), lpName);

	if(hItem)
	{
		Expand(hItem, TVE_EXPAND);
	}
}

void CMyTreeCtrl::ExpendAll(HTREEITEM hItem, BOOL isExpend)
{
	SeatchItem(hItem, (OperateFun)MyExpend, TVE_EXPAND);
}

// CMyTreeCtrl 消息处理程序

void CMyTreeCtrl::FillTreeNodeData(HTREEITEM pParent, CTreeNodeDataArray& nodeArray, int& i, CTreeNodeData* pParentTreeNodeData)
{
	HTREEITEM node = NULL;
	for(i = i; i < nodeArray.GetCount() ; i++)
	{
		if(pParent)
		{
			//TreeNodeData_Dump(pTreeNodeData[i]);
			ASSERT(pParent && pParentTreeNodeData);
			if(nodeArray[i]->m_Level == pParentTreeNodeData->m_Level + 1)
			{
				node = InsertItem(nodeArray[i]->m_pName, 0, 0, pParent);
				SetItemData(node, (DWORD_PTR)nodeArray[i]);
				i++;
				FillTreeNodeData(node, nodeArray, i, nodeArray[i-1]);
			}
			else
			{
				i--;
				return ;
			}
		}
		else if(nodeArray[i]->m_Level == 0)
		{
			//TreeNodeData_Dump(nodeArray[i]);
			node = InsertItem(nodeArray[i]->m_pName, 0, 0);
			SetItemData(node, (DWORD_PTR)nodeArray[i]);
			i++;
			FillTreeNodeData(node, nodeArray, i, nodeArray[i-1]);
		}
	}
}

#if 1
void CMyTreeCtrl::OnContextMenu(CPoint point)
{
	HTREEITEM hTreeItem = NULL;
	CTreeNodeData* pNodeData = NULL;

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		ScreenToClient(&ptTree);

		UINT flags = 0;
		hTreeItem = HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			SelectItem(hTreeItem);
			pNodeData = (CTreeNodeData*)GetItemData(hTreeItem);
		}
	}
	
	SetFocus();

	CMenu menu;

	if(hTreeItem && pNodeData->m_dwMenuID && menu.LoadMenu(pNodeData->m_dwMenuID))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		ENSURE(pPopup != NULL);

		pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
								point.x, point.y,
								this); // route commands through main window,AfxGetMainWnd()
	}
	
}
#endif

void CMyTreeCtrl::OnDbClick(HTREEITEM hItem)
{
	//RunTreeItem(0);
}