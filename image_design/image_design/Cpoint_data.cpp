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

void Cpoint_data::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Cpoint_data, CDialogEx)
END_MESSAGE_MAP()


// Cpoint_data 消息处理程序
