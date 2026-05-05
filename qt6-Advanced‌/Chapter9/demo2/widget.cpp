#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,150);
    setWindowTitle("QMediaPlayer、QAudioOutput");
    player=new QMediaPlayer();
    audioOutput=new QAudioOutput();
    player->setAudioOutput(audioOutput);
    //创建标签控件
    labelSource=new QLabel("文件来源：");
    //创建水平布局，添加按钮控件、滑块控件、标签控件
    btnOpen=new QPushButton("打开文件");
    btnPlay=new QPushButton();
    btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));//设置按钮的图标
    slider=new QSlider(Qt::Horizontal);
    slider->setRange(0,0);
    labelTime=new QLabel();
    hbox=new QHBoxLayout();
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnPlay);
    hbox->addWidget(slider);
    hbox->addWidget(labelTime);
    //创建垂直布局，并添加其他布局控件
    vbox=new QVBoxLayout(this);
    vbox->addWidget(labelSource);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
    connect(btnPlay,SIGNAL(clicked()),this,SLOT(btn_play()));
    connect(player,SIGNAL(playingChanged(bool)),this,SLOT(playing_changed(bool)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(position_changed(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(duration_changed(qint64)));
    connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(set_position(int)));
}

Widget::~Widget() {}
//打开文件按钮
void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="声频文件(*.mp3);;所有文件(*.*)";
    QString title="打开声频文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    player->setSource(QUrl::fromLocalFile(fileName));
    labelSource->setText("文件来源："+fileName);
}
//播放或暂停按钮
void Widget::btn_play(){
    if(player->isPlaying()==false){
        player->play();
    }
    else{
        player->pause();
    }
}
//当播放状态发生改变时，更改播放或暂停按钮的图标
void Widget::playing_changed(bool playing){
    if(playing==true){
        btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else{
        btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
//当滑块的位置改变时连接的槽函数
void Widget::position_changed(qint64 position){
    slider->setSliderPosition(position);
    int secs=position/1000;
    int mins=secs/60;
    secs=secs%60;
    positionTime=QString::number(mins)+":"+QString::number(secs);
    labelTime->setText(positionTime+"/"+durationTime);
}
//当声频文件的持续时间发生改变时连接的槽函数
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
