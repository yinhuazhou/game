#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setWindowTitle(tr("多语言界面"));
    setGeometry(300,300,560,220);
    layout=new QGridLayout(this);
    btnChinese=new QPushButton(tr("中文"));
    btnEnglish=new QPushButton(tr("英文"));
    btnOne=new QPushButton(tr("一"));
    btnTwo=new QPushButton(tr("二"));
    btnThree=new QPushButton(tr("三"));
    btnFile=new QPushButton(tr("文件"));
    btnEdit=new QPushButton(tr("编辑"));
    btnCopy=new QPushButton(tr("复制"));
    btnCut=new QPushButton(tr("剪切"));
    btnPaste=new QPushButton(tr("粘贴"));
    layout->addWidget(btnChinese,0,0);
    layout->addWidget(btnEnglish,0,1);
    layout->addWidget(btnOne,0,2);
    layout->addWidget(btnTwo,0,3);
    layout->addWidget(btnThree,0,4);
    layout->addWidget(btnFile,1,0);
    layout->addWidget(btnEdit,1,1);
    layout->addWidget(btnCopy,1,2);
    layout->addWidget(btnCut,1,3);
    layout->addWidget(btnPaste,1,4);

}

Widget::~Widget() {}

