#include <QCoreApplication>
#include <QTemporaryFile>
#include <QByteArray>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTemporaryFile temporary("XXXXXX.aaa");
    QByteArray byte1;
    byte1.insert(0,"hello world");
    if(temporary.open()){
        temporary.write(byte1);
        qDebug()<<temporary.fileName();
    }
    return a.exec();
}
