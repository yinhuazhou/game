#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QStringList>
#include <QtDebug>
#include <QtSql>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //数据库的路径和名称
    QString dbName="D:\\Chapter4\\student1.db";
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    //要输入的数据
    QList<QStringList> information={{"202401","孙悟空","79","88","89"},
                                      {"202402","猪八戒","83","81.5","80"},
                                      {"202403","小白龙","73.5","83","90"},
                                      {"202404","沙僧","75.5","96","90.8"}};
    if(db.open()){
        //创建数据表score1
        db.exec("CREATE TABLE score1(ID TEXT,name TEXT,语文 TEXT,数学 TEXT,英文 TEXT)");
        qDebug()<<db.tables();//打印数据表名
        //向数据库中插入数据
        QSqlQuery query(db);
        for(int i=0;i<information.size();i++){
            query.prepare("INSERT INTO score1 VALUES (?,?,?,?,?)");
            query.addBindValue(information[i][0]);//按顺序设置占位符(?)的值
            query.addBindValue(information[i][1]);
            query.addBindValue(information[i][2]);
            query.bindValue(3,information[i][3]);//按索引设置占位符(?)的值
            query.bindValue(4,information[i][4]);
            query.exec();
        }
        db.commit();//提交事务
    }
    return a.exec();
}
