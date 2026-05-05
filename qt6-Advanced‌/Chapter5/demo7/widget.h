#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsGridLayout>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QGraphicsView *view;//图像视图指针
    QGraphicsScene *scene;//图像场景指针
    QVBoxLayout *vbox;//垂直布局指针
    QGraphicsWidget *widget;//图形控件指针
    QGraphicsGridLayout *grid;//栅格布局指针
    //按钮指针
    QPushButton *btn1,*btn2,*btn3,*btn4,*btn5,*btn6,*btn7,*btn8,*btn9;
    //代理控件指针
    QGraphicsProxyWidget *p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8,*p9;
};
#endif // WIDGET_H
