#include "pixmapwidget.h"

PixmapWidget::PixmapWidget(QWidget *parent):QWidget{parent}
{
    resize(580,280);
    setWindowTitle("代理控件内的窗口");
    //设置窗口的布局为垂直布局
    vbox=new QVBoxLayout(this);
    //创建标签
    label=new QLabel();
    //创建按钮
    btn=new QPushButton("选择图像文件");
    vbox->addWidget(label);
    vbox->addWidget(btn);
    //使用信号/槽
    connect(btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));
}

void PixmapWidget::btn_clicked(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="图像文件(*.png *.bmp *.jpg *.jpeg);;所有文件(*.*)";
    QString title="打开图像文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QPixmap pix1(fileName);
    QPixmap pix2=pix1.scaled(580,280);
    label->setPixmap(pix2);
}
