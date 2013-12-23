#pragma once

#include "FruInfoCm.h"


#define GET_ROW_DWVALUE(pGroup, row)			pGroup->GetSubItem(row)->GetValue().ulVal
#define GET_ROW_DWVALUE_BYTE(pGroup, row)		(BYTE)GET_ROW_DWVALUE(pGroup, row)
#define SET_ROW_DWVALUE(pGroup, row, value)		(pGroup)->GetSubItem(row)->SetValue((_variant_t)((DWORD)value))


class CMFCPropertyGridCtrlEx;
class CMFCPropertyGridPropertyEx;

class CFruRecord
{
protected:
	RecordHeader*				m_pHeader;
	CMFCPropertyGridCtrlEx*		m_pGrid;
	CMFCPropertyGridPropertyEx* m_pGroup;
public:
	CFruRecord();
	virtual ~CFruRecord();

public:
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue);
	virtual void InitRecord(BYTE* pRecordData){};
	virtual void InitGrid(CMFCPropertyGridCtrlEx* pGrid){};
	virtual void InitRecordHeard(CMFCPropertyGridCtrlEx* pGrid, BYTE PicmgID, LPCTSTR lpTitle, LPCTSTR lpPicmgIdDesc);
	BOOL SetValue(int nRow, DWORD dwValue, LPCTSTR lpDesc = NULL);
	DWORD GetDwValue(int nRow);
	DWORD GetDwValue(int nRow, BYTE* pBuff, int len);
	BYTE GetByteValue(int nRow){return (BYTE)GetDwValue(nRow);};
	void SetByteValue(int nRow, BYTE value){SetValue(nRow, value);};
	BOOL SetStrValue(int nRow, const char* lpStr);
	BOOL SetStrValue(int nRow, LPCTSTR lpStr);
	DWORD GetSubDwValue(int nRow, int nSubRow);
	BOOL SetSubDwValue(int nRow, int nSubRow, DWORD dwValue);

	DWORD GetSubDwValue(CMFCPropertyGridPropertyEx* pProp, int nRow, int nSubRow);

	BOOL GetSubDwValue(int nRow, int nSubRow, BYTE* pBuff, int len);
	int GetStrValue(int nRow, BYTE* chBuff, int nLen, Bool isUnicode = FALSE);
	BOOL GetStrValue(int nRow, CString& str);
	BOOL GetSubStrValue(int nRow, int nSubRow, CString& str);
	BOOL GetIpAddr(int nRow, BYTE* pAddrBuff);
	virtual void Update(BOOL isFlag = TRUE);

	//virtual CRecoField* FindRecoField(LPCTSTR lpName);
	//virtual void RemoveRecoField(LPCTSTR lpName);
	//virtual void RemoteRecoField(CRecoField* pRecoField);
	//virtual void Update(BOOL isFlag){};

	//virtual void NumFieldTempChanged(CRecoField* pRecoField, LPCTSTR newStr, LPCTSTR olderStr){};
};
