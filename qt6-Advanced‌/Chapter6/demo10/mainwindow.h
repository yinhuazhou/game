#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QPolarChart>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <cmath>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QPolarChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QLineSeries *lineSeries;//数据序列指针
    QScatterSeries *scatterSeries;//数据序列指针
    QValueAxis *axisAngle,*axisRadius;//坐标轴指针
    float pi=3.14159265358979;//圆周率
};
#endif // MAINWINDOW_H
