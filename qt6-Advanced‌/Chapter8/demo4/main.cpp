#include <QCoreApplication>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QList<QNetworkInterface> inList=QNetworkInterface::allInterfaces();
    for(int i=0;i<inList.count();i++){
        QList<QNetworkAddressEntry> enList=inList[i].addressEntries();
        for(int j=0;j<enList.count();j++){
            qDebug()<<"IP地址："<<enList[j].ip().toString();
            qDebug()<<"子网掩码："<<enList[j].netmask().toString();
            qDebug()<<"广播地址："<<enList[j].broadcast().toString();
        }
    }
    return a.exec();
}
