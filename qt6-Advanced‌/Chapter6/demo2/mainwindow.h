#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <cmath>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QLineSeries *seriesSin;//折线数据序列指针
    QSplineSeries *seriesCos;//样条曲线数据序列指针
    QValueAxis *axisX,*axisY;//坐标轴指针
    float pi=3.14159265358979;//圆周率
};
#endif // MAINWINDOW_H
