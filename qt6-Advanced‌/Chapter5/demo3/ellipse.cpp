#include "ellipse.h"

Ellipse::Ellipse(int w,int h,QGraphicsItem *parent):
    _width(w),_height(h),QGraphicsItem(parent){}

QRectF Ellipse::boundingRect() const{
    QRectF rect(-5,-_height/2-10,_width+25,_height+40);
    return rect;
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen pen=painter->pen();
    pen.setWidth(3);
    painter->setPen(pen);
    //绘制椭圆
    painter->drawEllipse(-10,-_height/2-10,_width,_height);
    //绘制文字
    QFont font=painter->font();
    font.setPixelSize(20);
    painter->setFont(font);
    painter->drawText(QPointF(_width/2,0),"椭圆的中心");
}
