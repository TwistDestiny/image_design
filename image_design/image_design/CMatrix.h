#pragma once
#include <iostream>
class CMatrix
{
private: //出于程序安全，一般要将属性设为私有
	int m_Col;    // 矩阵的列数
	int m_Row;   // 矩阵的行数
	double** m_pMatrix;  // 通过二重指针来实现动态矩阵
public:
	CMatrix(){}; 
	CMatrix(int colNum)
	{
		m_Col = colNum;
		m_Row = colNum;
		if (colNum < 0)
			cout << "错误！方阵行列数不能为负数！" << endl;
		else
		{
			alloc(m_Col, m_Row);
			for (int i = 0; i < colNum; i++)
				for (int j = 0; j < colNum; j++)
					m_pMatrix[i][j] = 0;
			cout << "调用方阵构造函数，生成对象地址为：" << this << endl;
		}
	};
	CMatrix(int colNum, int rowNum)
	{
		m_Col = colNum;
		m_Row = rowNum;
		if (colNum < 0)
			cout << "错误！矩阵列数不能为负数！" << endl;
		else if (rowNum < 0)
			cout << "错误！矩阵行数不能为负数！" << endl;
		else
		{
			alloc(m_Col, m_Row);
			for (int i = 0; i < rowNum; i++)
				for (int j = 0; j < colNum; j++)
					m_pMatrix[i][j] = 0;
			cout << "调用一般矩阵构造函数，生成对象的地址为：" << this << endl;
		}
	};
	CMatrix(const CMatrix& copy_from_me)
	{
		m_Col = copy_from_me.m_Col;
		m_Row = copy_from_me.m_Row;
		alloc(m_Col, m_Row); //将右边矩阵的行数和列数赋给左边的矩阵，便于进行之后的操作
		for (int i = 0; i < m_Row; i++)
		{
			for (int j = 0; j < m_Col; j++)
			{
				this->m_pMatrix[i][j] = copy_from_me.m_pMatrix[i][j];
			}
		}
		cout << "调用复制构造函数，生成对象的地址为：" << this << endl;
	}; // 复制构造函数
	CMatrix(CMatrix&& copy_from_me)
	{
		cout << "调用移动复制构造函数" << endl;
		this->m_Col = copy_from_me.m_Col;
		this->m_Row = copy_from_me.m_Row;
		this->m_pMatrix = copy_from_me.m_pMatrix;
		copy_from_me.m_pMatrix = nullptr; //右边矩阵移动复制结束后，将其指针设为空指针
								  //以防止修改右边矩阵时破坏了左边矩阵
	}; //移动复制构造函数
	CMatrix operator + (const CMatrix& rightM)
	{
		cout << "调用重载加法运算符" << endl;
		CMatrix a; //定义一个临时矩阵a，以存储运算结果后返回
		if (this->m_Col == rightM.m_Col && this->m_Row == rightM.m_Row)
		{
			a.alloc(m_Col, m_Row);
		/*	a.setCol(m_Col);
			a.setRow(m_Row);*/
			a.m_Col = m_Col;
			a.m_Row = m_Row;
			for (int i = 0; i < this->m_Row; i++)
			{
				for (int j = 0; j < this->m_Col; j++)
				{
					a.m_pMatrix[i][j] = this->m_pMatrix[i][j] + rightM.m_pMatrix[i][j];
				}
			}
			return a;
		}
		else
		{
			cout << "两矩阵无法相加！" << endl;
			return 0;
		}
	};  // 重载加法运算符加实现矩阵整体相加
	CMatrix operator - (const CMatrix& rightM)
	{
		cout << "调用重载减法运算符" << endl;
		CMatrix a;//定义一个临时矩阵a，以存储运算结果后返回
		if (this->m_Col == rightM.m_Col && this->m_Row == rightM.m_Row)
		{
			a.alloc(m_Col, m_Row);
			a.m_Col = m_Col;
			a.m_Row = m_Row;
			//a.setCol(m_Col);
			//a.setRow(m_Row);
			for (int i = 0; i < this->m_Row; i++)
			{
				for (int j = 0; j < this->m_Col; j++)
				{
					a.m_pMatrix[i][j] = this->m_pMatrix[i][j] - rightM.m_pMatrix[i][j];
				}
			}
			return a;
		}
		else
		{
			cout << "两矩阵无法相减！" << endl;
			return 0;
		}
	};  // 重载减法运算符加实现矩阵整体相减
	CMatrix operator * (double k)
	{
		cout << "调用重载乘法运算符" << endl;
		for (int i = 0; i < this->m_Row; i++)
		{
			for (int j = 0; j < this->m_Col; j++)
			{
				this->m_pMatrix[i][j] = this->m_pMatrix[i][j] * k;
			}
		}
		return *this;
	};       // 重载乘法运算符加实现矩阵数乘
	CMatrix operator * (const CMatrix& rightM)
	{
		cout << "调用重载乘法运算符" << endl;
		CMatrix a{ rightM.m_Col,this->m_Row }; 
		if (this->m_Col == rightM.m_Row) 
		{
			for (int i = 0; i < a.m_Row; i++)
			{
				for (int j = 0; j < a.m_Col; j++)
				{
					a.m_pMatrix[i][j] = 0;
				}
			}
			for (int i = 0; i < this->m_Row; i++)
			{
				for (int j = 0; j < rightM.m_Col; j++)
				{
					for (int k = 0; k < rightM.m_Row; k++)
					{
						a.m_pMatrix[i][j] += this->m_pMatrix[i][k] * rightM.m_pMatrix[k][j];
					}
				}
			} //三重循环实现矩阵相乘
			return a;
		}
		else
		{
			cout << "两矩阵无法相乘！" << endl;
			return 0;
		}
	};  // 重载乘法运算符加实现两矩阵相乘
	CMatrix operator = (const CMatrix& rightM)
	{
		cout << "调用重载赋值运算符" << endl;
		if (this != &rightM) //判断等号两边矩阵是否相等，以防止自己给自己赋值，造成内存混乱，下同
		{
			delete[] m_pMatrix;
			m_Col = rightM.m_Col;
			m_Row = rightM.m_Row;
			alloc(m_Col, m_Row);  //将右边矩阵的行数和列数赋给左边的矩阵，便于进行之后的操作
			for (int i = 0; i < m_Row; i++)
			{
				for (int j = 0; j < m_Col; j++)
				{
					this->m_pMatrix[i][j] = rightM.m_pMatrix[i][j];
				}
			}
		}
		return *this;
	};  // 重载赋值运算符
	CMatrix operator = (CMatrix&& rightM)
	{
		cout << "调用重载移动赋值运算符" << endl;
		if (this != &rightM)
		{
			this->m_Col = rightM.m_Col;
			this->m_Row = rightM.m_Row;
			this->m_pMatrix = rightM.m_pMatrix;
			rightM.m_pMatrix = nullptr; //右边矩阵移动赋值结束后，将其指针设为空指针
				   //以防止修改右边矩阵时破坏了左边矩阵
		}
		return *this;
	};  // 重载移动赋值运算符
	friend std::ostream& operator << (std::ostream& os, const CMatrix& rightM)
	{
		cout << "调用重载插入运算符" << endl;
		cout << "输出的矩阵为：" << endl;
		for (int i = 0; i < rightM.m_Row; i++)
		{
			for (int j = 0; j < rightM.m_Col; j++)
			{
				if (j == 0)
					cout << " |  "; //为了美观，增加竖线分隔
				cout << rightM.m_pMatrix[i][j] << "  ";
			}
			cout << "|";
			cout << endl;
		}
		return os;
	}; // 重载插入运算符实现矩阵直接输出
	double getElement(int m, int n)
	{
		cout << "调用特殊位置返值函数" << endl;
		cout << "矩阵（" << m << '，' << n << "）位置的元素值为：";
		return m_pMatrix[m][n];
	};
	bool setElement(int m, int n, double val)
	{
		cout << "调用特殊位置赋值函数" << endl;
		if (m_pMatrix[m][n] = val)
			return true;
		else
		{
			cout << "设置失败" << endl;
			return false;
		}
	};  //将矩阵（m, n）位置元素的值设为val，成功返回true, 失败返回错误提示
	inline void alloc(int colNum, int rowNum)
	{
		cout << "调用动态内存分配函数" << endl;
		m_pMatrix = (double**)malloc(sizeof(double*) * rowNum);
		for (int i = 0; i < rowNum; i++)
		{
			m_pMatrix[i] = (double*)malloc(sizeof(double) * colNum);
		}
	};//实现动态内存分配，将此部分独立出来，防止代码重复出现过于累赘
	void showIt()
	{
		cout << "调用打印函数；" << endl;
		cout << "矩阵行数为：" << m_Row << endl;
		cout << "矩阵列数为：" << m_Col << endl;
		cout << "矩阵各元素为：" << endl;
		for (int i = 0; i < m_Row; i++)
		{
			for (int j = 0; j < m_Col; j++)
			{
				if (j == 0)
					cout << " |  ";
				cout << m_pMatrix[i][j] << "  ";
			}
			cout << "|";
			cout << endl;
		}
	};  // 打印出矩阵的行数、列数及各元素的值


};

