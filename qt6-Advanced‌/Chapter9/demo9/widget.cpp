#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,300);
    setWindowTitle("QCamera、QImageCapture");
    //创建媒体捕获器对象
    session=new QMediaCaptureSession(this);
    //创建显示视频的控件
    videoWidget=new QVideoWidget();
    session->setVideoOutput(videoWidget);//设置显示视频的控件

    QList<QCameraDevice> cList=QMediaDevices::videoInputs();
    camera=new QCamera(cList[0]);
    session->setCamera(camera);

    imageCapture=new QImageCapture(this);//创建摄像头拍照对象
    imageCapture->setQuality(QImageCapture::HighQuality);
    session->setImageCapture(imageCapture);

    //创建水平布局对象，添加4个按钮
    hbox=new QHBoxLayout();
    btnStart=new QPushButton("开启摄像头");
    btnClose=new QPushButton("关闭摄像头");
    btnCapture=new QPushButton("拍摄照片");
    btnExit=new QPushButton("退出");
    hbox->addWidget(btnStart);
    hbox->addWidget(btnClose);
    hbox->addWidget(btnCapture);
    hbox->addWidget(btnExit);
    //创建底部的标签控件
    labelTip=new QLabel();
    labelTip->setFixedHeight(10);
    //创建垂直布局，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox);
    vbox->addWidget(videoWidget);
    vbox->addWidget(labelTip);
    //使用信号/槽
    connect(btnStart,SIGNAL(clicked()),this,SLOT(btn_start()));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(btn_close()));
    connect(btnCapture,SIGNAL(clicked()),this,SLOT(btn_capture()));
    connect(imageCapture,SIGNAL(imageSaved(int,QString)),this,SLOT(do_imageSaved(int,QString)));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(close()));
}

Widget::~Widget() {}
//打开摄像头
void Widget::btn_start(){
    camera->start();
}
//关闭摄像头
void Widget::btn_close(){
    camera->stop();
}
//拍摄照片
void Widget::btn_capture(){
    if(imageCapture->isReadyForCapture()){
        imageCapture->captureToFile();
    }
    else{
        return;
    }
}
//保存图片时连接的槽函数
void Widget::do_imageSaved(int id, QString fileName){
    QString str1="保存图片为"+fileName;
    labelTip->setText(str1);
    Q_UNUSED(id);//没有使用参数id
}
