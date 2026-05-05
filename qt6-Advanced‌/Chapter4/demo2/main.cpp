#include <QCoreApplication>
#include <QtSql>
#include <QStringList>
#include <QString>
#include <QVariant>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //数据库的路径和名称
    QString dbName="D:\\Chapter4\\student1.db";
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open()){
        QSqlQuery query(db);
        //打印当前查询到内部位置
        qDebug()<<query.at();
        if(query.exec("SELECT * FROM score1")){
            while(query.next()){
                QVariant id=query.value("ID");
                QVariant name=query.value("name");
                QVariant chinese=query.value("语文");
                QVariant math=query.value("数学");
                QVariant english=query.value("英文");
                QStringList list={id.toString(),name.toString(),chinese.toString(),math.toString(),english.toString()};
                qDebug()<<list;
            }
        }
    }
    db.close();
    return a.exec();
}
