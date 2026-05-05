#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(550,200,580,280);
    //创建水平布局对象，添加4个按钮
    btnStart=new QPushButton("连接服务器");
    btnEnd=new QPushButton("断开服务器");
    btnClear=new QPushButton("清空文本框");
    btnExit=new QPushButton("退出");
    hbox1=new QHBoxLayout();
    hbox1->addWidget(btnStart);
    hbox1->addWidget(btnEnd);
    hbox1->addWidget(btnClear);
    hbox1->addWidget(btnExit);
    //创建2个标签、1个单行文本框、1个数字输入控件
    labelIP=new QLabel("监听地址：");
    lineIP=new QLineEdit();
    lineIP->setText("127.0.0.1");
    labelPort=new QLabel("监听端口：");
    spinPort=new QSpinBox();
    spinPort->setRange(1200,99999);
    hbox2=new QHBoxLayout();
    hbox2->addWidget(labelIP);
    hbox2->addWidget(lineIP);
    hbox2->addWidget(labelPort);
    hbox2->addWidget(spinPort);
    //创建1个单行文本框、1个按钮
    lineMsg=new QLineEdit();
    btnSend=new QPushButton("发送信息");
    hbox3=new QHBoxLayout();
    hbox3->addWidget(lineMsg);
    hbox3->addWidget(btnSend);
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
    tcpClient=new QTcpSocket();
    //使用信号/槽
    connect(btnStart,SIGNAL(clicked()),this,SLOT(btn_start()));
    connect(tcpClient,SIGNAL(connected()),this,SLOT(do_connected()));
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(do_disConnected()));
    connect(tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(do_socketStateChanged(QAbstractSocket::SocketState)));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(do_socketReadyRead()));
    connect(btnSend,SIGNAL(clicked()),this,SLOT(btn_send()));
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
//开始连接
void Widget::btn_start(){
    QString ip=lineIP->text();
    if(ip=="")
        return;
    QHostAddress address(ip);
    int port=spinPort->value();
    tcpClient->connectToHost(address,port);
}

void Widget::do_connected(){
    textEdit->appendPlainText("**已连接到服务器**");
    QString peerAddr=tcpClient->peerAddress().toString();
    textEdit->appendPlainText("**peer address:"+peerAddr);
    QString peerPort=QString::number(tcpClient->peerPort());
    textEdit->appendPlainText("**peer port:"+peerPort);
    btnStart->setEnabled(false);
    btnEnd->setEnabled(true);
}

void Widget::do_disConnected(){
    textEdit->appendPlainText("**已断开与服务器的连接");
    btnStart->setEnabled(true);
    btnEnd->setEnabled(false);
}
//读取传输数据
void Widget::do_socketReadyRead(){
    if(tcpClient->isReadable())
        textEdit->appendPlainText("[In] "+tcpClient->readLine());
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
//发送数据
void Widget::btn_send(){
    QString msg=lineMsg->text();
    if(msg=="")
        return;
    textEdit->appendPlainText("[Out] "+msg);
    lineMsg->clear();
    lineMsg->setFocus();
    QByteArray byteArray=msg.toUtf8();
    tcpClient->write(byteArray);
}
//清空文本框
void Widget::btn_clear(){
    textEdit->clear();
}
//断开服务器
void Widget::btn_end(){
    if(tcpClient->state()==QAbstractSocket::ConnectedState)
        tcpClient->disconnectFromHost();
    btnStart->setEnabled(true);
    btnEnd->setEnabled(false);
}

