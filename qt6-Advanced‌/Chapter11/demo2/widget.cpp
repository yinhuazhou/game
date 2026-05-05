#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QAxWidget");
    //创建垂直布局，并添加其他控件
    vbox=new QVBoxLayout(this);
    btn=new QPushButton("打开Excel文件");
    label=new QLabel();
    vbox->addWidget(btn);
    vbox->addWidget(label);
    //创建QAxWidget控件，父窗口为标签控件
    mywidget=new QAxWidget("Excel.Application",label);
    //使用信号/槽
    connect(btn,SIGNAL(clicked()),this,SLOT(btn_open()));
}

Widget::~Widget() {}

void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Excel文件(*.xlsx);;所有文件(*.*)";
    QString title="打开Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //不显示Office窗体
    mywidget->dynamicCall("SetVisible(bool Visible)",false);
    //屏蔽Office的警告消息框
    mywidget->setProperty("DispalyAlerts",false);
    //设置显示区的范围
    mywidget->setGeometry(0,0,label->geometry().width(),label->geometry().height());
    mywidget->setControl(fileName);//打开指定的文件
    mywidget->show();//显示内容
}
