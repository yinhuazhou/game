#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFileInfo info("D:/Chapter1/001.txt");
    qDebug()<<"扩展名："<<info.suffix();
    qDebug()<<"创建时间："<<info.birthTime();
    qDebug()<<"文件所有者ID："<<info.ownerId();
    qDebug()<<"文件的大小："<<info.size();
    return a.exec();
}
