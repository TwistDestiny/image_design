#pragma once
class C3DPoint;
class PhongLight
{
public:
	int light_r = 255;
	int light_g = 215;
	int light_b = 0;
	int Envirment_li = 50;
	int m_reflect_li = 70;
	int v_reflect_li = 70;
	float v_highlight_parm = 0.5;
	int v_hightlight_exp = 0.5;
	int de_li = 50;
	C3DPoint vect_source;
	COLORREF color_li= RGB(255, 255, 0);
	PhongLight();
};

