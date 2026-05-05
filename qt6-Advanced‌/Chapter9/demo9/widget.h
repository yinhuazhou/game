#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMediaCaptureSession *session;//对象指针
    QVideoWidget *videoWidget;
    QCamera *camera;
    QImageCapture *imageCapture;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
    QPushButton *btnStart,*btnClose,*btnCapture,*btnExit;
    QLabel *labelTip;
private slots:
    void btn_start();
    void btn_close();
    void btn_capture();
    void do_imageSaved(int id,QString fileName);
};
#endif // WIDGET_H
