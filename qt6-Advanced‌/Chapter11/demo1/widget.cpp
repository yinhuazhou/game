#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("操作Word文件");
    //创建水平布局对象，并添加2个按钮
    hbox=new QHBoxLayout();
    btnOpen=new QPushButton("打开Word文件");
    btnSave=new QPushButton("保存Word文件");
    hbox->addWidget(btnOpen);
    hbox->addWidget(btnSave);
    //创建垂直布局对象，并添加其他布局、控件
    vbox=new QVBoxLayout(this);
    textEdit=new QPlainTextEdit();
    vbox->addLayout(hbox);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
    connect(btnSave,SIGNAL(clicked()),this,SLOT(btn_save()));
}

Widget::~Widget() {}

void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Word文件(*.docx *.doc);;所有文件(*.*)";
    QString title="打开Word文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //清空多行纯文本框中的内容
    textEdit->clear();
    //创建Word应用程序
    wordApp=new QAxObject("Word.Application");
    //设置可见
    wordApp->setProperty("Visible",true);
    //创建文档集
    docs=wordApp->querySubObject("Documents");
    //打开Word文件
    docs->dynamicCall("Open(const QString&)",fileName);
    //当前活跃文档
    doc=wordApp->querySubObject("ActiveDocument");
    //所有的段落
    QAxObject *paragraphs=doc->querySubObject("Paragraphs");
    if(!paragraphs)
        return;
    int num=paragraphs->property("Count").toInt();
    //显示所有段落中的文本块
    for(int i=1;i<num+1;i++){
        QAxObject *paragraph=paragraphs->querySubObject("Item(int)",i);
        if(paragraph){
            QAxObject *range=paragraph->querySubObject("Range");//段落中的文本块
            QVariant strV=range->property("Text");
            QString str=strV.toString();
            textEdit->appendPlainText(str);
        }
    }
    doc->dynamicCall("Close()");//关闭文档
    wordApp->dynamicCall("Quit()");//退出Word应用程序
}

void Widget::btn_save(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="Word文件(*.docx);;所有文件(*.*)";
    QString title="保存Excel文件";//文件对话框的标题
    QString fileName=QFileDialog::getSaveFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    fileName.replace('/','\\');//将字符串中的/替换为\\
    //创建Word应用程序
    wordApp=new QAxObject("Word.Application");
    //设置可见
    wordApp->setProperty("Visible",true);
    //创建文档集
    docs=wordApp->querySubObject("Documents");
    //打开Word文件
    docs->dynamicCall("Open(const QString&)",fileName);
    //创建一个新文档
    doc=docs->querySubObject("Add()");
    //获取多行纯文本框中的文本
    QString text=textEdit->toPlainText();
    //创建文本块对象
    QAxObject *range=doc->querySubObject("Range()");
    //插入文本
    range->dynamicCall("InsertAfter(const QString&)",text);
    //保存Word文件
    doc->dynamicCall("SaveAs(const QString&)",fileName);
    QMessageBox::information(this,"提示消息","文件保存成功！");
    doc->dynamicCall("Close()");//关闭文档
    wordApp->dynamicCall("Quit()");//退出Word应用程序
}
