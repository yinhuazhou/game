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
    QString dbName=ui->lineEdit_dbName->text();
    QString tbName=ui->lineEdit_tbName->text();
    if(dbName=="" || tbName=="")
        return;
    if(db.open()==false)
        return;
    QSqlQuery query(db);
    query.exec("use "+dbName);
    QList<QStringList> data;//创建二维列表data
    //将数据表中的数据转存到二维列表data中
    if(query.exec("SELECT * FROM "+tbName)){
        while(query.next()){
            QString id=query.value("id").toString();
            QString name=query.value("name").toString();
            QString addresse=query.value("address").toString();
            QString email=query.value("email").toString();
            QStringList temp={id,name,addresse,email};
            data.append(temp);
        }
    }
    db.close();
    //根据二维列表data的行数、列数创建表格控件
    int rowNum=data.size();
    int columnNum=data[0].size();
    ui->tableWidget->setRowCount(rowNum);
    ui->tableWidget->setColumnCount(columnNum);
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<columnNum;j++){
            QTableWidgetItem *cell=new QTableWidgetItem();
            cell->setText(data[i][j]);
            ui->tableWidget->setItem(i,j,cell);
        }
    }
}
