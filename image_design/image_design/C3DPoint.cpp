#include "pch.h"
#include "C3DPoint.h"
extern int view_hight;
extern int drag_x, drag_y;
extern C3DPoint zero_point_3d;
void C3DPoint::To2dPoint(bool type)
{

	x = _x + zero_point_3d._x;
	y = _y + zero_point_3d._y;

	this->y = view_hight - y;
	if (type) {
		x += drag_x;
		y += drag_y;
	}
}

C3DPoint::C3DPoint(CPoint p, int p2)
{

	this->_x = p.x;
	this->_y = view_hight - p.y;
	this->_z = p2;
	_x = _x - zero_point_3d._x;
	_y = _y - zero_point_3d._y;
}

C3DPoint::C3DPoint(int x, int y, int z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

C3DPoint::C3DPoint()
{
	CPoint::CPoint();
}

void C3DPoint::VectorXVector(C3DPoint& p0, C3DPoint& p1, C3DPoint& p2, C3DPoint& pre)
{
	pre._z = (p1._x - p0._x) * (p2._y - p1._y) - (p2._x - p1._x) * (p1._y - p0._y);
	pre._x = (p1._y - p0._y) * (p2._z - p1._z) - (p2._y - p1._y) * (p1._z - p0._z);
	pre._y = (p1._z - p0._z) * (p2._x - p1._x) - (p2._z - p1._z) * (p1._x - p0._x);
}

double C3DPoint::VectorDotVecor(C3DPoint& p0, C3DPoint& p1,C3DPoint& p2)
{
	int _1x = p1._x - p0._x, _1y = p1._y - p0._y, _1z = p1._z - p0._z;
	int _2x = p2._x - p1._x, _2y = p2._y - p1._y, _2z = p2._z - p1._z;
	return (_1x * _2x + _1y * _2y + _1z * _2z) / sqrt((_1x * _1x + _1y * _1y + _1z * _1z) + (_2x * _2x + _2y * _2y + _2z * _2z));
}



