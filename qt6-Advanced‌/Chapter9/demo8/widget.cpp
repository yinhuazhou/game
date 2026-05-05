#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,260);
    setWindowTitle("QMediaDevices、QCamera");
    //创建垂直布局对象，添加1个按钮、1个纯文本框
    vbox=new QVBoxLayout(this);
    btnGet=new QPushButton("获取摄像头的特性");
    plainText=new QPlainTextEdit();
    vbox->addWidget(btnGet);
    vbox->addWidget(plainText);
    //使用信号/槽
    connect(btnGet,SIGNAL(clicked()),this,SLOT(btn_get()));
}

Widget::~Widget() {}

void Widget::btn_get(){
    QList<QCameraDevice> cList=QMediaDevices::videoInputs();
    QCamera camera(cList[0]);
    QMetaEnum enum1=QMetaEnum::fromType<QCamera::FlashMode>();
    QString str1=enum1.valueToKey(camera.flashMode());
    plainText->appendPlainText("闪光模式："+str1);
    QMetaEnum enum2=QMetaEnum::fromType<QCamera::ExposureMode>();
    QString str2=enum2.valueToKey(camera.exposureMode());
    plainText->appendPlainText("曝光模式："+str2);
    QString str3=QString::number(camera.exposureCompensation());
    plainText->appendPlainText("曝光补偿："+str3);

    QString str4=QString::number(camera.colorTemperature());
    plainText->appendPlainText("色温："+str4);
    QMetaEnum enum5=QMetaEnum::fromType<QCamera::FocusMode>();
    QString str5=enum5.valueToKey(camera.focusMode());
    plainText->appendPlainText("对焦模式："+str5);
    bool isActive=camera.isActive();
    QString str6=QString(isActive?"true":"false");
    plainText->appendPlainText("活跃状态："+str6);
}
