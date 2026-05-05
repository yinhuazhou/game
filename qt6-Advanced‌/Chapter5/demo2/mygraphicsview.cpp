#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent)
{}
//鼠标单击事件
void MyGraphicsView::mousePressEvent(QMouseEvent *e){
    emit sendPosition(e->scenePosition());//发送信号，参数是鼠标位置
    QGraphicsView::mousePressEvent(e);//调用父类的同名函数
}
//鼠标移动事件
void MyGraphicsView::mouseMoveEvent(QMouseEvent *e){
    emit sendPosition(e->scenePosition());//发送信号，参数是鼠标位置
    QGraphicsView::mouseMoveEvent(e);//调用父类的同名函数
}
//重写背景函数，设置背景颜色
void MyGraphicsView::drawBackground(QPainter *painter, const QRectF &rect){
    painter->fillRect(rect,Qt::gray);
}
