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
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushButton_clicked()));
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

void MainWindow::pushButton_clicked(){
    //创建记录对象
    QSqlRecord record=QSqlRecord(tableModel->record());
    int id=ui->spinBox_id->value();
    QString name=ui->lineEdit_name->text();
    float chinese=ui->doubleSpinBox_chinese->value();
    float math=ui->doubleSpinBox_math->value();
    float english=ui->doubleSpinBox_english->value();
    if(id<0 || name.isEmpty())
        return;
    //设置记录对象的值
    record.setValue("ID",id);
    record.setValue("Name",name);
    record.setValue("语文",chinese);
    record.setValue("数学",math);
    record.setValue("英文",english);
    ui->spinBox_id->setValue(ui->spinBox_id->value()+1);
    //获取当前行
    int currentRow=ui->tableView->currentIndex().row();
    if(!tableModel->insertRecord(currentRow+1,record))//插入行
        tableModel->select();//重新插入数据
}
