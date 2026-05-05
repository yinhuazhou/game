#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QHostInfo");
    //创建水平布局，添加控件
    btnName=new QPushButton("获取主机名称");
    lineEdit=new QLineEdit();
    hbox=new QHBoxLayout();
    hbox->addWidget(btnName);
    hbox->addWidget(lineEdit);
    //创建垂直布局，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    textEdit=new QPlainTextEdit();
    btnInfo=new QPushButton("获取主机的IP地址");
    vbox->addLayout(hbox);
    vbox->addWidget(btnInfo);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(btnName,SIGNAL(clicked()),this,SLOT(btn_name()));
    connect(btnInfo,SIGNAL(clicked()),this,SLOT(btn_info()));
}

Widget::~Widget() {}

void Widget::btn_name(){
    QString name=QHostInfo::localHostName();
    lineEdit->setText(name);
}

void Widget::btn_info(){
    textEdit->clear();
    QString name=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(name);
    QList<QHostAddress> adList=hostInfo.addresses();
    for(int i=0;i<adList.count();i++){
        QMetaEnum metaEum=QMetaEnum::fromType<QAbstractSocket::NetworkLayerProtocol>();
        QString str=metaEum.valueToKey(adList[i].protocol());//将枚举常量转换为字符串
        textEdit->appendPlainText("协议："+str);
        textEdit->appendPlainText("本机IP地址："+adList[i].toString());
    }
}
