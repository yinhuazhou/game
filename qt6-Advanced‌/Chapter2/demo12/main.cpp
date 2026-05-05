#include <QCoreApplication>
#include <QAxObject>
#include <QDebug>
#include <QVector>
#include <QVariantList>
#include <QString>

// 定义读取Excel文件的函数
QVector<QVector<QString>> readEntireExcelFile(const QString &filePath)
{
    QVector<QVector<QString>> data;

    // 创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    if (!excel) {
        qDebug() << "Failed to create Excel application object";
        return data;
    }
    //打开工作簿
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open(const QString&)","D:\\Chapter12\\001.xlsx");
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //打开活动工作表
    QAxObject *sheets=workbook->querySubObject("WorkSheets");
    QAxObject *sheet=workbook->querySubObject("Sheets(int)",1);
    QAxObject *cell=sheet->querySubObject("Range(QVariant,QVariant)","C3");
    QString str=cell->dynamicCall("Value()").toString();
    qDebug()<<str;


    // 获取工作表的最大行列数
    int numRows=sheet->property("UsedRange").value<QAxObject*>()->property("Rows").toInt();
    int numCols=sheet->property("UsedRange").value<QAxObject*>()->property("Columns").toInt();
    qDebug()<<numRows;
    qDebug()<<numCols;
   /* for (int row = 1; row <= numRows; ++row) {
        QVector<QString> rowData;
        for (int col = 1; col <= numCols; ++col) {
            // 获取单元格对象并读取其值
            QAxObject *cell = activeWorksheet->querySubObject("Range(QVariant,QVariant)", "C3");
            QString cellValue = cell->dynamicCall("Value2()").toString();
            qDebug()<<cellValue;
           rowData.append(cellValue);
        }
        data.append(rowData);
    //}*/

    // 关闭工作簿并清理资源
    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    return data;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 读取Excel文件
    QString filePath = "D:\\Chapter12\\001.xlsx";
    QVector<QVector<QString>> allData = readEntireExcelFile(filePath);

    // 打印读取到的数据
    for (const auto &row : allData) {
        for (const QString &cell : row) {
            qDebug() << cell;
        }
        qDebug() << ""; // 输出换行
    }

    return app.exec();
}
/*
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}*/
