#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>
#include <QStyle>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUrl>
#include <QSizeF>
#include <QIcon>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMediaPlayer *player;//对象指针
    QAudioOutput *audioOutput;
    QGraphicsView *videoView;
    QGraphicsScene *scene;
    QGraphicsVideoItem *videoItem;//视频项指针
    QHBoxLayout *hbox;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnOpen,*btnPlay,*btnPause,*btnStop;//按钮指针
    QPushButton *btnZoomIn,*btnZoomOut;//按钮指针
    QLabel *labelTime;//标签指针
    QSlider *slider;//滑块指针
    QString durationTime;//视频文件的持续时间
    QString positionTime;//视频文件播放的时间
private slots:
    void btn_open();
    void btn_play();
    void btn_pause();
    void btn_stop();
    void position_changed(qint64 position);
    void duration_changed(qint64 duration);
    void set_position(int pos);
    void btn_zoomIn();
    void btn_zoomOut();
};
#endif // WIDGET_H
