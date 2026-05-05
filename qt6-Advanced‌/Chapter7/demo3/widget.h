#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtDataVisualization>
#include <cmath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector3D>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox;//水平布局指针
    QPushButton *btnLeft,*btnRight,*btnUp;//按钮指针
    QPushButton *btnDown,*btnFar,*btnNear;//按钮指针
    QWidget *container;//三维图表容器指针
    Q3DScatter *graph3D;//三维散点图表指针
    QScatterDataProxy *dataProxy;//三维散点图的数据代理指针
    QScatter3DSeries *series;//数据序列指针
    QScatterDataArray *itemArray;//散点数组指针
private slots:
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void move_far();
    void move_near();
};
#endif // WIDGET_H
