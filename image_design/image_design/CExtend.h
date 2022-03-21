#pragma once


// CExtend 对话框

class CExtend : public CDialogEx
{
	DECLARE_DYNAMIC(CExtend)

public:
	CExtend(CWnd* pParent = nullptr);   // 标准构造函数
	void set_edit(CPoint p);
	virtual ~CExtend();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
};
