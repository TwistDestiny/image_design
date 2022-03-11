
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
	ON_COMMAND(ID_32777, &CimagedesignView::OnFill)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32778, &CimagedesignView::OnDrawPoint)
	ON_COMMAND(ID_32780, &CimagedesignView::OnDrawLine)
	ON_COMMAND(ID_32781, &CimagedesignView::OnPolyClose)
	ON_COMMAND(ID_32779, &CimagedesignView::OnDrawRect)
	ON_COMMAND(ID_32782, &CimagedesignView::OnPolyCut)
	ON_COMMAND(ID_32783, &CimagedesignView::OnClearImg)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
bool is_fill = false;
CPoint last_point,er_point;
int rect_count = 0,xmin=0,xmax=50000,ymin=0,ymax=50000;
// CimagedesignView 构造/析构
void InsertPoint_x(CArray<int, int>& m_x_Array, int m_x);
int GetInterPointX(int yx, int x0, int y0, int x1, int y1);
void DDA_Line(CDC*& pDC, CPoint& startPoint, CPoint& endPoint, COLORREF crColor);
void Insize(CPoint p);
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

void CimagedesignView::OnDraw(CDC* pDC)
{
	CimagedesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC h_dc;
	CDC *n_dc=&h_dc;
	
	CBitmap m_bitmap;
	h_dc.CreateCompatibleDC(pDC);
	CRect wndRect2;
	GetClientRect(wndRect2);
	m_bitmap.CreateCompatibleBitmap(pDC, wndRect2.Width(), wndRect2.Height());
	SelectObject(h_dc, m_bitmap);
	h_dc.FillSolidRect(0, 0, wndRect2.Width(), wndRect2.Height(), RGB(255, 255, 255));

	CPoint point;
	for (int i = 0; i < cli_pt.GetSize(); i++) {
		point = cli_pt.GetAt(i);
		CString str;
		//ZeroMemory(&str, sizeof(CString));
	/*	str.Format(_T("x=%d,y=%d"), point.x, point.y);
		h_dc.TextOut(point.x, point.y, str);*/
		/*h_dc.Ellipse(point.x - 3, point.y - 3, point.x + 3, point.y + 3);*/
		CBrush brush(RGB(0, 0, 0));
		CBrush* old = h_dc.SelectObject(&brush);
		CRect rect;
		rect.SetRect(point.x - 1, point.y - 1, point.x + 1, point.y + 1);
		rect.InflateRect(3, 3);
		h_dc.Ellipse(rect);
		h_dc.SelectObject(old);
		if (i>0&&pen_view != d_point) {
			last_p = cli_pt.GetAt(i-1);	
			DDA_Line(n_dc, last_p, point, RGB(0, 0, 0));

		}
	}
	if (is_closed && pen_view != d_point) {
		last_p = cli_pt.GetAt(0);
		DDA_Line(n_dc, point, last_p, RGB(0, 0, 0));

	}
	if(fillFlag)
		fillPolygon(n_dc);
	if (rect_count == 2) {

		n_dc->SelectObject(GetStockObject(NULL_BRUSH));
		n_dc->Rectangle(CRect(rect_pt.GetAt(0), rect_pt.GetAt(1)));
	}
	pDC->BitBlt(xmin, ymin, xmax-xmin, ymax-ymin, n_dc, 0, 0, SRCCOPY);

	m_bitmap.DeleteObject();		//删除位图对象
	h_dc.DeleteDC();
	n_dc = NULL;
	

	

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
	if (pen_view == d_rect) {
		Insize(point);
		rect_count++;
		rect_pt.Add(point);
		if (rect_count == 2) {
			pen_view = d_none;
			Invalidate();
		}
		return;
	}
	if (is_closed) {
		return;
	}
	else
		this->cli_pt.Add(point);
	Insize(point);
	last_point = point;
	er_point = point;
	CView::OnLButtonDown(nFlags, point);
	if (this->Cptdata != NULL) {
		Cpoint_data* ptr = (Cpoint_data*)this->Cptdata;
		ptr->position_X = point.x;
		ptr->position_Y = point.y;
		ptr->set_edit();

	}
	Invalidate();



	
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
		Cptdata->cli_ptr = &cli_pt;
	}

	 Cptdata->ShowWindow(SW_SHOW);


}


void CimagedesignView::OnFill()
{
	if(is_closed)
		fillFlag = !fillFlag;
	is_fill = fillFlag;
	Invalidate();

	// TODO: 在此添加命令处理程序代码
}
//扫描转换
	//得到最小和最大扫描线



void CimagedesignView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (last_point.x > 0 && !is_closed && !is_fill && point.x > 0 && point.y > 0) {
	//	CPoint tmp = point;
	//	if (point.x < 5 || point.y < 5)
	//		tmp = last_point;
	//	CDC* pDC = GetDC();
	//	//获得图形显示设备指针
	//	pDC->SetROP2(R2_NOT);
	//	DDA_Line(pDC, last_point, er_point, RGB(0, 255, 0));  //绘制上次的图形，进行擦除
	//	DDA_Line(pDC, last_point, tmp, RGB(0, 255, 0));   //绘制新位置的图形
	//	er_point = tmp;
	//}
	
		
	
	CView::OnMouseMove(nFlags, point);
}



void CimagedesignView::OnDrawPoint()
{
	// TODO: 在此添加命令处理程序代码
	if (is_closed)
		return;
	pen_view = d_point;
	Invalidate();
}


void CimagedesignView::OnDrawLine()
{
	// TODO: 在此添加命令处理程序代码
	if (is_fill|| is_closed)
		return;
	if (!is_closed)
		pen_view = d_line;
	Invalidate();
}


void CimagedesignView::OnPolyClose()
{
	// TODO: 在此添加命令处理程序代码
	
	if (is_fill|| pen_view == d_point) {
		return;
	}
	if (cli_pt.GetSize() >= 3) {
		is_closed = !is_closed;
	}
		
	Invalidate();
	//pen_view = pen_view == d_none ? d_line : d_none;
}


void CimagedesignView::OnDrawRect()
{
	// TODO: 在此添加命令处理程序代码
	/*CClientDC dc(this);
	CPoint t1(100, 100), t2(500, 500);
	dc.Rectangle(CRect(t1, t2));*/
	pen_view = d_rect;
	rect_count = 0;
	rect_pt.RemoveAll();
	CPoint p1(xmin, ymin), p2(xmax, ymax);
	CRect p(p1,p2);
	InvalidateRect(p);
}


void CimagedesignView::OnPolyCut()
{
	// TODO: 在此添加命令处理程序代码

}

void CimagedesignView::OnClearImg()
{
	cli_pt.RemoveAll();
	is_fill = false;
	fillFlag = false;
	is_closed = false;
	pen_view = d_none;
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}


void CimagedesignView::fillPolygon(CDC* pDC) {
	int ymin, ymax;
	if (this->cli_pt.GetAt(0).y <= this->cli_pt.GetAt(1).y) {
		ymin = this->cli_pt.GetAt(0).y;
		ymax = this->cli_pt.GetAt(1).y;
	}
	else {
		ymin = this->cli_pt.GetAt(1).y;
		ymax = this->cli_pt.GetAt(2).y;
	}
	for (int i = 2; i <= this->cli_pt.GetSize() - 1; i++) {
		if (this->cli_pt.GetAt(i).y < ymin)
			ymin = this->cli_pt.GetAt(i).y;
		else  if (this->cli_pt.GetAt(i).y > ymax)
			ymax = this->cli_pt.GetAt(i).y;
	}
	//从ymin扫描到 ymax
	CPoint tmp = cli_pt.GetAt(0);
	this->cli_pt.Add(tmp);//首先把首点加到最后一点，使首尾相接，最后删除
	//this->cli_pt.Add(cli_pt.GetAt(0));//首先把首点加到最后一点，使首尾相接，最后删除
	CArray<int, int> m_x_Array;//排序后的交点集合
	int m_x;//交点
	int j, k;
	for (int yi = ymin; yi <= ymax; yi++) {
		m_x_Array.RemoveAll();
		//判断扫描线和那些边相交，如相交，求交点,并排序
		for (int i = 0; i < cli_pt.GetSize() - 1; i++) {
			if (yi >= cli_pt.GetAt(i).y && yi < cli_pt.GetAt(i + 1).y) {
				//求交点				
				m_x = GetInterPointX(yi, cli_pt.GetAt(i).x, cli_pt.GetAt(i).y, cli_pt.GetAt(i + 1).x, cli_pt.GetAt(i + 1).y);
				InsertPoint_x(m_x_Array, m_x);
			}
			else if (yi >= cli_pt.GetAt(i + 1).y && yi < cli_pt.GetAt(i).y) {
				//求交点				
				m_x = GetInterPointX(yi, cli_pt.GetAt(i).x, cli_pt.GetAt(i).y, cli_pt.GetAt(i + 1).x, cli_pt.GetAt(i + 1).y);
				InsertPoint_x(m_x_Array, m_x);
			}
			else if (yi == cli_pt.GetAt(i + 1).y && yi == cli_pt.GetAt(i).y) {
				//是水平线,则将两个端点加入点集
				InsertPoint_x(m_x_Array, cli_pt.GetAt(i).x);
				InsertPoint_x(m_x_Array, cli_pt.GetAt(i + 1).x);
			}
		}
		//填充色
		for (j = 0; j <= m_x_Array.GetSize() - 2; j++, j++) {
			for (k = m_x_Array.GetAt(j); k <= m_x_Array.GetAt(j + 1); k++)
				pDC->SetPixel(k, yi, RGB(0, 255, 0));

		}
	}
	this->cli_pt.RemoveAt(cli_pt.GetSize() - 1);
}

int GetInterPointX(int yx, int x0, int y0, int x1, int y1)
{
	//求扫描线和线段交点
	int m_x;
	if (yx == y0)
		m_x = x0;
	else if (yx == y1)
		m_x = x1;
	else
	{
		int a = y0 - y1;
		int b = x1 - x0;
		int c = x0 * y1 - x1 * y0;
		double m_dblX;
		m_dblX = (-1) * (double(b * yx + c)) / (double)a;
		m_x = (int)(m_dblX + 0.5);
	}
	return  m_x;
}

void InsertPoint_x(CArray<int, int>& m_x_Array, int m_x)
{
	//排序
	int i_flag = 0;
	for (int i = 0; i < m_x_Array.GetSize(); i++)
	{
		i_flag = 0;
		if (m_x < m_x_Array.GetAt(i))
		{
			//插入中间
			m_x_Array.InsertAt(i, m_x);
			i_flag = 1;
			break;
		}
	}
	if (i_flag == 0)//加入尾部
		m_x_Array.Add(m_x);
}

void DDA_Line(CDC*& pDC, CPoint& startPoint, CPoint& endPoint, COLORREF crColor)
{
	//数值微分法画线
	if (endPoint.x != startPoint.x && endPoint.y != startPoint.y)
	{
		//非特殊位置直线，一般直线
		double x, y;
		double k;
		//斜率
		k = ((float)(endPoint.y - startPoint.y)) / ((float)(endPoint.x - startPoint.x));
		x = (double)startPoint.x;
		y = (double)startPoint.y;
		pDC->SetPixel((int)x, (int)y, crColor);
		if (fabs(k) <= 1.0)
		{
			for (int i = 0; i < abs(endPoint.x - startPoint.x); i++)
			{   //x增量
				if (endPoint.x > startPoint.x)
				{
					x += 1;
					y += k;
				}
				else
				{
					x -= 1;
					y -= k;
				}
				pDC->SetPixel((int)x, (int)(y + 0.5), crColor);
			}
		}
		else if (fabs(k) > 1.0)
		{
			for (int i = 0; i < abs(endPoint.y - startPoint.y); i++)
			{	//y增量
				if (endPoint.y > startPoint.y)
				{
					y += 1;
					x += 1.0 / k;
				}
				else
				{
					y -= 1;
					x -= 1.0 / k;
				}
				pDC->SetPixel((int)(x + 0.5), (int)(y), crColor);
			}
		}
	}
	else if (startPoint.x == endPoint.x)
	{
		//铅垂线
		if (startPoint.y < endPoint.y)
		{
			for (int i = startPoint.y; i <= endPoint.y; i++)
			{
				pDC->SetPixel(startPoint.x, i, crColor);
			}
		}
		else if (startPoint.y > endPoint.y)
		{
			for (int i = startPoint.y; i >= endPoint.y; i--)
			{
				pDC->SetPixel(startPoint.x, i, crColor);
			}
		}
	}
}

void Insize(CPoint p) {
	xmin = min(xmin, p.x-20);
	ymin = min(ymin, p.y-20);
	xmax = max(xmax, p.x+20);
	ymax = max(ymax, p.y+20);
}

BOOL CimagedesignView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
}
