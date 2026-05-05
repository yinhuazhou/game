#include <QCoreApplication>
#include <QtSql>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");//数据库主机名
    db.setPort(3306);//端口号
    db.setDatabaseName("mysql");//此处为在ODBC中创建的Data Source Name
    db.setUserName("root");//用户名
    db.setPassword("");//密码
    if(db.open()){
        QSqlQuery query(db);
        query.exec("use company;");
        query.exec("CREATE TABLE clients(id int NOT NULL AUTO_INCREMENT,name char(50) NOT NULL,address char(50) NULL, email char(50) NULL,PRIMARY KEY (id))ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;");
    }
    qDebug()<<db.lastError().text();//若有错误打印出错信息
    db.close();
    return a.exec();
}
