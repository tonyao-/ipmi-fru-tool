#include "StdAfx.h"
#include "MFCPropertyGridPropertyEx.h"
#include "MFCPropertyGridCtrlEx.h"

IMPLEMENT_DYNAMIC(CMFCPropertyGridPropertyEx, CMFCPropertyGridProperty)

CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList, LPCTSTR lpDesc)
	:CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	SetDescription(lpDesc);
}

CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr
	, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	:CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(CField* pCField, CMFCPropertyGridPropertyEx* pParentProp)
	:CMFCPropertyGridProperty(pCField->m_strName, 0, TRUE)
{
	this->SetDescription(pCField->m_strDesc);
	m_strFormat = L"";

	//CreatePropEx(pCField, this);
	
	pParentProp->AddSubItem(this);
	
	CMFCPropertyGridPropertyEx* pSubProp = NULL;
	CField* pSubCField = NULL;

	for(int i = 0; i < pCField->m_FieldArray.GetCount(); i++)
	{
		m_bGroup = TRUE;
		pSubCField = pCField->m_FieldArray[i];
		
		pSubProp = CreateProp(pSubCField, this);
	}
	
}

CMFCPropertyGridPropertyEx::~CMFCPropertyGridPropertyEx()
{
}

CMFCPropertyGridPropertyEx* CMFCPropertyGridPropertyEx::CreateProp(CFieldArray& fieldArray, CMFCPropertyGridPropertyEx* pParent)
{
	CMFCPropertyGridPropertyEx* pSubProp = NULL;

	for(int i = 0; i < fieldArray.GetCount(); i++)
	{
		pSubProp =  CreateProp(fieldArray[i], pParent);
	}
	return pSubProp;
}


CMFCPropertyGridPropertyEx* CMFCPropertyGridPropertyEx::CreateProp(CField* pField, CMFCPropertyGridPropertyEx* pParent)
{
	CMFCPropertyGridPropertyEx* pProp = NULL;
	if(pField->IsKindOf(RUNTIME_CLASS(CFieldNum)))
	{
		pProp = new CMFCPropertyGridPropertyNum((CFieldNum*)pField, pParent);
	}
	else if(pField->IsKindOf(RUNTIME_CLASS(CFieldCombo)))
	{
		pProp = new CMFCPropertyGridPropertyCombo((CFieldCombo*)pField, pParent);
		pParent->AllowEdit(FALSE);
	}
	else if(pField->IsKindOf(RUNTIME_CLASS(CFieldStr)))
	{
		pProp = new CMFCPropertyGridPropertyStr((CFieldStr*)pField, pParent);
	}
	else if(pField->IsKindOf(RUNTIME_CLASS(CField)))
	{
		pProp = new CMFCPropertyGridPropertyEx(pField, pParent);
	}
	else
	{
		ASSERT(FALSE);
		return NULL;
	}

	return pProp;
}
void CMFCPropertyGridPropertyEx::SetValue(const COleVariant& varValue)
{
	m_varValue = varValue;
	CMFCPropertyGridProperty::SetValue(varValue);
}

CMFCPropertyGridPropertyEx* CMFCPropertyGridPropertyEx::GetItemByName(LPCTSTR lpName)
{
	for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*) m_lstSubItems.GetNext(pos);
		ASSERT_VALID(pSubItem);

		if(wcscmp(pSubItem->GetName(), lpName) == 0)
		{
			return pSubItem;
		}
	}
	return NULL;
}

void CMFCPropertyGridPropertyEx::RemoveSubItem(LPCTSTR lpName)
{
	CMFCPropertyGridProperty*pProp = GetItemByName(lpName);
	if(pProp)
	{
		CMFCPropertyGridProperty::RemoveSubItem(pProp);
	}
}

CString CMFCPropertyGridPropertyEx::FormatProperty()
{
	CString strVal;
	if(m_strFormat != L"" && (VT_UINT == m_varValue.vt || VT_UI4 == m_varValue.vt))
	{
		strVal.Format(m_strFormat, m_varValue.ulVal);
		return strVal;
	}
	else
	{
		return CMFCPropertyGridProperty::FormatProperty();
	}
}

void CMFCPropertyGridPropertyEx::OnPropertyChanged(CMFCPropertyGridPropertyEx* pParentProp, CMFCPropertyGridPropertyEx* pChangedProp, const COleVariant& olderValue)
{
	pParentProp->UpdateData();
}

BOOL CMFCPropertyGridPropertyEx::OnUpdateValue()
{
	CString strText;
	m_pWndInPlace->GetWindowText(strText);
	BOOL bIsChanged = FormatProperty() != strText;
	COleVariant m_OlderValue = m_varValue;

	if(bIsChanged && m_varValue.vt == VT_BSTR)
	{
		if(strText.GetLength() > m_nMaxValue)
		{
			strText = strText.Left(m_nMaxValue);
			m_pWndInPlace->SetWindowText(strText);
		}
		else if(strText.GetLength() < m_nMinValue)
		{
			return FALSE;
		}
	}

	if(CMFCPropertyGridProperty::OnUpdateValue())
	{
		if(bIsChanged)
		{
			CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)GetParent();
			while(pParent && pParent->GetParent())
			{
				pParent = (CMFCPropertyGridPropertyEx*)pParent->GetParent();
			}

			if(pParent)
			{
				pParent->OnPropertyChanged(pParent, this, m_OlderValue);
			}
			((CMFCPropertyGridCtrlEx*)m_pWndList)->OnPropertyChanged(this, m_OlderValue);
		}
		return TRUE;
	}
	return FALSE;
}

DWORD CMFCPropertyGridPropertyEx::GetDwValue(int nRow, BYTE* pBuff, int len) const
{
	if(nRow >= GetSubItemsCount())
		return FALSE;

	DWORD dw = ((CMFCPropertyGridPropertyEx*)GetSubItem(nRow))->GetDwValue();
	ASSERT(len <= 4);

	memcpy(pBuff, &dw, len);

	return TRUE;
}

BOOL CMFCPropertyGridPropertyEx::GetStrValue(int nRow, CString& str)
{
	if(nRow >= GetSubItemsCount())
		return FALSE;

	str = GetSubItem(nRow)->GetValue().bstrVal;
	return TRUE;
}

BOOL CMFCPropertyGridPropertyEx::GetStrValue(int nRow, int nSubRow, CString& str)
{
	if(nRow >= GetSubItemsCount()) return FALSE;

	CMFCPropertyGridPropertyEx* pSubItem = (CMFCPropertyGridPropertyEx*)GetSubItem(nRow);
	if(pSubItem == NULL) return 0;

	if(nSubRow >= pSubItem->GetSubItemsCount())
		return FALSE;

	str = pSubItem->GetSubItem(nSubRow)->GetValue().bstrVal;
	return TRUE;
}

int CMFCPropertyGridPropertyEx::GetStrValue(BYTE* chBuff, int nLen, Bool isUnicode)
{
	int len = 0;

	if(isUnicode)
	{
		CString str(GetValue().bstrVal);

		len = str.GetLength() << 1;
		if(len > nLen)
		{
			return 0;
		}

		memcpy(chBuff, str, len);
	}
	else
	{
		CStringA str(GetValue().bstrVal);

		len = str.GetLength();
		if(len > nLen)
		{
			return 0;
		}

		memcpy(chBuff, str, len);
	}

	return len;
};

BOOL CMFCPropertyGridPropertyEx::GetIpAddr(int nRow, BYTE* pAddrBuff)
{
	int i = 3;
	int iStart = 0;
	int iEnd = 0;


	CStringA strIpAddr(GetSubItem(nRow)->GetValue().bstrVal);

	iEnd = strIpAddr.Find(".", iStart);
	while(iEnd >= 0 && i >= 0)
	{
		CStringA str = strIpAddr.Mid(iStart, iEnd - iStart);
		pAddrBuff[i--] = (BYTE)atoi(str);

		iStart = iEnd + 1;
		if(i > 0)
		{
			iEnd = strIpAddr.Find(".", iStart);
		}
		else
		{
			iEnd = strIpAddr.GetLength();
		}
	}

	if(i == 0)
		return TRUE;
	else
		return FALSE;
}

void CMFCPropertyGridPropertyEx::SetIpAddr(int nRow, BYTE* pAddrBuff)
{
	CString ipAddr;
	ipAddr.Format(L"%03d.%03d.%03d.%03d", pAddrBuff[3], pAddrBuff[2], pAddrBuff[1], pAddrBuff[0]);
	SetStrValue(nRow, ipAddr);
}

int CMFCPropertyGridPropertyEx::GetByArrayValue(BYTE* chBuff, int nLen)
{
	CStringA strA(m_varValue.bstrVal);
	CHAR* chArray = strA.GetBuffer();
	//hextoi(strA.GetBuffer(nLen));
	int nSize = (strA.GetLength() > nLen) ? nLen : strA.GetLength();
	for(int i = 0; i < nSize; i++)
	{
		chBuff[i] = Char2Num(chArray[i]);
	}
	return nSize;
}

void CMFCPropertyGridPropertyEx::SetByArrayValue(BYTE* chBuff, int nLen)
{
	CHAR* chArray =  new CHAR[nLen + 1];

	memset(chArray, 0, nLen + 1);
	for(int i = 0; i < nLen; i++)
	{
		chArray[i] = Num2Char(chBuff[i]);
	}
	m_varValue = CString(chArray);

	delete[] chArray;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMFCPropertyGridPropertyNum, CMFCPropertyGridPropertyEx)

CMFCPropertyGridPropertyNum::CMFCPropertyGridPropertyNum(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	:CMFCPropertyGridPropertyEx(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyGridPropertyNum::CMFCPropertyGridPropertyNum(CFieldNum* pCField, CMFCPropertyGridPropertyEx* pParentProp)
	:CMFCPropertyGridPropertyEx(pCField->m_strName, (_variant_t)pCField->m_dwDefault, pCField->m_strDesc, 0)
{
	pParentProp->AddSubItem(this);
	m_strFormat = pCField->m_strFormat;
	m_nMinValue = pCField->m_dwMin;
	m_nMaxValue = pCField->m_dwMax;
	if(m_nMinValue == m_nMaxValue)
	{
		//AllowEdit(FALSE);
		Enable(FALSE);
	}
}


CMFCPropertyGridPropertyNum::CMFCPropertyGridPropertyNum(const CString& strName, DWORD varValue, LPCTSTR lpszDescr
	, LPCTSTR lpszFormat, DWORD dwMin, DWORD dwMax, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	:CMFCPropertyGridPropertyEx(strName, (_variant_t)varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	m_strFormat = lpszFormat;

	m_nMinValue = dwMin;
	m_nMaxValue = dwMax;

	if(m_nMinValue == m_nMaxValue)
	{
		//AllowEdit(FALSE);
		Enable(FALSE);
	}
}

CMFCPropertyGridPropertyNum::~CMFCPropertyGridPropertyNum()
{
}

CString CMFCPropertyGridPropertyNum::FormatProperty()
{
	CString strVal;
	if(m_strFormat != L"" && (VT_UINT == m_varValue.vt || VT_UI4 == m_varValue.vt))
	{
		strVal.Format(m_strFormat, m_varValue.ulVal);
		return strVal;
	}
	else
	{
		return CMFCPropertyGridProperty::FormatProperty();
	}
}


BOOL CMFCPropertyGridPropertyNum::TextToVar(const CString& strText)
{
	BOOL bIsChanged = FormatProperty() != strText;

	if(bIsChanged)
	{
		if(m_varValue.vt == VT_UINT || m_varValue.vt == VT_UI4)
		{
			if(m_strFormatLong.Find(L"x", 0) >= 0 || strText.Find(L"X", 0))
			{
				m_varValue.ulVal = hextoi(strText);
				if((int)m_varValue.ulVal > m_nMaxValue)
				{
					m_varValue.ulVal = m_nMaxValue;
					//strMsg.Format(L"[%s] max value is [%d(0x%x)]", this->m_strName, m_nMaxValue, m_nMaxValue);
				}
				else if((int)m_varValue.ulVal < m_nMinValue)
				{
					m_varValue.ulVal = m_nMinValue;
					//strMsg.Format(L"[%s] min value is [%d(0x%x)]", this->m_strName, m_nMinValue, m_nMinValue);
				}
			}
			return TRUE;
		}
		return CMFCPropertyGridProperty::TextToVar(strText);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMFCPropertyGridPropertyStr, CMFCPropertyGridPropertyEx)

CMFCPropertyGridPropertyStr::CMFCPropertyGridPropertyStr(CFieldStr* pField, CMFCPropertyGridPropertyEx* pParentProp)
	:CMFCPropertyGridPropertyEx(pField->m_strName, (_variant_t)pField->m_strDefault, pField->m_strDesc
		, 0, pField->m_strEditMask, pField->m_strEditTemp, pField->m_strValidChars)
{
	m_nMinValue = pField->m_dwMin;
	m_nMaxValue = pField->m_dwMax;

	pParentProp->AddSubItem(this);
}

CMFCPropertyGridPropertyStr::CMFCPropertyGridPropertyStr(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	:CMFCPropertyGridPropertyEx(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyGridPropertyStr::CMFCPropertyGridPropertyStr(const CString& strName, LPCTSTR varValue, LPCTSTR lpszDescr
	, DWORD dwMin, DWORD dwMax, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars, DWORD_PTR dwData)
	:CMFCPropertyGridPropertyEx(strName, (_variant_t)varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	m_nMinValue = dwMin;
	m_nMaxValue = dwMax;

}

CMFCPropertyGridPropertyStr::~CMFCPropertyGridPropertyStr()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMFCPropertyGridPropertyCombo::CMFCPropertyGridPropertyCombo(const CString& strName, LPCTSTR lpValue, DwString* pDwStringArray, int count, LPCTSTR lpszDescr)
	: CMFCPropertyGridPropertyEx(strName, (_variant_t)lpValue, lpszDescr, 0, 0, 0, 0)
{
	for(int n = 0; n < count; n++, pDwStringArray++)
	{
		m_ValueArray.Add(*pDwStringArray);
		AddOption(pDwStringArray->m_String);

		if(wcscmp(pDwStringArray->m_String, lpValue) == 0) 
		{
			SetData((DWORD_PTR)pDwStringArray->m_dwValue);
		}
	}

	AllowEdit(FALSE);
}

CMFCPropertyGridPropertyCombo::CMFCPropertyGridPropertyCombo(CFieldCombo* pField, CMFCPropertyGridPropertyEx* pParentProp)
	: CMFCPropertyGridPropertyEx(pField->m_strName, (_variant_t)pField->m_strDefault, pField->m_strDesc, 0, 0, 0, 0)
{
	pParentProp->AddSubItem(this);
	InitDwString(pField->m_strDwString, pField->m_strDefault);

	AllowEdit(FALSE);
}

//lpDwString format = 0=Select|1=Selecyt2|...
CMFCPropertyGridPropertyCombo::CMFCPropertyGridPropertyCombo(const CString& strName, LPCTSTR lpDefaultValue, LPCTSTR lpDwString, LPCTSTR lpszDescr)
	: CMFCPropertyGridPropertyEx(strName, (_variant_t)lpDefaultValue, lpszDescr, 0, 0, 0, 0)
{
	InitDwString(lpDwString, lpDefaultValue);
	AllowEdit(FALSE);
}

BOOL CMFCPropertyGridPropertyCombo::InitDwString(LPCTSTR lpDwString, LPCTSTR lpDefaultValue)
{
	CStringArray strArray;
	DwString dwString;

	ParseString(lpDwString, L"|", strArray);

	ASSERT(strArray.GetCount());

	for(int i = 0; i < strArray.GetCount(); i++)
	{
		CStringArray array1;

		ParseString(strArray[i], L"=", array1);
		ASSERT(array1.GetCount() == 2);

		if(array1.GetCount() == 2)
		{
			dwString.m_dwValue = _wtoi(array1[0].Trim());
			dwString.m_String = array1[1].Trim();
			m_ValueArray.Add(dwString);
			AddOption(dwString.m_String);

			if(wcscmp(dwString.m_String, lpDefaultValue) == 0) 
			{
				SetData((DWORD_PTR)dwString.m_dwValue);
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMFCPropertyGridPropertyCombo::OnUpdateValue()
{
	CString strText;
	m_pWndInPlace->GetWindowText(strText);
	BOOL bIsChanged = FormatProperty() != strText;
	COleVariant m_OlderValue = m_varValue;

	if(bIsChanged && m_varValue.vt == VT_BSTR)
	{
		for(int i = 0; i < m_ValueArray.GetCount(); i++)
		{
			if(wcscmp(m_ValueArray[i].m_String, strText) == 0) 
			{
				SetData((DWORD_PTR)m_ValueArray[i].m_dwValue);
			}
		}
	}

	if(CMFCPropertyGridProperty::OnUpdateValue())
	{
		if(bIsChanged)
		{
			CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)GetParent();
			while(pParent && pParent->GetParent())
			{
				pParent = (CMFCPropertyGridPropertyEx*)pParent->GetParent();
			}

			if(pParent)
			{
				pParent->OnPropertyChanged(pParent, this, m_OlderValue);
			}
			((CMFCPropertyGridCtrlEx*)m_pWndList)->OnPropertyChanged(this, m_OlderValue);
		}
		return TRUE;
	}

	return FALSE;
}

void CMFCPropertyGridPropertyCombo::SetDwValue(DWORD dwValue)
{
	for(int i = 0; i < m_ValueArray.GetCount(); i++)
	{
		if(m_ValueArray[i].m_dwValue == dwValue) 
		{
			CMFCPropertyGridPropertyEx::SetStrValue(m_ValueArray[i].m_String);
			SetData((DWORD_PTR)m_ValueArray[i].m_dwValue);
		}
	}
}

void CMFCPropertyGridPropertyCombo::SetStrValue(LPCTSTR str)
{
	for(int i = 0; i < m_ValueArray.GetCount(); i++)
	{
		if(wcscmp(m_ValueArray[i].m_String, str) == 0) 
		{
			CMFCPropertyGridPropertyEx::SetStrValue(m_ValueArray[i].m_String);
			SetData((DWORD_PTR)m_ValueArray[i].m_dwValue);
		}
	}
}
