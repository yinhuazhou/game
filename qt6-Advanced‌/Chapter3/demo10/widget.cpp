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
    //将工作表中的数据读取到二维列表data中
    QList<QStringList> data;
    for (int row = 1; row <= rowNum; ++row) {
        QStringList rowData;
        for (int col = 1; col <=columnNum; ++col) {
            // 获取单元格对象并读取其值
            QAxObject *cell=sheet->querySubObject("Cells(int,int)", row,col);
            QString cellValue = cell->dynamicCall("Value()").toString();
            rowData.append(cellValue);
        }
        data.append(rowData);
    }
    //设置标准数据模型的表头
    standardModel->setHorizontalHeaderLabels(data[0]);
    //将二维列表转换为数据项，并添加到标准数据模型下
    for(int i=1;i<rowNum;i++){
        QList<QStandardItem *> tempItems;
        for(int j=0;j<columnNum;j++){
            QStandardItem *item=new QStandardItem(data[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            tempItems.append(item);
        }
        standardModel->appendRow(tempItems);
    }
    tableView->setModel(standardModel);//设置数据模型
    workbooks->dynamicCall("Close()");//关闭工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}
