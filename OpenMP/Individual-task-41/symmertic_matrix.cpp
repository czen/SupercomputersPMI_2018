#include "symmertic_matrix.h"
using namespace std;
SymmetricMatrix::SymmetricMatrix(const vector<vector<double>>& matrix) {

	// ���������� ������� � ���� ������ 
	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j <= i; ++j)
			data_.push_back(matrix[i][j]);

	// ���������� ������������ 
	sym_rows = matrix.size(); 
	sym_cols = matrix[0].size(); //������� ������ vector<vector<double>>(����������� �������� ������� �������)

	// ���������� ���������� � ���������� ���������
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
	if (j_f > i_f) return at(j_f, i_f); //���� �������� ������� �� �������� ������� ����

	return data_[sizes_[i_f] + j_f]; //���������� �� ������� � ����� ������� � ����� �������� 
}

//������ � ������ ������ ����������� ������������ �������
void SymmetricMatrix::print() const {
	//������� ����������� ���� �� ������ ��������� �������� ��� ���������� ��������� 
	auto data_iter =data_.begin();

	for (int i = 0; i < sym_rows; ++i) {

		for (int j = 0; j <= i; ++j, ++data_iter)
			cout << *data_iter << ' ';

		cout << '\n';
	}
}
