
#include "matrix_proc.h"

#include <iostream>
#include <fstream>
#include <sstream>

// индекс строки с максимальным эелментом в столбце i
int matrix_processing::findMaxInColumn(const vector<vector<TYPE>>& linear_system, int i_col, int i_from) {
  TYPE max_value = linear_system[0][i_col];
  int max_index = i_from;

  for (int i = i_from; i < linear_system.size(); ++i)
    if (linear_system[i][i_col] > max_value) {
      max_value = linear_system[i][i_col];
      max_index = i;
    }

  return max_index;
}

// аоменять местами 2 строки матрицы 
void matrix_processing::swapRows(vector<vector<TYPE>>& linear_system, int i_from, int i_to) {
  for (int j = 0; j < linear_system[0].size(); ++j)
    swap(linear_system[i_from][j], linear_system[i_to][j]);
}

// поделить все элементы строки на число 
void matrix_processing::divideRowBy(vector<vector<TYPE>>& linear_system, int i_row, TYPE x) {
  for (auto &j : linear_system[i_row]) {
#ifdef WITH_ZERO_DIVISION_CHECK
    if (abs(x) < ACCURACY)
      throw runtime_error("Division by zero");
#endif // WITH_ZERO_DIVISION_CHECK

    if (j) // !!!!!!!!!!!!!!!!!!
      j /= x;
  }
}

// вычитание строк матрицы
void matrix_processing::subRows(vector<vector<TYPE>>& linear_system, int i_row, int i_row_arg) {
  for (int j = 0; j < linear_system[0].size(); ++j)
    linear_system[i_row][j] -= linear_system[i_row_arg][j];
}

// умножить строку матрицы на вектор
TYPE matrix_processing::multiplyRowBy(const vector<vector<TYPE>>& linear_system, const int i, const vector<TYPE>& x) {
  TYPE res = 0;
  for (int j = 0; j < x.size(); ++j)
    res += linear_system[i][j] * x[j];

  return res;
}

// печать матриы
void matrix_processing::printMatrix(const vector<vector<TYPE>>& linear_system) {
  for (const auto &i : linear_system) {
    for (const auto j : i)
      cout << j << ' ';
    cout << '\n';
  }
}

// сгенерировать случайную СЛАУ 
vector<vector<TYPE>> matrix_processing::generateLinearSystem(int n, int m, bool small_numbers) {
  vector<vector<TYPE>> system(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      // get non zero random number
      TYPE x = rand();
      while (abs(x) < ACCURACY)
        x = rand();

      system[i].push_back((int)x);
    }

    while (system[i].size() < m)
      system[i].push_back(0);
  }

  for (int i = 0; i < n; ++i)
    system[i][m - 1] = (int)rand();

  if (small_numbers) {
    for (auto &i : system)
      for (auto &j : i)
        j = (int)j % 100;
  }

  return system;
}

vector<vector<TYPE>> matrix_processing::dowloadlinear_system(const std::string & filePath) {
  std::ifstream source(filePath);
  if (!source.is_open())
    return {};

  TYPE temp;
  char *c, *firstLine = new char[100];
  size_t linear_systemWidth = 1;   // ширина матрицы
  std::stringstream *sstream;
  char *buf = new char[BUFF_READ_SIZE]; // буфер под считывание чисел
  std::vector<TYPE> numbers;  // упорядоченное хранилище считанных чисел

  source.getline(firstLine, 100);
  size_t bufI = 0;
  for (char *c = firstLine; *c != '\0'; ++c)  // считаем сколько элементов в 1ой строке матрицы
  {
    if (*c == ' ')
    {
      ++linear_systemWidth;
      // добавляем в хранилищие элементы первой строки
      buf[bufI] = '\0';
      sstream = new std::stringstream;
      *sstream << buf;
      *sstream >> temp;
      numbers.push_back(temp);
      bufI = 0;
      delete sstream;
    }
    else
    {
      buf[bufI] = *c;
      ++bufI;
    }
  }

  sstream = new std::stringstream;
  buf[bufI] = '\0';
  *sstream << buf;
  *sstream >> temp;
  numbers.push_back(temp);
  delete sstream;
  delete[] firstLine;

  source.getline(buf, linear_systemWidth, ' ');
  while (source) {
    sstream = new std::stringstream;
    *sstream << buf;
    *sstream >> temp;
    numbers.push_back(temp);
    delete sstream;
    source.getline(buf, BUFF_READ_SIZE, ' ');
  }

  size_t n_strings = numbers.size() / linear_systemWidth;
  vector<vector<TYPE>> linear_system(n_strings);

  int i_row = 0;
  for (int i = 0; i < numbers.size(); ++i)
    linear_system[(int)(i / linear_systemWidth)].push_back(numbers[i]);

  return linear_system;
}

// вывод вектора
void matrix_processing::printVector(const vector<TYPE>& v) {
  for (auto i : v)
    cout << i << ' ';
  cout << endl;
}
