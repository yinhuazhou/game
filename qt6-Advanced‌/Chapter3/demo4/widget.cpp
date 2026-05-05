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
    open_xlsx();
}

Widget::~Widget() {}

void Widget::open_xlsx(){
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿集
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open(const QString&)","D:\\Chapter3\\001.xlsx");
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
    listModel->setStringList(data);
    listView->setModel(listModel);  //设置数据模型
    workbooks->dynamicCall("Close()");//关闭工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}
