// SpaceOperate.cpp: 实现文件
//

#include "pch.h"
#include "image_design.h"

#include "SpaceOperate.h"
#include "afxdialogex.h"
#include "C3DPoint.h"
#include "CMatrix.h"
#include "CImgVisiable.h"
#include "PhongLight.h"

// SpaceOperate 对话框

IMPLEMENT_DYNAMIC(SpaceOperate, CDialogEx)
int rotate_3d_count = 0;
int expand_3d_count = 0;
int rotate_type = 0;
bool Move3d = false;
extern CArray<C3DPoint> cli_pt_3d,cli_pt_out;
extern int d3_type;
extern int light_r, light_g, light_b;
CMatrix Moperate(4), Moperate2(4, 1), Moperate3(4, 1), Moperate4(4, 1);
extern CArray<CImgVisiable*> flat_collection,flat_show;
extern bool is_hide_line;
extern bool is_3D_fill;
extern pen_state* mypen;
extern C3DPoint zero_point_3d;
SpaceOperate::SpaceOperate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_combo(_T("Z轴旋转"))
	, isHide(FALSE)
	, isLight(FALSE)
{

}

SpaceOperate::~SpaceOperate()
{
}

void SpaceOperate::set_edit()
{
	CString strx,stry,str1,str2,strr,strg,strb;
	strx.Format(_T("%d"), expand_3d_count * 3);
	SetDlgItemTextW(IDC_EDIT3, strx);
	stry.Format(_T("%d"), rotate_3d_count * 3);
	SetDlgItemTextW(IDC_EDIT4, stry);
	str1.Format(_T("%d"), zero_point_3d._x);
	str2.Format(_T("%d"), zero_point_3d._y);
	SetDlgItemTextW(IDC_EDIT1, str1);
	SetDlgItemTextW(IDC_EDIT2, str2);
	PhongLight p;
	light_r = p.light_r;
	light_g = p.light_g;
	light_b = p.light_b;
	strr.Format(_T("%d"), light_r);
	strg.Format(_T("%d"), light_g);
	strb.Format(_T("%d"), light_b);
	SetDlgItemTextW(IDC_EDIT6, strr);
	SetDlgItemTextW(IDC_EDIT7, strg);
	SetDlgItemTextW(IDC_EDIT8, strb);
	

}

void SpaceOperate::expand(int count)
{
	C3DPoint temp;
	for (int z = 0; z < 4; z++) {
		for (int r = 0; r < 4; r++) {
			Moperate.setElement(z, r, 0);
		}
	}
	for (int j = 0; j < 4; j++) {
		Moperate.setElement(j, j, 1);
	}
	//Moperate.setElement(3, 0, count/abs(count));
	//Moperate.setElement(3, 1, count / abs(count));
	//Moperate.setElement(3, 2, count / abs(count));
	//Moperate.showIt();
	CArray<C3DPoint>* te_3d;
	te_3d = &(flat_show.GetAt(1)->img_pt);
	for(int i = 0; i < te_3d->GetSize(); i++) {
		Moperate2.setElement(0, 0, te_3d->GetAt(i)._x);
		Moperate2.setElement(0, 1, te_3d->GetAt(i)._y);
		Moperate2.setElement(0, 2, te_3d->GetAt(i)._z + count);
		Moperate2.setElement(0, 3, 1);
		//Moperate2.showIt();
		Moperate3 = Moperate2 * Moperate;
		//Moperate3.showIt();
		temp._x = Moperate3.getElement(0, 0);
		temp._y = Moperate3.getElement(0, 1);
		temp._z = Moperate3.getElement(0, 2);
		te_3d->SetAt(i, temp);

	}

	for (int i = 2; i < flat_show.GetSize(); i++) {
		te_3d = &(flat_show.GetAt(i)->img_pt);
		te_3d->GetAt(0)._z = flat_show.GetAt(1)->img_pt.GetAt(0)._z;
		te_3d->GetAt(1)._z = flat_show.GetAt(1)->img_pt.GetAt(0)._z;
	}


	
}

void SpaceOperate::rotate(int count)
{
	float pi = 3.1415726;
	cout << "rotate_type:" << rotate_type << endl;
	int xmin = cli_pt_3d.GetAt(0)._x;
	int ymin = cli_pt_3d.GetAt(0)._y;

	C3DPoint temp;
	float theta = pi / 180.0 * count;
	CArray<C3DPoint>* te_3d,*show_3d;
	for (int z = 0; z < 4; z++) {
		for (int r = 0; r < 4; r++) {
			Moperate.setElement(z, r, 0);
		}
	}
	if (rotate_type == 1) {
		Moperate.setElement(0, 0, 1);
		Moperate.setElement(1, 1, cos(theta));
		Moperate.setElement(1, 2, sin(theta));
		Moperate.setElement(2, 1, -1 * sin(theta));
		Moperate.setElement(2, 2, cos(theta));
		Moperate.setElement(3, 3, 1);
	}
	else if(rotate_type == 2) {
		Moperate.setElement(0, 0, cos(theta));
		Moperate.setElement(1,1,1);
		Moperate.setElement(2, 2, cos(theta));
		Moperate.setElement(3, 3, 1);
		Moperate.setElement(0, 2, -1 * sin(theta));
		Moperate.setElement(2, 0, sin(theta));
	}
	else if (rotate_type == 3) {
		Moperate.setElement(0, 0, 1 + count / 100.0);
		Moperate.setElement(1, 1, 1 + count / 100.0);
		Moperate.setElement(2, 2, 1 + count / 100.0);
		Moperate.setElement(3, 3, 1 + count / 100.0);

	}

	else {
		Moperate.setElement(0, 0, cos(theta));
		Moperate.setElement(1, 1, cos(theta));
		Moperate.setElement(2, 2, 1);
		Moperate.setElement(3, 3, 1);
		Moperate.setElement(0, 1, sin(theta));
		Moperate.setElement(1, 0, -1 * sin(theta));
	}


	for (int j = 0; j < flat_collection.GetSize(); j++) {
		te_3d = &(flat_collection.GetAt(j)->img_pt);
		show_3d = &(flat_show.GetAt(j)->img_pt);
		for (int i = 0; i < te_3d->GetSize(); i++) {
			Moperate2.setElement(0, 0, te_3d->GetAt(i)._x);
			Moperate2.setElement(0, 1, te_3d->GetAt(i)._y);
			Moperate2.setElement(0, 2, te_3d->GetAt(i)._z);
			Moperate2.setElement(0, 3, 1);



			Moperate4 = Moperate2 * Moperate;
			temp._x = Moperate4.getElement(0, 0);
			temp._y = Moperate4.getElement(0, 1);
			temp._z = Moperate4.getElement(0, 2);
			show_3d->SetAt(i, temp);
	}
	

		//Moperate4.showIt();

	}
}

void SpaceOperate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_COMBO1, m_combo_tool);
	DDX_Control(pDX, IDC_SPIN2, rotate_ctrl);
	DDX_Check(pDX, IDC_CHECK1, isHide);
	DDX_Check(pDX, IDC_CHECK2, isLight);
}


BEGIN_MESSAGE_MAP(SpaceOperate, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &SpaceOperate::OnDeltaposSpin_expand)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &SpaceOperate::OnDeltaposSpin_rotate)
	ON_BN_CLICKED(IDOK, &SpaceOperate::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &SpaceOperate::OnBnClickedButtonAnimationStart)
	ON_BN_CLICKED(IDC_BUTTON2, &SpaceOperate::OnBnClickedButtonAnimationEnd)
	ON_BN_CLICKED(IDC_CHECK1, &SpaceOperate::OnBnClickedCheckHide)
	ON_BN_CLICKED(IDC_BUTTON4, &SpaceOperate::OnBnClickedButtonMirror)
	ON_BN_CLICKED(IDC_BUTTON3, &SpaceOperate::OnBnClickedButton3DMove)
	ON_BN_CLICKED(IDC_CHECK2, &SpaceOperate::OnBnClickedCheckLight)
END_MESSAGE_MAP()


// SpaceOperate 消息处理程序


void SpaceOperate::OnDeltaposSpin_expand(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头

	{
		expand_3d_count--;

	}

	else if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头

	{
		expand_3d_count++;


	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strz;
	strz.Format(_T("%d"), expand_3d_count * 3);
	SetDlgItemTextW(IDC_EDIT3, strz);
	//if (expand_3d_count > 0)
	expand(pNMUpDown->iDelta * -3);
	GetParent()->Invalidate();
	OnBnClickedOk();
	
}


void SpaceOperate::OnDeltaposSpin_rotate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头

	{
		rotate_3d_count--;

	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头

	{
		rotate_3d_count++;


	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strz;
	strz.Format(_T("%d"), rotate_3d_count*3);
	SetDlgItemTextW(IDC_EDIT4, strz);
	rotate(rotate_3d_count * 3);
	for (int i = 0; i < flat_show.GetSize(); i++) {
		flat_show.GetAt(i)->getline();
	}
	GetParent()->Invalidate();
}


void SpaceOperate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CDialogEx::OnOK();*/
	UpdateData(TRUE);
	rotate_3d_count = 0;
	if (m_combo == "Z轴旋转")
		rotate_type = 0;
	else if (m_combo == "X轴旋转") 
		rotate_type = 1;
	else if (m_combo == "图形伸缩")
		rotate_type = 3;
	else 
		rotate_type = 2;
	for (int i = 0; i < flat_collection.GetSize(); i++) {
		flat_collection.GetAt(i)->img_pt.Copy(flat_show.GetAt(i)->img_pt);
	}
	set_edit();

	//MessageBox(strz, NULL, MB_OK);
}


void SpaceOperate::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	d3_type = 0;
	CDialogEx::OnClose();
}


void SpaceOperate::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	rotate_3d_count++;
	rotate_3d_count = rotate_3d_count % 120;
	CDialogEx::OnTimer(nIDEvent);
	CString strz;
	strz.Format(_T("%d"), rotate_3d_count * 3);
	SetDlgItemTextW(IDC_EDIT4, strz);
	rotate(rotate_3d_count * 3);
	for (int i = 0; i < flat_show.GetSize(); i++) {
		flat_show.GetAt(i)->getline();
	}
	GetParent()->Invalidate();


}


void SpaceOperate::OnBnClickedButtonAnimationStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(1, 100, NULL);
}


void SpaceOperate::OnBnClickedButtonAnimationEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
}


void SpaceOperate::OnBnClickedCheckHide()
{

	UpdateData(TRUE);
	is_hide_line = false;
	if (isHide) {
		is_hide_line = true;
		
	}
	GetParent()->Invalidate();
	// TODO: 在此添加控件通知处理程序代码
}


void SpaceOperate::OnBnClickedCheckLight()
{
	UpdateData(TRUE);
	is_3D_fill = false;
	if (isLight) {
		is_3D_fill = true;
	}
		
	
	GetParent()->Invalidate();
}


void SpaceOperate::OnBnClickedButton3DMove()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("ok"), NULL, MB_OK);
	Move3d = true;

}


void SpaceOperate::OnBnClickedButtonMirror()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("ok"), NULL, MB_OK);
	*mypen = d_symmetry;
	GetParent()->Invalidate();
}
