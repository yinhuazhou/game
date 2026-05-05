#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(200,200,580,230);
    setWindowTitle("QPrintDialog");
    //创建按钮
    btnPrinter=new QPushButton("显示打印对话框",this);
    vbox=new QVBoxLayout(this);
    vbox->addWidget(btnPrinter);
    //创建打印对话框对象
    printDialog=new QPrintDialog(this);
    //使用信号/槽
    connect(btnPrinter,SIGNAL(clicked()),this,SLOT(btn_printer()));
    connect(printDialog,SIGNAL(accepted(QPrinter*)),this,SLOT(printDialog_accepted(QPrinter*)));
}

Widget::~Widget() {}

void Widget::btn_printer(){
    printDialog->exec();
}

void Widget::printDialog_accepted(QPrinter *printer){
    if(printer->isValid()==false)
        return;
    QPainter painter;
    if(painter.begin(printer)){
        QPen pen;//钢笔
        pen.setWidth(3);//线条宽度
        painter.setPen(pen);//设置钢笔
        painter.drawRect(80,30,300,100);//绘制矩形
        painter.end();
    }
}
