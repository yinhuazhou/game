#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent):QGraphicsView(parent) {}
//按下鼠标按键事件
void MyGraphicsView::mousePressEvent(QMouseEvent *e){
    emit sendPoint(e->position());
    QGraphicsView::mousePressEvent(e);
}
//鼠标移动事件
void MyGraphicsView::mouseMoveEvent(QMouseEvent *e){
    emit movePoint(e->position());
    QGraphicsView::mouseMoveEvent(e);
}
//鼠标按键被释放
void MyGraphicsView::mouseReleaseEvent(QMouseEvent *e){
    emit releasePoint(e->position());
    QGraphicsView::mouseReleaseEvent(e);
}
