
#pragma once

#include <vector>
#include <omp.h>

using namespace std;

// последовательное умножение матриц
void multiplyCons(const UpperTriangularMatrix &u, const SymmetricMatrix &s, vector<vector<double>> &res, const int block_size);

// попарное параллельное умножение блоков
void multiplyConcurrentPairs(const UpperTriangularMatrix &u, const SymmetricMatrix &s, vector<vector<double>> &res, int block_size);

// распределение параллельного умножения между заданным количеством потоков 
void multiplyConcurrent(const UpperTriangularMatrix &u, const SymmetricMatrix &s, vector<vector<double>> &res, int n_threads, int block_size);

double multiplyRowByColumn(const UpperTriangularMatrix & u, 
	const SymmetricMatrix & s,
	pair<int, int> block1,
	pair<int, int> block2,
	int block_size, 
	int i_s,
	int j_c
);

void multiplyBlocks(const UpperTriangularMatrix & up_matrix, 
	const SymmetricMatrix & symm_matrix, 
	vector<vector<double>>& res, 
	pair<int, int> block1, 
	pair<int, int> block2, 
	pair<int, int> block_res, 
	int block_size
);

using block_t = pair<int, int>;

struct Division {
	block_t target_block; //pair of multiplyblocks
	vector<pair<block_t, block_t>> pairs; // пары блоков, которые нужно умножить, чтобы получить блок target_block

	void add(int i1, int j1, int i2, int j2) {
		pairs.push_back(pair<block_t, block_t>(pair<int, int>(i1, j1), pair<int, int>(i2, j2)));
	}
};

// Получение информации о том какие блоки нужно умножить
vector<Division> divideToBlocks(const vector<vector<double>>& matrix, int block_size);


