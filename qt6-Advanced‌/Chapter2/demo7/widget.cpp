#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,260);
    setWindowTitle("QTableWidget、QTableWidgetItem");
    vbox=new QVBoxLayout(this);
    //创建5个按钮
    btnAddColumn=new QPushButton("添加列");
    btnRemoveColumn=new QPushButton("删除列");
    btnAddRow=new QPushButton("添加行");
    btnRemoveRow=new QPushButton("删除行");
    btnSelectAll=new QPushButton("全选");
    hbox=new QHBoxLayout();
    hbox->addWidget(btnAddColumn);
    hbox->addWidget(btnRemoveColumn);
    hbox->addWidget(btnAddRow);
    hbox->addWidget(btnRemoveRow);
    hbox->addWidget(btnSelectAll);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnAddColumn,SIGNAL(clicked()),this,SLOT(add_column()));
    connect(btnRemoveColumn,SIGNAL(clicked()),this,SLOT(remove_column()));
    connect(btnAddRow,SIGNAL(clicked()),this,SLOT(add_row()));
    connect(btnRemoveRow,SIGNAL(clicked()),this,SLOT(remove_row()));
    connect(btnSelectAll,SIGNAL(clicked()),this,SLOT(select_all()));
    //创建表格控件
    tableWidget=new QTableWidget();
    tableWidget->setFont(QFont("黑体",14));
    vbox->addWidget(tableWidget);
}

Widget::~Widget() {}

void Widget::add_column(){
    int count=tableWidget->columnCount();
    if(count==0)
        tableWidget->insertColumn(0);
    else
        tableWidget->insertColumn(count);
}

void Widget::add_row(){
    int count=tableWidget->rowCount();
    if(count==0)
        tableWidget->insertRow(0);
    else
        tableWidget->insertRow(count);
}

void Widget::remove_column(){
    int num=tableWidget->currentColumn();
    if(num<0)
        return;
    QString title="删除列";
    QString label="确定要删除这一列？";
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,title,label,QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes)
        tableWidget->removeColumn(num);
}

void Widget::remove_row(){
    int num=tableWidget->currentRow();
    if(num<0)
        return;
    QString title="删除行";
    QString label="确定要删除这一行？";
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,title,label,QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes)
        tableWidget->removeRow(num);
}

void Widget::select_all(){
    int rowNum=tableWidget->rowCount();
    int columnNum=tableWidget->columnCount();
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<columnNum;j++){
            QTableWidgetItem *item=tableWidget->item(i,j);
            if(item==nullptr)
                return;
            item->setCheckState(Qt::Checked);
        }
    }
}
