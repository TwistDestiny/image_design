
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
END_MESSAGE_MAP()
bool is_fill = false;
// CimagedesignView 构造/析构
void InsertPoint_x(CArray<int, int>& m_x_Array, int m_x);
int GetInterPointX(int yx, int x0, int y0, int x1, int y1);
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
	CDC* pDC = this->GetDC();
	HDC hdc;
	CPoint point;
	hdc = ::GetDC(m_hWnd);
	for (int i = 0; i < cli_pt.GetSize(); i++) {
		point = cli_pt.GetAt(i);
		CString str;
		//ZeroMemory(&str, sizeof(CString));
		str.Format(_T("x=%d,y=%d"), point.x, point.y);
		pDC->TextOut(point.x, point.y, str);
		if (i>0) {
			last_p = cli_pt.GetAt(i-1);	
			MoveToEx(hdc, last_p.x, last_p.y, NULL);
			LineTo(hdc, point.x, point.y);

		}
	}
	is_closed = false;
	if (Cptdata != NULL && Cptdata->is_closed) {
		is_closed = true;
		last_p = cli_pt.GetAt(0);
		MoveToEx(hdc, point.x, point.y, NULL);
		LineTo(hdc, last_p.x, last_p.y);

	}
	::ReleaseDC(m_hWnd, hdc);
	if(fillFlag)
		fillPolygon(pDC);
	this->ReleaseDC(pDC);

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
	if (is_closed|| fillFlag)
		return;
	else
		this->cli_pt.Add(point);

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
		Cptdata->is_closed = false;
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

	CArray<int, int> m_x_Array;//排序后的交点集合
	int m_x;//交点
	int j, k;
	for (int yi = ymin; yi <= ymax; yi++) {
		m_x_Array.RemoveAll();
		//判断扫描线和那些边相交，如相交，求交点,并排序
		for (int i = 0; i < cli_pt.GetSize() - 1; i++) {
			if (yi >= cli_pt.GetAt(i).y && yi < cli_pt.GetAt(i + 1).y) {
				//求交点				
				m_x=GetInterPointX(yi,cli_pt.GetAt(i).x,cli_pt.GetAt(i).y,cli_pt.GetAt(i+1).x,cli_pt.GetAt(i+1).y);
				InsertPoint_x(m_x_Array, m_x);
			}
			else if (yi >= cli_pt.GetAt(i + 1).y && yi < cli_pt.GetAt(i).y) {
				//求交点				
				m_x=GetInterPointX(yi,cli_pt.GetAt(i).x,cli_pt.GetAt(i).y,cli_pt.GetAt(i+1).x,cli_pt.GetAt(i+1).y);
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