//---------------------------------------------------------------------------


//#pragma hdrstop
#pragma once

//#include <vcl.h>
#include "stdafx.h"

#include "dmatrix.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)

//---------------------------------------------------------------------------

DMatrix temp, E;    // ������ ���������� ����������
//DMatrix *pTemp;

//--------------------------------------------------------------------------- 

DMatrix::DMatrix()
{
  data = NULL;           // �� ���������� � ������� ����������
}

//---------------------------------------------------------------------------

DMatrix DMatrix::operator+(DMatrix op2)
{
  int i, j;                                     // op2 - ������ �������; ������ ������� ����� - this,
                                                // n, k, m - ����� ����� ������� (� �������� ����������)

  if(n!=op2.n || m!=op2.m)
  {
	AfxMessageBox("��������� ������������ ������ �� ���������!");
    oblom = true;
    return *this;     // ���������� ������ �������, �� ���� ������ �� ������
  }


  temp.data = NULL;
  temp.data = new double*[m];
                                               
  for (j = 0; j < m; j++)
  {
    temp.data[j] = new double[n];
  }
  temp.n = n; temp.m = m; temp.k = temp.m;


  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
    {
      temp.data[i][j] = data[i+k-m][j] + op2.data[i + op2.k-op2.m][j];
    }


  return temp;
}

//------------------------------------------------------------------------------

DMatrix DMatrix::operator*(DMatrix op2)
{
  int i, j, s;
  double x;


  if(n!=op2.m)
  {
	  AfxMessageBox("����� �������� ������ ���������\n\n �� ���������\n\n � ������ ����� ������� ���������!");
    oblom = true;
    return *this;     // ���������� ������ �������, �� ���� ������ �� ������
  }

  
  temp.data = NULL;
  temp.data = new double*[m];
                                               
  for (j = 0; j < m; j++)
  {
    temp.data[j] = new double[op2.n];    // �������� - ������� ��, ������� � ������� ��������
  }
  temp.m = m;                            // ����� - ������� ��, ������� � ������� ��������
  temp.n = op2.n;                        // �������� - ������� ��, ������� � ������� ��������
  temp.k = temp.m;


  for (i = 0; i < m; i++)                       // ���� �� ������� ������ �������
    for (j = 0; j < op2.n; j++)                 // ���� �� �������� ������ �������
    {
      x = 0;
      for (s = 0; s < n; s++)                   // �������� ������ �� �������
      {
        x = x + (data[i+k-m][s] * op2.data[s+op2.k-op2.m][j]);
      }
      temp.data[i][j] = x;
    }


  return temp;
}

//------------------------------------------------------------------------------

DMatrix DMatrix::operator*(double op2)
{
  int i, j;


  temp.data = NULL;
  temp.data = new double*[m];            // �������� ������ ��� temp.data
                                                
  for (j = 0; j < m; j++)
  {
    temp.data[j] = new double[n];
  }
  temp.n = n; temp.m = m; temp.k = temp.m;


  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
    {
      temp.data[i][j] = data[i+k-m][j] * op2;
    }


  return temp;
}

//------------------------------------------------------------------------------

DMatrix DMatrix::operator=(DMatrix op2)
{
	int i, j;


	if (oblom || n != op2.n || m != op2.m)
	{
		if (n != op2.n || m != op2.m)  AfxMessageBox("��������� ������ �� ��������� (��� ����������)!");
		oblom = true;
		if (temp.data) { temp.de_allocate(); temp.data = NULL; }
		return *this;     // ���������� ������ �������, �� ���� ������ �� ������
	}


	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
		{
			data[i + k - m][j] = op2.data[i + op2.k - op2.m][j];  // ���������� ������ ������ ���������� �� "������" ����� ������ ������� � "�����" ���� �����
		}


	if (temp.data) { temp.de_allocate(); temp.data = NULL; }
	return *this;
}

//------------------------------------------------------------------------------  

DMatrix /*__fastcall*/ DMatrix::T()
{
  int i, j;
  
  /*pTemp = new DMatrix();
  pTemp->data = NULL;
  pTemp->data = new double*[n];

  for (j = 0; j < n; j++)
  {
	  pTemp->data[j] = new double[m];
  }
  pTemp->m = n;  pTemp->n = m;  pTemp->k = temp.m;


  for (i = 0; i<pTemp->m; i++)
	  for (j = 0; j<pTemp->n; j++)
	  {
		  pTemp->data[i][j] = data[j + k - m][i];
	  }


  return pTemp;*/


  temp.data = NULL;
  temp.data = new double*[n];
                                                
  for (j = 0; j < n; j++)
  {
    temp.data[j] = new double[m];
  }     
  temp.m = n;  temp.n = m;  temp.k = temp.m;
  
                                                                                 
  for(i=0; i<temp.m; i++)
    for(j=0; j<temp.n; j++)
    {
      temp.data[i][j] = data[j+k-m][i];
    }

    
  return temp;
}    

//------------------------------------------------------------------------------  

double /*__fastcall*/ DMatrix::det()
{
  return det(false);
}

//------------------------------------------------------------------------------  

double /*__fastcall*/ DMatrix::det(bool inversion)       // ����� ������
{
  int           i1, i2, i3;
  bool          nl;
  double   fraction, res;


  // m = n, ������� ����� m �� ����������!!!
  if(n != m)  { AfxMessageBox("������� - �� ����������!");  oblom = true;  return 0; }


  temp.data = NULL;
  temp.data = new double*[n];
  for (i1 = 0; i1 < n; i1++)
    temp.data[i1] = new double[n];
  temp.n = n; temp.m = m; temp.k = temp.m;  
  for (i1 = 0; i1 < n; i1++)
    for (i2 = 0; i2 < n; i2++)
      temp.data[i1][i2] = data[i1 + k - n][i2];

  if(inversion)
  {
    E.data = NULL;
    E.data = new double*[n];
    for (i1 = 0; i1 < n; i1++)
      E.data[i1] = new double[n];
    E.n = n; E.m = m; E.k = E.m;
    for (i1 = 0; i1 < n; i1++)
      for (i2 = 0; i2 < n; i2++)
        {
          if(i1 == i2)  E.data[i1][i2] = 1;
          else          E.data[i1][i2] = 0;
        }
  }


  res = 1;
  for(i1 = 0; i1 < n - 1; i1++)       // ���� �� ��������� (�� �� �����)
  {
    if(temp.data[i1][i1] == 0)        // ��������� ������ ���� �� ���������
    {
      nl = true;
      for(i2 = i1 + 1; i2 < n; i2++)  // ������� ���� ������ � ��������� ��������� � ���� �������...
        if(temp.data[i2][i1] != 0)
        {
          for(i3 = 0; i3 < n; i3++)   // ...� ���������� ��� ������
          {
            temp.data[i1][i3] = temp.data[i1][i3] + temp.data[i2][i3];
            if(inversion)  E.data[i1][i3] = E.data[i1][i3] + E.data[i2][i3];
          }
          nl = false;
          break;
        }
      if(nl)
      {
        if(!inversion && temp.data)
        { temp.de_allocate();  temp.data = NULL; }  // ���� ������� ������, �.�. det() ���������� ����� � ���������� ��������� "=" �� ����������, � ������ �������� temp ���������� ��� ����������
        return 0;
      }
    }

    res = res * temp.data[i1][i1];

    // ��������� � ������ ������� ��� ����������
    for(i2 = i1 + 1; i2 < n; i2++)    // ���� �� �������
    {
      fraction = temp.data[i2][i1] / temp.data[i1][i1];
      for(i3 = 0; i3 < n; i3++)       // ���� �� ��������
      {
        temp.data[i2][i3] = temp.data[i2][i3] - (fraction * temp.data[i1][i3]);
        if(inversion)  E.data[i2][i3] = E.data[i2][i3] - (fraction * E.data[i1][i3]);
      }
    }
  }

  res = res * temp.data[n - 1][n - 1];     // ��������� �� ��������� ������� ���������


  if(!inversion && temp.data)
  { temp.de_allocate();  temp.data = NULL; }  // ���� ������� ������, �.�. det() ���������� ����� � ���������� ��������� "=" �� ����������, � ������ �������� temp ���������� ��� ����������
  return res;
}  

//------------------------------------------------------------------------------  

DMatrix /*__fastcall*/ DMatrix::Inverse()    // ����� ������-������� (��. cleverstudents.ru/matrix/finding_the_inverse_matrix.html)
{                                          // !!! ������ ����, ��������� ��� ����������, �������� � det() !!!
  int           i1, i2, i3;
  double   mult, dt;


  dt = det(true);
  if(oblom) return *this;     // ���������� �������� �������, �� ���� ������ �� ������
  if(dt == 0)
  {
	  AfxMessageBox("������� - �����������!");
    oblom = true;
    if(E.data)     { E.de_allocate();  E.data = NULL; }
    return *this;     // ���������� �������� �������, �� ���� ������ �� ������
  }

  // ������ �� ��������� �������
  for(i1 = 0; i1 < n; i1++)        
  {
    mult = temp.data[i1][i1];
    for(i2 = 0; i2 < n; i2++)
    {
      temp.data[i1][i2] = temp.data[i1][i2] / mult;
      E.data[i1][i2] = E.data[i1][i2] / mult;
    }
  }

  // �������� ��� ���� ���������
  for(i1 = 1; i1 < n; i1++)            // ���� �� ���������
  {
    for(i2 = i1 - 1; i2 >= 0; i2--)    // ���� �� �������
    {
      mult = temp.data[i2][i1];
      for(i3 = 0; i3 < n; i3++)        // ���� �� ��������
      {
        temp.data[i2][i3] = temp.data[i2][i3] - (temp.data[i1][i3] * mult);
        E.data[i2][i3] = E.data[i2][i3] - (E.data[i1][i3] * mult);
      }
    }
  }


  // ���������� ������ � temp
  for(i1 = 0; i1 < n; i1++)
    for(i2 = 0; i2 < n; i2++)
      temp.data[i1][i2] = E.data[i1][i2];

  if(E.data)     { E.de_allocate();  E.data = NULL; }
  return temp;
}   

//------------------------------------------------------------------------------

void /*__fastcall*/  DMatrix::Ini(int m0, int n0, int M0, int k0)
{
int   i;


  m = m0;
  n = n0;
  M = M0;
  k = k0;
  oblom = false;

  data = new double*[M];

  for(i = 0; i < M; i++)    data[i] = NULL;

  for(i = 0; i < k; i++)    data[i] = new double[n];
}

//------------------------------------------------------------------------------

void /*__fastcall*/  DMatrix::de_allocate()
{
  for (int i = k-m; i < k && k >= m; i++)
    if(data[i])  { delete[] data[i];  data[i] = NULL; }                

  delete[] data;
  data = NULL;
}

//------------------------------------------------------------------------------

void /*__fastcall*/  DMatrix::Allocate(int k0)
{
  if(k0 < M)  data[k0] = new double[n];    // �������� ������ ��� ������
  else  oblom = true;
}

//------------------------------------------------------------------------------

void /*__fastcall*/  DMatrix::Delete(int k0)
{
  if(k0 < M && data[k0])  { delete[] data[k0];  data[k0] = NULL; }   // ������� ������
  else  oblom = true;
}
