#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QFile、QByteArray");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开文本文件");
    actionSave=fileMenu->addAction("保存文本文件");
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
    QString filter="文本文件(*.txt);;程序文件(*.h *.cpp);;所有文件(*.*)";
    QString title="打开文本文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    plainText->clear();
    if(file.exists()==false)
        return;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QByteArray allLines=file.readAll();//读取文件的全部内容
    QString text(allLines);
    file.close();
    plainText->setPlainText(text);
    status->showMessage("打开文件成功！");
}

void MainWindow::action_save(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="文本文件(*.txt);;程序文件(*.h *.cpp);;所有文件(*.*)";
    QString title="保存文本文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QString str=plainText->toPlainText();//获取纯文本框中的内容
    QByteArray bytes=str.toUtf8();//转换为字节数组对象，采用UTF-8编码
    file.write(bytes,bytes.length());//将数据写入文件
    file.close();
    status->showMessage("文件保存成功！");
}
