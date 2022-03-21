// CExtend.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"
#include "CExtend.h"
#include "afxdialogex.h"


// CExtend 对话框

IMPLEMENT_DYNAMIC(CExtend, CDialogEx)
extern CPoint zero_pt;
extern int extend_detla;
CExtend::CExtend(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

void CExtend::set_edit(CPoint p)
{
	extend_detla = 100;
	CString strx, stry, strz;
	strx.Format(_T("%d"), p.x);
	stry.Format(_T("%d"), p.y);
	strz.Format(_T("%d"), extend_detla);
	SetDlgItemTextW(IDC_EDIT2, strx);
	SetDlgItemTextW(IDC_EDIT3, stry);
	SetDlgItemTextW(IDC_EDIT1, strz);
	zero_pt.x = p.x;
	zero_pt.y = p.y;
	GetParent()->Invalidate();
}

CExtend::~CExtend()
{
}

void CExtend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExtend, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CExtend::OnDeltaposSpin1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CExtend 消息处理程序


void CExtend::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头

	{
		extend_detla--;
	
	}

	else if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头

	{
		extend_detla++;


	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strz;
	strz.Format(_T("%d"), extend_detla);
	SetDlgItemTextW(IDC_EDIT1, strz);
	GetParent()->Invalidate();
}


void CExtend::OnClose()
{
	extend_detla = 100;
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
