#pragma once


// SpaceOperate 对话框

class SpaceOperate : public CDialogEx
{
	DECLARE_DYNAMIC(SpaceOperate)

public:
	SpaceOperate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SpaceOperate();
	void set_edit();
	void expand(int count);
	void rotate(int count);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin_expand(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin_rotate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	CString m_combo;
	CComboBox m_combo_tool;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAnimationStart();
	afx_msg void OnBnClickedButtonAnimationEnd();
	CSpinButtonCtrl rotate_ctrl;
	afx_msg void OnBnClickedCheckHide();
	BOOL isHide;
	afx_msg void OnBnClickedCheckLight();
	BOOL isLight;
	afx_msg void OnBnClickedButton3DMove();
	afx_msg void OnBnClickedButtonMirror();
};
