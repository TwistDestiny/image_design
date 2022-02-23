#pragma once


// Cpoint_data 对话框

class Cpoint_data : public CDialogEx
{
	DECLARE_DYNAMIC(Cpoint_data)

public:
	Cpoint_data(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Cpoint_data();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
