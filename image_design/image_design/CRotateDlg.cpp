// CRotateDlg.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"
#include "CRotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg 对话框

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, rotate_data);
	DDX_Control(pDX, IDC_EDIT_EXPAND, rotate_zero);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CRotateDlg::OnDeltaposSpin1)
END_MESSAGE_MAP()


// CRotateDlg 消息处理程序


void CRotateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("消息框"), NULL, MB_OK);
	//CDialogEx::OnOK();
}


void CRotateDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	MessageBox(_T("消息框"), NULL, MB_OK);
}


void CRotateDlg::set_edit(CPoint p )
{
	CString strx, stry;
	strx.Format(_T("%d"),p.x );
	stry.Format(_T("%d"), p.y);
	SetDlgItemTextW(IDC_EDIT_EXPAND, strx);
	SetDlgItemTextW(IDC_EDIT2, stry);
}