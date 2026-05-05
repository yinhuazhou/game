#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setWindowTitle("QPrintPreviewDialog");
    showMaximized();
    //创建菜单栏、添加菜单、动作
    menuBar=new QMenuBar();
    fileMenu=menuBar->addMenu("文件");
    openAction=fileMenu->addAction("显示内容");
    previewAction=fileMenu->addAction("打印预览");
    printAction=fileMenu->addAction("打印");
    fileMenu->addSeparator();
    exitAction=fileMenu->addAction("退出");
    //创建多行文本输入控件
    textEdit=new QTextEdit();
    //创建垂直布局控件，并添加控件
    vbox=new QVBoxLayout(this);
    vbox->addWidget(menuBar);
    vbox->addWidget(textEdit);
    //使用信号/槽
    connect(openAction,SIGNAL(triggered()),this,SLOT(open_action()));
    connect(previewAction,SIGNAL(triggered()),this,SLOT(preview_action()));
    connect(printAction,SIGNAL(triggered()),this,SLOT(print_action()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
}

Widget::~Widget() {}
//向多行文本框中添加文本
void Widget::open_action(){
    QFont font=textEdit->font();
    font.setPointSize(30);
    font.setFamily("楷体");
    textEdit->setFont(font);
    for(int i=0;i<2;i++){
        textEdit->append("温故而知新，可以为师矣。");
        textEdit->append("学而时习之，不亦悦乎！");
        textEdit->append("学而不思则罔，思而不学则殆。");
        textEdit->append("逝者如斯夫，不舍昼夜。");
        textEdit->append("知之为知之，不知为不知，是知也。");
        textEdit->append("吾十有五而志于学，三十而立，四十而不惑，五十而知天命，六十而耳顺，七十而从心所欲不逾矩");
        textEdit->append("知人者智，自知者明。胜人者有力，自胜者强。知足者富。强行者有志。");
    }
}
//显示打印预览对话框
void Widget::preview_action(){
    previewDialog=new QPrintPreviewDialog(this,Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
    //使用信号/槽
    connect(previewDialog,SIGNAL(paintRequested(QPrinter*)),this,SLOT(preview_paintRequested(QPrinter*)));
    previewDialog->exec();
    printer=previewDialog->printer();
}
//与信号连接槽函数
void Widget::preview_paintRequested(QPrinter *printer){
    textEdit->print(printer);//打印多行文本输入框的内容
}
//显示打印对话框
void Widget::print_action(){
    printDialog=new QPrintDialog(printer);
    //使用信号/槽
    connect(printDialog,SIGNAL(accepted(QPrinter*)),this,SLOT(printDialog_accepted(QPrinter*)));
    printDialog->exec();
}
//与信号连接的槽函数
void Widget::printDialog_accepted(QPrinter *printer){
    textEdit->print(printer);//打印多行文本输入框的内容
}
