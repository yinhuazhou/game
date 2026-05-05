#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("显示数据表");
    vbox=new QVBoxLayout(this);
    //创建表格控件
    tableWidget=new QTableWidget();
    tableWidget->setFont(QFont("黑体",14));
    vbox->addWidget(tableWidget);
    open_database();
}

Widget::~Widget() {}

void Widget::open_database(){
    //数据库的路径和名称
    QString dbName="D:\\Chapter14\\student2.db";
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    QList<QStringList> data;//创建二维列表data
    if(db.open()){
        QSqlQuery query(db);
        //将数据表中的数据转存到二维列表data中
        if(query.exec("SELECT * FROM score1")){
            while(query.next()){
                QVariant id=query.value("ID");
                QVariant name=query.value("name");
                QVariant chinese=query.value("语文");
                QVariant math=query.value("数学");
                QVariant english=query.value("英文");
                QStringList temp={id.toString(),name.toString(),chinese.toString(),math.toString(),english.toString()};
                data.append(temp);
            }
        }
    }
    db.close();
    //根据二维列表data的行数、列数创建表格控件
    int rowNum=data.size();
    int columnNum=data[0].size();
    QStringList labels={"学号","姓名","语文","数学","英文"};
    tableWidget->setRowCount(rowNum);
    tableWidget->setColumnCount(columnNum);
    tableWidget->setHorizontalHeaderLabels(labels);
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<columnNum;j++){
            QTableWidgetItem *cell=new QTableWidgetItem();
            cell->setText(data[i][j]);
            tableWidget->setItem(i,j,cell);
        }
    }
}
