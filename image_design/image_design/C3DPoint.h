#pragma once
#include <atltypes.h>
class C3DPoint : public CPoint
{
public:
	int _x = 0;
	int _y = 0;
	int _z = 0;
	int s = 0;
	void To2dPoint(bool type = true);
	C3DPoint(CPoint p, int p2 = 0);
	C3DPoint(int x, int y, int z);
	C3DPoint();
	void VectorXVector(C3DPoint& p0, C3DPoint& p1, C3DPoint& p2, C3DPoint& pre);
	//��������������������Ĳ��
	double VectorDotVecor(C3DPoint& p0, C3DPoint& p1, C3DPoint& p2);
	//��������������������ĵ�˲��ҵ�λ��
};

