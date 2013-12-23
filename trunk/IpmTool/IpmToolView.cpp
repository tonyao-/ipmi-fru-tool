
// IpmToolView.cpp : CIpmToolView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "IpmTool.h"
#endif

#include "IpmToolDoc.h"
#include "IpmToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIpmToolView

IMPLEMENT_DYNCREATE(CIpmToolView, CView)

BEGIN_MESSAGE_MAP(CIpmToolView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIpmToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CIpmToolView ����/����

CIpmToolView::CIpmToolView()
{
	// TODO: �ڴ˴���ӹ������

}

CIpmToolView::~CIpmToolView()
{
}

BOOL CIpmToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CIpmToolView ����

void CIpmToolView::OnDraw(CDC* /*pDC*/)
{
	CIpmToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CIpmToolView ��ӡ


void CIpmToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIpmToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CIpmToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CIpmToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CIpmToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIpmToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIpmToolView ���

#ifdef _DEBUG
void CIpmToolView::AssertValid() const
{
	CView::AssertValid();
}

void CIpmToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIpmToolDoc* CIpmToolView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIpmToolDoc)));
	return (CIpmToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CIpmToolView ��Ϣ�������
