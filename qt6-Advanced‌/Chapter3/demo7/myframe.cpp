#include "myframe.h"

MyFrame::MyFrame(QWidget *parent):QFrame(parent) {
    resize(300,300);
    setFrameShape(QFrame::Box);
    path="";
}

MyFrame::~MyFrame(){}

void MyFrame::setPath(QString p){
    path=p;
}

void MyFrame::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pixmap(path);
    QRect rect1=this->rect();
    painter.drawPixmap(rect1,pixmap);
    QFrame::paintEvent(e);
}
