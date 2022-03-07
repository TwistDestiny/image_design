
// image_designView.h: CimagedesignView 类的接口
//

#pragma once

class Cpoint_data;
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
	CPoint last_p;
	CArray<CPoint> cli_pt;
	bool fillFlag = false;
	bool is_closed = false;
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
};

#ifndef _DEBUG  // image_designView.cpp 中的调试版本
inline CimagedesignDoc* CimagedesignView::GetDocument() const
   { return reinterpret_cast<CimagedesignDoc*>(m_pDocument); }
#endif

