// data.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"
#include "data.h"
#include "afxdialogex.h"


// data 对话框

IMPLEMENT_DYNAMIC(data, CDialogEx)

data::data(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

data::~data()
{
}

void data::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(data, CDialogEx)
END_MESSAGE_MAP()


// data 消息处理程序
