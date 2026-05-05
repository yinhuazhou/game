#include <QCoreApplication>
#include <QTemporaryDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTemporaryDir dir1;
    QTemporaryDir dir2("abcXXXXXX");
    if(dir1.isValid())
        qDebug()<<dir1.path();
    if(dir2.isValid())
        qDebug()<<dir2.path();
    return a.exec();
}
