#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,200);
    setWindowTitle("QNetworkRequest、QNetworkAccessManager、QNetworkReply");
    //创建水平布局，添加标签、单行文本框、按钮
    labelUrl=new QLabel("URL：");
    lineUrl=new QLineEdit();
    btnDownLoad=new QPushButton("下载");
    hbox1=new QHBoxLayout();
    hbox1->addWidget(labelUrl);
    hbox1->addWidget(lineUrl);
    hbox1->addWidget(btnDownLoad);
    //创建水平布局，添加标签、单行文本框、按钮
    labelDir=new QLabel("下载文件保存路径：");
    lineDir=new QLineEdit();
    btnDir=new QPushButton("默认路径");
    hbox2=new QHBoxLayout();
    hbox2->addWidget(labelDir);
    hbox2->addWidget(lineDir);
    hbox2->addWidget(btnDir);
    //创建水平布局对象，添加标签、进度条控件
    labelProgress=new QLabel("文件下载进度：");
    progressBar=new QProgressBar();
    hbox3=new QHBoxLayout();
    hbox3->addWidget(labelProgress);
    hbox3->addWidget(progressBar);
    //设置主窗口的布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addLayout(hbox3);
    //创建网络操作对象
    networkManager=new QNetworkAccessManager();
    //使用信号/槽
    connect(btnDir,SIGNAL(clicked()),this,SLOT(btn_dir()));
    connect(btnDownLoad,SIGNAL(clicked()),this,SLOT(btn_download()));
}

Widget::~Widget() {}
//单击“默认路径”按钮
void Widget::btn_dir(){
    QString curPath=QDir::currentPath();
    QDir curDir(curPath);
    curDir.mkdir("temp");//创建子路径
    lineDir->setText(curPath+"\\temp\\");
}
//单击“下载”按钮
void Widget::btn_download(){
    QString strUrl=lineUrl->text();
    if(strUrl=="")
        return;
    QUrl newUrl=QUrl::fromUserInput(strUrl);
    if(newUrl.isValid()==false){
        QMessageBox::information(this,"错误","该地址为无效URL网址");
        return;
    }
    QString tempDir=lineDir->text();
    if(tempDir==""){
        QMessageBox::information(this,"错误","请输入下载文件的路径");
        return;
    }
    QString fullFileName=tempDir+newUrl.fileName();
    if(QFile::exists(fullFileName))
        QFile::remove(fullFileName);
    newFile.setFileName(fullFileName);
    if(newFile.open(QIODevice::WriteOnly)==false){
        QMessageBox::information(this,"错误","临时文件打开错误");
        return;
    }
    btnDownLoad->setEnabled(false);
    //发起网络请求，获取网络响应对象
    reply=networkManager->get(QNetworkRequest(newUrl));
    connect(reply,SIGNAL(readyRead()),this,SLOT(do_readyRead()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(do_downloadProgress(qint64,qint64)));
    connect(reply,SIGNAL(finished()),this,SLOT(do_finished()));
}
//下载到本地
void Widget::do_readyRead(){
    newFile.write(reply->readAll());
}
//进度条显示进度
void Widget::do_downloadProgress(qint64 byteRead, qint64 totalBytes){
    progressBar->setMaximum(totalBytes);
    progressBar->setValue(byteRead);
}
//下载完成
void Widget::do_finished(){
    btnDownLoad->setEnabled(true);
}
