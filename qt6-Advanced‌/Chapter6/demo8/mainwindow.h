#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QValueAxis>
#include <QBarCategoryAxis>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QCandlestickSeries *candlestickSeries;//蜡烛图数据序列指针
    QCandlestickSet *candleSet;//数据项指针
    QValueAxis *axisY;//坐标轴指针
    QBarCategoryAxis *axisX;//坐标轴指针
};
#endif // MAINWINDOW_H
