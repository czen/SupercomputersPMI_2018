
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <omp.h>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <exception>

#include <qapplication.h>

#include "matrix_proc.h"
#include "consistent_gauss.h"
#include "concurrent_gauss.h"
#include "result-chart.h"

using namespace std;
using namespace matrix_processing;

#define TYPE long double // тип данных для вычислений 
#define ACCURACY 1e-5 // точность вычислений 
#define BUFF_READ_SIZE 64
// #define WITH_ZERO_DIVISION_CHECK // проверять деление на ноль 
// #define SIMPLE_NUMBERS // использователь целые числа 0..10 в матрицах

void printResults(const vector<pair<int, double>> &res, const string title) {
  cout << "| " << title << " |" << '\n';
  cout << "n x n |       time ms     |" << '\n';

  for (const auto &i : res)
    cout << i.first << " | " << i.second << '\n';

  cout << endl;
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  QApplication app(argc, argv);

  int from = 200, to = 400, step = 30;

  cout << "Start matrix dimenstion >> ";
  cin >> from;
  cout << "End matrix dimenstion >> ";
  cin >> to;
  cout << "Step of increasing matrix dimension >> ";
  cin >> step;
   
  auto res_consistent = consistent_gauss::consistentSolutionExperiment(from, to, step);
  auto res_concurrent = concurrent_gauss::concurrentSolutionExperiment(from, to, step);

  printResults(res_consistent, "Consistent results");
  printResults(res_concurrent, "Concurrent results");
  
  ResultChart::chowResult(res_concurrent, res_consistent);  

  return app.exec();
}
