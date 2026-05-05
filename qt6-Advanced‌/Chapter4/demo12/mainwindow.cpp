#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(300,300,620,300);
    //使用信号/槽
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(action_close()));
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(comboBox_changed(QString)));
    connect(ui->pushButton_deleteNum,SIGNAL(clicked()),this,SLOT(pushButton_num()));
    connect(ui->pushButton_deleteCur,SIGNAL(clicked()),this,SLOT(pushButton_cur()));
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
    ui->comboBox->clear();
    //连接数据库
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open()){
        tableModel=new QSqlTableModel(this,db);//数据库表格模型
        tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tableView->setModel(tableModel);
        QStringList tables=db.tables();
        if(tables.size()>0)
            ui->comboBox->addItems(tables);
    }
}

void MainWindow::comboBox_changed(QString text){
    tableModel->setTable(text);
    tableModel->select();
    //获取头部字段的记录
    QSqlRecord header=tableModel->record();
    for(int i=0;i<header.count();i++)
        tableModel->setHeaderData(i,Qt::Horizontal,header.fieldName(i),Qt::DisplayRole);
}

void MainWindow::action_close(){
    if(db.isOpen()){
        db.close();
        tableModel->clear();
        ui->comboBox->clear();
    }
}

void MainWindow::pushButton_num(){
    int row=ui->spinBox->value();
    if(row>0 && row<=tableModel->rowCount()){
        if(tableModel->removeRow(row-1))//删除行
            tableModel->select();       //重新查询数据
    }
}

void MainWindow::pushButton_cur(){
    int currentRow=ui->tableView->currentIndex().row();
    if(tableModel->removeRow(currentRow))//删除行
        tableModel->select();            //重新查询数据
}
