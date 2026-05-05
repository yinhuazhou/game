#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QPdfWriter");
    //创建按钮控件
    btn=new QPushButton("创建PDF文档");
    //创建垂直布局对象，并添加控件
    vbox=new QVBoxLayout(this);
    vbox->addWidget(btn);
    //使用信号/槽
    connect(btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));
}

Widget::~Widget() {}

void Widget::btn_clicked(){
    pdfWriter=new QPdfWriter("D:\\Chapter10\\test.pdf");//创建PDF文档生成器
    QPageSize pageSize(QPageSize::A4);//纸张尺寸
    pdfWriter->setPageSize(pageSize);//设置纸张尺寸
    pdfWriter->setPdfVersion(QPdfWriter::PdfVersion_1_6);//设置版本号
    QPainter painter;
    if(painter.begin(pdfWriter)){
        QPen pen;       //钢笔
        pen.setWidth(3);//线条宽度
        painter.setPen(pen);//设置钢笔
        int pageCopies=2;//页数
        for(int i=1;i<pageCopies+1;i++){
            painter.drawRect(90,30,3000,1000);//绘制矩形
            painter.drawText(50,30,"空山新雨后，天气晚来秋。");
            qDebug()<<"正在打印第"+QString::number(i)+"页，共"+QString::number(pageCopies)+"页";
            if(i!=pageCopies)
                pdfWriter->newPage();
        }
        painter.end();
    }
}
