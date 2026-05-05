#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(300,300,620,320);
    queryModel1=new QSqlQueryModel(this);
    queryModel2=new QSqlQueryModel(this);
    //使用信号/槽
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(action_close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::action_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="SQLite(*.db *.db3);;所有文件(*.*)";
    QString title="打开SQLite数据库";//文件对话框的标题
    QString dbName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(dbName.isEmpty())
        return;
    setWindowTitle(dbName);
    //连接数据库
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open()){
        queryModel1->setQuery("SELECT * FROM score1");
        ui->tableView_1->setModel(queryModel1);
        queryModel2->setQuery("SELECT * FROM score2");
        ui->tableView_2->setModel(queryModel2);
        //创建关系表格模型
        relationModel=new QSqlRelationalTableModel(this,db);
        relationModel->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
        //设置内连接模式
        relationModel->setJoinMode(QSqlRelationalTableModel::InnerJoin);
        relationModel->setTable("score1");
        //设置映射关系
        relationModel->setRelation(3,QSqlRelation("score2","ID","数学"));
        relationModel->setRelation(4,QSqlRelation("score2","ID","英文"));
        ui->tableView_3->setModel(relationModel);
        //重新查询数据
        relationModel->select();
    }
}

void MainWindow::action_close(){
    if(db.isOpen()){
        db.close();
        queryModel1->clear();
        queryModel2->clear();
        relationModel->clear();
    }
}
