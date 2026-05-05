#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,560,220);
    setWindowTitle("QListWidget、QListWidgetItem");
    vbox=new QVBoxLayout(this);
    //创建列表控件
    listWidget=new QListWidget();
    listWidget->setFont(QFont("黑体",14));
    vbox->addWidget(listWidget);
}

Widget::~Widget() {}

void Widget::contextMenuEvent(QContextMenuEvent *e){
    menu=new QMenu(this);
    actionAdd=menu->addAction("添加");
    actionEdit=menu->addAction("编辑");
    actionRemove=menu->addAction("删除");
    menu->addSeparator();
    actionSelectAll=menu->addAction("全选");
    actionInverse=menu->addAction("反选");
    actionSelectNone=menu->addAction("全不选");
    connect(actionAdd,SIGNAL(triggered()),this,SLOT(add_item()));
    connect(actionEdit,SIGNAL(triggered()),this,SLOT(edit_item()));
    connect(actionRemove,SIGNAL(triggered()),this,SLOT(remove_item()));
    connect(actionSelectAll,SIGNAL(triggered()),this,SLOT(select_all()));
    connect(actionInverse,SIGNAL(triggered()),this,SLOT(inverse_select()));
    connect(actionSelectNone,SIGNAL(triggered()),this,SLOT(select_none()));
    menu->exec(e->globalPos());//显示上下文菜单
}

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

void Widget::select_all(){
    int count=listWidget->count();
    for(int i=0;i<count;i++){
        QListWidgetItem *item=listWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void Widget::inverse_select(){
    int count=listWidget->count();
    for(int i=0;i<count;i++){
        QListWidgetItem *item=listWidget->item(i);
        if(item->checkState()==Qt::Unchecked)
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
    }
}

void Widget::select_none(){
    int count=listWidget->count();
    for(int i=0;i<count;i++){
        QListWidgetItem *item=listWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}
