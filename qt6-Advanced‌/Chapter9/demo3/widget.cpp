#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,150);
    setWindowTitle("QSoundEffect");
    sound=new QSoundEffect();
    //创建标签控件
    labelSource=new QLabel("文件来源：");
    //创建水平布局，添加按钮控件、滑块控件、标签控件
    btnOpen=new QPushButton("打开文件");
    btnPlay=new QPushButton();
    btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));//设置按钮的图标
    btnPlay->setEnabled(false);
    labelVolume=new QLabel("音量大小：");
    slider=new QSlider(Qt::Horizontal);
    slider->setRange(0,100);
    slider->setSliderPosition(50);
    sound->setVolume(0.5);
    hbox=new QHBoxLayout();
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnPlay);
    hbox->addWidget(labelVolume);
    hbox->addWidget(slider);
    //创建垂直布局，并添加其他布局控件
    vbox=new QVBoxLayout(this);
    vbox->addWidget(labelSource);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
    connect(btnPlay,SIGNAL(clicked()),this,SLOT(btn_play()));
    connect(sound,SIGNAL(playingChanged()),this,SLOT(playing_changed()));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(set_position(int)));
}

Widget::~Widget() {}
//打开文件按钮
void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="声频文件(*.wav);;所有文件(*.*)";
    QString title="打开声频文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    sound->setSource(QUrl::fromLocalFile(fileName));
    btnPlay->setEnabled(true);
    labelSource->setText("文件来源："+fileName);
}
//播放或暂停按钮
void Widget::btn_play(){
    if(sound->isPlaying()==false){
        sound->play();
    }
    else{
        sound->stop();
    }
}
//当播放状态发生改变时，更改播放或暂停按钮的图标
void Widget::playing_changed(){
    if(sound->isPlaying()==true){
        btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else{
        btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
//当滑块的数值发生改变时连接的槽函数
void Widget::set_position(int pos){
    float pos1=static_cast<float>(pos);
    float vol=pos1/100;
    sound->setVolume(vol);
}
