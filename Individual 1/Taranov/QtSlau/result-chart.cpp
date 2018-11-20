
#include "result-chart.h"

#include <qchart.h>
#include <qlayout.h>
#include <qchartview.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qchartglobal.h>
#include <qlineseries.h>
#include <qpoint.h>
#include <qvalueaxis.h>
#include <qscatterseries.h>
#include <qsplineseries.h>

QT_CHARTS_USE_NAMESPACE

QChartView* createChart(const std::vector<std::pair<int, double>>& data, const QString &chart_name, std::pair<double, double> y_range) {

  auto series = new QLineSeries();
  series->setName(chart_name);
  for (const auto &i : data)
    series->append(i.first, i.second);  

  // chart concurrent creation
  auto chart = new QChart();
  chart->addSeries(series);
  chart->setAnimationOptions(QChart::AnimationOption::AllAnimations);
  chart->setTheme(QChart::ChartTheme::ChartThemeDark);

  // ось Х
  auto axisX = new QValueAxis();
  axisX->setTitleText(QStringLiteral("Размерность матрицы"));
  axisX->setLabelFormat("%i");
  axisX->setTickCount(1);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // ось Y 
  auto axisY = new QValueAxis();
  axisY->setTitleText(QStringLiteral("Время решения, мс"));
  axisY->setLabelFormat("%g");
  axisY->setTickCount(1);
  axisY->setMinorTickCount(1);
  axisY->setRange(y_range.first, y_range.second);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  // chart view creation
  auto view = new QChartView();
  view->setChart(chart);
  return view;
}

void ResultChart::chowResult(const std::vector<std::pair<int, double>>& res_concurrent,
  const std::vector<std::pair<int, double>>& res_consistent) 
{    
  double y_max_consist = 0;
  for (auto i : res_consistent)
    if (i.second > y_max_consist)
      y_max_consist = i.second;

  auto window = new QWidget();
  auto window_layout = new QHBoxLayout(window);
  std::pair<double, double> y_range(res_concurrent[0].second, y_max_consist);
  window_layout->addWidget(createChart(res_concurrent, QStringLiteral("Параллельное решение"), y_range));
  window_layout->addWidget(createChart(res_consistent, QStringLiteral("Последовательное решение"), y_range));
  window->show();
}
