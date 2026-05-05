#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(200,100,580,280);
    //创建水平布局对象，添加4个按钮
    btnListen=new QPushButton("开始监听");
    btnStop=new QPushButton("停止监听");
    btnClear=new QPushButton("清空文本框");
    btnExit=new QPushButton("退出");
    hbox1=new QHBoxLayout();
    hbox1->addWidget(btnListen);
    hbox1->addWidget(btnStop);
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
    //创建2个标签控件
    labelState=new QLabel("监听状态：");
    labelSocket=new QLabel("Socket状态：");
    hbox4=new QHBoxLayout();
    hbox4->addWidget(labelState);
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
    //创建TCP服务器对象
    tcpServer=new QTcpServer();
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(do_newConnection()));
    //使用信号/槽
    connect(btnListen,SIGNAL(clicked()),this,SLOT(btn_listen()));
    connect(btnStop,SIGNAL(clicked()),this,SLOT(btn_stop()));
    connect(btnSend,SIGNAL(clicked()),this,SLOT(btn_send()));
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
//开始监听
void Widget::btn_listen(){
    QString ip=lineIP->text();
    if(ip=="")
        return;
    QHostAddress address(ip);
    int port=spinPort->value();
    tcpServer->listen(address,port);
    textEdit->appendPlainText("**开始监听**");
    QString serverAddress=tcpServer->serverAddress().toString();
    textEdit->appendPlainText("**服务器地址："+serverAddress);
    QString serverPort=QString::number(tcpServer->serverPort());
    textEdit->appendPlainText("**服务器端口："+serverPort);
    btnListen->setEnabled(false);
    labelState->setText("监听状态：正在监听");
}
//建立连接
void Widget::do_newConnection(){
    tcpSocket=tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(do_clientConnected()));
    do_clientConnected();
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(do_clientDisConnected()));
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(do_socketStateChanged(QAbstractSocket::SocketState)));
    do_socketStateChanged(tcpSocket->state());
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(do_socketReadyRead()));
}

void Widget::do_clientConnected(){
    textEdit->appendPlainText("**client socket connectd");
    QString peerAddr=tcpSocket->peerAddress().toString();
    textEdit->appendPlainText("**peer address:"+peerAddr);
    QString peerPort=QString::number(tcpSocket->peerPort());
    textEdit->appendPlainText("**peer port:"+peerPort);
}

void Widget::do_clientDisConnected(){
    textEdit->appendPlainText("**client socket disconnectd");
    tcpSocket->abort();
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
//读取传输数据
void Widget::do_socketReadyRead(){
    if(tcpSocket->isReadable())
        textEdit->appendPlainText("[In] "+tcpSocket->readLine());
}
//停止监听
void Widget::btn_stop(){
    if(tcpServer->isListening()){
        if(tcpSocket==nullptr)
            return;
        if(tcpSocket->state()==QAbstractSocket::ConnectedState)
            tcpSocket->disconnectFromHost();
        tcpServer->close();
        btnListen->setEnabled(true);
        btnStop->setEnabled(false);
        labelState->setText("监听状态：已停止监听");
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
    tcpSocket->write(byteArray);
}
//清空文本框
void Widget::btn_clear(){
    textEdit->clear();
}


