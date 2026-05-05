#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMediaCaptureSession *session;//对象指针
    QVideoWidget *videoWidget;
    QAudioInput *audioInput;
    QCamera *camera;
    QMediaRecorder *recorder;
    QHBoxLayout *hbox1,*hbox2;
    QVBoxLayout *vbox;
    QPushButton *btnStart,*btnClose,*btnExit;
    QPushButton *btnStartRecord,*btnStopRecord;
    QLineEdit *line;
    QLabel *labelTip,*labelTitle;
private slots:
    void btn_start();
    void btn_close();
    void start_record();
    void do_durationChanged(qint64 duration);
    void stop_record();
};
#endif // WIDGET_H
