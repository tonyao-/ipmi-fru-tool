#include "StdAfx.h"
#include "Field.h"
#include "MFCPropertyGridPropertyEx.h"

#define CREATE_SUB_ITEM() {	\
	va_list argp;	\
	va_start(argp, nSubItemCount);	\
	AddItem(nSubItemCount, argp);	\
	va_end( argp);	\
}

IMPLEMENT_DYNAMIC(CField, CObject)

CField::CField(LPCTSTR lpName, LPCTSTR lpDesc, int nSubItemCount, ...) : m_strName(lpName), m_strDesc(lpDesc)
{
	//TRACE("%s() \n", _FUNC_);
	CREATE_SUB_ITEM();
	//m_FieldArray.SetSize(50);
}

void CField::AddItem(int nSubItemCount, va_list ap)
{
	CField* pField = NULL;
	for(int i = 0; i < nSubItemCount; i++)
	{
		pField = (CField*)va_arg(ap, DWORD);
		m_FieldArray.Add(pField);
	}
}

CField::~CField(void)
{
	//TRACE("%s() \n", _FUNC_);
	CField* pField = NULL;
	for(int i = 0; i < m_FieldArray.GetCount(); i++)
	{
		delete m_FieldArray[i];
	}
}

void CField::InitProp(CMFCPropertyGridPropertyEx* pProp)
{
}

//CFieldNum//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CFieldNum, CField)
CFieldNum::CFieldNum(LPCTSTR lpName, DWORD	dwDefault, DWORD dwMin, DWORD dwMax, LPCTSTR lpFormat, LPCTSTR lpDesc, int nSubItemCount, ...)
	: CField(lpName, lpDesc), m_dwDefault(dwDefault), m_dwMin(dwMin), m_dwMax(dwMax), m_strFormat(lpFormat)
{
	CREATE_SUB_ITEM();
}

void CFieldNum::InitProp(CMFCPropertyGridPropertyEx* pProp)
{
	pProp->m_strFormat = m_strFormat;
	pProp->m_nMinValue = m_dwMin;
	pProp->m_nMaxValue = m_dwMax;
	if(m_dwMin == m_dwMax)
	{
		pProp->Enable(FALSE);
	}
}

//CFieldStr//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CFieldStr, CField)
CFieldStr::CFieldStr(LPCTSTR lpName, LPCTSTR strDefault, DWORD dwMin, DWORD dwMax, LPCTSTR lpEditMask, LPCTSTR lpEditTemp, LPCTSTR lpValidChars, LPCTSTR lpDesc, int nSubItemCount, ...)
	: CField(lpName, lpDesc), m_strDefault(strDefault), m_dwMin(dwMin), m_dwMax(dwMax), m_strEditMask(lpEditMask), m_strEditTemp(lpEditTemp), m_strValidChars(lpValidChars)
{
	CREATE_SUB_ITEM();
}

//CFieldCombo//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CFieldCombo, CField)
CFieldCombo::CFieldCombo(LPCTSTR lpName, LPCTSTR strDefault, LPCTSTR lpDwString, LPCTSTR lpDesc, int nSubItemCount, ...)
	: CField(lpName, lpDesc), m_strDefault(strDefault), m_strDwString(lpDwString)
{
	CREATE_SUB_ITEM();
}

void CFieldCombo::InitProp(CMFCPropertyGridPropertyEx* pProp)
{
	CMFCPropertyGridPropertyCombo* pComboProp = (CMFCPropertyGridPropertyCombo*)pProp;
	pComboProp->InitDwString(m_strDwString, m_strDefault);
}