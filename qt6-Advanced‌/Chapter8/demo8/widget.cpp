#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(550,200,580,280);
    //创建水平布局对象，添加4个按钮
    btnStart=new QPushButton("加入组播");
    btnEnd=new QPushButton("退出组播");
    btnClear=new QPushButton("清空文本框");
    btnExit=new QPushButton("退出");
    hbox1=new QHBoxLayout();
    hbox1->addWidget(btnStart);
    hbox1->addWidget(btnEnd);
    hbox1->addWidget(btnClear);
    hbox1->addWidget(btnExit);
    //创建2个标签、1个单行文本框、1个数字输入控件
    labelPort=new QLabel("组播端口：");
    port=new QSpinBox();
    port->setRange(33331,99999);
    labelIP=new QLabel("目标地址：");
    lineIP=new QLineEdit();
    lineIP->setText("224.0.1.22");
     hbox2=new QHBoxLayout();
    hbox2->addWidget(labelPort);
    hbox2->addWidget(port);
    hbox2->addWidget(labelIP);
    hbox2->addWidget(lineIP);
     //创建1个单行文本框、1个按钮
    lineMsg=new QLineEdit();
    btnMulticast=new QPushButton("组播信息");
    hbox3=new QHBoxLayout();
    hbox3->addWidget(lineMsg);
    hbox3->addWidget(btnMulticast);
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
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);
    //使用信号/槽
    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(do_socketStateChanged(QAbstractSocket::SocketState)));
    do_socketStateChanged(udpSocket->state());//调用一次槽函数
    connect(btnStart,SIGNAL(clicked()),this,SLOT(btn_start()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(do_socketReadyRead()));
    connect(btnMulticast,SIGNAL(clicked()),this,SLOT(btn_multicast()));
    connect(btnEnd,SIGNAL(clicked()),this,SLOT(btn_end()));
    connect(btnClear,SIGNAL(clicked()),this,SLOT(btn_clear()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(close()));
}

Widget::~Widget() {}
//获取本机IP地址
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
//Socket状态改变
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
//加入组播
void Widget::btn_start(){
    QString ip=lineIP->text();
    groupAddress.setAddress(ip);//多播组地址
    int groupPort=port->value();
    bool isConnected=udpSocket->bind(QHostAddress::AnyIPv4,groupPort,QUdpSocket::ShareAddress);
    if(isConnected==true){
        udpSocket->joinMulticastGroup(groupAddress);
        textEdit->appendPlainText("**加入组播成功");
        textEdit->appendPlainText("**组播IP地址："+ip);
        textEdit->appendPlainText("**绑定端口："+QString::number(groupPort));
        btnStart->setEnabled(false);
        btnEnd->setEnabled(true);
        port->setEnabled(false);
        lineIP->setEnabled(false);
        btnMulticast->setEnabled(true);
    }
    else{
        textEdit->appendPlainText("**绑定端口失败");
    }
}
//读取、显示传送的信息
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
//组播信息
void Widget::btn_multicast(){
    int groupPort=port->value();
    QString msg=lineMsg->text();
    if(msg=="")
        return;
    QByteArray datagram=msg.toUtf8();
    udpSocket->writeDatagram(datagram,groupAddress,groupPort);
    textEdit->appendPlainText("[multicast] "+msg);
    lineMsg->clear();
    lineMsg->setFocus();
}
//退出组播
void Widget::btn_end(){
    udpSocket->leaveMulticastGroup(groupAddress);
    udpSocket->abort();
    btnStart->setEnabled(true);
    btnEnd->setEnabled(false);
    port->setEnabled(true);
    lineIP->setEnabled(true);
    btnMulticast->setEnabled(false);
    textEdit->appendPlainText("**已退出组播，解除端口绑定");
}
//清空文本框
void Widget::btn_clear(){
    textEdit->clear();
}
