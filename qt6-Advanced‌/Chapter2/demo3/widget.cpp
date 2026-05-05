#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,260);
    setWindowTitle("QListWidget、QListWidgetItem");
    vbox=new QVBoxLayout(this);
    //创建4个按钮
    btnAdd=new QPushButton("添加");
    btnEdit=new QPushButton("编辑");
    btnRemove=new QPushButton("删除");
    btnSort=new QPushButton("排序");
    hbox=new QHBoxLayout();
    hbox->addWidget(btnAdd);
    hbox->addWidget(btnEdit);
    hbox->addWidget(btnRemove);
    hbox->addWidget(btnSort);
    vbox->addLayout(hbox);
    //使用信号/槽
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(add_item()));
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(edit_item()));
    connect(btnRemove,SIGNAL(clicked()),this,SLOT(remove_item()));
    connect(btnSort,SIGNAL(clicked()),this,SLOT(sort_item()));
    //创建列表控件
    listWidget=new QListWidget();
    listWidget->setFont(QFont("黑体",14));
    vbox->addWidget(listWidget);
}

Widget::~Widget() {}

void Widget::add_item(){
    int row=listWidget->currentRow();
    QString title="添加项";
    bool ok;
    QString data=QInputDialog::getText(this,title,title,QLineEdit::Normal,QDir::home().dirName(),&ok);
    if(ok && !data.isEmpty())
        listWidget->insertItem(row,data);
}

void Widget::edit_item(){
    QListWidgetItem *item=listWidget->currentItem();
    if(item==nullptr)
        return;
    QString title="编辑项";
    bool ok;
    QString data=QInputDialog::getText(this,title,title,QLineEdit::Normal,item->text(),&ok);
    if(ok && !data.isEmpty())
        item->setText(data);
}

void Widget::remove_item(){
    int row=listWidget->currentRow();
    QListWidgetItem *item=listWidget->item(row);
    if(item==nullptr)
        return;
    QString title="删除项";
    QString label="确定要删除？";
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,title,label,QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes)
        listWidget->takeItem(row);
}

void Widget::sort_item(){
    listWidget->sortItems();
}
