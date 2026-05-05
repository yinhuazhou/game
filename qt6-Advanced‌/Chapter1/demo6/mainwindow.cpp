#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QDir");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开路径");
    actionClose=fileMenu->addAction("关闭");
    setMenuBar(menuBar);
    //创建多行纯文本控件
    plainText=new QPlainTextEdit();
    setCentralWidget(plainText);
    //创建状态栏
    status=statusBar();
    //使用信号/槽
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(actionClose,SIGNAL(triggered()),this,SLOT(close()));
}

MainWindow::~MainWindow() {}

void MainWindow::action_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString caption="选择路径";
    QString path=QFileDialog::getExistingDirectory(this,caption,curPath);
    if(path.isEmpty())
        return;
    QDir dir1(path);
    dir1.setFilter(QDir::Files);//只显示文件
    if(dir1.exists()==false)
        return;
    QFileInfoList infoList=dir1.entryInfoList();//获取文件信息列表
    int num=infoList.count();//获取文件数量
    if(num==0)
        return;
    QString msg="选择路径："+QDir::toNativeSeparators(path)+",该路径下有"+QString::number(num)+"个文件";
    status->showMessage(msg);//状态栏显示信息
    plainText->clear();
    QString line1=QDir::toNativeSeparators(path)+"下的文件如下：";
    plainText->appendPlainText(line1);
    for(int i=0;i<infoList.size();i++){
        QFileInfo info=infoList.at(i);
        QString name=info.fileName();
        int size=info.size();
        QString sizeStr=QString::number(size);
        QString birthTime=info.birthTime().toString();
        QString modifiedTime=info.lastModified().toString();
        QString string="文件名："+name+"，文件大小："+sizeStr+"，创建日期："+birthTime+"，修改日期："+modifiedTime;
        plainText->appendPlainText(string);
    }
}
