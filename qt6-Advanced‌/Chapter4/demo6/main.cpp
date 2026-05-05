#include <QCoreApplication>
#include <QtSql>
#include <QString>
#include <QDebug>

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
        query.exec("select version();");
        if(query.next()){
            QString version=query.value(0).toString();
            qDebug()<<version;//打印数据库版本号
        }
    }
    qDebug()<<db.lastError().text();//若有错误打印出错信息
    db.close();
    return a.exec();
}
