#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <cmath>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QWidget *container;//三维图表容器指针
    Q3DScatter *graph3D;//三维散点图表指针
    QScatterDataProxy *dataProxy;//三维散点图的数据代理指针
    QScatter3DSeries *series;//数据序列指针
    QScatterDataArray *itemArray;//散点数组指针
};
#endif // MAINWINDOW_H
