#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QUrl>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox;//水平布局指针
    QLabel *labelSource,*labelVolume;//标签指针
    QPushButton *btnOpen,*btnPlay;//按钮指针
    QSlider *slider;//滑块指针
    QSoundEffect *sound;
private slots:
    void btn_open();
    void btn_play();
    void playing_changed();
    void set_position(int pos);
};
#endif // WIDGET_H
