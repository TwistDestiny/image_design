#pragma once
#include <iostream>
class CMatrix
{
private: //���ڳ���ȫ��һ��Ҫ��������Ϊ˽��
	int m_Col;    // ���������
	int m_Row;   // ���������
	double** m_pMatrix;  // ͨ������ָ����ʵ�ֶ�̬����
public:
	CMatrix(){}; 
	CMatrix(int colNum)
	{
		m_Col = colNum;
		m_Row = colNum;
		if (colNum < 0)
			cout << "���󣡷�������������Ϊ������" << endl;
		else
		{
			alloc(m_Col, m_Row);
			for (int i = 0; i < colNum; i++)
				for (int j = 0; j < colNum; j++)
					m_pMatrix[i][j] = 0;
			cout << "���÷����캯�������ɶ����ַΪ��" << this << endl;
		}
	};
	CMatrix(int colNum, int rowNum)
	{
		m_Col = colNum;
		m_Row = rowNum;
		if (colNum < 0)
			cout << "���󣡾�����������Ϊ������" << endl;
		else if (rowNum < 0)
			cout << "���󣡾�����������Ϊ������" << endl;
		else
		{
			alloc(m_Col, m_Row);
			for (int i = 0; i < rowNum; i++)
				for (int j = 0; j < colNum; j++)
					m_pMatrix[i][j] = 0;
			cout << "����һ������캯�������ɶ���ĵ�ַΪ��" << this << endl;
		}
	};
	CMatrix(const CMatrix& copy_from_me)
	{
		m_Col = copy_from_me.m_Col;
		m_Row = copy_from_me.m_Row;
		alloc(m_Col, m_Row); //���ұ߾��������������������ߵľ��󣬱��ڽ���֮��Ĳ���
		for (int i = 0; i < m_Row; i++)
		{
			for (int j = 0; j < m_Col; j++)
			{
				this->m_pMatrix[i][j] = copy_from_me.m_pMatrix[i][j];
			}
		}
		cout << "���ø��ƹ��캯�������ɶ���ĵ�ַΪ��" << this << endl;
	}; // ���ƹ��캯��
	CMatrix(CMatrix&& copy_from_me)
	{
		cout << "�����ƶ����ƹ��캯��" << endl;
		this->m_Col = copy_from_me.m_Col;
		this->m_Row = copy_from_me.m_Row;
		this->m_pMatrix = copy_from_me.m_pMatrix;
		copy_from_me.m_pMatrix = nullptr; //�ұ߾����ƶ����ƽ����󣬽���ָ����Ϊ��ָ��
								  //�Է�ֹ�޸��ұ߾���ʱ�ƻ�����߾���
	}; //�ƶ����ƹ��캯��
	CMatrix operator + (const CMatrix& rightM)
	{
		cout << "�������ؼӷ������" << endl;
		CMatrix a; //����һ����ʱ����a���Դ洢�������󷵻�
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
			cout << "�������޷���ӣ�" << endl;
			return 0;
		}
	};  // ���ؼӷ��������ʵ�־����������
	CMatrix operator - (const CMatrix& rightM)
	{
		cout << "�������ؼ��������" << endl;
		CMatrix a;//����һ����ʱ����a���Դ洢�������󷵻�
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
			cout << "�������޷������" << endl;
			return 0;
		}
	};  // ���ؼ����������ʵ�־����������
	CMatrix operator * (double k)
	{
		cout << "�������س˷������" << endl;
		for (int i = 0; i < this->m_Row; i++)
		{
			for (int j = 0; j < this->m_Col; j++)
			{
				this->m_pMatrix[i][j] = this->m_pMatrix[i][j] * k;
			}
		}
		return *this;
	};       // ���س˷��������ʵ�־�������
	CMatrix operator * (const CMatrix& rightM)
	{
		cout << "�������س˷������" << endl;
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
			} //����ѭ��ʵ�־������
			return a;
		}
		else
		{
			cout << "�������޷���ˣ�" << endl;
			return 0;
		}
	};  // ���س˷��������ʵ�����������
	CMatrix operator = (const CMatrix& rightM)
	{
		cout << "�������ظ�ֵ�����" << endl;
		if (this != &rightM) //�жϵȺ����߾����Ƿ���ȣ��Է�ֹ�Լ����Լ���ֵ������ڴ���ң���ͬ
		{
			delete[] m_pMatrix;
			m_Col = rightM.m_Col;
			m_Row = rightM.m_Row;
			alloc(m_Col, m_Row);  //���ұ߾��������������������ߵľ��󣬱��ڽ���֮��Ĳ���
			for (int i = 0; i < m_Row; i++)
			{
				for (int j = 0; j < m_Col; j++)
				{
					this->m_pMatrix[i][j] = rightM.m_pMatrix[i][j];
				}
			}
		}
		return *this;
	};  // ���ظ�ֵ�����
	CMatrix operator = (CMatrix&& rightM)
	{
		cout << "���������ƶ���ֵ�����" << endl;
		if (this != &rightM)
		{
			this->m_Col = rightM.m_Col;
			this->m_Row = rightM.m_Row;
			this->m_pMatrix = rightM.m_pMatrix;
			rightM.m_pMatrix = nullptr; //�ұ߾����ƶ���ֵ�����󣬽���ָ����Ϊ��ָ��
				   //�Է�ֹ�޸��ұ߾���ʱ�ƻ�����߾���
		}
		return *this;
	};  // �����ƶ���ֵ�����
	friend std::ostream& operator << (std::ostream& os, const CMatrix& rightM)
	{
		cout << "�������ز��������" << endl;
		cout << "����ľ���Ϊ��" << endl;
		for (int i = 0; i < rightM.m_Row; i++)
		{
			for (int j = 0; j < rightM.m_Col; j++)
			{
				if (j == 0)
					cout << " |  "; //Ϊ�����ۣ��������߷ָ�
				cout << rightM.m_pMatrix[i][j] << "  ";
			}
			cout << "|";
			cout << endl;
		}
		return os;
	}; // ���ز��������ʵ�־���ֱ�����
	double getElement(int m, int n)
	{
		cout << "��������λ�÷�ֵ����" << endl;
		cout << "����" << m << '��' << n << "��λ�õ�Ԫ��ֵΪ��";
		return m_pMatrix[m][n];
	};
	bool setElement(int m, int n, double val)
	{
		cout << "��������λ�ø�ֵ����" << endl;
		if (m_pMatrix[m][n] = val)
			return true;
		else
		{
			cout << "����ʧ��" << endl;
			return false;
		}
	};  //������m, n��λ��Ԫ�ص�ֵ��Ϊval���ɹ�����true, ʧ�ܷ��ش�����ʾ
	inline void alloc(int colNum, int rowNum)
	{
		cout << "���ö�̬�ڴ���亯��" << endl;
		m_pMatrix = (double**)malloc(sizeof(double*) * rowNum);
		for (int i = 0; i < rowNum; i++)
		{
			m_pMatrix[i] = (double*)malloc(sizeof(double) * colNum);
		}
	};//ʵ�ֶ�̬�ڴ���䣬���˲��ֶ�����������ֹ�����ظ����ֹ�����׸
	void showIt()
	{
		cout << "���ô�ӡ������" << endl;
		cout << "��������Ϊ��" << m_Row << endl;
		cout << "��������Ϊ��" << m_Col << endl;
		cout << "�����Ԫ��Ϊ��" << endl;
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
	};  // ��ӡ���������������������Ԫ�ص�ֵ


};

