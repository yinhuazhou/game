#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,230);
    setWindowTitle("QPrinter");
    //创建下拉列表，并添加选项
    comboBox=new QComboBox();
    QStringList printerNames=QPrinterInfo::availablePrinterNames();
    comboBox->addItems(printerNames);
    comboBox->setCurrentText(QPrinterInfo::defaultPrinterName());
    //创建数字输入框
    spinNum=new QSpinBox();
    spinNum->setRange(1,100);
    //创建复选框
    checkBox=new QCheckBox("输出到文件");
    //创建单行文本框
    lineFile=new QLineEdit();
    lineFile->setText("D:\\test.pdf");
    lineFile->setEnabled(checkBox->isChecked());
    //创建按钮
    btnPrinter=new QPushButton("打印");
    //将主窗口的布局设置为表单布局
    formLayout=new QFormLayout(this);
    formLayout->addRow("请选择打印机：",comboBox);
    formLayout->addRow("请设置打印份数：",spinNum);
    formLayout->addRow(checkBox,lineFile);
    formLayout->addRow(btnPrinter);
    //创建打印机对象
    printer=new QPrinter(QPrinterInfo::defaultPrinter());
    //使用信号/槽
    connect(comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(do_currentText(QString)));
    connect(checkBox,SIGNAL(clicked(bool)),this,SLOT(do_clicked(bool)));
    connect(btnPrinter,SIGNAL(clicked()),this,SLOT(btn_printer()));
}

Widget::~Widget() {}

void Widget::do_currentText(QString text){
    QPrinterInfo printInfo=QPrinterInfo::printerInfo(text);
    printer=new QPrinter(printInfo);//创建打印机对象
    printer->setPageOrientation(QPageLayout::Portrait);//设置打印顺序
    printer->setFullPage(false);//设置是否为整页模式
    printer->setPageSize(QPageSize::A4);//设置打印纸张的尺寸
    printer->setColorMode(QPrinter::GrayScale);//设置颜色模式
}

void Widget::do_clicked(bool checked){
    lineFile->setEnabled(checked);
}

void Widget::btn_printer(){
    printer->setOutputFileName("");
    if(checkBox->isChecked())
        printer->setOutputFileName(lineFile->text());//设置打印文件名
    if(printer->isValid()==false)
        return;
    QPainter painter;
    if(painter.begin(printer)){//绘图设备为打印机的纸张
        QPen pen;//创建钢笔
        pen.setWidth(3);//设置线条宽度
        painter.setPen(pen);//设置钢笔
        int num=spinNum->value();
        for(int i=1;i<num+1;i++){
            painter.drawRect(80,30,300,100);//绘制矩形
            qDebug()<<"正在提交第"+QString::number(i)+"页，共"+QString::number(num)+"页";
            if(i!=num)
                printer->newPage();
        }
        painter.end();
    }
}
