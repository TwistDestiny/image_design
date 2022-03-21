// CRotateDlg.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"
#include "CRotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg 对话框

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)
extern int rotate_degree;
extern CPoint zero_pt;
int total_degree = 0;
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
	ON_WM_CLOSE()
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
	if (pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头

	{
		rotate_degree--;
		total_degree--;
	}

	else if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头

	{
		rotate_degree++;
		total_degree++;

	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strz;
	strz.Format(_T("%d"), rotate_degree);
	SetDlgItemTextW(IDC_EDIT1, strz);
	GetParent()->Invalidate();
	
}


void CRotateDlg::set_edit(CPoint p )
{
	CString strx, stry,strz;
	rotate_degree = 0;
	total_degree = 0;
	strx.Format(_T("%d"),p.x );
	stry.Format(_T("%d"), p.y);
	strz.Format(_T("%d"), total_degree);
	SetDlgItemTextW(IDC_EDIT_EXPAND, strx);
	SetDlgItemTextW(IDC_EDIT2, stry);
	SetDlgItemTextW(IDC_EDIT1, strz);
	zero_pt.x = p.x;
	zero_pt.y = p.y;

	GetParent()->Invalidate();
}

void CRotateDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	total_degree = rotate_degree = 0;
	CDialogEx::OnClose();
}
