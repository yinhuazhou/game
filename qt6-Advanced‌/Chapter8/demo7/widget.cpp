#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(550,200,580,280);
    //创建水平布局对象，添加4个按钮
    btnBind=new QPushButton("绑定端口");
    btnEnd=new QPushButton("解除绑定");
    btnClear=new QPushButton("清空文本框");
    btnExit=new QPushButton("退出");
    hbox1=new QHBoxLayout();
    hbox1->addWidget(btnBind);
    hbox1->addWidget(btnEnd);
    hbox1->addWidget(btnClear);
    hbox1->addWidget(btnExit);
    //创建3个标签、1个单行文本框、2个数字输入控件
    labelPort=new QLabel("绑定端口：");
    port=new QSpinBox();
    port->setRange(1200,99999);
    labelTarget=new QLabel("目标地址：");
    lineTarget=new QLineEdit();
    lineTarget->setText("127.0.0.1");
    labelTargetPort=new QLabel("目标端口：");
    targetPort=new QSpinBox();
    targetPort->setRange(1200,99999);
    hbox2=new QHBoxLayout();
    hbox2->addWidget(labelPort);
    hbox2->addWidget(port);
    hbox2->addWidget(labelTarget);
    hbox2->addWidget(lineTarget);
    hbox2->addWidget(labelTargetPort);
    hbox2->addWidget(targetPort);
    //创建1个单行文本框、2个按钮
    lineMsg=new QLineEdit();
    btnUnicast=new QPushButton("发送信息");
    btnBroadcast=new QPushButton("广播信息");
    hbox3=new QHBoxLayout();
    hbox3->addWidget(lineMsg);
    hbox3->addWidget(btnUnicast);
    hbox3->addWidget(btnBroadcast);
    //创建1个多行纯文本框
    textEdit=new QPlainTextEdit();
    //创建1个标签控件
    labelSocket=new QLabel("Socket状态：");
    hbox4=new QHBoxLayout();
    hbox4->addWidget(labelSocket);
    //设置主窗口的布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addLayout(hbox3);
    vbox->addWidget(textEdit);
    vbox->addLayout(hbox4);
    QString localIP=get_localIP();
    setWindowTitle("本地IP地址："+localIP);
    //创建套接字
    udpSocket=new QUdpSocket();
    //使用信号/槽
    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(do_socketStateChanged(QAbstractSocket::SocketState)));
    do_socketStateChanged(udpSocket->state());//调用一次槽函数
    connect(btnBind,SIGNAL(clicked()),this,SLOT(btn_bind()));
    connect(btnUnicast,SIGNAL(clicked()),this,SLOT(btn_unicast()));
    connect(btnBroadcast,SIGNAL(clicked()),this,SLOT(btn_broadcast()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(do_socketReadyRead()));
    connect(btnEnd,SIGNAL(clicked()),this,SLOT(btn_end()));
    connect(btnClear,SIGNAL(clicked()),this,SLOT(btn_clear()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(close()));
}

Widget::~Widget() {}

QString Widget::get_localIP(){
    QString name=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(name);
    QList<QHostAddress> ipList=hostInfo.addresses();
    for(int i=0;i<ipList.size();i++){
        if(ipList[i].protocol()==QAbstractSocket::IPv4Protocol){
            QString localIP=ipList[i].toString();
            return localIP;
        }
    }
}

void Widget::do_socketStateChanged(QAbstractSocket::SocketState state){
    if(state==QAbstractSocket::UnconnectedState){
        labelSocket->setText("Socket状态：UnconnectedState");
        return;
    }
    else if(state==QAbstractSocket::HostLookupState){
        labelSocket->setText("Socket状态：HostLookupState");
        return;
    }
    else if(state==QAbstractSocket::ConnectingState){
        labelSocket->setText("Socket状态：ConnectingState");
        return;
    }
    else if(state==QAbstractSocket::ConnectedState){
        labelSocket->setText("Socket状态：ConnectedState");
        return;
    }
    else if(state==QAbstractSocket::BoundState){
        labelSocket->setText("Socket状态：BoundState");
        return;
    }
    else if(state==QAbstractSocket::ClosingState){
        labelSocket->setText("Socket状态：ClosingState");
        return;
    }
    else if(state==QAbstractSocket::ListeningState){
        labelSocket->setText("Socket状态：ListeningState");
        return;
    }
}
//绑定端口
void Widget::btn_bind(){
    int port1=port->value();
    if(udpSocket->bind(port1)){
        textEdit->appendPlainText("**已成功绑定");
        int port2=udpSocket->localPort();
        QString str=QString::number(port2);
        textEdit->appendPlainText("绑定端口："+str);
        btnBind->setEnabled(false);
        btnEnd->setEnabled(true);
        btnUnicast->setEnabled(true);
        btnBroadcast->setEnabled(true);
    }
}
//发送信息
void Widget::btn_unicast(){
    QString targetIP=lineTarget->text();
    QHostAddress targetAddress(targetIP);
    quint16 targetPort1=targetPort->value();
    QString msg=lineMsg->text();
    if(msg=="")
        return;
    QByteArray byte=msg.toUtf8();
    udpSocket->writeDatagram(byte,targetAddress,targetPort1);
    textEdit->appendPlainText("[out] "+msg);
    lineMsg->clear();
    lineMsg->setFocus();
}

void Widget::do_socketReadyRead(){
    if(udpSocket->hasPendingDatagrams()==false)
        return;
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());//等待读取的数据报的字节数
    QHostAddress peerAddr;//对方地址
    quint16 peerPort;//对方端口
    udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
    QString str=datagram.data();
    QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"]";
    textEdit->appendPlainText(peer+str);
}
//广播信息
void Widget::btn_broadcast(){
    quint16 targetPort1=targetPort->value();
    QString msg=lineMsg->text();
    QByteArray byte=msg.toUtf8();
    udpSocket->writeDatagram(byte,QHostAddress::Broadcast,targetPort1);
    textEdit->appendPlainText("[broadcast] "+msg);
    lineMsg->clear();
    lineMsg->setFocus();
}
//解除绑定
void Widget::btn_end(){
    udpSocket->abort();
    btnBind->setEnabled(true);
    btnEnd->setEnabled(false);
    btnUnicast->setEnabled(false);
    btnBroadcast->setEnabled(false);
    textEdit->appendPlainText("**已解除绑定");
}
//清空文本框
void Widget::btn_clear(){
    textEdit->clear();
}
