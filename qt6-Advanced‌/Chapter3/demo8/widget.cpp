#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,620,300);
    setWindowTitle("QTreeView、QListView、QFileSystemModel");
    //创建系统文件模型
    fileModel=new QFileSystemModel(this);
    //设置根路径
    fileModel->setRootPath("C:\\");
    //创建树视图控件
    treeView=new QTreeView();
    //设置模型
    treeView->setModel(fileModel);
    //创建列表视图控件
    listView=new QListView();
    //设置模型
    listView->setModel(fileModel);
    //创建分割器
    hSplitter =new QSplitter(Qt::Horizontal);
    //向分割器控件中添加控件
    hSplitter->addWidget(treeView);
    hSplitter->addWidget(listView);
    //设置窗口的布局
    hbox=new QHBoxLayout(this);
    hbox->addWidget(hSplitter);
    //使用信号/槽
    connect(treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(view_clicked(QModelIndex)));
}

Widget::~Widget() {}

void Widget::view_clicked(QModelIndex index){
    //如果为文件夹，则展开文件夹
    if(fileModel->isDir(index)){
        listView->setRootIndex(index);
        treeView->expand(index);
        treeView->setCurrentIndex(index);
    }
}
