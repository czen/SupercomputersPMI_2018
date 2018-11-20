
#pragma once

#include <vector>

#define TYPE long double // тип данных для вычислений 
#define ACCURACY 1e-2 // точность вычислений 
#define BUFF_READ_SIZE 64
// #define WITH_ZERO_DIVISION_CHECK // проверять деление на ноль 
// #define SIMPLE_NUMBERS // использователь целые числа 0..10 в матрицах

using namespace std;

namespace consistent_gauss {
  // приведение матрицы к верхней треугольной
  void toUpperTriangular(vector<vector<TYPE>> &linear_system);

  // решение слау с матрицей уравненеий приведенной к верхнему треугольному виду 
  vector<TYPE> solveUppertriangular(const vector<vector<TYPE>> &linear_system);

  // проверка корректности вектора решений СЛАУ 
  bool checkSolution(const vector<vector<TYPE>> &linear_system, const vector<TYPE> &x, const bool withPrint = false);

  // замер производительности последовательного решения 
  vector<pair<int, double>> consistentSolutionExperiment(int m_size_from, int m_size_to, int step, bool with_print = false);
}