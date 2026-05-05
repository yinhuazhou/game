#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QBoxPlotSeries>
#include <QBoxSet>
#include <QList>
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
    QBoxPlotSeries *seriesBox;//箱形图数据序列指针
    QBoxSet *boxSet;//数据项指针
    QValueAxis *axisY;//坐标轴指针
    QBarCategoryAxis *axisX;//坐标轴指针
};
#endif // MAINWINDOW_H
