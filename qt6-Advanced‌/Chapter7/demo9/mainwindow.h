#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <cmath>
#include <QWidget>
#include <QVector3D>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QWidget *container;//三维图表容器指针
    Q3DSurface *graph3D;//三维曲面图表指针
    QSurfaceDataProxy *dataProxy;//三维曲面图的数据代理指针
    QSurface3DSeries *series;//数据序列指针
    QSurfaceDataArray *itemArray;//散点数组指针
    QValue3DAxis *axisX,*axisY,*axisZ;//三维坐标轴指针
};
#endif // MAINWINDOW_H
