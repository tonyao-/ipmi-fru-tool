#pragma once

#include "FruInfoCm.h"
#include "MFCPropertyGridCtrlEx.h"

#define END_CODE 0XC1

class CPropFruInfo : public CMFCPropertyGridPropertyEx
{
protected:
	CFieldArray m_FieldArray;

	DWORD m_nMaxBufferLen;

public:
	DWORD m_FruDataLen;
	BYTE* m_pFruData;
	CMFCPropertyGridCtrlEx* m_pGrid;
public:
	CPropFruInfo(CMFCPropertyGridCtrlEx* pGrid, const CString& name, DWORD maxLen, BYTE* pData, int nLen, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE, LPCTSTR lpDesc = NULL);
	~CPropFruInfo(void);

	virtual BOOL UpdateData(BOOL isUpdateData = TRUE){return TRUE;};
	void CreateProp(LPCTSTR lpAfterName = L"");
	void CreateProp(CField* pFieldArray, int count, CMFCPropertyGridPropertyEx* pParent);
};

//CPropCommonHeader//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPropCommonHeader : public CPropFruInfo
{
	const static DWORD m_dwMaxCommonSize = sizeof(FruHeader);
public:
	FruHeader*		m_pFruHeader;
public:
	CPropCommonHeader(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, LPCTSTR lpFruRepoName);
	~CPropCommonHeader(void){};

public:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);

};

extern LPCTSTR g_CommonHeaderStr;

//CPropBoardInfo//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPropBoardInfo : public CPropFruInfo
{
	const static DWORD m_dwMaxSize = 6 + 33 * 4 + 10;
public:
	BoardAreaInfo*		m_pBoardAreaInfo;
public:
	CPropBoardInfo(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE, LPCTSTR lpDesc = NULL);
	~CPropBoardInfo(void);

public:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);

};

extern LPCTSTR g_BoardInfoAreaStr;

//CPropProductInfo//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPropProductInfo : public CPropFruInfo
{
	const static DWORD m_dwMaxSize = 3 + 33 * 6 + 10;
protected:
public:
	ProductInfo*		m_pProductInfo;
public:

	CPropProductInfo(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE, LPCTSTR lpDesc = NULL);
	~CPropProductInfo(void);

public:
	virtual BOOL UpdateData(BOOL isUpdateData = TRUE);

};

extern LPCTSTR g_PorductInfoAreaStr;
extern LPCTSTR g_StrTypeLength;
extern LPCTSTR g_StrLength;