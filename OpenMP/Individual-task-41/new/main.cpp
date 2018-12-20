
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <omp.h>


#include "upper_triangular_matrix.h"
#include "symmertic_matrix.h"
#include "matrix_generation.h"
#include "matrix_multiplication.h"


#include <qwidget.h>
#include <qapplication.h>
#include <qchartview.h>
#include <qchart.h>
#include <qlayout.h>
#include <QValueAxis>
#include <QChart>
#include <QLineSeries>
#include <string>

QT_CHARTS_USE_NAMESPACE

using namespace std;

// размерность матриц
const int N = 512;
const int M = 512;

// диапазон значений элементов матрицы
const int FROM = 0;
const int TO = 4;

// печать матрицы 
void printMatrix(const vector<vector<double>> &matrix) {
	for (auto i : matrix) {
		for (auto j : i)
			cout << j << ' ';

		cout << '\n';
	}
}

//func for graph
QChartView* BuildGraph(const vector<pair<int, double>>& vector, QString &str) {
	

	//chart view
	QChartView *viewGraph = new QChartView();
	viewGraph->setRenderHint(QPainter::Antialiasing);
	//create and add data series to the graph 
	QLineSeries *lines = new QLineSeries();
	for (auto const& vecpairs : vector) {
		lines->append(vecpairs.first, vecpairs.second);
	}
	// Create graph
	QChart *graphchart = new QChart();
	graphchart->addSeries(lines);
	graphchart->legend()->hide();
	graphchart->setTitle("Graphic");


	//Axis setting

	//XAxis
	QValueAxis *axisX = new QValueAxis();
	axisX->setTitleText("SIZE OF BLOCK MATRIX");
	axisX->setLabelFormat("%i");
	axisX->setTickCount(1);
	graphchart->addAxis(axisX, Qt::AlignBottom);
	lines->attachAxis(axisX);
	lines->setColor(Qt::green);

	//YAxis
	QValueAxis *axisY = new QValueAxis();
	axisY->setTitleText(str);
	axisY->setLabelFormat("%g");
	axisY->setTickCount(1);
	axisY->setRange(0, 20);
	graphchart->addAxis(axisY, Qt::AlignLeft);
	lines->attachAxis(axisY);

	//Set the graph in the view
	viewGraph->setChart(graphchart);

	viewGraph->resize(500, 500);
	viewGraph->show();

	return viewGraph;
}

int main() {

	srand(time(nullptr)); //генерация разных чисел при запуске программы для текущего времени

	int argc = 0;
	char** argv;

	QApplication app(argc, argv);
	
	auto upper_raw = randUpMatrix(N, M, { FROM, TO });
	auto symmetric_raw = randSymmMatrix(N, M, { FROM, TO });

	double start_time;
	double end_time;
	 
	UpperTriangularMatrix upper_matrix(upper_raw); //in the form of a one - dimensional array
	SymmetricMatrix symm_matrix(symmetric_raw); //in the form of a one - dimensional array			

	vector<vector<double>> res(N);
	for (auto &i : res) i.resize(M); 

	cout << "SIZE OF MATRIX: " << N << " x " << M << endl;	

	//// CHECK___________________________________
	//printMatrix(upper_triangular);
	//cout << "\n\n";
	//printMatrix(symmetric_matrix);
	//cout << "\n\n";

	//multiplyConcurrentPairs(upper_matrix, symmetric_matrix, res, 2);

	//cout << "Res:\n";
	//printMatrix(res);

	//return 0;
	////

	int block_size = 2;

	vector<pair<int,double>> res_data_blockpair_paral;
	vector<pair<int, double>> res_data_coreblocks_paral;
	vector<pair<int, double>> res_data_coreblocks_sequential;
	vector<pair<int, double>> res_data_sequential;
	
	cout << "Block size | 1 variant | 2 variant | consistently blocks | consist \n";

	while (block_size <= N / 2) {

		//time of 1 parallel method
		start_time = omp_get_wtime();
	    multiplyConcurrentPairs(upper_matrix, symm_matrix, res, block_size);
		end_time = omp_get_wtime() - start_time;
		res_data_blockpair_paral.push_back(pair<int, double>(block_size,end_time));  //add size of matrix and time

		cout << block_size << " \t " << end_time << " \t ";
		
       //time of 2 parallel method
	   start_time = omp_get_wtime();
	   multiplyConcurrent(upper_matrix, symm_matrix, res, 4, block_size);
	   end_time = omp_get_wtime() - start_time;
	   res_data_coreblocks_paral.push_back(pair<int, double>(block_size, end_time)); //add size of matrix and time
       
	   cout << end_time << "\t";

	   //sequential method with blocks
	   start_time = omp_get_wtime();
	   multiplyConsBlocks(upper_matrix, symm_matrix, res, block_size);
	   end_time = omp_get_wtime() - start_time;
	   res_data_coreblocks_sequential.push_back(pair<int, double>(block_size, end_time));

	   cout << end_time << "   ";

	   //sequential method
	   start_time = omp_get_wtime();
	   multiplyCons(upper_matrix, symm_matrix, res);
	   end_time = omp_get_wtime() - start_time;
	   res_data_sequential.push_back(pair<int, double>(block_size, end_time));

	   cout << end_time << endl;

		block_size *= 2;
	}			

	//CREATE Widget drawing graphics
	QWidget * window = new QWidget;

	//widget layout
	QHBoxLayout *hLayout = new QHBoxLayout(window);
	QString str1 ="EXECUTION TIME FOR 1 CASE";
	QString str2 = "EXECUTION TIME FOR 2 CASE";
	QString str3 = "EXECUTION TIME FOR CONSISTENT BLOCK CASE";
	QString str4 = "EXECUTION TIME FOR CONSISTENT CASE";
	hLayout->addWidget(BuildGraph(res_data_blockpair_paral, str1));
	hLayout->addWidget(BuildGraph(res_data_coreblocks_paral, str2));
	hLayout->addWidget(BuildGraph(res_data_coreblocks_sequential, str3));
	hLayout->addWidget(BuildGraph(res_data_sequential, str4));
	window->show();
	
	return app.exec();
}