
// IpmToolDoc.h : CIpmToolDoc ��Ľӿ�
//


#pragma once


class CIpmToolDoc : public CDocument
{
protected: // �������л�����
	CIpmToolDoc();
	DECLARE_DYNCREATE(CIpmToolDoc)

// ����
public:
	static const DWORD	m_dwMaxFruDataLen = 4096;
	BYTE	m_FruDataBuff[m_dwMaxFruDataLen];
	DWORD	m_FruDataLen;
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CIpmToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
