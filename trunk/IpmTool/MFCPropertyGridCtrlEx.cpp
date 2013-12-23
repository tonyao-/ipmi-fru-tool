#include "StdAfx.h"
#include "MFCPropertyGridCtrlEx.h"

IMPLEMENT_DYNAMIC(CMFCPropertyGridCtrlEx, CMFCPropertyGridCtrl)

BEGIN_MESSAGE_MAP(CMFCPropertyGridCtrlEx, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()

CMFCPropertyGridCtrlEx::CMFCPropertyGridCtrlEx(void)
{
	m_pGroup = NULL;
}


CMFCPropertyGridCtrlEx::~CMFCPropertyGridCtrlEx(void)
{
}

void CMFCPropertyGridCtrlEx::OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue)
{
	AdjustLayout();
}

void CMFCPropertyGridCtrlEx::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

CMFCPropertyGridPropertyEx* CMFCPropertyGridCtrlEx::GetPropertyByName(LPCTSTR lpName)
{
	for(int i = 0; i < this->GetPropertyCount(); i++)
	{
		if(wcscmp(GetProperty(i)->GetName(), lpName) == 0)
			return (CMFCPropertyGridPropertyEx*)GetProperty(i);
	}
	return NULL;
}

BOOL CMFCPropertyGridCtrlEx::DeletePropertyByName(LPCTSTR lpName)
{
	CMFCPropertyGridProperty* pProperty = GetPropertyByName(lpName);

	if(pProperty)
	{
		return DeleteProperty(pProperty);
	}

	return FALSE;
}

int CMFCPropertyGridCtrlEx::InsertProperty(CMFCPropertyGridPropertyEx* pProp, LPCTSTR lpName, BOOL bRedraw, BOOL bAdjustLayout)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pProp);

	POSITION dstPos = 0;

	for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridPropertyEx* pListProp = (CMFCPropertyGridPropertyEx*)m_lstProps.GetNext(pos);

		ASSERT_VALID(pListProp);

		if (pListProp == pProp || pListProp->IsSubItem(pProp))
		{
			// Can't ad the same property twice
			ASSERT(FALSE);
			return -1;
		}

		if(lpName && wcscmp(pListProp->GetName(), lpName) == 0)
		{
			dstPos = pos;
		}
	}

	pProp->SetOwnerList(this);
	if(dstPos)
	{
		m_lstProps.InsertBefore(dstPos, pProp);
	}
	else
	{
		m_lstProps.AddTail(pProp);
	}


	int nIndex = (int) m_lstProps.GetCount() - 1;

	if (bAdjustLayout)
	{
		AdjustLayout();

		if (bRedraw && GetSafeHwnd() != NULL)
		{
			pProp->Redraw();
		}
	}

	return nIndex;
}
