
#include "consistent_gauss.h"
#include "matrix_proc.h"

#include <omp.h>

#include <iostream>

using namespace matrix_processing;

// приведение матрицы к верхней треугольной
void consistent_gauss::toUpperTriangular(vector<vector<TYPE>>& linear_system) {
  for (int j = 0; j < linear_system.size() - 1; ++j) {

    // выбираем наибольшый элемент в столбце меняем его строку с j-ой строкой
    int max_row = findMaxInColumn(linear_system, j, j);
    swapRows(linear_system, j, max_row);

    // нормируем строки по j-му элементу 
    for (int i = j; i < linear_system.size(); ++i)
      if (linear_system[i][j] != 0)
        divideRowBy(linear_system, i, linear_system[i][j]);

    // вычитаем j-ую строку из всех остальных 
    for (int i = 0; i < linear_system.size(); ++i)
      if (i != j)
        subRows(linear_system, i, j);
  }
}

// решение слау с матрицей уравненеий приведенной к верхнему треугольному виду 
vector<TYPE> consistent_gauss::solveUppertriangular(const vector<vector<TYPE>>& linear_system) {
  vector<TYPE> x(linear_system.size(), 0);

  int last_row = linear_system.size() - 1;
  int last_col = linear_system[0].size() - 1;

#ifdef WITH_ZERO_DIVISION_CHECK
  if (abs(linear_system[last_row][last_col - 1]) < ACCURACY)
    throw runtime_error("Division by zero");
#endif // WITH_ZERO_DIVISION_CHECK

  x[last_row] = linear_system[last_row][last_col] / linear_system[last_row][last_col - 1];

  for (int i = last_row - 1; i >= 0; --i) {

    // сумма элементов справа от диагонального (кроме последнего столбца) с учетом известных х
    TYPE sum = 0;
    for (int j = i + 1; j <= last_col - 1; ++j)
      sum += linear_system[i][j] * x[j];

#ifdef WITH_ZERO_DIVISION_CHECK
    if (linear_system[i][i] < ACCURACY)
      throw runtime_error("Division by zero");
#endif // WITH_ZERO_DIVISION_CHECK

    x[i] = (linear_system[i][last_col] - sum) / linear_system[i][i];
  }

  return x;
}

// проверка корректности вектора решений СЛАУ 
bool consistent_gauss::checkSolution(const vector<vector<TYPE>>& linear_system, const vector<TYPE>& x, const bool withPrint) {
  for (int i = 0; i < linear_system.size(); ++i) {
    TYPE m = multiplyRowBy(linear_system, i, x);
    TYPE m_right = linear_system[i][linear_system[0].size() - 1];

    if (abs(m - m_right) > ACCURACY) { // mistake was found
      if (withPrint)
        cout << "Mistake was found in row " << i << " " << m << " != " << m_right << "\n";

      return false;
    }
    else { // current checking is succeed
      cout << m << " = " << m_right << " ok\n";
    }
  }

  if (withPrint)
    cout << "Solution is write\n";

  return true;
}

// замер производительности последовательного решения 
vector<pair<int, double>> consistent_gauss::consistentSolutionExperiment(int m_size_from, int m_size_to, int step, bool with_print) {
  vector<pair<int, double>> res;

  for (int i = m_size_from; i <= m_size_to; i += step) {

    // generation of source linear system
#ifdef SIMPLE_NUMBERS
    auto linear_system = generateLinearSystem(i, i + 1, true);
#else
    auto linear_system = generateLinearSystem(i, i + 1, false);
#endif  

    // print
    //if (with_print) {
    //  cout << "Generated linear system with size " << i << " x " << i + 1 << '\n';
    //  printMatrix(linear_system);
    //}

    try {
      // solution
      double ms_start = omp_get_wtime();
      toUpperTriangular(linear_system);
      //if (with_print) {
      //  cout << "Upper triangular:\n";
      //  printMatrix(linear_system);
      //}
      auto v_solution = solveUppertriangular(linear_system);
      double ms_time = omp_get_wtime() - ms_start;

      // print 
      if (with_print) {
        cout << "Solution: ";
        printVector(v_solution);

        cout << "Time = " << ms_time << endl;

        checkSolution(linear_system, v_solution, true);
      }

      res.push_back({ i, ms_time });
    }
    catch (exception e) {
      res.push_back({ i, -1 });
    }
  }

  return res;
}
