#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QMediaPlayer、QMediaMetaData");
    player=new QMediaPlayer(this);
    btnOpen=new QPushButton("打开文件");
    textEdit=new QPlainTextEdit();
    vbox=new QVBoxLayout(this);//设置主窗口的布局方式
    vbox->addWidget(btnOpen);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
}

Widget::~Widget() {}

void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="声频文件(*.mp3 *.wav *.wma);;所有文件(*.*)";
    QString title="打开声频文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    player->setSource(QUrl::fromLocalFile(fileName));
    textEdit->clear();
    //使用信号/槽
    connect(player,SIGNAL(metaDataChanged()),this,SLOT(show_data()));
}

void Widget::show_data(){
    meta=player->metaData();//获取当前媒体的元数据对象
    if(meta.isEmpty()==true)
        return;
    QString duration=meta.value(QMediaMetaData::Duration).toString();
    QString str1="媒体文件的持续时间为 "+duration+"毫秒";
    textEdit->appendPlainText(str1);
    QString title=meta.stringValue(QMediaMetaData::Title);
    QString str2="媒体的标题文件为 "+title;
    textEdit->appendPlainText(str2);
    QString bitRate=meta.stringValue(QMediaMetaData::AudioBitRate);
    QString str3="声频比特率为 "+bitRate;
    textEdit->appendPlainText(str3);
    QString code=meta.stringValue(QMediaMetaData::AudioCodec);
    QString str4="声频编码为 "+code;
    textEdit->appendPlainText(str4);
    QString format=meta.value(QMediaMetaData::FileFormat).toString();
    QString str5="媒体文件格式为 "+format;
    textEdit->appendPlainText(str5);
}
