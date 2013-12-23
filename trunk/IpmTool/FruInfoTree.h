#pragma once

#include "MyTreeCtrl.h"
// CFruInfoTree

class CFruInfoTree : public CMyTreeCtrl
{
	DECLARE_DYNAMIC(CFruInfoTree)

public:
	CFruInfoTree();
	virtual ~CFruInfoTree();

protected:
	DECLARE_MESSAGE_MAP()
// ÖØÐ´
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

};


