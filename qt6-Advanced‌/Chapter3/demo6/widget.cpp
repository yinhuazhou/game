#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QTreeView、QFileSystemModel");
    vbox=new QVBoxLayout(this);
    //创建视图控件
    treeView=new QTreeView();
    vbox->addWidget(treeView);
    //创建数据模型
    fileModel =new QFileSystemModel(this);
    //设置根路径
    fileModel->setRootPath("C:\\");
    treeView->setModel(fileModel);
    //创建标签控件
    label=new QLabel();
    vbox->addWidget(label);
    //使用信号/槽
    connect(treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(treeView_clicked(QModelIndex)));
}

Widget::~Widget() {}

void Widget::treeView_clicked(QModelIndex index){
    QString path=fileModel->filePath(index);
    label->setText(path);
}
