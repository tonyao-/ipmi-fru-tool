#pragma once
//#include "MFCPropertyGridProperty.h"
#include "Field.h"


class CMFCPropertyGridPropertyEx : public CMFCPropertyGridProperty
{
	friend class CFieldNum;
	DECLARE_DYNAMIC(CMFCPropertyGridPropertyEx)

	CString m_strFormat;
protected:
	CMFCPropertyGridPropertyEx* CreateProp(CFieldArray& fieldArray, CMFCPropertyGridPropertyEx* pParent);
public:
	CMFCPropertyGridPropertyEx(CField* pCField, CMFCPropertyGridPropertyEx* pParentProp);
	CMFCPropertyGridPropertyEx(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE, LPCTSTR lpDesc = NULL);
	CMFCPropertyGridPropertyEx(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0
		,	LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	virtual ~CMFCPropertyGridPropertyEx();

	BOOL GetIpAddr(int nRow, BYTE* pAddrBuff);
	void SetIpAddr(int nRow, BYTE* pAddrBuff);

public:
	virtual CString FormatProperty();
	virtual void SetValue(const COleVariant& varValue);
	virtual BOOL OnUpdateValue();
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue);
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE){return TRUE;};

	BOOL IsSubItem(CMFCPropertyGridProperty* pProp) const {return CMFCPropertyGridProperty::IsSubItem(pProp);};
	void SetOwnerList(CMFCPropertyGridCtrl* pWndList){return CMFCPropertyGridProperty::SetOwnerList(pWndList);};

	virtual DWORD GetDwValue() const { return m_varValue.ulVal; }
	virtual DWORD GetDwValue(int nRow) const { return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue(); }
	virtual DWORD GetDwValue(int nRow, int nSubRow) const { return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue(nSubRow); }
	virtual DWORD GetDwValue(int nRow, int nSubRow,int nSubRow1) const { return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue(nSubRow, nSubRow1); }
	virtual DWORD GetDwValue(int nRow, int nSubRow,int nSubRow1, int nSubRow2) const { return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue(nSubRow, nSubRow1, nSubRow2); }
	virtual DWORD GetDwValue(int nRow, BYTE* pBuff, int len) const;
	virtual DWORD GetDwValue(int nRow, int nSubRow, BYTE* pBuff, int len) const {return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue(nSubRow, pBuff, len);};

	virtual BYTE GetByteValue() const { return (BYTE)m_varValue.ulVal; }
	virtual BYTE GetByteValue(int nRow){return (BYTE)((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue();};
	virtual BYTE GetByteValue(int nRow, int nSubRow){return (BYTE)((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetByteValue(nSubRow);};
	virtual BYTE GetByteValue(int nRow, int nSubRow, int nSubRow1){return (BYTE)((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetByteValue(nSubRow, nSubRow1);};

	BOOL GetStrValue(CString& str);
	BOOL GetStrValue(int nRow, CString& str);
	BOOL GetStrValue(int nRow, int nSubRow, CString& str);
	int GetStrValue(BYTE* chBuff, int nLen, Bool isUnicode = FALSE);
	int GetStrValue(int nRow, BYTE* chBuff, int nLen, Bool isUnicode = FALSE){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetStrValue(chBuff, nLen, isUnicode);};
	//int GetStrValue(int nRow, int nSubRow, BYTE* chBuff, int nLen, Bool isUnicode = FALSE){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetStrValue(nSubRow, chBuff, nLen, isUnicode);};

	//virtual void SetByteValue(BYTE* chBuff, int nLen){m_varValue.bVal};

	virtual void SetDwValue(DWORD dwValue){m_varValue = (_variant_t)dwValue;};
	virtual void SetDwValue(int nRow, DWORD dwValue){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetDwValue(dwValue);};
	virtual void SetDwValue(int nRow, int nSubRow, DWORD dwValue){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetDwValue(nSubRow, dwValue);};
	virtual void SetDwValue(int nRow, int nSubRow, int nSubRow1, DWORD dwValue){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetDwValue(nSubRow, nSubRow1, dwValue);};
	virtual void SetDwValue(int nRow, int nSubRow, int nSubRow1, int nSubrow2, DWORD dwValue){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetDwValue(nSubRow, nSubRow1, nSubrow2, dwValue);};

	
	virtual void SetStrValue(LPCTSTR str){m_varValue = (_variant_t)str;};
	virtual void SetStrValue(int nRow, const char* lpStr) {((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetValue((_variant_t)(lpStr));}
	virtual void SetStrValue(int nRow, LPCTSTR lpStr) {((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetValue((_variant_t)(lpStr));}

	virtual int GetByArrayValue(BYTE* chBuff, int nLen);
	virtual int GetByArrayValue(int nRow, BYTE* chBuff, int nLen){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetByArrayValue(chBuff, nLen);};
	virtual int GetByArrayValue(int nRow, int nSubRow, BYTE* chBuff, int nLen){return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetByArrayValue(nSubRow, chBuff, nLen);};

	virtual void SetByArrayValue(BYTE* chBuff, int nLen);
	virtual void SetByArrayValue(int nRow, BYTE* chBuff, int nLen){((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetByArrayValue(chBuff, nLen);};
	virtual void SetByArrayValue(int nRow, int nSubRow, BYTE* chBuff, int nLen){((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->SetByArrayValue(nSubRow, chBuff, nLen);};

	DWORD GetMaxValue() const {return m_nMaxValue;};
	DWORD GetMaxValue(int nRow) const {return ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetMaxValue();};
public:
	static CMFCPropertyGridPropertyEx* CreateProp(CField* pField, CMFCPropertyGridPropertyEx* pParent);

	void RemoveSubItem(LPCTSTR lpName);
	CMFCPropertyGridPropertyEx* GetItemByName(LPCTSTR lpName);

private:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMFCPropertyGridPropertyNum : public CMFCPropertyGridPropertyEx
{
	DECLARE_DYNAMIC(CMFCPropertyGridPropertyNum)

public:
	CMFCPropertyGridPropertyNum(CFieldNum* pCField, CMFCPropertyGridPropertyEx* pParentProp);
	CMFCPropertyGridPropertyNum(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyGridPropertyNum(const CString& strName, DWORD varValue, LPCTSTR lpszDescr = NULL, LPCTSTR lpszFormat = 0
		, DWORD dwMin = 0, DWORD dwMax = 0, DWORD_PTR dwData = 0, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	virtual ~CMFCPropertyGridPropertyNum();

protected:
	virtual CString FormatProperty();
	CString m_strFormat;

public:
	virtual BOOL TextToVar(const CString& strText);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMFCPropertyGridPropertyStr : public CMFCPropertyGridPropertyEx
{
	DECLARE_DYNAMIC(CMFCPropertyGridPropertyStr)

public:
	CMFCPropertyGridPropertyStr(CFieldStr* pField, CMFCPropertyGridPropertyEx* pParentProp);
	CMFCPropertyGridPropertyStr(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyGridPropertyStr(const CString& strName, LPCTSTR lpValue, LPCTSTR lpszDescr = NULL
		, DWORD dwMin = 0, DWORD dwMax = 0, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL, DWORD_PTR dwData = 0);

	virtual ~CMFCPropertyGridPropertyStr();

protected:

public:
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _DwString
{
	DWORD	m_dwValue;
	CString	m_String;
}DwString;

typedef CArray<DwString, DwString&> DwStringArray;
class CMFCPropertyGridPropertyCombo : public CMFCPropertyGridPropertyEx
{
	DwStringArray m_ValueArray;
public:
	CMFCPropertyGridPropertyCombo(CFieldCombo* pField, CMFCPropertyGridPropertyEx* pParentProp);
	CMFCPropertyGridPropertyCombo(const CString& strName, LPCTSTR lpValue, DwString* pDwStringArray, int count, LPCTSTR lpszDescr = NULL);
	CMFCPropertyGridPropertyCombo(const CString& strName, LPCTSTR lpDefaultValue, LPCTSTR lpDwString, LPCTSTR lpszDescr = NULL);

	virtual BOOL InitDwString( LPCTSTR lpDwString, LPCTSTR lpDefaultValue);

	virtual ~CMFCPropertyGridPropertyCombo(){};
	virtual DWORD GetDwValue() const { return (DWORD)GetData(); }
	virtual void SetDwValue(DWORD dwValue);
	virtual void SetStrValue(LPCTSTR str);

protected:
	virtual BOOL OnUpdateValue();

public:
	
};

