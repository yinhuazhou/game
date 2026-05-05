#include <QCoreApplication>
#include <QSaveFile>
#include <QByteArray>
#include <QIODevice>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSaveFile save1;
    save1.setFileName("D:/Chapter1/005.txt");
    QByteArray byte1;
    byte1.insert(0,"One World,One Dream.");
    if(save1.open(QIODevice::WriteOnly)){
        save1.write(byte1);
        save1.commit();
    }
    return a.exec();
}
