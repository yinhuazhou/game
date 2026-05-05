#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QByteArray>
#include <QHostInfo>
#include <QAbstractSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QList>
#include <QString>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox1,*hbox2,*hbox3,*hbox4;//水平布局指针
    QPushButton *btnStart,*btnEnd,*btnClear,*btnExit;//按钮对象指针
    QPushButton *btnMulticast;//按钮对象指针
    QLabel *labelPort,*labelIP,*labelSocket;//标签指针
    QLineEdit *lineIP,*lineMsg;//单行文本框指针
    QSpinBox *port;//数字输入控件指针
    QPlainTextEdit *textEdit;//多行纯文本框指针
    QUdpSocket *udpSocket;//UDP通信的套接字对象指针
    QHostAddress groupAddress;//多播组地址
    QString get_localIP();
private slots:
    void do_socketStateChanged(QAbstractSocket::SocketState state);
    void do_socketReadyRead();
    void btn_start();
    void btn_end();
    void btn_multicast();
    void btn_clear();
};
#endif // WIDGET_H
