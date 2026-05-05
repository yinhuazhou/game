#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,620,300);
    setWindowTitle("显示图像文件");
    hbox=new QHBoxLayout(this);
    //创建系统文件模型
    fileModel=new QFileSystemModel(this);
    //设置根路径
    fileModel->setRootPath("C:\\");
    //创建树视图控件
    treeView=new QTreeView();
    //设置模型
    treeView->setModel(fileModel);
    //创建自定义的框架控件
    frame=new MyFrame();
    //创建分割器
    hSplitter=new QSplitter(Qt::Horizontal);
    //向分割器中添加控件
    hSplitter->addWidget(treeView);
    hSplitter->addWidget(frame);
    hbox->addWidget(hSplitter);
    //使用信号/槽
    connect(treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(view_clicked(QModelIndex)));
}

Widget::~Widget() {}

void Widget::view_clicked(QModelIndex index){
    //如果为文件夹，则展开文件夹，否则传递文件路径
    if(fileModel->isDir(index)){
        treeView->expand(index);
        treeView->setCurrentIndex(index);
    }
    else{
        QString path=fileModel->filePath(index);
        frame->setPath(path);
        frame->update();//刷新屏幕，绘制图片
    }
}
