#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QLogValueAxis>
#include <QValueAxis>
#include <QRandomGenerator>
#include <QPen>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QLineSeries *lineSeries;//折线数据序列指针
    QValueAxis *axisX;//坐标轴指针
    QLogValueAxis *axisY;//坐标轴指针
    QRandomGenerator random;//生成随机数对象
};
#endif // MAINWINDOW_H
