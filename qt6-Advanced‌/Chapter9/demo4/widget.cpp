#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,150);
    setWindowTitle("QMediaCaptureSession、QMediaRecorder");
    //创建、设置QMediaCaptureSession对象
    session=new QMediaCaptureSession();
    recorder=new QMediaRecorder();
    audioInput=new QAudioInput(this);
    session->setAudioInput(audioInput);
    session->setRecorder(recorder);
    //创建水平布局对象，添加4个按钮控件
    hbox1=new QHBoxLayout();
    btnRecord=new QPushButton("录音");
    btnRecord->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    btnPause=new QPushButton("暂停");
    btnPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    btnStop=new QPushButton("停止");
    btnStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    btnExit=new QPushButton("退出");
    btnExit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    hbox1->addWidget(btnRecord);
    hbox1->addWidget(btnPause);
    hbox1->addWidget(btnStop);
    hbox1->addWidget(btnExit);
    //创建水平布局对象，添加2个标签控件、1个单行文本输入框
    hbox2=new QHBoxLayout();
    labelTip=new QLabel("输出文件：");
    linePath=new QLineEdit();
    labelTime=new QLabel("已录制0s");
    hbox2->addWidget(labelTip);
    hbox2->addWidget(linePath);
    hbox2->addWidget(labelTime);
    //创建垂直布局，并添加其他布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    //使用信号/槽
    connect(btnRecord,SIGNAL(clicked()),this,SLOT(btn_record()));
    connect(btnStop,SIGNAL(clicked()),this,SLOT(btn_stop()));
    connect(recorder,SIGNAL(durationChanged(qint64)),this,SLOT(duration_changed(qint64)));
    connect(btnPause,SIGNAL(clicked()),this,SLOT(btn_pause()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(close()));
}

Widget::~Widget() {}
//开始录制
void Widget::btn_record(){
    QString pathName=linePath->text();
    if(pathName=="")
        return;
    if(QFile::exists(pathName))
        QFile::remove(pathName);
    //设置输出文件
    recorder->setOutputLocation(QUrl::fromLocalFile(pathName));
    QMediaFormat format1(QMediaFormat::WMA);
    format1.setAudioCodec(QMediaFormat::AudioCodec::WMA);
    recorder->setMediaFormat(format1);//设置媒体文件格式
    recorder->setAudioSampleRate(16000);//设置声频采样频率
    recorder->setAudioChannelCount(1);//设置声频通道数
    recorder->setAudioBitRate(32000);//设置声频比特率
    recorder->setQuality(QMediaRecorder::HighQuality);
    //设置编码模式
    recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);
    recorder->record();
    qDebug()<<recorder->recorderState();//打印录制状态
    btnRecord->setEnabled(false);
}
//停止录制
void Widget::btn_stop(){
    recorder->stop();
    btnRecord->setEnabled(true);
}
//显示录制文件的持续时间
void Widget::duration_changed(qint64 duration){
    int time1=duration/1000;
    QString str1="已录制"+QString::number(time1)+"秒";
    labelTime->setText(str1);
}
//暂停录制
void Widget::btn_pause(){
    QMediaRecorder::RecorderState state=recorder->recorderState();
    if(state==QMediaRecorder::RecordingState){
        recorder->pause();
        btnRecord->setEnabled(true);
    }
}

