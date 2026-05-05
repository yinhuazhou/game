#include <QCoreApplication>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();
    for(int i=0;i<list.count();i++){
        qDebug()<<"设备名称："<<list[i].humanReadableName();
        qDebug()<<"硬件地址："<<list[i].hardwareAddress();
        qDebug()<<"接口类型："<<list[i].type();
    }
    return a.exec();
}
