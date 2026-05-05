#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListView、QStringListModel");
    vbox=new QVBoxLayout(this);
    //创建视图控件
    listView=new QListView();
    vbox->addWidget(listView);
    //创建数据模型
    listModel=new QStringListModel(this);
    open_csv();
}

Widget::~Widget() {}

void Widget::open_csv(){
    QFile file("D:\\Chapter3\\data1.csv");
    if(file.exists()==false)
        return;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    in.setAutoDetectUnicode(true);//自动检测Unicode
    QStringList data;//创建字符串列表，用来存放数据
    while(!in.atEnd()){
        QString line=in.readLine();//读取一行数据
        //向字符串列表中添加一个字符串
        data.append(line);
    }
    listModel->setStringList(data);
    listView->setModel(listModel);
    file.close();
}
