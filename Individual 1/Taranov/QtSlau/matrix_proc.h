
#pragma once

#include <vector>

using namespace std;

#define TYPE long double // тип данных для вычислений 
#define ACCURACY 1e-5 // точность вычислений 
#define BUFF_READ_SIZE 64
// #define WITH_ZERO_DIVISION_CHECK // проверять деление на ноль 
// #define SIMPLE_NUMBERS // использователь целые числа 0..10 в матрицах

namespace matrix_processing {
 
  // индекс строки с максимальным эелментом в столбце i
  int findMaxInColumn(const vector<vector<TYPE>> &linear_system, int i_col, int i_from = 0);

  // аоменять местами 2 строки матрицы 
  void swapRows(vector<vector<TYPE>> &linear_system, int i_from, int i_to);

  // поделить все элементы строки на число 
  void divideRowBy(vector<vector<TYPE>> &linear_system, int i_row, TYPE x);

  // вычитание строк матрицы
  void subRows(vector<vector<TYPE>> &linear_system, int i_row, int i_row_arg);

  // умножить строку матрицы на вектор
  TYPE multiplyRowBy(const vector<vector<TYPE>> &linear_system, const int i, const vector<TYPE> &x);

  // печать матриы
  void printMatrix(const vector<vector<TYPE>> &linear_system);

  // сгенерировать случайную СЛАУ 
  vector<vector<TYPE>> generateLinearSystem(int n, int m, bool small_numbers = false);

  // считать матрицу из файла 
  vector<vector<TYPE>> dowloadlinear_system(const std::string &filePath);

  // вывод вектора
  void printVector(const vector<TYPE> &v);
}
