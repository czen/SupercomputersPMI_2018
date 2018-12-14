
#pragma once
#include <iostream>
#include <vector>
using namespace std;
class SymmetricMatrix {
	vector<double> data_;
	vector<int> sizes_; // количество элементов до строки
	int sym_rows, sym_cols; 

public:
	SymmetricMatrix(const vector<vector<double>> &matrix); 

	int rowCounts() const;
	int colCounts() const;

	double at(int i_f, int j_f) const;
	void print() const;
};