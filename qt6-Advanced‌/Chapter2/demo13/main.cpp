#include "widget.h"

#include <QApplication>
#include <QAxObject>
#include <QDebug>
#include <QString>
#include <QList>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open(const QString&)","D:\\Chapter12\\001.xlsx");
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //获取工作表
    QAxObject *sheets=workbook->querySubObject("WorkSheets");
    QAxObject *sheet=workbook->querySubObject("Sheets(int)",1);
    QAxObject *cell=sheet->querySubObject("Range(QVariant,QVariant)","C3");
    QString str=cell->dynamicCall("Value()").toString();
    qDebug()<<str;
    // 获取工作表的最大行列数
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
    qDebug()<<data;
    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
    return a.exec();
}
