#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListView、QStringListModel");
    hbox=new QHBoxLayout(this);
    //创建1个按钮、1个列表视图，然后放置在垂直布局对象中
    btnOpen=new QPushButton("打开");
    listView1=new MyListView();
    vbox1=new QVBoxLayout();
    vbox1->addWidget(btnOpen);
    vbox1->addWidget(listView1);
    //创建1个按钮、1个列表视图，然后放置在垂直布局对象中
    btnAdd=new QPushButton("添加");
    listView2=new MyListView();
    vbox2=new QVBoxLayout();
    vbox2->addWidget(btnAdd);
    vbox2->addWidget(listView2);
    //向水平布局中添加2个垂直布局对象S
    hbox->addLayout(vbox1);
    hbox->addLayout(vbox2);
    //设置2个视图控件的选择模式
    listView1->setSelectionMode(QListView::ExtendedSelection);
    listView2->setSelectionMode(QListView::ExtendedSelection);
    //创建2个数据模型
    model1=new QStringListModel(this);
    model2=new QStringListModel(this);
    //设置数据模型
    listView1->setModel(model1);
    listView2->setModel(model2);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btnOpen_clicked()));
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(btnAdd_clicked()));
}

Widget::~Widget() {}

void Widget::btnOpen_clicked(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Excel文件(*.xlsx);;所有文件(*.*)";
    QString title="打开Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿集
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open(const QString&)",fileName);
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //获取工作表集
    QAxObject *sheets=workbook->querySubObject("WorkSheets");
    QAxObject *sheet=workbook->querySubObject("Sheets(int)",1);
    // 获取工作表的最大行数、列数
    QAxObject *usedRange=sheet->querySubObject("UsedRange");
    QAxObject *rows=usedRange->querySubObject("Rows");
    QAxObject *columns=usedRange->querySubObject("Columns");
    int rowNum=rows->property("Count").toInt();
    int columnNum=columns->property("Count").toInt();
    //读取工作表中的数据,保存在字符串列表data中
    QStringList data;
    for (int row = 1; row <= rowNum; ++row) {
        QString rowData="";
        for (int col = 1; col <=columnNum; ++col) {
            // 获取单元格对象并读取其值
            QAxObject *cell=sheet->querySubObject("Cells(int,int)", row,col);
            QString cellValue = cell->dynamicCall("Value()").toString();
            rowData=rowData+cellValue+" ";
        }
        data.append(rowData);
    }
    model1->setStringList(data);
    workbooks->dynamicCall("Close()");//关闭工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}

void Widget::btnAdd_clicked(){
    QModelIndexList selectedIndexs=listView1->selectedIndexes();
    if(selectedIndexs.count()==0)
        return;
    QModelIndex index=selectedIndexs[0];
    //获取数据
    QVariant value=model1->data(index,Qt::DisplayRole);
    model1->removeRow(index.row());
    //获取行的数量
    int count=model2->rowCount();
    //在末尾插入数据
    model2->insertRow(count);
    QModelIndex lastIndex=model2->index(count,0);
    //设置末尾的数据
    model2->setData(lastIndex,value,Qt::DisplayRole);
}
