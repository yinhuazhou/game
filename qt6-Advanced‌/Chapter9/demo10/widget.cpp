#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,300);
    setWindowTitle("QCamera、QMediaRecorder");
    //创建媒体捕获器对象
    session=new QMediaCaptureSession(this);
    //创建显示视频的控件
    videoWidget=new QVideoWidget();
    session->setVideoOutput(videoWidget);//设置显示视频的控件

    audioInput=new QAudioInput(this);
    session->setAudioInput(audioInput);//设置声频输入设备

    QList<QCameraDevice> cList=QMediaDevices::videoInputs();
    camera=new QCamera(cList[0]);
    session->setCamera(camera);//设置摄像头

    recorder=new QMediaRecorder(this);//创建QMediaRecorder对象，用于录像
    recorder->setQuality(QMediaRecorder::HighQuality);
    session->setRecorder(recorder);

    //创建水平布局对象，添加3个按钮
    hbox1=new QHBoxLayout();
    btnStart=new QPushButton("开启摄像头");
    btnClose=new QPushButton("关闭摄像头");
    btnClose->setEnabled(false);
    btnExit=new QPushButton("退出");
    hbox1->addWidget(btnStart);
    hbox1->addWidget(btnClose);
    hbox1->addWidget(btnExit);
    //创建水平布局对象，添加标签、单行文本输入框、2个按钮
    hbox2=new QHBoxLayout();
    labelTitle=new QLabel("保存的文件：");
    line=new QLineEdit();
    btnStartRecord=new QPushButton("开始录像");
    btnStartRecord->setEnabled(false);
    btnStopRecord=new QPushButton("停止录像");
    btnStopRecord->setEnabled(false);
    hbox2->addWidget(labelTitle);
    hbox2->addWidget(line);
    hbox2->addWidget(btnStartRecord);
    hbox2->addWidget(btnStopRecord);
    //创建底部的标签控件
    labelTip=new QLabel();
    labelTip->setFixedHeight(10);
    //创建垂直布局对象，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addWidget(videoWidget);
    vbox->addWidget(labelTip);
    //使用信号/槽
    connect(btnStart,SIGNAL(clicked()),this,SLOT(btn_start()));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(btn_close()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(close()));
    connect(btnStartRecord,SIGNAL(clicked()),this,SLOT(start_record()));
    connect(recorder,SIGNAL(durationChanged(qint64)),this,SLOT(do_durationChanged(qint64)));
    connect(btnStopRecord,SIGNAL(clicked()),this,SLOT(stop_record()));
}

Widget::~Widget() {}
//打开摄像头
void Widget::btn_start(){
    camera->start();
    btnClose->setEnabled(true);
    btnStartRecord->setEnabled(true);
    btnStopRecord->setEnabled(true);
}
//关闭摄像头
void Widget::btn_close(){
    stop_record();
    camera->stop();
    btnClose->setEnabled(false);
    btnStartRecord->setEnabled(false);
    btnStopRecord->setEnabled(false);
}
//开始录制
void Widget::start_record(){
    QString pathName=line->text();
    if(pathName=="")
        return;
    if(QFile::exists(pathName))
        QFile::remove(pathName);
    QMediaFormat mediaFormat;//创建媒体格式对象
    mediaFormat.setVideoCodec(QMediaFormat::VideoCodec::MPEG4);
    mediaFormat.setFileFormat(QMediaFormat::MPEG4);
    recorder->setMediaFormat(mediaFormat);
    recorder->setOutputLocation(QUrl::fromLocalFile(pathName));
    recorder->record();
    qDebug()<<recorder->errorString();
}
//停止录制
void Widget::stop_record(){
    if(recorder->recorderState()==QMediaRecorder::RecordingState){
        recorder->stop();
    }
    else{
        return;
    }
}
//当录制时间发生变化时连接的槽函数
void Widget::do_durationChanged(qint64 duration){
    double dura=static_cast<double>(duration);
    double time=dura/1000;
    QString str1="录制时间："+QString::number(time,'g',3)+"秒";
    labelTip->setText(str1);
}

