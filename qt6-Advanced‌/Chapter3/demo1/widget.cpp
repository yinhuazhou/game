#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListView、QStringListModel");
    vbox=new QVBoxLayout(this);
    //创建视图控件
    listView=new QListView();
    vbox->addWidget(listView);
    //创建数据模型
    listModel=new QStringListModel(this);
    QStringList strings={"三国演义","水浒传","西游记","红楼梦"};
    listModel->setStringList(strings);
    //设置数据模型
    listView->setModel(listModel);
}

Widget::~Widget() {}
