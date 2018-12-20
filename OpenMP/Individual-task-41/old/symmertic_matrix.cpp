#include "symmertic_matrix.h"
using namespace std;
SymmetricMatrix::SymmetricMatrix(const vector<vector<double>>& matrix) {

	// сохранение матрицы в одну строку 
	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j <= i; ++j)
			data_.push_back(matrix[i][j]);

	// сохранение размерностей 
	sym_rows = matrix.size(); 
	sym_cols = matrix[0].size(); //нулевой вектор vector<vector<double>>(размерность нулевого вектора матрицы)

	// сохранение информации о количестве элементов
	int distance = 0;
	sizes_.push_back(distance);
	for (int i = 0; i < sym_rows; ++i) {
		for (int j = 0; j <= i; ++j)
			++distance;

		sizes_.push_back(distance);
	}
}

int SymmetricMatrix::rowCounts() const {
	return sym_rows;
}

int SymmetricMatrix::colCounts() const {
	return sym_cols;
}

//create method item access by index for element 
double SymmetricMatrix::at(int i_f, int j_f) const {
	if (j_f > i_f) return at(j_f, i_f); //если получили элемент из верхнего правого угла

	return data_[sizes_[i_f] + j_f]; //сместились по вектору и нашли элемент с таким индексом 
}

//проход и печать нижней треугольной симметричной матрицы
void SymmetricMatrix::print() const {
	//автомат определение типа на овнове инициализ значения для переменной итератора 
	auto data_iter =data_.begin();

	for (int i = 0; i < sym_rows; ++i) {

		for (int j = 0; j <= i; ++j, ++data_iter)
			cout << *data_iter << ' ';

		cout << '\n';
	}
}
