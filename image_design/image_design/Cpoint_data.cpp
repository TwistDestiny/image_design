// Cpoint_data.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"
#include "Cpoint_data.h"
#include "afxdialogex.h"


// Cpoint_data 对话框

IMPLEMENT_DYNAMIC(Cpoint_data, CDialogEx)

Cpoint_data::Cpoint_data(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Cpoint_data::~Cpoint_data()
{
	
}

void Cpoint_data::set_edit()
{
	CString strx, stry;
	strx.Format(_T("%d"), this->position_X);
	stry.Format(_T("%d"), this->position_Y);
	SetDlgItemTextW(IDC_EDIT1, strx);
	SetDlgItemTextW(IDC_EDIT2, stry);
}

void Cpoint_data::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_EDIT2, pos_y);
}


BEGIN_MESSAGE_MAP(Cpoint_data, CDialogEx)
	ON_BN_CLICKED(IDOK, &Cpoint_data::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cpoint_data::OnBnClickedButton1)
END_MESSAGE_MAP()


// Cpoint_data 消息处理程序


void Cpoint_data::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str2,str_y,str3;
	int x,y;
	CRect wndRect2;
	GetParent()->GetClientRect(wndRect2);
	GetDlgItemTextW(IDC_EDIT1, str);
	GetDlgItemTextW(IDC_EDIT2, str_y);
	//MessageBox(str, NULL, MB_OK);

	x = this->position_X = _ttoi(str);
	y = this->position_Y = _ttoi(str_y);
	
	
	
	str2.Format(_T("x=%d,y=%d"), x, y);
	str3.Format(_T("该点超出绘制范围！x最大为:%d, y最大为:%d"), wndRect2.Width(), wndRect2.Height());

	if (x <= 0 || y <= 0 || x >= wndRect2.Width() || y >= wndRect2.Height()) {
		MessageBox(str3, NULL, MB_OK);
	}
	CDC* pDC = GetParent()->GetDC();
	pDC->TextOut(this->position_X, this->position_Y, str2);
	this->ReleaseDC(pDC);
	//CDialogEx::OnOK();
	
}


void Cpoint_data::OnBnClickedButton1()
{
	MessageBox(_T("ddd"), NULL, MB_OK);
	// TODO: 在此添加控件通知处理程序代码
}
