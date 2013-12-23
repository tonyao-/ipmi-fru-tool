
// IpmToolView.cpp : CIpmToolView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIpmToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CIpmToolView 构造/析构

CIpmToolView::CIpmToolView()
{
	// TODO: 在此处添加构造代码

}

CIpmToolView::~CIpmToolView()
{
}

BOOL CIpmToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CIpmToolView 绘制

void CIpmToolView::OnDraw(CDC* /*pDC*/)
{
	CIpmToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CIpmToolView 打印


void CIpmToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIpmToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CIpmToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CIpmToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CIpmToolView 诊断

#ifdef _DEBUG
void CIpmToolView::AssertValid() const
{
	CView::AssertValid();
}

void CIpmToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIpmToolDoc* CIpmToolView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIpmToolDoc)));
	return (CIpmToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CIpmToolView 消息处理程序
