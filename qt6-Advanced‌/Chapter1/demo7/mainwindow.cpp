#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QFileSystemWatcher");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("添加被监视的文件路径");
    actionClose=fileMenu->addAction("关闭");
    setMenuBar(menuBar);
    //创建多行纯文本控件
    plainText=new QPlainTextEdit();
    setCentralWidget(plainText);
    //创建状态栏
    status=statusBar();
    //创建文件和路径监视器
    watcher=new QFileSystemWatcher(this);
    //使用信号/槽
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(file_changed(QString)));
}

MainWindow::~MainWindow() {}

void MainWindow::action_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString caption="选择文件";
    QString filter="所有文件(*.*)";
    QString fileName=QFileDialog::getOpenFileName(this,caption,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFileInfo info(fileName);
    if(info.isFile()==false)
        return;
    watcher->addPath(fileName);
    status->showMessage("添加文件路径成功");
}

void MainWindow::file_changed(QString fileName){
    QString str="路径为"+fileName+"的文件被修改";
    plainText->appendPlainText(str);
}
