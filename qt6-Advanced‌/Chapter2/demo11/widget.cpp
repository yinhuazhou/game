#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,260);
    setWindowTitle("处理Excel文件");
    vbox=new QVBoxLayout(this);
    //创建2个按钮
    btnOpen=new QPushButton("打开Excel文件");
    btnSave=new QPushButton("保存Excel文件");
    hbox=new QHBoxLayout();
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnSave);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(open_excel()));
    connect(btnSave,SIGNAL(clicked()),this,SLOT(save_excel()));
    //创建表格控件
    tableWidget=new QTableWidget();
    vbox->addWidget(tableWidget);
}

Widget::~Widget() {}

void Widget::open_excel(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Excel文件(*.xlsx);;所有文件(*.*)";
    QString title="打开Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //清空表格控件的内容
    tableWidget->clear();
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿集
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open(const QString&)",fileName);
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //获取工作表集
    QAxObject *sheets=workbook->querySubObject("WorkSheets");
    QAxObject *sheet=workbook->querySubObject("Sheets(int)",1);
    QAxObject *cell=sheet->querySubObject("Range(QVariant,QVariant)","C3");
    QString str=cell->dynamicCall("Value()").toString();
    qDebug()<<str;
    // 获取工作表的最大行数、列数
    QAxObject *usedRange=sheet->querySubObject("UsedRange");
    QAxObject *rows=usedRange->querySubObject("Rows");
    QAxObject *columns=usedRange->querySubObject("Columns");
    int rowNum=rows->property("Count").toInt();
    int columnNum=columns->property("Count").toInt();
    qDebug()<<rowNum;
    qDebug()<<columnNum;
    //读取工作表中的数据
    QList<QStringList> data;
    for (int row = 1; row <= rowNum; ++row) {
        QStringList rowData;
        for (int col = 1; col <=columnNum; ++col) {
            // 获取单元格对象并读取其值
            QAxObject *cell=sheet->querySubObject("Cells(int,int)", row,col);
            QString cellValue = cell->dynamicCall("Value()").toString();
            // qDebug()<<cellValue;
            rowData.append(cellValue);
        }
        data.append(rowData);
    }
    //根据data的行数、列数创建表格项
    tableWidget->setRowCount(rowNum);
    tableWidget->setColumnCount(columnNum);
    tableWidget->setHorizontalHeaderLabels(data[0]);
    for(int i=1;i<rowNum;i++){
        for(int j=0;j<columnNum;j++){
            QTableWidgetItem *cel=new QTableWidgetItem();
            cel->setText(data[i][j]);
            tableWidget->setItem(i,j,cel);
        }
    }
    workbooks->dynamicCall("Close()");//关闭工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}

void Widget::save_excel(){
   QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Excel文件(*.xlsx);;所有文件(*.*)";
    QString title="保存Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿集
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");//添加一个工作簿
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //获取工作表集
    QAxObject *sheets=workbook->querySubObject("Sheets");
    //获取活动工作表
    //sheets->dynamicCall("Add");//添加一个工作表
    QAxObject *sheet=workbook->querySubObject("ActiveSheet");
    //创建元素类型为字符串的二维列表lists
    QList<QStringList> lists;
    QStringList temp1;
    int rowNum=tableWidget->rowCount();
    int columnNum=tableWidget->columnCount();
    //将表头数据添加到lists列表中
    for(int j=0;j<columnNum;j++){
        temp1.append(tableWidget->horizontalHeaderItem(j)->text());
    }
    lists.append(temp1);
    //将表格数据添加到lists列表中
    for(int i=1;i<rowNum;i++){
        QStringList temp2;
        for(int j=0;j<columnNum;j++){
            temp2.append(tableWidget->item(i,j)->text());
        }
        lists.append(temp2);
    }
    //qDebug()<<lists;
    //向Excel的单元格中写入数据
    for(int i=1;i<=rowNum;i++){
        for(int j=1;j<=columnNum;j++){
            QAxObject *cell=sheet->querySubObject("Cells(int,int)", i,j);
            cell->dynamicCall("SetValue(const QVariant&)",QVariant(lists[i-1][j-1]));
        }
    }
    //保存Excel
    workbook->dynamicCall("SaveAs(const QString&)",fileName);
    QMessageBox::information(this,"提示消息","文件保存成功！");
    workbooks->dynamicCall("Close()");//关闭Excel工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}
