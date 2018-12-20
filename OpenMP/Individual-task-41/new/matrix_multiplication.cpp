
#include "symmertic_matrix.h"
#include "upper_triangular_matrix.h"

#include <vector>
#include "matrix_multiplication.h"

using namespace std;

//sequential multiplication blocks
void multiplyConsBlocks(const UpperTriangularMatrix & u, const SymmetricMatrix & s, vector<vector<double>>& res, const int block_size) {
	vector<Division> targets = divideToBlocks(res, block_size); //all result blocks	
	
for (int i = 0; i < targets.size(); ++i)
		for (int j = 0; j < targets[i].pairs.size(); ++j)
			multiplyBlocks(u, s, res, targets[i].pairs[j].first, targets[i].pairs[j].second, targets[i].target_block, block_size);
}
//sequential multiplication
void multiplyCons(const UpperTriangularMatrix & u, const SymmetricMatrix & s, vector<vector<double>>& res) {
	for (int i = 0; i < res.size(); ++i)
		for (int j = 0; j < res.size(); ++j)
		{
			res[i][j] = 0;
			for (int k = 0; k < res.size(); ++k)
				res[i][j] += u.at(i, k) * s.at(k, j);
		}
}

//parallel multiplication of pairs of blocks
void multiplyConcurrentPairs(const UpperTriangularMatrix & u, const SymmetricMatrix & s, vector<vector<double>>& res, int block_size) {
vector<Division> targets = divideToBlocks(res, block_size); //all result blocks

//for all threads variables
#pragma omp parallel shared(u, s, res, targets) num_threads(4)
	
	{
		for (int i = 0; i < targets.size(); ++i)
		#pragma omp for schedule(dynamic, 1)// перемножение пары блоков(1 итерация цикла) для каждого потока
			for (int j = 0; j < targets[i].pairs.size(); ++j) //для каждого результирующего блока перемножение пар в 4 потоках
				multiplyBlocks(u, s, res, targets[i].pairs[j].first, targets[i].pairs[j].second, targets[i].target_block, block_size);
	}
}

//multiplication of blocks in cores
void multiplyConcurrent(const UpperTriangularMatrix & u, const SymmetricMatrix & s, vector<vector<double>>& res, int n_threads, int block_size) {
vector<Division> targets = divideToBlocks(res, block_size);

//common variable list for all threads
#pragma omp parallel shared(u, s, res, targets) num_threads(n_threads) 
	{
#pragma omp for schedule(static, targets.size() / n_threads)//static distribution of iterations between threads
		for (int i = 0; i < targets.size(); ++i)
			for (const auto &j : targets[i].pairs)
				multiplyBlocks(u, s, res, j.first, j.second, targets[i].target_block, block_size);
	}
}

// Умножить i-ую строку первого блока на j-ый столбец второго блока
// NOTE: нумерация в пределах каждого блока начинается с нуля
inline double multiplyRowByColumn(const UpperTriangularMatrix &u,
	const SymmetricMatrix &s,
	pair<int, int> block1, // координата левого верхнего угла блока
	pair<int, int> block2, // координата левого верхнего угла блока
	int block_size, // размер блока(считается, что все они квадратные)
	int i_s, // индекс строки для умножения
	int j_c) // индекс столбца для умножения  
{
	double sum = 0;
	for (int k = 0; k < block_size; ++k) //по столбцу движемся до размера блока
		sum += u.at(block1.first + i_s, block1.second + k) * s.at(block2.first + k, block2.second + j_c);//для смещения по строкам и столбцам каждого блока от углового левого элемента и суммирование
																									    
		return sum;
}

// умножение пары блоков 
void multiplyBlocks(const UpperTriangularMatrix& up_matrix,
	const SymmetricMatrix& symm_matrix,
	vector<vector<double>> &res, // матрица для записи результата умножения 
	pair<int, int> block1, // координата левого верхнего угла блока для умножения
	pair<int, int> block2, // координата левого верхнего угла блока для умножения
	pair<int, int> block_res, // координата левого верхнего угла блока для записи результата
	int block_size // размер блоков для умножения 
)
{
	// block_res : 0 2
	// 0 * 0 -> res[0 2]
	// 0 * 1 -> res[0 3] 
	// 1 * 0 -> res[1 2]
	// 1 * 1 -> res[1 3]

	//какие строки и столбцы в каждом блоке умножаем с учётом смещения в блоке
	for (int i = 0; i < block_size; ++i)
		for (int j = 0; j < block_size; ++j)
			res[block_res.first + i][block_res.second + j] += multiplyRowByColumn(up_matrix, symm_matrix, block1, block2, block_size, i, j);
}

bool isEmptyInUpperTr(pair<int, int> block, int block_size) {
	return block_size  <= block.first && block.second < block.first; //1 условие-блок под главной диагональю и j<i
}

vector<Division> divideToBlocks(const vector<vector<double>> &matrix, int block_size) {
	vector<Division> res; //вектор результирующих блоков

	for (int i_b = 0; i_b < matrix.size(); i_b += block_size)
		for (int j_b = 0; j_b < matrix[0].size(); j_b += block_size)
		{
			Division current_block;
			current_block.target_block = {i_b, j_b}; //result block

			// current block inf collection 
			int i1 = i_b, j1 = 0;
			int i2 = 0, j2 = j_b;
			while (i2 < matrix.size()) {

				// проверка на пустой блок(левый блок ниже главной диагонали в верхней треугольной матрице) и не берем парный ему блок в симмеричной матрице
				if (!isEmptyInUpperTr({ i1, j1 }, block_size))
					current_block.add(i1, j1, i2, j2);

				j1 += block_size;
				i2 += block_size;
			}
			
			
			res.push_back(current_block);
		}

	return res;
}