#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent):QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");//数据库主机名
    db.setPort(3306);//端口号
    db.setDatabaseName("mysql");//此处为在ODBC中创建的Data Source Name
    db.setUserName("root");//用户名
    db.setPassword("");//密码
    //使用信号/槽
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushButton_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::pushButton_clicked(){
    QString name=ui->lineEdit_name->text();
    QString address=ui->lineEdit_address->text();
    QString email=ui->lineEdit_email->text();
    if(name=="" || address=="" || email=="")
        return;
    if(db.open()==false)
        return;
    QSqlQuery query(db);
    query.exec("use company;");
    query.prepare("INSERT INTO clients VALUES (:id,:name,:address,:email)");
    query.bindValue(1,name);//按索引设置占位符的值
    query.bindValue(2,address);
    query.bindValue(3,email);
    if(query.exec()&&db.commit()){
        QMessageBox::information(this,"消息","插入数据成功！");
        ui->lineEdit_name->clear();
        ui->lineEdit_address->clear();
        ui->lineEdit_email->clear();
    }
    db.close();
}
