#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListWidget");
    vbox=new QVBoxLayout(this);
    //创建列表控件
    listWidget=new QListWidget();
    listWidget->setFont(QFont("黑体",14));
    listWidget->setStyleSheet("background-color:yellowgreen");
    vbox->addWidget(listWidget);
    //插入项
    listWidget->insertItem(0,"C++");
    listWidget->insertItem(1,"Python");
    listWidget->insertItem(2,"Java");
    listWidget->insertItem(3,"PHP");
    listWidget->insertItem(4,"JavaScript");
}

Widget::~Widget() {}
