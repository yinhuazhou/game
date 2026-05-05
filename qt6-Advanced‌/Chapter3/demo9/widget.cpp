#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QTableView、QStandardItemModel");
    hbox=new QHBoxLayout(this);
    //创建表格视图控件
    tableView=new QTableView();
    hbox->addWidget(tableView);
    //创建标准数据模型
    standardModel=new QStandardItemModel();
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
    QList<QStringList> lists;//创建二维字符串列表，用来存放数据
    while(!in.atEnd()){
        QString line=in.readLine();//读取一行数据
        //使用逗号作为分隔符，分割成字符串列表
        QStringList fields=line.split(',',Qt::SkipEmptyParts);
        lists.append(fields);
    }
    //获取lists的行数、列数
    int rowNum=lists.size();
    int columnNum=lists[0].size();
    standardModel->setHorizontalHeaderLabels(lists[0]);//设置表头
    //将二维列表转换为数据项，并添加到标准数据模型下
    for(int i=1;i<rowNum;i++){
        QList<QStandardItem *> tempItems;
        for(int j=0;j<columnNum;j++){
            //QString text=lists[i][j];
            QStandardItem *item=new QStandardItem(lists[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            tempItems.append(item);
        }
        standardModel->appendRow(tempItems);
    }
    tableView->setModel(standardModel);//设置数据模型
}
