#pragma once

class CField;
typedef CArray<CField* , CField*> CFieldArray;
class CMFCPropertyGridPropertyEx;
class CField : public CObject
{
	DECLARE_DYNAMIC(CField)
	friend class CMFCPropertyGridPropertyEx;
	CFieldArray m_FieldArray;
protected:
	void AddItem(int nSubItem, va_list ap);
public:
	CString m_strName;
	CString m_strDesc;
public:
	CField(LPCTSTR lpName, LPCTSTR lpDesc, int nSubItemCount = 0, ...);
	void AddItem(CField* pField){m_FieldArray.Add(pField);};
	virtual ~CField(void);
public:
	virtual void InitProp(CMFCPropertyGridPropertyEx* pProp);
};

//CFieldNum//////////////////////////////////////////////////////////////////////
class CFieldNum : public CField
{
	DECLARE_DYNAMIC(CFieldNum)
public:
	DWORD	m_dwDefault;
	DWORD	m_dwMin;
	DWORD	m_dwMax;
	CString m_strFormat;
public:
	CFieldNum(LPCTSTR lpName, DWORD	dwDefault = 0, DWORD dwMin = 0, DWORD dwMax = 0, LPCTSTR lpFormat = 0, LPCTSTR lpDesc = 0, int nSubItem = 0, ...);
	virtual ~CFieldNum(void){};
	virtual void InitProp(CMFCPropertyGridPropertyEx* pProp);
};

//CFieldStr//////////////////////////////////////////////////////////////////////
class CFieldStr : public CField
{
	DECLARE_DYNAMIC(CFieldStr)
public:
	CString m_strDefault;
	DWORD	m_dwMin;
	DWORD	m_dwMax;
	CString m_strEditMask;
	CString m_strEditTemp;
	CString m_strValidChars;
public:
	CFieldStr(LPCTSTR lpName
		, LPCTSTR strDefault
		, DWORD dwMin = 0
		, DWORD dwMax = 0
		, LPCTSTR lpEditMask = 0
		, LPCTSTR lpEditTemp = 0
		, LPCTSTR lpValidChars = 0
		, LPCTSTR lpDesc = 0
		, int nSubItem = 0, ...);

	virtual ~CFieldStr(void){};
};

//CFieldCombo//////////////////////////////////////////////////////////////////////
class CFieldCombo : public CField
{
	DECLARE_DYNAMIC(CFieldCombo)
public:
	CString m_strDefault;
	CString m_strDwString;
public:
	CFieldCombo(LPCTSTR lpName, LPCTSTR strDefault, LPCTSTR lpDwString = 0, LPCTSTR lpDesc = 0, int nSubItem = 0, ...);
	virtual ~CFieldCombo(void){};

	virtual void InitProp(CMFCPropertyGridPropertyEx* pProp);
};