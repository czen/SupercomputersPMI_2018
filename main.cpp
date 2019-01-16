#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct m_block
{
	int* value;
};

struct m_block_r
{
	m_block* b;
	int length;
};

struct m_block_c
{
	m_block* b;
	int length;
};

struct m_block_r_made
{
	m_block_r* r;
	int length;
	int size;
};

struct m_block_c_made
{
	m_block_c* c;
	int length;
	int size;
};

void Multiplication_PS_b(int *A_b, int *B_b, int *C_b, int size)
{
#pragma omp parallel for schedule(static, size/4) clapse(2) shared(A_b, B_b, C_b, size) num_threads(4)
	for (int i=0; i<size; i++)
		{
		    for (int j=0; j<size; j++)
			{
			    for (int k=0; k<size; k++)
				{
				    C_b[i*size+j]+=A_b[i*size+k]*B_b[k*size+j];
				}
			}
		}
};

void Multiplication_PD_b(int *A_b, int *B_b, int *C_b, int size)
{
#pragma omp parallel for schedule(dynamic, size/4) clapse(2) shared(A_b, B_b, C_b, size) num_threads(4)
	for (int i=0; i<size; i++)
		{
		    for (int j=0; j<size; j++)
			{
			    for (int k=0; k<size; k++)
				{
				    C_b[i*size+j]+=A_b[i*size+k]*B_b[k*size+j];
				}
			}
		}
};

void Multiplication_PS(m_block_c_made& A, m_block_r_made& B, m_block_r_made& C)
{
#pragma omp parallel for schedule(static, C.length/4) clapse(2) shared(A, B, C) num_threads(4)
	for (int i=0; i<C.length; i++)
		{
		    for (int j=0; j<C.length; j++)
			{
			    for (int k=0; k<C.length; k++)
                {
                    if (j-C.r[k].length+B.r[k].length>=0)
                    {
                        int n=j-C.r[k].length+B.r[k].length;
                        Multiplication_PS_b(A.c[i].b[k].value, B.r[k].b[n].value, C.r[i].b[j].value, B.size);
                    }
                }
			}
		}
};

void Multiplication_PD(m_block_c_made& A, m_block_r_made& B, m_block_r_made& C)
{
#pragma omp parallel for schedule(dynamic, C.length/4) clapse(2) shared(A, B, C) num_threads(4)
	for (int i=0; i<C.length; i++)
		{
		    for (int j=0; j<C.length; j++)
			{
			    for (int k=0; k<C.length; k++)
                {
                    if (j-C.r[k].length+B.r[k].length>=0)
                    {
                        int n=j-C.r[k].length+B.r[k].length;
                        Multiplication_PD_b(A.c[i].b[k].value, B.r[k].b[n].value, C.r[i].b[j].value, B.size);
                    }
                }
			}
		}
};

void Multiplication_NP(m_block_c_made& A, m_block_r_made& B, m_block_r_made& C)
{
	for (int i=0; i<C.length;i++)
		{
		    for (int j=0; j<C.length; j++)
			{
			    for (int k=0; k<C.length; k++)
				{
				    if (j-C.r[k].length+B.r[k].length>=0)
                    {
                        int n=j-C.r[k].length+B.r[k].length;
                        for (int q=0; q<C.size; q++)
                            {
                                for (int w=0; w<C.size; w++)
                                {
                                    for (int e=0; e<C.size; e++)
                                    C.r[i].b[j].value[q*C.size+w]+=A.c[i].b[k].value[e*C.size+q]*B.r[k].b[n].value[e*C.size+w];
                                }
                            }
                    }
				}
			}
		}
};

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	int m_size=1024;
	int b_size=2;
	cout<<"Размер матрицы : "<<m_size<<"x"<<m_size<<endl<<endl;
	while (b_size<m_size)
    {
		cout<<"Размер блока : "<<b_size<<"x"<<b_size<<endl;
		int size=m_size/b_size;
		m_block_c_made A;
		m_block_r_made B;
		A.length=B.length=size;
		A.size=B.size=b_size;
		A.c=new m_block_c[size];
		B.r=new m_block_r[size];
		for (int i=0; i<size; i++)
		{
			A.c[i].b=new m_block[size];
			A.c[i].length=size;
			B.r[i].b=new m_block[size-i];
			B.r[i].length=size-i;
			for (int j=0; j<size; j++)
			{
				A.c[i].b[j].value=new int[b_size*b_size];
				for (int k=0; k < b_size*b_size; k++)
					{
					    A.c[i].b[j].value[k]=rand()%10;
					}
                if (j<size-i)
                {
                    B.r[i].b[j].value=new int[b_size*b_size];
                    for (int k=0; k<b_size*b_size; k++)
                        {
                            B.r[i].b[j].value[k]=rand()%10;
                        }
                    for (int q=0; q<b_size; q++)
                        {
                            for (int w=0; w<b_size; w++)
                            {
                                if (q>w)
                                {
                                    B.r[i].b[0].value[q*b_size+w]=0;
                                }
                            }
                        }
                }
			}
		}

		m_block_r_made C_1, C_2, C_3;
		C_1.length=C_2.length=C_3.length=size;
		C_1.size=C_2.size=C_3.size=b_size;
		C_1.r=new m_block_r[size];
		C_2.r=new m_block_r[size];
		C_3.r=new m_block_r[size];
		for (int i=0; i<size; i++)
		{
			C_1.r[i].b=new m_block[size];
			C_2.r[i].b=new m_block[size];
			C_3.r[i].b=new m_block[size];
			C_1.r[i].length=C_2.r[i].length=C_3.r[i].length=size;
			for (int j=0; j<size; j++)
			{
				C_1.r[i].b[j].value=new int[b_size*b_size];
				C_2.r[i].b[j].value=new int[b_size*b_size];
				C_3.r[i].b[j].value=new int[b_size*b_size];
				for (int k=0; k<b_size*b_size; ++k)
					{
					    C_1.r[i].b[j].value[k]=C_2.r[i].b[j].value[k]=C_3.r[i].b[j].value[k]=0;
					}
			}

		}

		double start_time, end_time;
		if (m_size!=4)
        {
            start_time=clock();
        }
		Multiplication_PS(A, B, C_1);
		if (m_size!=4)
        {
            end_time=clock();
            cout<<"Время работы параллельного статического алгоритма : "<<(end_time-start_time)/1000.<<endl;
            start_time=clock();
        }
		Multiplication_PD(A, B, C_2);
		if (m_size!=4)
		{
		    end_time=clock();
            cout<<"Время работы параллельного динамического алгоритма : "<<(end_time-start_time)/1000.<<endl;
            start_time=clock();
		}
		Multiplication_NP(A, B, C_3);
		if (m_size!=4)
		{
		    end_time=clock();
            cout<<"Время работы непараллельного алгоритма : "<<(end_time-start_time)/1000.<< endl;
		}

		if (m_size==4)
        {
            cout<<endl<<"Матрица, полученная при помощи параллельного статического алгоритма :"<<endl;
            for (int i=0; i<size; i++)
            {
                for (int j=0; j<size; j++)
                {
                    for (int k=0; k<b_size*b_size; ++k)
                        {
                            cout<<C_1.r[i].b[j].value[k]<<" ";
                        }
                    cout<<"|"<<endl;
                }
            }
            cout<<endl<<"Матрица, полученная при помощи параллельного динамического алгоритма :"<<endl;
            for (int i=0; i<size; i++)
            {
                for (int j=0; j<size; j++)
                {
                    for (int k=0; k<b_size*b_size; ++k)
                        {
                            cout<<C_2.r[i].b[j].value[k]<<" ";
                        }
                    cout<<"|"<<endl;
                }
            }
            cout<<endl<<"Матрица, полученная при помощи непараллельного алгоритма :"<<endl;
            for (int i=0; i<size; i++)
            {
                for (int j=0; j<size; j++)
                {
                    for (int k=0; k<b_size*b_size; ++k)
                        {
                            cout<<C_3.r[i].b[j].value[k]<<" ";
                        }
                    cout<<"|"<<endl;
                }
            }
        }

		cout<<endl;
		b_size*=2;
	}
	system("pause");
	return 0;
}
