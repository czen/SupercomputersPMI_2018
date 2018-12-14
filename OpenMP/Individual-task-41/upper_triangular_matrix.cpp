#include "upper_triangular_matrix.h"
using namespace std;
UpperTriangularMatrix::UpperTriangularMatrix(const vector<vector<double>>& matrix) {

	// сохранение матрицы в одну строку 
	for (int i = 0; i < matrix.size(); ++i)
		for (int j = i; j < matrix[0].size(); ++j)
			data_.push_back(matrix[i][j]);

	// сохренение размерностей 
	up_tr_rows = matrix.size();
	up_tr_cols = matrix[0].size();

	// сохранение информации о количестве элементов
	int distance = 0;
	sizes_.push_back(distance);
	for (int i = 0; i < up_tr_rows; ++i) {
		for (int j = i; j < up_tr_cols; ++j)
			++distance;

		sizes_.push_back(distance);
	}
}

int UpperTriangularMatrix::rowCounts() const {
	return up_tr_rows;
}

int UpperTriangularMatrix::colCounts() const {
	return up_tr_cols;
}

double UpperTriangularMatrix::at(int i_f, int j_f) const {
	if (j_f < i_f) return 0;

	return data_[sizes_[i_f] + j_f - i_f];
}

// проход и печать верхней треугольной матрицы
void UpperTriangularMatrix::print() const {
	auto data_iter =data_.begin();
	//автомат определение типа на основе инициализ значения для переменной итератора 
	for (int i = 0; i < up_tr_rows; ++i) {

		for (int j = i; j < up_tr_cols; ++j, ++data_iter)
			cout << *data_iter << ' ';

		cout << '\n';
	}
}
