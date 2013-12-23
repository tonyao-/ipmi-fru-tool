
// IpmToolDoc.cpp : CIpmToolDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "IpmTool.h"
#endif

#include "IpmToolDoc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIpmToolDoc

IMPLEMENT_DYNCREATE(CIpmToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CIpmToolDoc, CDocument)
END_MESSAGE_MAP()


// CIpmToolDoc ����/����

CIpmToolDoc::CIpmToolDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_FruDataLen = 0;
}

CIpmToolDoc::~CIpmToolDoc()
{
}

BOOL CIpmToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

BOOL CIpmToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(!CDocument::OnSaveDocument(lpszPathName))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CIpmToolDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}

	return TRUE;
}
// CIpmToolDoc ���л�

void CIpmToolDoc::Serialize(CArchive& ar)
{
	LPCTSTR pTagStr = L"FRU DATA V1.0 ";
	CString tagStr(pTagStr);
	CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();

	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}

	pFrame->m_wndProperties.Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CIpmToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CIpmToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CIpmToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIpmToolDoc ���

#ifdef _DEBUG
void CIpmToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIpmToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIpmToolDoc ����
