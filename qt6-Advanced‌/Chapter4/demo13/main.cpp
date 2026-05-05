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
    QString dbName="D:\\Chapter4\\student4.db";
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    //要输入的数据
    QList<QStringList> infor1={{"6601","孙悟空","79","6601","6601"},
                                 {"6602","猪八戒","83","6602","6602"},
                                 {"6603","小白龙","73.5","6603","6604"},
                                 {"6604","沙僧","75.5","6604","6604"}};
    QList<QStringList> infor2={{"6601","孙悟空","88","89"},
                                 {"6602","猪八戒","81.5","80"},
                                 {"6603","小白龙","83","90"},
                                 {"6604","沙僧","96","90.8"}};
    if(db.open()){
        //创建数据表score1
        db.exec("CREATE TABLE score1(ID TEXT,name TEXT,语文 TEXT,数学 TEXT,英文 TEXT)");
        qDebug()<<db.tables();//打印数据表名
        //向数据表score1中插入数据
        if(db.transaction()){
            QSqlQuery query(db);
            for(int i=0;i<infor1.size();i++){
                query.prepare("INSERT INTO score1 VALUES (?,?,?,?,?)");
                query.addBindValue(infor1[i][0]);//按顺序设置占位符(?)的值
                query.addBindValue(infor1[i][1]);
                query.addBindValue(infor1[i][2]);
                query.bindValue(3,infor1[i][3]);//按索引设置占位符(?)的值
                query.bindValue(4,infor1[i][4]);
                query.exec();
            }
            db.commit();//提交事务
        }
        //创建数据表score2
        db.exec("CREATE TABLE score2(ID TEXT,name TEXT,数学 TEXT,英文 TEXT)");
        qDebug()<<db.tables();//打印数据表名
        //向数据表score2中插入数据
        if(db.transaction()){
            QSqlQuery query(db);
            for(int i=0;i<infor2.size();i++){
                query.prepare("INSERT INTO score2 VALUES (:ID,:name,:math,:english)");
                query.bindValue(0,infor2[i][0]);//按索引设置占位符的值
                query.bindValue(1,infor2[i][1]);
                query.bindValue(":math",infor2[i][2]);//按名称设置占位符的值
                query.bindValue(":english",infor2[i][3]);
                query.exec();
            }
            db.commit();//提交事务
        }
    }
    db.close();
    return a.exec();
}
