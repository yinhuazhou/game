#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

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
    QGraphicsLinearLayout *linear;//线性布局指针
    QLabel *label;//标签控件指针
    QPushButton *btn1,*btn2;//按钮指针
    QGraphicsProxyWidget *p1,*p2,*p3;//代理控件指针
};
#endif // WIDGET_H
