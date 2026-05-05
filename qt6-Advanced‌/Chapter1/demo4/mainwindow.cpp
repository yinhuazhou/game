#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QFile、QDataStream");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开二进制文件");
    actionSave=fileMenu->addAction("保存二进制文件");
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
    QString filter="二进制文件(*.bin);;所有文件(*.*)";
    QString title="打开二进制文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    plainText->clear();
    if(file.exists()==false)
        return;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QDataStream reader(&file);
    reader.setVersion(QDataStream::Qt_6_6);
    reader.setByteOrder(QDataStream::BigEndian);
    QString value;
    while(reader.atEnd()==false){
        reader>>value;      //读取数据流中的数据
        plainText->appendPlainText(value);
    }
    file.close();
    status->showMessage("打开文件成功！");
}

void MainWindow::action_save(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="二进制文件(*.bin);;所有文件(*.*)";
    QString title="保存二进制文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QString str=plainText->toPlainText();//获取纯文本框中的内容
    QDataStream writer(&file);
    writer.setVersion(QDataStream::Qt_6_6);
    writer.setByteOrder(QDataStream::BigEndian);
    writer<<str;        //向数据流中写入数据
    file.close();
    status->showMessage("文件保存成功！");
}
