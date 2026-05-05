#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QMediaDevices");
    //创建水平布局对象，添加3个按钮
    hbox=new QHBoxLayout();
    btnCamera=new QPushButton("摄像头信息");
    btnInputs=new QPushButton("声频输入设备信息");
    btnOutputs=new QPushButton("声频输出设备信息");
    hbox->addWidget(btnCamera);
    hbox->addWidget(btnInputs);
    hbox->addWidget(btnOutputs);
    //创建纯文本输入控件
    textEdit=new QPlainTextEdit();
    //创建垂直布局对象，添加其他布局、控件
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(btnCamera,SIGNAL(clicked()),this,SLOT(btn_camera()));
    connect(btnInputs,SIGNAL(clicked()),this,SLOT(btn_inputs()));
    connect(btnOutputs,SIGNAL(clicked()),this,SLOT(btn_outputs()));
}

Widget::~Widget() {}

void Widget::btn_camera(){
    QList<QCameraDevice> cList=QMediaDevices::videoInputs();
    for(int i=0;i<cList.size();i++){
        textEdit->appendPlainText("摄像头："+cList[i].description());
    }
}

void Widget::btn_inputs(){
    QList<QAudioDevice> iList=QMediaDevices::audioInputs();
    for(int i=0;i<iList.size();i++){
        textEdit->appendPlainText("声频输入设备："+iList[i].description());
    }
}

void Widget::btn_outputs(){
    QList<QAudioDevice> oList=QMediaDevices::audioOutputs();
    for(int i=0;i<oList.size();i++){
        textEdit->appendPlainText("声频输出设备："+oList[i].description());
    }
}
