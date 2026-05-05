#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QLineSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QBarSeries *barSeries;//条形图数据序列指针
    QLineSeries *lineSeries;//折线图数据序列指针
    QBarSet *set1,*set2,*set3;//数据项指针
    QBarCategoryAxis *axisX;//坐标轴指针
    QValueAxis *axisY;//坐标轴指针
};
#endif // MAINWINDOW_H
