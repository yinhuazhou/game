#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QPrinterInfo");
    btnShow=new QPushButton("显示打印机信息");
    textEdit=new QPlainTextEdit();
    vbox=new QVBoxLayout(this);
    vbox->addWidget(btnShow);
    vbox->addWidget(textEdit);
    connect(btnShow,SIGNAL(clicked()),this,SLOT(btn_show()));
}

Widget::~Widget() {}

void Widget::btn_show(){
    QStringList names=QPrinterInfo::availablePrinterNames();
    for(int i=0;i<names.size();i++)
        textEdit->appendPlainText(names[i]);
}
