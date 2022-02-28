
// image_designView.cpp: CimagedesignView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Cpoint_data.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Cpoint_data.h"
#include "image_design.h"
#endif

#include "image_designDoc.h"
#include "image_designView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CimagedesignView

IMPLEMENT_DYNCREATE(CimagedesignView, CView)

BEGIN_MESSAGE_MAP(CimagedesignView, CView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SHOW, &CimagedesignView::OnShow)
	ON_COMMAND(ID_32775, &CimagedesignView::OnClick)
END_MESSAGE_MAP()

// CimagedesignView 构造/析构

CimagedesignView::CimagedesignView() noexcept
{
	// TODO: 在此处添加构造代码
	last_p.x = -1;
	last_p.y = -1;

}

CimagedesignView::~CimagedesignView()
{
	if (this->Cptdata != NULL)
		delete this->Cptdata;
}

BOOL CimagedesignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CimagedesignView 绘图

void CimagedesignView::OnDraw(CDC* /*pDC*/)
{
	CimagedesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CimagedesignView 诊断

#ifdef _DEBUG
void CimagedesignView::AssertValid() const
{
	CView::AssertValid();
}

void CimagedesignView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimagedesignDoc* CimagedesignView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimagedesignDoc)));
	return (CimagedesignDoc*)m_pDocument;
}
#endif //_DEBUG


// CimagedesignView 消息处理程序


void CimagedesignView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	//ZeroMemory(&str, sizeof(CString));
	str.Format(_T("x=%d,y=%d"), point.x, point.y);

	CDC* pDC = this->GetDC();
	
	pDC->TextOut(point.x, point.y, str);

	if (last_p.x > 0 && last_p.y > 0) {
		HDC hdc;
		hdc = ::GetDC(m_hWnd);
		MoveToEx(hdc, last_p.x, last_p.y, NULL);
		LineTo(hdc, point.x, point.y);
		::ReleaseDC(m_hWnd, hdc);
	}
	this->ReleaseDC(pDC);
	last_p = point;
	CView::OnLButtonDown(nFlags, point);
	if (this->Cptdata != NULL) {
		Cpoint_data* ptr = (Cpoint_data*)this->Cptdata;
		ptr->position_X = point.x;
		ptr->position_Y = point.y;
		ptr->set_edit();
	}



	
}


void CimagedesignView::OnShow()
{
	// TODO: 在此添加命令处理程序代码
	//MessageBox(_T("消息框"), NULL, MB_OK);
}


void CimagedesignView::OnClick()
{
	//MessageBox(_T("消息框"), NULL, MB_OK);
	// TODO: 在此添加命令处理程序代码
	if (Cptdata == NULL) {
		Cptdata = new Cpoint_data;
		Cptdata->Create(IDD_DIALOG1, this);
	}

	 Cptdata->ShowWindow(SW_SHOW);


}
