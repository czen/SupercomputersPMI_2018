
#include <vector>
#include "matrix_generation.h"

using namespace std;

// создать матрицу, заполненную случайными числами из заданного диапазаона 
vector<vector<double>> randMatrix(int n, int m, pair<int, int> range) {
	vector<vector<double>> matrix(n);

	for (auto &i : matrix)
		i.resize(m); //size of every vector in all matrix(n)

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			matrix[i][j] = range.first + (rand() % (range.second - range.first));

	return matrix;
}

// создать верхне-треугольную матрицу, заполненную случайными числами из заданного диапазона
vector<vector<double>> randUpMatrix(int n, int m, pair<int, int> range) {
	auto matrix = randMatrix(n, m, range);

	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < i; ++j)
			matrix[i][j] = 0; //null of lower-triangle matrix
	 
	return matrix;
}

// сгенерировать симметричную матрицу
vector<vector<double>> randSymmMatrix(int n, int m, pair<int, int> range) {
	auto matrix = randMatrix(n, m, range);

	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < i; ++j)
			matrix[i][j] = matrix[j][i]; //symmetric regarding to main diagonal

	return matrix;
}