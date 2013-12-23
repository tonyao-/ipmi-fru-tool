
// IpmToolDoc.h : CIpmToolDoc 类的接口
//


#pragma once


class CIpmToolDoc : public CDocument
{
protected: // 仅从序列化创建
	CIpmToolDoc();
	DECLARE_DYNCREATE(CIpmToolDoc)

// 特性
public:
	static const DWORD	m_dwMaxFruDataLen = 4096;
	BYTE	m_FruDataBuff[m_dwMaxFruDataLen];
	DWORD	m_FruDataLen;
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CIpmToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
