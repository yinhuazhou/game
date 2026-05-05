#ifndef COS_H
#define COS_H

#include <QGraphicsItem>
#include <QPainter>
#include <cmath>
#include <QRectF>
#include <QPolygonF>
#include <QPen>

class Cos :public QGraphicsItem
{
public:
    Cos(int w=1,int h=1,QGraphicsItem *parent=nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
private:
    int _width;
    int _height;
};
#endif // COS_H
