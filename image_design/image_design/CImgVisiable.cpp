#include "pch.h"
#include "CMatrix.h"
#include "C3DPoint.h"
#include "CImgVisiable.h"


extern CMatrix Moperate, Moperate2, Moperate3, Moperate4;
C3DPoint CImgVisiable::getline()
{
	C3DPoint p, temp;
	int xs, ys, zs, xa, ya, za, xb, yb, zb;
	int D, E, F;
	temp = img_pt.GetAt(0);
	xs = temp._x;
	ys = temp._y;
	zs = temp._z;
	temp = img_pt.GetAt(1);
	xa = temp._x;
	ya = temp._y;
	za = temp._z;
	temp = img_pt.GetAt(2);
	xb = temp._x;
	yb = temp._y;
	zb = temp._z;
	D = (ya - ys) * (zb - za) - (za - zs) * (yb - ya);
	E = (za - zs) * (xb - xa) - (xa - xs) * (zb - za);
	F = (xa - xs) * (yb - ya) - (ya - ys) * (xb - xa);
	p._x = D;
	p._y = E;
	p._z = F;
	flat_vector = p;
	return p;
}

bool CImgVisiable::is_visiable()
{
	//C3DPoint  visi_vect, line, temp;
	//int re;

	//line = flat_vector;
	//temp = img_pt.GetAt(0);
	//int totalx = 0, totaly = 0;
	//for (int i = 0; i < view_p->GetSize(); i++) {
	//	totalx += view_p->GetAt(i)._x;
	//	totaly += view_p->GetAt(i)._y;

	//}
	//view_point._x = totalx / view_p->GetSize();
	//view_point._y = totaly / view_p->GetSize();
	//view_point._z = 1000;
	//visi_vect._x = view_point._x - temp._x;
	//visi_vect._y = view_point._y - temp._y;
	//visi_vect._z = view_point._z - temp._z;
	//re = visi_vect._x * line._x + visi_vect._y * line._y + visi_vect._z * line._z;
	if (flat_vector._z > 0)
		return true;
	else
		return false;
}
// ((xi - xi-1),(yi - yi-1)) x ((xi+1 - xi),(yi+1 - yi)) = (xi - xi-1) * (yi+1 - yi) - (yi - yi-1) * (xi+1 - xi)
void CImgVisiable::setdir(bool is_left)
{
	C3DPoint p1, p2, p3;
	p1 = img_pt.GetAt(0);
	p2 = img_pt.GetAt(1);
	p3 = img_pt.GetAt(2);
	int rotate = (p2._x - p1._x) * (p3._y - p2._y) - (p2._y - p1._y) * (p3._x - p2._x);
	if (is_left && rotate<0) {
		for (int i = 1; i <= (img_pt.GetSize() - 1) / 2; i++) {
			swap(img_pt.GetAt(i), img_pt.GetAt(img_pt.GetSize() - i));
		}
	}
	if (!is_left && rotate > 0) {
		for (int i = 1; i <= (img_pt.GetSize() - 1) / 2; i++) {
			swap(img_pt.GetAt(i), img_pt.GetAt(img_pt.GetSize() - i));
		}
	}
}

void CImgVisiable::vector_rotate(int degree,int rotate_type)
{
	float pi = 3.1415726;

	C3DPoint temp;
	float theta = pi / 180.0 * degree;

	Moperate2.setElement(0, 0, flat_vector._x);
	Moperate2.setElement(0, 1, flat_vector._y);
	Moperate2.setElement(0, 2, flat_vector._z);
	Moperate2.setElement(0, 3, 1);
	Moperate4 = Moperate2 * Moperate;
	flat_vector._x = Moperate4.getElement(0, 0);
	flat_vector._y = Moperate4.getElement(0, 1);
	flat_vector._z = Moperate4.getElement(0, 2);
}


