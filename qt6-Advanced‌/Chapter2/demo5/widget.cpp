#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QTableWidget");
    vbox=new QVBoxLayout(this);
    //创建表格控件
    tableWidget=new QTableWidget();
    tableWidget->setRowCount(3);
    tableWidget->setColumnCount(5);
    tableWidget->setFont(QFont("黑体",12));
    vbox->addWidget(tableWidget);
    //设置表头
    QTableWidgetItem *item00=new QTableWidgetItem("学号");
    QTableWidgetItem *item01=new QTableWidgetItem("姓名");
    QTableWidgetItem *item02=new QTableWidgetItem("语文成绩");
    QTableWidgetItem *item03=new QTableWidgetItem("数学成绩");
    QTableWidgetItem *item04=new QTableWidgetItem("总分");
    tableWidget->setItem(0,0,item00);
    tableWidget->setItem(0,1,item01);
    tableWidget->setItem(0,2,item02);
    tableWidget->setItem(0,3,item03);
    tableWidget->setItem(0,4,item04);
    //插入第1行数据
    QTableWidgetItem *item10=new QTableWidgetItem("001");
    QTableWidgetItem *item11=new QTableWidgetItem("孙悟空");
    QTableWidgetItem *item12=new QTableWidgetItem("90");
    QTableWidgetItem *item13=new QTableWidgetItem("90");
    QTableWidgetItem *item14=new QTableWidgetItem("180");
    tableWidget->setItem(1,0,item10);
    tableWidget->setItem(1,1,item11);
    tableWidget->setItem(1,2,item12);
    tableWidget->setItem(1,3,item13);
    tableWidget->setItem(1,4,item14);
    //插入第2行数据
    QTableWidgetItem *item20=new QTableWidgetItem("002");
    QTableWidgetItem *item21=new QTableWidgetItem("猪八戒");
    QTableWidgetItem *item22=new QTableWidgetItem("80");
    QTableWidgetItem *item23=new QTableWidgetItem("80");
    QTableWidgetItem *item24=new QTableWidgetItem("160");
    tableWidget->setItem(2,0,item20);
    tableWidget->setItem(2,1,item21);
    tableWidget->setItem(2,2,item22);
    tableWidget->setItem(2,3,item23);
    tableWidget->setItem(2,4,item24);
}

Widget::~Widget() {}
