#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QDir>
#include <QUrl>
#include <QFile>
#include <QIODevice>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox1,*hbox2,*hbox3;//水平布局指针
    QLabel *labelUrl,*labelDir,*labelProgress;//标签指针
    QLineEdit *lineUrl,*lineDir;//单行文本框指针
    QPushButton *btnDownLoad,*btnDir;//按钮指针
    QProgressBar *progressBar;//进度条指针
    QNetworkAccessManager *networkManager;//网络操作对象指针
    QNetworkReply *reply;//网络响应对象指针
    QFile newFile;
private slots:
    void btn_dir();
    void btn_download();
    void do_readyRead();
    void do_downloadProgress(qint64 byteRead,qint64 totalBytes);
    void do_finished();
};
#endif // WIDGET_H
