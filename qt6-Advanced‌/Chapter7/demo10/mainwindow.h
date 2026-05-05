#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <QRandomGenerator>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QWidget *container;//三维图表容器指针
    Q3DBars *graph3D;//三维曲面图表指针
    QBarDataProxy *dataProxy;//三维柱状图的数据代理指针
    QBar3DSeries *series;//数据序列指针
    QRandomGenerator generator;//随机数产生器
};
#endif // MAINWINDOW_H
