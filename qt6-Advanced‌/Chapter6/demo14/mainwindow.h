#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QList>
#include <QDateTime>
#include <QDate>
#include <QTime>

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
    QDateTimeAxis *axisX;//坐标轴指针
    QValueAxis *axisY;//坐标轴指针
};
#endif // MAINWINDOW_H
