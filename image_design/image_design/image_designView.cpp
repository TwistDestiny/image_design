
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
#include "CRotateDlg.h"
#include "CExtend.h"
#include "SpaceOperate.h"
#include "C3DPoint.h"
#include "CImgVisiable.h"
#include "PhongLight.h"
#endif

#include "image_designDoc.h"
#include "image_designView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


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
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32784, &CimagedesignView::OnDrag)
	ON_COMMAND(ID_32785, &CimagedesignView::OnExpand)
	ON_COMMAND(ID_32786, &CimagedesignView::OnRotate)
	ON_COMMAND(ID_32787, &CimagedesignView::OnSymmetry)
	ON_COMMAND(ID_32812, &CimagedesignView::On3DOperate)
END_MESSAGE_MAP()
bool is_fill = false;
bool is_hide_line = false;
bool is_3D_fill = false;
CPoint last_point,er_point;
int rect_count = 0,xmin=0,xmax=50000,ymin=0,ymax=50000, drag_count = 0, symmetry_count = 0,rotate_degree = 0,extend_detla = 100,d3_type = 0;
int view_hight = 0;
C3DPoint zero_point_3d;
CPoint zero_pt(500,500);
pen_state* mypen;
CArray<C3DPoint> cli_pt_3d,cli_pt_out;
int drag_x = 0, drag_y = 0;
int light_r = 255, light_g = 120, light_b = 110;
COLORREF light_phong = RGB(light_r, light_g, light_b);
CArray<CImgVisiable*> flat_collection,flat_show;
// CimagedesignView 构造/析构
extern bool Move3d;
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
	if (this->Crotate != NULL)
		delete this->Crotate;
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
	rotate_img();
	extend_img();
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
	view_hight = wndRect2.Height();
	m_bitmap.CreateCompatibleBitmap(pDC, wndRect2.Width(), wndRect2.Height());
	SelectObject(h_dc, m_bitmap);
	h_dc.FillSolidRect(0, 0, wndRect2.Width(), wndRect2.Height(), RGB(255, 255, 255));
	CPoint point;
	//-----2D--------------//
	if (pen_view != d_3d&& d3_type ==0) {
		
		if (pen_view == d_rotate || pen_view == d_extend)
			cli_pt.Add(zero_pt);
		for (int i = 0; i < cli_pt.GetSize(); i++) {
			point = cli_pt.GetAt(i);
			CString str;
			//ZeroMemory(&str, sizeof(CString));
			str.Format(_T("x=%d,y=%d"), point.x, point.y);
			h_dc.TextOut(point.x, point.y, str);
			/*h_dc.Ellipse(point.x - 3, point.y - 3, point.x + 3, point.y + 3);*/
			CBrush brush(RGB(0, 0, 0));
			CBrush* old = h_dc.SelectObject(&brush);
			CRect rect;
			rect.SetRect(point.x - 1, point.y - 1, point.x + 1, point.y + 1);
			rect.InflateRect(3, 3);
			h_dc.Ellipse(rect);
			h_dc.SelectObject(old);
			if (i > 0 && pen_view != d_point) {
				if ((pen_view == d_rotate || pen_view == d_extend) && i == cli_pt.GetSize() - 1)
					continue;
				last_p = cli_pt.GetAt(i - 1);
				DDA_Line(n_dc, last_p, point, RGB(0, 0, 0));

			}
		}
		if (pen_view == d_rotate || pen_view == d_extend)
			cli_pt.RemoveAt(cli_pt.GetSize() - 1);
		if (is_closed && pen_view != d_point) {
			last_p = cli_pt.GetAt(0);
			DDA_Line(n_dc, cli_pt.GetAt(cli_pt.GetSize() - 1), last_p, RGB(0, 0, 0));

		}
		if (fillFlag) {
			CArray<C3DPoint> cli_pt_r;
			for (int i = 0; i < cli_pt.GetSize(); i++) {
				C3DPoint temp(cli_pt.GetAt(i));
				cli_pt_r.Add(temp);
			}
			fillPolygon(n_dc, cli_pt_r);
		}
	}
	//-----2D--------------//
	//-----3D--------------//
	else{
		for (int s = 0; s < flat_show.GetSize(); s++) {
			cout << "\n当前面" <<  s <<":";
			for (int k = 0; k < flat_show.GetAt(s)->img_pt.GetSize(); k++) {
				cout << "(x:" << flat_show.GetAt(s)->img_pt.GetAt(k)._x<<",y:" << flat_show.GetAt(s)->img_pt.GetAt(k)._y<< ",z:" << flat_show.GetAt(s)->img_pt.GetAt(k)._z << ") ";
			}
			cout << endl;
		}
		bool flag_noline = false;
		CArray<C3DPoint> *te_3d,*re_3d;
		for (int j = 0; j < flat_show.GetSize(); j++) {
			if (j == 1)
				flag_noline = true;

			cout << "可见面：" << flat_show.GetAt(j)->is_visiable() << endl;
			bool t_f = flat_show.GetAt(j)->is_visiable();
			if (j > 1)
				t_f = !t_f;
			if (t_f ||!is_hide_line) {
				te_3d = &(flat_show.GetAt(j)->img_pt);
				C3DPoint p1(te_3d->GetAt(0)._x, te_3d->GetAt(0)._y, te_3d->GetAt(0)._z), p3, last_p_3d, last_p_out;
				te_3d->Add(p1);
				for (int i = 0; i < te_3d->GetSize(); i++) {
					if (j > 1 && i == 0 && flag_noline)//不画重复的线
						continue;
					if (j > 1 && i == 2 && !flag_noline)
						continue;
					p3 = te_3d->GetAt(i);
					p3.To2dPoint();
					p3.x = p3.x ;
					p3.y = p3.y ;
					CString str2;
					//str2.Format(_T("x=%d,y=%d"), p3.x , p3.y );
					//dc.TextOut(p3.x, p3.y, str2);
					CString str;
					//ZeroMemory(&str, sizeof(CString));

					/*h_dc.Ellipse(point.x - 3, point.y - 3, point.x + 3, point.y + 3);*/
					CBrush brush(RGB(0, 0, 0));
					CBrush* old = h_dc.SelectObject(&brush);
					CRect rect;
					rect.SetRect(p3.x - 1, p3.y - 1, p3.x + 1, p3.y + 1);
					rect.InflateRect(2, 2);
					h_dc.Ellipse(rect);
					h_dc.SelectObject(old);
					if (i > 0 && pen_view != d_point) {
						last_p_3d = te_3d->GetAt(i - 1);
						last_p_3d.To2dPoint();
						last_p_3d.x = last_p_3d.x ;
						last_p_3d.y = last_p_3d.y ;
						DDA_Line(n_dc, last_p_3d, p3, RGB(0, 0, 0));
					}
				}
				te_3d->RemoveAt(te_3d->GetSize() - 1);
				if (is_3D_fill) {
					COLORREF col = fill_3Dlight(flat_show.GetAt(j)->flat_vector);
					fillPolygon(n_dc, flat_show.GetAt(j)->img_pt, col);
				}

				DDA_Line(n_dc, last_p_3d, p3, RGB(0, 0, 0));
			}

		}
	}
	//-----3D--------------//

	
		
	if (rect_count == 2&&pen_view == d_rect) {

		n_dc->SelectObject(GetStockObject(NULL_BRUSH));
		n_dc->Rectangle(CRect(rect_pt.GetAt(0), rect_pt.GetAt(1)));
	}
	if (pen_view == d_symmetry && symmetry_count == 2) {
		DDA_Line(n_dc, symmetry_pt.GetAt(0), symmetry_pt.GetAt(1), RGB(0, 0, 0));
	}
	pDC->BitBlt(0, 0, wndRect2.Width(), wndRect2.Height(), n_dc, 0, 0, SRCCOPY);

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
		if (rect_count < 2) {
			Insize(point);
			rect_count++;
			rect_pt.Add(point);
		}
		if (rect_count == 2) {
			Invalidate();
		}
		return;
	}
	if (pen_view == d_drag || Move3d) {
		drag_count++;
		drag_pt.Add(point);
		if (drag_count == 2) {
			int dx = drag_pt.GetAt(1).x - drag_pt.GetAt(0).x, dy = drag_pt.GetAt(1).y - drag_pt.GetAt(0).y;
			CPoint temp;
			C3DPoint te2,te3;
			for (int t = 0; t < cli_pt.GetSize(); t++) {
				temp = cli_pt.GetAt(t);

				temp.x += dx;
				temp.y += dy;
				cli_pt.SetAt(t, temp);
				/*te2 = cli_pt_3d.GetAt(t);
				te3 = cli_pt_out.GetAt(t);*/
				te2.x += dx;
				te2.y += dy;
				te3.x += dx;
				te3.y += dy;
				/*cli_pt_3d.SetAt(t, te2);
				cli_pt_out.SetAt(t, te3);*/
				
			}
			if (d3_type) {
				drag_x += dx;
				drag_y += dy;
			}
			drag_count = 0;
			drag_pt.RemoveAll();
			Invalidate();
			Move3d = false;
		}
	}
	if (pen_view == d_symmetry) {
		if (symmetry_count < 2) {
			symmetry_count++;
			symmetry_pt.Add(point);
			Invalidate();
		}
	}
	if (pen_view == d_rotate) {
		Crotate->set_edit(point);
		rotate_pt.Copy(cli_pt);

	}
	if (pen_view == d_extend) {
		Cextend_dlg->set_edit(point);
		extend_pt.Copy(cli_pt);
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

bool flag = false;
void CimagedesignView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (last_point.x > 0 && !is_closed && !is_fill && point.x > 0 && point.y > 0&&pen_view == d_line) {
		CPoint tmp = point;
		if (point.x < 5 || point.y < 5)
			tmp = last_point;
		CDC* pDC = GetDC();
		//获得图形显示设备指针
		pDC->SetROP2(R2_NOT);
		DDA_Line(pDC, last_point, er_point, RGB(0, 255, 0));  //绘制上次的图形，进行擦除
		DDA_Line(pDC, last_point, tmp, RGB(0, 255, 0));   //绘制新位置的图形
		flag = true;
		er_point = tmp;
		ReleaseDC(pDC);
	}
	//else if (flag) {
	//	CDC* pDC1 = GetDC();
	//	pDC1->SetROP2(R2_NOT);
	//	DDA_Line(pDC1, last_point, er_point, RGB(0, 255, 0));  //绘制上次的图形，进行擦除
	//	ReleaseDC(pDC1);
	//	flag = false;
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
	Invalidate();
}




void CimagedesignView::OnClearImg()
{
	if(cli_pt.GetSize()>0)
		cli_pt.RemoveAll();
	if (rect_pt.GetSize() > 0)
		rect_pt.RemoveAll();
	if (!drag_pt.IsEmpty())
		drag_pt.RemoveAll();
	if (!symmetry_pt.IsEmpty())
		symmetry_pt.RemoveAll();
	is_fill = false;
	fillFlag = false;
	is_closed = false;
	pen_view = d_none;
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}

void CimagedesignView::OnPolyCut()
{
	if (rect_pt.GetSize() >= 2&&is_closed) {
		CArray<CPoint> line_up, line_down, line_left, line_right;
		if (rect_pt.GetAt(0).x < rect_pt.GetAt(1).x) {
			line_left.Add(rect_pt.GetAt(0));
			line_left.Add(CPoint(rect_pt.GetAt(0).x, rect_pt.GetAt(1).y));
			line_right.Add(CPoint(rect_pt.GetAt(1).x, rect_pt.GetAt(0).y));
			line_right.Add(rect_pt.GetAt(1));
			
		}
		else {
			line_right.Add(rect_pt.GetAt(0));
			line_right.Add(CPoint(rect_pt.GetAt(0).x, rect_pt.GetAt(1).y));
			line_left.Add(CPoint(rect_pt.GetAt(1).x, rect_pt.GetAt(0).y));
			line_left.Add(rect_pt.GetAt(1));
		}
		if (rect_pt.GetAt(0).y < rect_pt.GetAt(1).y) {
			line_up.Add(rect_pt.GetAt(0));
			line_up.Add(CPoint(rect_pt.GetAt(1).x, rect_pt.GetAt(0).y));
			line_down.Add(CPoint(rect_pt.GetAt(0).x, rect_pt.GetAt(1).y));
			line_down.Add(rect_pt.GetAt(1));
		}
		else {
			line_down.Add(rect_pt.GetAt(0));
			line_down.Add(CPoint(rect_pt.GetAt(1).x, rect_pt.GetAt(0).y));
			line_up.Add(CPoint(rect_pt.GetAt(0).x, rect_pt.GetAt(1).y));
			line_up.Add(rect_pt.GetAt(1));
		}
		//------------------right-----------------------------------------//
	
		CPoint tmp = cli_pt.GetAt(0);
		cli_pt.Add(tmp);
		for (int j = 0; j < cli_pt.GetSize() - 1; j++) {
			CPoint p1 = cli_pt.GetAt(j);
			CPoint p2 = cli_pt.GetAt(j + 1),p3;
			double k = ((double)(p2.y - p1.y)) / (p2.x - p1.x);
			if (p1.x > line_right.GetAt(0).x) {
				if (p2.x <= line_right.GetAt(0).x) {
					p3.x = line_right.GetAt(0).x;
					p3.y = (int)(k * (p3.x - p1.x)) + p1.y;
					cut_pt.Add(p3);
				}
				//else{
				//	p3.x = line_right.GetAt(0).x;
				//	p3.y = p1.y;
				//	cut_pt.Add(p3);
				//}
			}
			else {
				cut_pt.Add(p1);
				if (p2.x > line_right.GetAt(0).x) {
					p3.x = line_right.GetAt(0).x;
					p3.y = (int)(k * (p3.x - p1.x)) + p1.y;
					cut_pt.Add(p3);
				}
			
			}
		}
		cli_pt.RemoveAll();
		cli_pt.Copy(cut_pt);
		cut_pt.RemoveAll();
		//------------------right----------------------//
		//------------------left----------------------//
		tmp = cli_pt.GetAt(0);
		cli_pt.Add(tmp);
		for (int j = 0; j < cli_pt.GetSize() - 1; j++) {
			CPoint p1 = cli_pt.GetAt(j);
			CPoint p2 = cli_pt.GetAt(j + 1), p3;
			double k = ((double)(p2.y - p1.y)) / (p2.x - p1.x);
			if (p1.x < line_left.GetAt(0).x) {
				if (p2.x >= line_left.GetAt(0).x) {
					p3.x = line_left.GetAt(0).x;
					p3.y = (int)(k * (p3.x - p1.x)) + p1.y;
					cut_pt.Add(p3);
				}
	/*			else {
					p3.x = line_left.GetAt(0).x;
					p3.y = p1.y;
					cut_pt.Add(p3);
				}*/
			}
			else {
				cut_pt.Add(p1);
				if (p2.x < line_left.GetAt(0).x) {
					p3.x = line_left.GetAt(0).x;
					p3.y = (int)(k * (p3.x - p1.x)) + p1.y;
					cut_pt.Add(p3);
				}

			}
		}
		cli_pt.RemoveAll();
		cli_pt.Copy(cut_pt);
		cut_pt.RemoveAll();
		//------------------left----------------------//
		//------------------up----------------------//
		tmp = cli_pt.GetAt(0);
		cli_pt.Add(tmp);
		for (int j = 0; j < cli_pt.GetSize() - 1; j++) {
			CPoint p1 = cli_pt.GetAt(j);
			CPoint p2 = cli_pt.GetAt(j + 1), p3;
			double t = ((double)(p2.x - p1.x)) / (p2.y - p1.y);
			if (p1.y < line_up.GetAt(0).y) {
				if (p2.y >= line_up.GetAt(0).y) {
					p3.y = line_up.GetAt(0).y;
					p3.x = (int)(t * (p3.y - p1.y)) + p1.x;
					cut_pt.Add(p3);
				}
				//else {
				//	p3.y = line_up.GetAt(0).y;
				//	p3.x = p1.x;
				//	cut_pt.Add(p3);
				//}
			}
			else {
				cut_pt.Add(p1);
				if (p2.y < line_up.GetAt(0).y) {
					p3.y = line_up.GetAt(0).y;
					p3.x = (int)(t * (p3.y - p1.y)) + p1.x;
					cut_pt.Add(p3);
				}

			}
		}
		cli_pt.RemoveAll();
		cli_pt.Copy(cut_pt);
		cut_pt.RemoveAll();
		//------------------up----------------------//
		//------------------down----------------------//
		tmp = cli_pt.GetAt(0);
		cli_pt.Add(tmp);
		for (int j = 0; j < cli_pt.GetSize() - 1; j++) {
			CPoint p1 = cli_pt.GetAt(j);
			CPoint p2 = cli_pt.GetAt(j + 1), p3;
			double t = ((double)(p2.x - p1.x)) / (p2.y - p1.y);
			if (p1.y > line_down.GetAt(0).y) {
				if (p2.y <= line_down.GetAt(0).y) {
					p3.y = line_down.GetAt(0).y;
					p3.x = (int)(t * (p3.y - p1.y)) + p1.x;
					cut_pt.Add(p3);
				}
			/*	else {
					p3.y = line_down.GetAt(0).y;
					p3.x = p1.x;
					cut_pt.Add(p3);
				}*/
			}
			else {
				cut_pt.Add(p1);
				if (p2.y > line_down.GetAt(0).y) {
					p3.y = line_down.GetAt(0).y;
					p3.x = (int)(t * (p3.y - p1.y)) + p1.x;
					cut_pt.Add(p3);
				}

			}
		}
		cli_pt.RemoveAll();
		cli_pt.Copy(cut_pt);
		cut_pt.RemoveAll();
		//------------------down----------------------//
	}

	Invalidate();

}

void CimagedesignView::fillPolygon(CDC* pDC,CArray<C3DPoint>& draw_pt,COLORREF col) {
	int ymin, ymax;
	cout << "\n当前面" << ":";
	for (int j = 0; j < draw_pt.GetSize(); j++) {
		draw_pt.GetAt(j).To2dPoint(d3_type);
		cout << "(x:" << draw_pt.GetAt(j).x << ",y:" << draw_pt.GetAt(j).y  <<",color:"<< col << ") ";
	}
	//cout << endl;
		
	if (draw_pt.GetAt(0).y <= draw_pt.GetAt(1).y) {
		ymin = draw_pt.GetAt(0).y;
		ymax = draw_pt.GetAt(1).y;
	}
	else {
		ymin = draw_pt.GetAt(1).y;
		ymax = draw_pt.GetAt(0).y;
	}
	for (int i = 2; i <= draw_pt.GetSize() - 1; i++) {
		if (draw_pt.GetAt(i).y < ymin)
			ymin = draw_pt.GetAt(i).y;
		else  if (draw_pt.GetAt(i).y > ymax)
			ymax = draw_pt.GetAt(i).y;
	}
	//从ymin扫描到 ymax
	C3DPoint tmp = draw_pt.GetAt(0);
	draw_pt.Add(tmp);//首先把首点加到最后一点，使首尾相接，最后删除
	//draw_pt.Add(draw_pt.GetAt(0));//首先把首点加到最后一点，使首尾相接，最后删除
	CArray<int, int> m_x_Array;//排序后的交点集合
	int m_x;//交点
	int j, k;
	for (int yi = ymin; yi <= ymax; yi++) {
		m_x_Array.RemoveAll();
		//判断扫描线和那些边相交，如相交，求交点,并排序

		for (int i = 0; i < draw_pt.GetSize() - 1; i++) {
			if (yi >= draw_pt.GetAt(i).y && yi < draw_pt.GetAt(i + 1).y) {
				//求交点				
				m_x = GetInterPointX(yi, draw_pt.GetAt(i).x, draw_pt.GetAt(i).y, draw_pt.GetAt(i + 1).x, draw_pt.GetAt(i + 1).y);
				InsertPoint_x(m_x_Array, m_x);
			}
			else if (yi >= draw_pt.GetAt(i + 1).y && yi < draw_pt.GetAt(i).y) {
				//求交点				
				m_x = GetInterPointX(yi, draw_pt.GetAt(i).x, draw_pt.GetAt(i).y, draw_pt.GetAt(i + 1).x, draw_pt.GetAt(i + 1).y);
				InsertPoint_x(m_x_Array, m_x);
			}
			else if (yi == draw_pt.GetAt(i + 1).y && yi == draw_pt.GetAt(i).y) {
				//是水平线,则将两个端点加入点集
				InsertPoint_x(m_x_Array, draw_pt.GetAt(i).x);
				InsertPoint_x(m_x_Array, draw_pt.GetAt(i + 1).x);
			}
		}
		//填充色
		for (j = 0; j <= m_x_Array.GetSize() - 2; j++, j++) {
			for (k = m_x_Array.GetAt(j); k <= m_x_Array.GetAt(j + 1); k++)
				pDC->SetPixel(k, yi, col);

		}
	}
	draw_pt.RemoveAt(draw_pt.GetSize() - 1);
}

//void CetRectLineX(CPoint p1, CPoint p2, CPoint t1, CPoint t2,int flag) {
//	if (flag == 0) { //upline
//		if (t1.y > p1.y && t2.y > p1.y)
//			;
//		else if (t1.y > p1.y&& t2.y < p1.y) {
//			int mx = GetInterPointX(p1.y, t1.x, t1.y, t2.x, t2.y);
//			if(mx>= min(p1.x,p2.x)&& mx <= max(p1.x, p2.x))
//		}
//	}
//	
//
//
//}

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
	else if (startPoint.y == endPoint.y) {
		int min = min(startPoint.x, endPoint.x), max = max(startPoint.x, endPoint.x);
		for (int i = min; i<= max; i++)
		{
			pDC->SetPixel(i, startPoint.y, crColor);
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


void CimagedesignView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pen_view == d_line)
		OnPolyClose();
	else if (is_closed && rect_count == 2&&pen_view == d_rect)
		OnPolyCut();
	else if (is_closed && symmetry_count == 2) {
		int x1 = symmetry_pt.GetAt(0).x, y1 = symmetry_pt.GetAt(0).y, x2 = symmetry_pt.GetAt(1).x, y2 = symmetry_pt.GetAt(1).y;
		if (d3_type) {

			y1 = view_hight - y1;
			x1 = x1 - zero_point_3d._x;
			y1 = y1 - zero_point_3d._y;
			y2 = view_hight - y2;
			x2 = x2 - zero_point_3d._x;
			y2 = y2 - zero_point_3d._y;
		}
		CPoint temp;
		if (x1 == x2) {
			for (int z = 0; z < cli_pt.GetSize(); z++) {
				temp = cli_pt.GetAt(z);
				temp.x = 2 * x1 - temp.x;
				cli_pt.SetAt(z, temp);
			}
			if (d3_type) {   //3D模型镜像变换
				for (int zr = 0; zr < flat_show.GetSize(); zr++) {
					for (int zj = 0; zj < flat_show.GetAt(zr)->img_pt.GetSize(); zj++) {
						flat_show.GetAt(zr)->img_pt.GetAt(zj)._x = 2 * x1 - flat_show.GetAt(zr)->img_pt.GetAt(zj)._x;
						flat_show.GetAt(zr)->img_pt.GetAt(zj)._y = 2 * x1 - flat_show.GetAt(zr)->img_pt.GetAt(zj)._y;
						flat_collection.GetAt(zr)->img_pt.GetAt(zj)._x = 2 * x1 - flat_collection.GetAt(zr)->img_pt.GetAt(zj)._x;
						flat_collection.GetAt(zr)->img_pt.GetAt(zj)._y = 2 * x1 - flat_collection.GetAt(zr)->img_pt.GetAt(zj)._y;
					}
				}
				symmetry_count = 0;
				if (symmetry_pt.GetSize() > 0) {
					symmetry_pt.RemoveAll();
				}
				pen_view = d_3d;
			}	
		}
		else {
			int A = y1 - y2, B = x2 - x1, C = (x1 - x2) * y1 - (y1 - y2) * x1;
			float M = A * A + B * B;
			for (int z = 0; z < cli_pt.GetSize(); z++) {
				temp = cli_pt.GetAt(z);
				int ja = temp.x, jb = temp.y;
				temp.x = -1 * (2 * A * B * jb + (A * A - B * B) * ja + 2 * A * C) / M;
				temp.y = -1 * (2 * A * B * ja + (-A * A + B * B) * jb + 2 * B * C) / M;
				cli_pt.SetAt(z, temp);
			}
			if (d3_type) {  //3D模型镜像变换
				for (int zr = 0; zr < flat_show.GetSize(); zr++) {
					for (int zj = 0; zj < flat_show.GetAt(zr)->img_pt.GetSize(); zj++) {
						int ja = flat_show.GetAt(zr)->img_pt.GetAt(zj)._x, jb = flat_show.GetAt(zr)->img_pt.GetAt(zj)._y;
						flat_show.GetAt(zr)->img_pt.GetAt(zj)._x = -1 * (2 * A * B * jb + (A * A - B * B) * ja + 2 * A * C) / M;
						flat_show.GetAt(zr)->img_pt.GetAt(zj)._y = -1 * (2 * A * B * ja + (-A * A + B * B) * jb + 2 * B * C) / M;
						ja = flat_collection.GetAt(zr)->img_pt.GetAt(zj)._x;
						jb = flat_collection.GetAt(zr)->img_pt.GetAt(zj)._y;
						flat_collection.GetAt(zr)->img_pt.GetAt(zj)._x = -1 * (2 * A * B * jb + (A * A - B * B) * ja + 2 * A * C) / M;
						flat_collection.GetAt(zr)->img_pt.GetAt(zj)._y = -1 * (2 * A * B * ja + (-A * A + B * B) * jb + 2 * B * C) / M;
					}
				}
				symmetry_count = 0;
				if (symmetry_pt.GetSize() > 0) {
					symmetry_pt.RemoveAll();
				}
				pen_view = d_3d; 
			}
		}
		Invalidate();
	}
	CView::OnRButtonDown(nFlags, point);
}


void CimagedesignView::OnDrag()
{
	drag_count = 0;
	if (!drag_pt.IsEmpty())
		drag_pt.RemoveAll();
	if (is_closed) {
		pen_view = d_drag;
	}
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}


void CimagedesignView::OnExpand()
{
	if (Cextend_dlg== NULL) {
		Cextend_dlg = new CExtend;
		Cextend_dlg->Create(IDD_DIALOG3, this);

	}
	if (is_closed) {
		pen_view = d_extend;
		zero_pt = cli_pt.GetAt(0);
		extend_pt.Copy(cli_pt);
		Cextend_dlg->ShowWindow(SW_SHOW);
		Cextend_dlg->set_edit(zero_pt);
	}
	// TODO: 在此添加命令处理程序代码
}


void CimagedesignView::OnRotate()
{
	//MessageBox(_T("消息框"), NULL, MB_OK);
	// TODO: 在此添加命令处理程序代码
	if (Crotate == NULL) {
		Crotate = new CRotateDlg;
		Crotate->Create(IDD_DIALOG2, this);

	}
	if (is_closed) {
		rotate_pt.Copy(cli_pt);
		pen_view = d_rotate;
		zero_pt = cli_pt.GetAt(0);
		Crotate->ShowWindow(SW_SHOW);
		Crotate->set_edit(zero_pt);
	}
	
}


void CimagedesignView::OnSymmetry()
{
	symmetry_count = 0;
	if (symmetry_pt.GetSize()>0) {
		symmetry_pt.RemoveAll();
	}
	if (is_closed) {
		pen_view = d_symmetry;
	}
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}

void CimagedesignView::rotate_img()
{
	CRect wndRect2;
	GetClientRect(wndRect2);
	if (rotate_degree != 0) {
		CPoint temp;
		int x, y, x1, y1,x2 = zero_pt.x,y2 = zero_pt.y, row,theta = rotate_degree,pi = 3.14159;
		row = wndRect2.Height();
		for (int i = 0; i < rotate_pt.GetSize(); i++) {
			temp = rotate_pt.GetAt(i);
			x1 = temp.x;
			y1 = temp.y;
			x1 = x1;
			y1 = y1;
			x2 = x2;
			y2 = y2;
			x = (x1 - x2) * cos(pi / 180.0 * theta) - (y1 - y2) * sin(pi / 180.0 * theta) + x2;
			y = (x1 - x2) * sin(pi / 180.0 * theta) + (y1 - y2) * cos(pi / 180.0 * theta) + y2;
			x = x;
			y = y;
			temp.x = x;
			temp.y = y;
			cli_pt.SetAt(i, temp);

		}
	
	}
}

void CimagedesignView::extend_img()
{
	if (extend_detla != 100) {
		CPoint temp;
		int x0 = zero_pt.x, y0 = zero_pt.y, x1, y1,x,y;
		for (int i = 0; i < extend_pt.GetSize(); i++) {
			temp = extend_pt.GetAt(i);
			x1 = temp.x;
			y1 = temp.y;
			x = extend_detla / 100.0 * x1 + (1.00 - extend_detla / 100.0) * x0;
			y = extend_detla / 100.0 * y1 + (1.00 - extend_detla / 100.0) * y0;
			temp.x = x;
			temp.y = y;
			cli_pt.SetAt(i, temp);
		}
	}
}
COLORREF CimagedesignView::fill_3Dlight(C3DPoint vector_line)
{
	PhongLight this_light;
	int p_z_1 = abs(vector_line._z);
	int p_x_1 = vector_line._x;
	int p_y_1 = vector_line._y;
	double p_x = (p_x_1 * 3) / (abs(p_z_1) + abs(p_x_1) + abs(p_y_1));
	double p_y = (p_y_1 * 3) / (abs(p_z_1) + abs(p_x_1) + abs(p_y_1));
	double p_z = (p_z_1 * 3) / (abs(p_z_1) + abs(p_x_1) + abs(p_y_1));
	float mdblE = this_light.Envirment_li / 100.0;
	float mdblD = this_light.m_reflect_li / 100.0;
	float mdblM = this_light.v_reflect_li / 100.0;
	float mtotal;
	cout << "sqrt = " << (p_x * p_x + p_y * p_y + p_z * p_z) << endl;
	float _zldotn = p_z <= 0 ? 0 : p_z /sqrt(p_x * p_x + p_y * p_y + p_z * p_z);
	float mdblFatt = this_light.de_li / 100.0, mdblDefuse = 0.5;
	mdblD = mdblFatt * mdblDefuse * _zldotn;
	float  m_hdot = _zldotn;
	mdblM = this_light.v_highlight_parm* pow(_zldotn,this_light.v_hightlight_exp);
	mtotal = mdblE * 0.1 + mdblD * 0.3 + mdblM * 0.6;
	cout << "(x:" << p_x << ",y:" << p_y << ",z:" << p_z << ",_zldotn:"<< _zldotn << ") ";
	cout << "color = " << mtotal << endl;
	return RGB(this_light.light_r * mtotal, this_light.light_g * mtotal, this_light.light_b * mtotal);

}
//拉伸 旋转 平移 镜像

void CimagedesignView::On3DOperate()
{
	if (C3DOperate == NULL) {
		C3DOperate = new SpaceOperate;
		C3DOperate->Create(IDD_DIALOG4, this);

	}
	if (is_closed) {
		zero_point_3d._x = cli_pt.GetAt(0).x;
		zero_point_3d._y = view_hight - cli_pt.GetAt(0).y;
		zero_point_3d._z = 0;
		for (int i = 0; i < cli_pt.GetSize(); i++) {
			C3DPoint temp(cli_pt.GetAt(i));
			cli_pt_3d.Add(temp);
			C3DPoint temp2(cli_pt.GetAt(i),10);
			cli_pt_out.Add(temp2);
		}

		CImgVisiable* p1 = new CImgVisiable();
		CImgVisiable* p2 = new CImgVisiable();
		p1->img_pt.Copy(cli_pt_3d);
		p2->img_pt.Copy(cli_pt_out);
		p1->setdir(true);
		p2->setdir(true);
		flat_collection.Add(p1);
		flat_collection.Add(p2);
		//保存两份防止多次运算累计误差

		CImgVisiable* p3 = new CImgVisiable();
		CImgVisiable* p4 = new CImgVisiable();
		p3->img_pt.Copy(p1->img_pt);
		p4->img_pt.Copy(p2->img_pt);
		flat_show.Add(p3);
		flat_show.Add(p4);

		C3DPoint temp3dpoint1 = p1->img_pt.GetAt(0);
		p1->img_pt.Add(temp3dpoint1);
		C3DPoint temp3dpoint2 = p2->img_pt.GetAt(0);
		p2->img_pt.Add(temp3dpoint2);

		for (int i = 0; i < p1->img_pt.GetSize() - 1; i++) {
			CImgVisiable* ptemp =  new CImgVisiable();
			CArray<C3DPoint> temp;
			temp.Add(p2->img_pt.GetAt(i));
			temp.Add(p2->img_pt.GetAt(i + 1));
			temp.Add(p1->img_pt.GetAt(i + 1));
			temp.Add(p1->img_pt.GetAt(i));
			ptemp->img_pt.Copy(temp);
			flat_collection.Add(ptemp);

			CImgVisiable* ptemp2 = new CImgVisiable();
			ptemp2->img_pt.Copy(ptemp->img_pt);
			flat_show.Add(ptemp2);
		}
		
		p1->img_pt.RemoveAt(p1->img_pt.GetSize() - 1);
		p2->img_pt.RemoveAt(p2->img_pt.GetSize() - 1);
		p1->setdir(false);
		flat_show.GetAt(0)->setdir(false);
		for (int j = 0; j < flat_collection.GetSize(); j++) {
			flat_show.GetAt(j)->getline();
		}

		mypen = &pen_view;
		pen_view = d_3d;
		C3DOperate->ShowWindow(SW_SHOW);
		C3DOperate->set_edit();
		d3_type = 1;

	}

	// TODO: 在此添加命令处理程序代码
}
