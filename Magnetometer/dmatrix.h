//---------------------------------------------------------------------------

#ifndef mymatrixH
#define mymatrixH

//#include <StdCtrls.hpp>  
              


//---------------------------------------------------------------------------

// ЕСЛИ СЧИТАЕМ МАТРИЦУ СТАТИЧЕСКОЙ, ПРИ ЕЕ ОПРЕДЕЛЕНИИ ПИШЕМ:  op2.k = op2.m; op2.M = op2.m      !!!!!

class DMatrix
{

public:

  int M;                 // Максимальный номер строки (+ 1)
  int k;                 // Текущий номер последней строки (+ 1)
  int m;                 // THE NUMBER OF ROWS
  int n;                 // THE NUMBER OF COLUMNS
  double **data;
  bool   oblom;          // Признак аварийного результата выполнения оператора

  //TListBox *ListBox11;
                                           
  void        /*__fastcall*/      Ini(int m0, int n0, int M0 = 0, int k0 = 0);
  void        /*__fastcall*/      de_allocate();
  void        /*__fastcall*/      Allocate(int k0);
  void        /*__fastcall*/      Delete(int k0);
  double /*__fastcall*/      det(bool inversion);  // inversion - признак того, что дальше будем считать обратную матрицу методом Гаусса-Жордана
                                                    // Вызывается только из Inverse(), после чего в Inverse() происходит чистка матрицы E
  double /*__fastcall*/      det();
  DMatrix     /*__fastcall*/      T();
  DMatrix     /*__fastcall*/      Inverse();                 

  DMatrix();            // Конструктор

  DMatrix operator=(DMatrix op2);
  DMatrix operator+(DMatrix op2);         // Сложение матриц
  DMatrix operator*(DMatrix op2);         // Умножение матриц
  DMatrix operator*(double op2);     // Умножение матрицы на число (справа!)

};

#endif
