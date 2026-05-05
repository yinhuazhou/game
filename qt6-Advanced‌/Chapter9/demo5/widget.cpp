#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QVideoWidget、QMediaPlayer");
    player=new QMediaPlayer();
    audioOutput=new QAudioOutput();
    player->setAudioOutput(audioOutput);//设置声频输出设备
    videoWidget=new QVideoWidget();
    player->setVideoOutput(videoWidget);//设置显示视频帧的控件
    //创建水平布局对象，添加4个按钮、1个标签、1个滑块控件
    hbox=new QHBoxLayout();
    btnOpen=new QPushButton();
    btnOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    btnPlay=new QPushButton();
    btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    btnPause=new QPushButton();
    btnPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    btnStop=new QPushButton();
    btnStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    slider=new QSlider(Qt::Horizontal);
    slider->setRange(0,0);
    labelTime=new QLabel();
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnPlay);
    hbox->addWidget(btnPause);
    hbox->addWidget(btnStop);
    hbox->addWidget(labelTime);
    hbox->addWidget(slider);
    //创建垂直布局对象，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    vbox->addWidget(videoWidget);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
    connect(btnPlay,SIGNAL(clicked()),this,SLOT(btn_play()));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(position_changed(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(duration_changed(qint64)));
    connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(set_position(int)));
    connect(btnPause,SIGNAL(clicked()),this,SLOT(btn_pause()));
    connect(btnStop,SIGNAL(clicked()),this,SLOT(btn_stop()));
}

Widget::~Widget() {}
//打开视频文件
void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="视频文件(*.mp4 *.wmv);;所有文件(*.*)";
    QString title="打开视频文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    player->setSource(QUrl::fromLocalFile(fileName));
    int duration=player->duration();
    int secs=duration/1000;
    int mins=secs/60;
    secs=secs%60;
    durationTime=QString::number(mins)+":"+QString::number(secs);
}
//播放视频
void Widget::btn_play(){
    if(player->isPlaying()==false){
        player->play();
    }
    else{
        return;
    }
}
//暂停播放
void Widget::btn_pause(){
    if(player->isPlaying()==true){
        player->pause();
    }
    else{
        return;
    }
}
//停止播放
void Widget::btn_stop(){
    player->stop();
    slider->setSliderPosition(0);
}
//当视频播放位置发生改变时连接的槽函数
void Widget::position_changed(qint64 position){
    slider->setSliderPosition(position);
    int secs=position/1000;
    int mins=secs/60;
    secs=secs%60;
    positionTime=QString::number(mins)+":"+QString::number(secs);
    labelTime->setText(positionTime+"/"+durationTime);
}
//当视频文件的持续时间发生改变时连接的槽函数
void Widget::duration_changed(qint64 duration){
    slider->setMaximum(duration);
    int secs=duration/1000;
    int mins=secs/60;
    secs=secs%60;
    durationTime=QString::number(mins)+":"+QString::number(secs);
}
//当滑块的位置发生改变时连接的槽函数
void Widget::set_position(int pos){
    slider->setSliderPosition(pos);
    player->setPosition(pos);
}
