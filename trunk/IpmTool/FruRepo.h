#pragma once

#include "PropCommonHeader.h"
#include "PropFruInfoPart.h"


#define MAX_FRU_RECORD	10
#define MAX_FRU_SIZE	1024

class CFruRepoDataArray;
class CFruRepoData : public CObject
{
	DECLARE_DYNAMIC(CFruRepoData)
	friend CFruRepoDataArray;
protected:
	uint8 m_nDeviceId;
	int m_nUsedLen;
	int m_nMaxLen;
	uint8* m_pData;
	CString m_strName;
public:
	CFruRepoData(uint8 nDeviceId, int nMaxLen, LPCTSTR lpName);
	~CFruRepoData();
public:
	void Reset(){m_nUsedLen = 0;};

	uint8* GetData(){return m_pData;};
	BOOL SetData(uint8* pData, int dataLen);

	int GetMaxLen(){return m_nMaxLen;};
	CString GetName(){return m_strName;};

	int GetUsedLen(){return m_nUsedLen;};
	void SetUsedLen(int nUsedLen){ASSERT(m_nUsedLen <= m_nMaxLen); m_nUsedLen = nUsedLen;};

	virtual void Serialize(CArchive& ar);
};

typedef CArray<CPropFruInfo*, CPropFruInfo*> CPropFruInfoArray;
class CFruRepo : public CMFCPropertyGridCtrlEx
{
protected:
	CFruRepoData*	m_pFruRepoData;

	BOOL	m_isIncludeMultiRecord;

	CPropFruInfoArray m_PropArray;
public:
	CFruRepo();
	~CFruRepo(void);

public:
	void Reset();

	BOOL Init(BOOL isIncludeInnerUseArea
		, BOOL isIncludeChassisInfoArea
		, BOOL isIncludeBoardAreaInfo
		, BOOL isIncludeProductAreaInfo
		, CFruRepoData* pRepo
		);

	BOOL Init(InnerUseArea* pInnerUseArea
		, ChassisInfoArea* pChassisInfoArea
		, BoardAreaInfo* pBoardAreaInfo
		, ProductInfo* pProductAreaInfo
		);

	static int CreateCommHeader(uint8* pBuff, int nLen);

	void AddCommHeader();
	void AddBoardInfoArea();
	void AddProductInfoArea();

	CPropFruReco* AddRecord(RecordHeader* pRecord, uint8 nLen);
	void RemoveRecord(uint8 reocrdId, uint8 nPicmgRecordID);
	void RemoveInnerUseArea();
	void RemoveChassisInfoArea();
	void RemoveBoardAreaInfo();
	void RemoveProductAreaInfo();

	void UpdateCommHeader();

	virtual int GetFruData(BYTE* pFruData, int nBufLen);
	virtual void SetFruData(BYTE* pFruData, int nLen);
	virtual void SetFruData(CFruRepoData* pRepo);
	virtual CPropFruReco* CreateProp(RecordHeader* pFruReco);

protected:
	CPropCommonHeader*	m_pCommonHeaderProp;
	CPropBoardInfo*		m_pBoardInfoProp;
	CPropProductInfo*	m_pProductInfoProp;
	
	FruInfo 		m_FruInfo;
	RecordHeader*	m_pFruRecordArray[MAX_FRU_RECORD];
	uint8			m_FruRepoBuff[MAX_FRU_SIZE];

protected:
	virtual void OnPropertyChanged(CMFCPropertyGridPropertyEx* pProp, const COleVariant& olderValue);
	BOOL Update(CMFCPropertyGridPropertyEx* pProp, BOOL isDataUpdate = TRUE);
	BOOL CommonHeaderInit();
};
