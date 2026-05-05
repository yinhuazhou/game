#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtDataVisualization>
#include <cmath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnType1,*btnType2,*btnBack,*btnGrid;//标签指针
    QWidget *container;//三维图表容器指针
    Q3DScatter *graph3D;//三维散点图表指针
    QScatterDataProxy *dataProxy;//三维散点图的数据代理指针
    QScatter3DSeries *series;//数据序列指针
    QScatterDataArray *itemArray;//散点数组指针
private slots:
    void change_type1();
    void change_type2();
    void show_back();
    void hide_grid();
};
#endif // WIDGET_H
