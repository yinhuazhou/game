#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QBuffer");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionCreate=fileMenu->addAction("生成数据");
    actionShow=fileMenu->addAction("显示数据");
    setMenuBar(menuBar);
    //创建多行纯文本控件
    plainText=new QPlainTextEdit();
    setCentralWidget(plainText);
    //创建状态栏
    status=statusBar();
    //创建缓存对象
    buffer=new QBuffer(this);
    //使用信号/槽
    connect(actionCreate,SIGNAL(triggered()),this,SLOT(action_create()));
    connect(actionShow,SIGNAL(triggered()),this,SLOT(action_show()));
}

MainWindow::~MainWindow() {}

void MainWindow::action_create(){
    if(buffer->open(QIODevice::WriteOnly)==false)
        return;
    QDataStream writer(buffer);
    writer.setVersion(QDataStream::Qt_6_6);
    writer.setByteOrder(QDataStream::BigEndian);
    writer<<QString("昨夜江边春水生，");
    writer<<QString("艨艟巨舰一毛轻。");
    writer<<QString("向来枉费推移力，");
    writer<<QString("此日中流自在行。");
    status->showMessage("写入数据成功！");
    buffer->close();
}

void MainWindow::action_show(){
    if(buffer->open(QIODevice::ReadOnly)){
        QDataStream reader(buffer);
        reader.setVersion(QDataStream::Qt_6_6);
        reader.setByteOrder(QDataStream::BigEndian);
        plainText->clear();
        QString value;
        while(!reader.atEnd()){
            reader>>value;
         plainText->appendPlainText(value);
         status->showMessage("读取数据成功！");
        }
    }
    buffer->close();
}
