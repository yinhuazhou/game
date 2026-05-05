#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QPieSeries>
#include <QPieSlice>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
    QPieSeries *seriesPie;//饼图数据序列指针
    QPieSlice *first,*second;//切片指针
};
#endif // MAINWINDOW_H
