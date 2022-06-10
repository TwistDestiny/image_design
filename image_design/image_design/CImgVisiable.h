#pragma once

class C3DPoint;
class CImgVisiable
{
public:
	CArray<C3DPoint> img_pt;
	C3DPoint view_point =  C3DPoint();
	C3DPoint flat_vector;
	C3DPoint getline();

	void setdir(bool is_left);
	void vector_rotate(int degree, int rotate_type);
	bool is_visiable();
	CImgVisiable() {};



};

