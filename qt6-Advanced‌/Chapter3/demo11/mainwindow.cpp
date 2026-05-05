#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,620,300);
    setWindowTitle("QListView、QTableView、QStandardItemModel");
    //创建菜单栏、菜单、动作
    menuBar=new QMenuBar(this);
    fileMenu=menuBar->addMenu("文件");
    actionOpen=fileMenu->addAction("打开");
    actionSave=fileMenu->addAction("保存");
    setMenuBar(menuBar);//设置主窗口的菜单栏
    //创建包含2个视图控件、分割器的框架控件
    listView=new QListView();
    tableView=new QTableView();
    hSplitter=new QSplitter(Qt::Horizontal);//创建分割器
    frame=new QFrame();
    //向分割器中添加控件
    hSplitter->addWidget(listView);
    hSplitter->addWidget(tableView);
    hbox=new QHBoxLayout(frame);
    hbox->addWidget(hSplitter);
    setCentralWidget(frame);//设置主窗口的中心控件
    //创建数据模型
    standardModel=new QStandardItemModel();
    //使用信号/槽
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(action_open()));
    connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(listView_clicked(QModelIndex)));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(action_save()));
}

MainWindow::~MainWindow() {}

void MainWindow::action_open(){
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
    //获取工作表的数量
    QAxObject *sheets=workbook->querySubObject("WorkSheets");
    QVariant sheetNumV=sheets->property("Count");
    int sheetNum=sheetNumV.toInt();
    //遍历并获取工作表的名称
    for(int i=1;i<=sheetNum;i++){
        //获取指定索引的工作表
        QAxObject *sheet=workbook->querySubObject("Sheets(int)",i);
        //获取工作表的名称
        QVariant sheetNameV=sheet->property("Name");
        QString sheetName=sheetNameV.toString();
        //获取工作表的最大行数、列数
        QAxObject *usedRange=sheet->querySubObject("UsedRange");
        QAxObject *rows=usedRange->querySubObject("Rows");
        QAxObject *columns=usedRange->querySubObject("Columns");
        int rowNum=rows->property("Count").toInt();
        int columnNum=columns->property("Count").toInt();
        //将工作表中的数据读取到二维列表data中
        QList<QStringList> data;
        for (int row = 1; row <= rowNum; row++) {
            QStringList rowData;
            for (int col = 1; col <=columnNum; col++) {
                // 获取单元格对象并读取其值
                QAxObject *cell=sheet->querySubObject("Cells(int,int)", row,col);
                QString cellValue = cell->dynamicCall("Value()").toString();
                rowData.append(cellValue);
            }
            data.append(rowData);
        }
        //将二维列表转换为有层次的QStandardItem指针
        QStandardItem *parentItem=new QStandardItem(sheetName);//根索引下的顶层数据项
        parentItem->setColumnCount(data[0].size());//设置列的数量
        //将二维列表转换为数据项，并添加到标准数据模型下
        for(int i=1;i<rowNum;i++){
            QList<QStandardItem *> tempItems;
            for(int j=0;j<columnNum;j++){
                QStandardItem *item=new QStandardItem(data[i][j]);//子数据项
                item->setTextAlignment(Qt::AlignCenter);
                tempItems.append(item);
            }
            parentItem->appendRow(tempItems);//将子数据项添加到顶层项中
            standardModel->appendRow(parentItem);
        }
        //设置水平表头
        standardModel->setHorizontalHeaderLabels(data[0]);
        //设置视图控件的数据模型
        listView->setModel(standardModel);
        tableView->setModel(standardModel);
        //设置表格视图控件的数据模型
        QModelIndex index=standardModel->index(0,0);
        tableView->setRootIndex(index);
    }
    workbooks->dynamicCall("Close()");//关闭工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}

void MainWindow::listView_clicked(QModelIndex index){
    QStandardItem *item=standardModel->itemFromIndex(index);
    if(item->hasChildren()){
        tableView->setRootIndex(index);
        int rowCount=item->rowCount();
        QStringList labels;
        for(int i=1;i<=rowCount;i++){
            QString text=QString::number(i);
            labels.append(text);
        }
        //设置列表头显示的文字
        standardModel->setVerticalHeaderLabels(labels);
    }
}

void MainWindow::action_save(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Excel文件(*.xlsx);;所有文件(*.*)";
    QString title="保存Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //获取根索引下数据项的数量
    int sheetNum=standardModel->rowCount(QModelIndex());
    //创建Excel应用程序对象
    QAxObject *excel=new QAxObject("Excel.Application");
    //打开工作簿集
    QAxObject *workbooks=excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");//添加一个工作簿
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    //获取工作表集
    QAxObject *sheets=workbook->querySubObject("Sheets");
    for(int i=0;i<sheetNum;i++){
        //获取顶层索引
        QModelIndex parentIndex=standardModel->index(i,0,QModelIndex());
        //获取顶层数据项
        QStandardItem *parentItem=standardModel->itemFromIndex(parentIndex);
        if(parentItem->hasChildren()){
            //获取活动工作表
            QAxObject *sheet=workbook->querySubObject("ActiveSheet");
            //创建元素类型为字符串的二维列表lists
            QList<QStringList> lists;
            QStringList temp1;
            int rowNum=standardModel->rowCount(parentIndex);
            int columnNum=standardModel->columnCount(parentIndex);
            //将表头数据添加到lists列表中
            for(int j=0;j<columnNum;j++){
                QStandardItem *item=standardModel->horizontalHeaderItem(j);
                QVariant v=item->data(Qt::DisplayRole);
                temp1.append(v.toString());
            }
            lists.append(temp1);
            //将表格数据添加到lists列表中
            for(int row=0;row<rowNum;row++){
                QStringList temp2;
                for(int col=0;col<columnNum;col++){
                    QStandardItem *item=parentItem->child(row,col);
                    QVariant value=item->data(Qt::DisplayRole);
                    temp2.append(value.toString());
                }
                lists.append(temp2);
            }
            //向Excel的单元格中写入数据
            for(int i=1;i<=rowNum+1;i++){
                for(int j=1;j<=columnNum;j++){
                    QAxObject *cell=sheet->querySubObject("Cells(int,int)", i,j);
                    cell->dynamicCall("SetValue(const QVariant&)",QVariant(lists[i-1][j-1]));
                }
            }
            sheets->dynamicCall("Add");//添加一个工作表
        }
    }

    //保存Excel
    workbook->dynamicCall("SaveAs(const QString&)",fileName);
    QMessageBox::information(this,"提示消息","文件保存成功！");
    workbooks->dynamicCall("Close()");//关闭Excel工作簿集
    excel->dynamicCall("Quit()");//关闭Excel应用程序
    //删除不需要的指针
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;
}
