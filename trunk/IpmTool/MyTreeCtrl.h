#pragma once

typedef enum _SEARCH_FALG
{
	 FLAG_BREAK = 0
	,FLAG_CONTINUE
}SEARCH_FALG;
typedef SEARCH_FALG (*OperateFun)(void* pTreeCtrl, HTREEITEM hItem,  BOOL isParent, DWORD param);

// CMyTreeCtrl
class CTreeNodeData
{
public:
	int				m_Level;
	LPCTSTR			m_pName;
	DWORD			m_dwMenuID;
	const FruAttr*		m_FruAttr;
public:
	CTreeNodeData(int level, LPCTSTR name, const FruAttr* pFruAttr = NULL, DWORD menuId = 0){m_Level = level; m_pName = name; m_dwMenuID = menuId;m_FruAttr = pFruAttr;};
	virtual ~CTreeNodeData(){};
};

typedef CArray<CTreeNodeData*, CTreeNodeData*> CTreeNodeDataArray;

class CMyTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMyTreeCtrl)

public:
	CMyTreeCtrl();
	virtual ~CMyTreeCtrl();
public:
	SEARCH_FALG SeatchItem(HTREEITEM hItem, OperateFun Operate, DWORD param);
	HTREEITEM FindItem(HTREEITEM hItem, void* pItemData);
	HTREEITEM FindItem(HTREEITEM hItem, LPCTSTR lpName);
	void ExpendAll(HTREEITEM hItem, BOOL isExpend);
	void Expend(LPCTSTR lpName, BOOL isExpend);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCarrierFruRepoCreate();
	afx_msg void OnCarrierFruRepoOpen();
	afx_msg void OnConfigInfoCreate();
	afx_msg void OnConfigInfoOpen();

protected:
	void OnDbClick(HTREEITEM hItem);
public:
	void OnContextMenu(CPoint point);
	void FillTreeNodeData(HTREEITEM pParent, CTreeNodeDataArray& nodeArray, int& nIndex, CTreeNodeData* pParentTreeNodeData);
};


