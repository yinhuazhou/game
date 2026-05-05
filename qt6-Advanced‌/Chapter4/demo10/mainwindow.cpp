#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,620,300);
    setWindowTitle("QSqlQueryModel");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开数据库");
    actionClose=fileMenu->addAction("关闭数据库");
    //创建标签、下拉列表、表格视图、框架控件
    label=new QLabel("请选择要显示的数据表：");
    label->setFont(QFont("黑体",12));
    label->setAlignment(Qt::AlignCenter);
    combox=new QComboBox();
    tableView=new QTableView();
    frame=new QFrame();
    //向框架控件中添加其他控件
    hbox=new QHBoxLayout();
    hbox->addWidget(label);
    hbox->addWidget(combox);
    vbox=new QVBoxLayout(frame);
    vbox->addLayout(hbox);
    vbox->addWidget(tableView);
    //设置主菜单的菜单栏、中心控件
    setMenuBar(menuBar);
    setCentralWidget(frame);
    //创建模型
    queryModel=new QSqlQueryModel();
    //使用信号/槽
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(actionClose,SIGNAL(triggered()),this,SLOT(action_close()));
    connect(combox,SIGNAL(currentIndexChanged(int)),this,SLOT(combox_changed()));
}

MainWindow::~MainWindow() {}

void MainWindow::action_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="SQLite(*.db *.db3);;所有文件(*.*)";
    QString title="打开SQLite数据库";//文件对话框的标题
    QString dbName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(dbName.isEmpty())
        return;
    setWindowTitle(dbName);
    combox->clear();
    //连接数据库
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open()){
        QStringList tables=db.tables();
        if(tables.size()>0)
            combox->addItems(tables);
    }
}

void MainWindow::action_close(){
    if(db.isOpen()){
        db.close();
        queryModel->clear();
        combox->clear();
    }
}

void MainWindow::combox_changed(){
    QString text=combox->currentText();
    QString sql="SELECT * FROM "+text;
    //设置查询
    queryModel->setQuery(sql,db);
    //获取字段头部的记录
    QSqlRecord header=queryModel->record();
    for(int i=0;i<header.count();i++)
        queryModel->setHeaderData(i,Qt::Horizontal,header.fieldName(i),Qt::DisplayRole);
    tableView->setModel(queryModel);
}
