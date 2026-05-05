#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QRectF>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;  //垂直布局指针
    QGraphicsView *graphicsView;//图像视图指针
    QGraphicsScene *graphicsScene;//图像场景指针
    QGraphicsRectItem *rectItem;//矩形图形项指针
    QGraphicsEllipseItem *ellipseItem;//椭圆图形项指针
};
#endif // WIDGET_H
