
// image_designView.h: CimagedesignView 类的接口
//

#pragma once

class Cpoint_data;
class CRotateDlg;
class CExtend;
enum pen_state { d_none = 0,d_point, d_line, d_rect, d_cut ,d_drag,d_symmetry,d_rotate,d_extend};
class CimagedesignView : public CView
{
protected: // 仅从序列化创建
	CimagedesignView() noexcept;
	DECLARE_DYNCREATE(CimagedesignView)

// 特性
public:
	CimagedesignDoc* GetDocument() const;
	
	
 
// 操作
public:
	Cpoint_data* Cptdata = NULL;
	CRotateDlg* Crotate = NULL;
	CExtend* Cextend_dlg = NULL;
	CPoint last_p;
	CArray<CPoint> cli_pt,rect_pt,cut_pt,drag_pt, symmetry_pt,rotate_pt,extend_pt;
	bool fillFlag = false;
	bool is_closed = false;
	pen_state pen_view = d_line;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CimagedesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShow();
	afx_msg void OnClick();
	afx_msg void OnFill();
	void fillPolygon(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawPoint();
	afx_msg void OnDrawLine();
	afx_msg void OnPolyClose();
	afx_msg void OnDrawRect();
	afx_msg void OnPolyCut();
	afx_msg void OnClearImg();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrag();
	afx_msg void OnExpand();
	afx_msg void OnRotate();
	afx_msg void OnSymmetry();
	void rotate_img();
	void extend_img();
};

#ifndef _DEBUG  // image_designView.cpp 中的调试版本
inline CimagedesignDoc* CimagedesignView::GetDocument() const
   { return reinterpret_cast<CimagedesignDoc*>(m_pDocument); }
#endif

