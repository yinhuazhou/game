#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListView、QStringListModel");
    hbox=new QHBoxLayout(this);
    //创建视图控件
    listView1=new QListView();
    listView2=new QListView();
    hbox->addWidget(listView1);
    hbox->addWidget(listView2);
    //创建数据模型
    listModel=new QStringListModel(this);
    QStringList strings={"四世同堂","水浒传","西游记","红楼梦"};
    listModel->setStringList(strings);
    //设置数据模型
    listView1->setModel(listModel);
    listView2->setModel(listModel);
}

Widget::~Widget() {}
