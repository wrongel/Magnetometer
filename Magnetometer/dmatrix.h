//---------------------------------------------------------------------------

#ifndef mymatrixH
#define mymatrixH

//#include <StdCtrls.hpp>  
              


//---------------------------------------------------------------------------

// ���� ������� ������� �����������, ��� �� ����������� �����:  op2.k = op2.m; op2.M = op2.m      !!!!!

class DMatrix
{

public:

  int M;                 // ������������ ����� ������ (+ 1)
  int k;                 // ������� ����� ��������� ������ (+ 1)
  int m;                 // THE NUMBER OF ROWS
  int n;                 // THE NUMBER OF COLUMNS
  double **data;
  bool   oblom;          // ������� ���������� ���������� ���������� ���������

  //TListBox *ListBox11;
                                           
  void        /*__fastcall*/      Ini(int m0, int n0, int M0 = 0, int k0 = 0);
  void        /*__fastcall*/      de_allocate();
  void        /*__fastcall*/      Allocate(int k0);
  void        /*__fastcall*/      Delete(int k0);
  double /*__fastcall*/      det(bool inversion);  // inversion - ������� ����, ��� ������ ����� ������� �������� ������� ������� ������-�������
                                                    // ���������� ������ �� Inverse(), ����� ���� � Inverse() ���������� ������ ������� E
  double /*__fastcall*/      det();
  DMatrix     /*__fastcall*/      T();
  DMatrix     /*__fastcall*/      Inverse();                 

  DMatrix();            // �����������

  DMatrix operator=(DMatrix op2);
  DMatrix operator+(DMatrix op2);         // �������� ������
  DMatrix operator*(DMatrix op2);         // ��������� ������
  DMatrix operator*(double op2);     // ��������� ������� �� ����� (������!)

};

#endif
