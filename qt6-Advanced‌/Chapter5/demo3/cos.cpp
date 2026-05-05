#include "cos.h"

Cos::Cos(int w,int h,QGraphicsItem *parent):_width(w),_height(h)
    ,QGraphicsItem(parent){}

QRectF Cos::boundingRect() const{
    QRectF rect(-5,-_height/2-20,_width+25,_height+40);
    return rect;
}

void Cos::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPolygonF p_cos;
    for(int i=0;i<=360;i++){
        float x_value=i*_width/360;
        float y_value=cos(i*3.1415926/180)*_height/2*(-1);
        p_cos.append(QPointF(x_value,y_value));
    }
    QPen pen=painter->pen();
    pen.setWidth(3);
    painter->setPen(pen);
    //绘制余弦曲线
    painter->drawPolyline(p_cos);
}
