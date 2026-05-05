#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QHostInfo");
    //创建水平布局对象，添加控件
    label=new QLabel("输入主机名：");
    lineEdit=new QLineEdit();
    hbox=new QHBoxLayout();
    hbox->addWidget(label);
    hbox->addWidget(lineEdit);
    //创建垂直布局对象，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    btnInfo=new QPushButton("获取主机的IP地址");
    textEdit=new QPlainTextEdit();
    vbox->addLayout(hbox);
    vbox->addWidget(btnInfo);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(btnInfo,SIGNAL(clicked()),this,SLOT(btn_info()));
}

Widget::~Widget() {}

void Widget::btn_info(){
    QString name=lineEdit->text();
    if(name=="")
        return;
    QHostInfo::lookupHost(name,this,lookedUp);
}

void Widget::lookedUp(QHostInfo host){
    textEdit->clear();
    if(host.error()!=QHostInfo::NoError){
        textEdit->appendPlainText("Lookup failed:"+host.errorString());
        return;
    }
    QList<QHostAddress> adList=host.addresses();
    for(int i=0;i<adList.count();i++){
        textEdit->appendPlainText("Found address:"+adList[i].toString());
    }
}
