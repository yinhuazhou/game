#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QAreaSeries>
#include <QValueAxis>
#include <cmath>
#include <QColor>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QLineSeries *seriesSin,*seriesCos;//折线数据序列指针
    QAreaSeries *seriesArea;//面积数据序列指针
    QValueAxis *axisX,*axisY;//坐标轴指针
    float pi=3.14159265358979;//圆周率
};
#endif // MAINWINDOW_H
