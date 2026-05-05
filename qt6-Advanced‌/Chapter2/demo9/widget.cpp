#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,300);
    setWindowTitle("QTreeWidget、QTreeWidgetItem");
    vbox=new QVBoxLayout(this);
    //创建树结构控件
    treeWidget=new QTreeWidget();
    treeWidget->setFont(QFont("黑体",12));
    label=new QLabel("提示：");
    label->setFont(QFont("楷体",14));
    vbox->addWidget(treeWidget);
    vbox->addWidget(label);
    //向树结构中添加表头数据
    treeWidget->setColumnCount(2);
    QTreeWidgetItem *header=new QTreeWidgetItem();
    header->setText(0,"地区范围");
    header->setText(1,"人口数量（万人）");
    header->setTextAlignment(0,Qt::AlignLeft);
    header->setTextAlignment(1,Qt::AlignLeft);
    treeWidget->setHeaderItem(header);
    //添加顶层项
    QTreeWidgetItem *topItem1=new QTreeWidgetItem(treeWidget);
    topItem1->setText(0,"东北");
    QTreeWidgetItem *child1=new QTreeWidgetItem(topItem1,{"黑龙江","3099"});
    QTreeWidgetItem *child2=new QTreeWidgetItem(topItem1,{"吉林","2399"});
    QTreeWidgetItem *child3=new QTreeWidgetItem(topItem1,{"辽宁"});
    QTreeWidgetItem *child4=new QTreeWidgetItem(child3,{"沈阳","915"});
    QTreeWidgetItem *child5=new QTreeWidgetItem(child3,{"大连","753"});
    //添加顶层项
    QTreeWidgetItem *topItem2=new QTreeWidgetItem(treeWidget);
    topItem2->setText(0,"华东");
    QTreeWidgetItem *child6=new QTreeWidgetItem(topItem2,{"江苏","8526"});
    QTreeWidgetItem *child7=new QTreeWidgetItem(topItem2,{"上海","2475"});
    treeWidget->expandAll();
    //使用信号/槽
    connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(clicked_treeWidget(QTreeWidgetItem*,int)));
}

Widget::~Widget() {}

void Widget::clicked_treeWidget(QTreeWidgetItem *item, int column){
    if(item->text(1)!=""){
        QString str="地区范围："+item->text(0)+",人口数量（万人）："+item->text(1);
        label->setText(str);
    }
}
