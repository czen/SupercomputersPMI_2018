
#pragma once

#include <vector>
#include <iostream>
using namespace std;
class UpperTriangularMatrix {
	vector<double> data_;
	vector<int> sizes_; // количество элементов до строки
	int up_tr_rows, up_tr_cols;

public:
	UpperTriangularMatrix(const vector<vector<double>> &matrix);

	int rowCounts() const;
	int colCounts() const;

	double at(int i_f, int j_f) const;
	void print() const;
};