#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,260);
    setWindowTitle("处理CSV文件");
    vbox=new QVBoxLayout(this);
    //创建2个按钮
    btnOpen=new QPushButton("打开CSV文件");
    btnSave=new QPushButton("保存CSV文件");
    hbox=new QHBoxLayout();
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnSave);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(open_csv()));
    connect(btnSave,SIGNAL(clicked()),this,SLOT(save_csv()));
    //创建表格控件
    tableWidget=new QTableWidget();
    tableWidget->setFont(QFont("黑体",14));
    vbox->addWidget(tableWidget);
}

Widget::~Widget() {}

void Widget::open_csv(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="CSV文件(*.csv);;所有文件(*.*)";
    QString title="打开CSV文本文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    tableWidget->clear();
    if(file.exists()==false)
        return;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    in.setAutoDetectUnicode(true);//自动检测Unicode
    QList<QStringList> lists;//创建二维字符串列表，用来存放数据
    while(!in.atEnd()){
        QString line=in.readLine();//读取一行数据
        //使用逗号作为分隔符，分割成字符串列表
        QStringList fields=line.split(',',Qt::SkipEmptyParts);
        lists.append(fields);
    }
    //根据data的行数、列数创建表格项
    int rowNum=lists.size();
    int columnNum=lists[0].size();
    tableWidget->setRowCount(rowNum);
    tableWidget->setColumnCount(columnNum);
    tableWidget->setHorizontalHeaderLabels(lists[0]);
    for(int i=0;i<rowNum-1;i++){
        for(int j=0;j<columnNum;j++){
            QTableWidgetItem *cell=new QTableWidgetItem();
            cell->setText(lists[i+1][j]);
            tableWidget->setItem(i+1,j,cell);
        }
    }
    file.close();
}

void Widget::save_csv(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="CSV文件(*.csv);;所有文件(*.*)";
    QString title="保存CSV文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    QStringList lists;
    QStringList temp1;
    int rowNum=tableWidget->rowCount();
    int columnNum=tableWidget->columnCount();
    //将表头数据添加到lists列表中
    for(int j=0;j<columnNum;j++){
        temp1.append(tableWidget->horizontalHeaderItem(j)->text());
    }
    QString str1=temp1.join(",");
    lists.append(str1+"\n");
    //将表格数据添加到lists列表中
    for(int i=1;i<rowNum;i++){
        QStringList temp2;
        for(int j=0;j<columnNum;j++){
            temp2.append(tableWidget->item(i,j)->text());
        }
        QString str2=temp2.join(",");
        lists.append(str2+"\n");
    }
    for(int i=0;i<lists.size();i++)
        out<<lists[i];//写入数据
    file.close();
}
