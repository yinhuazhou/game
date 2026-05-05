#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStyle>
#include <QUrl>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMediaCaptureSession *session;//媒体捕获器指针
    QMediaRecorder *recorder;//媒体录制器指针
    QAudioInput *audioInput;//音频输入设备指针
    QHBoxLayout *hbox1,*hbox2;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnRecord,*btnPause,*btnStop,*btnExit;//按钮指针
    QLabel *labelTip,*labelTime;//标签指针
    QLineEdit *linePath;//单行文本框指针
private slots:
    void btn_record();
    void btn_stop();
    void duration_changed(qint64 duration);
    void btn_pause();
};
#endif // WIDGET_H
