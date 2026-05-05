#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <QPointF>
//自定义椭圆图形项
class Ellipse :public QGraphicsItem
{
public:
    Ellipse(int w=1,int h=1,QGraphicsItem *parent=nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
private:
    int _width,_height;
};
#endif // ELLIPSE_H
