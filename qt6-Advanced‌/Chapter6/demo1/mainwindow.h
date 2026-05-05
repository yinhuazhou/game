#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QList>
#include <QPointF>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QLineSeries *series;//数据序列指针
    QChart *chart;//图表指针
    QChartView *chartView;//图表视图指针
};
#endif // MAINWINDOW_H
