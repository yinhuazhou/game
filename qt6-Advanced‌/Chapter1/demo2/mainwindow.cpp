#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QFile、QByteArray");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开十六进制文件");
    actionSave=fileMenu->addAction("保存十六进制文件");
    setMenuBar(menuBar);
    //创建多行纯文本控件
    plainText=new QPlainTextEdit();
    setCentralWidget(plainText);
    //创建状态栏
    status=statusBar();
    //使用信号/槽
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(action_save()));
}

MainWindow::~MainWindow() {}

void MainWindow::action_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Hex文件(*.hex);;所有文件(*.*)";
    QString title="打开Hex文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    plainText->clear();
    if(file.exists()==false)
        return;
    if(!file.open(QIODevice::ReadOnly))
        return;
    while(!file.atEnd()){
        QByteArray line=file.readLine();//逐行读取数据
        QByteArray byte=QByteArray::fromHex(line);//从16进制数据中解码
        QString str=QString(byte);//将字节数组对象转换为字符串
        plainText->appendPlainText(str);
}
    file.close();
    status->showMessage("打开文件成功！");
}

void MainWindow::action_save(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Hex文件(*.hex);;所有文件(*.*)";
    QString title="保存Hex文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QString str=plainText->toPlainText();//获取纯文本框中的内容
    QByteArray byte=str.toUtf8();//将字符串转换为字节数组对象
    QByteArray hex=byte.toHex(); //转换为hex编码的字节数组对象
    file.write(hex);
    file.close();
    status->showMessage("保存文件成功！");
}
