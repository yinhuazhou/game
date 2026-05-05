#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("图形控件的布局");
    //创建图像视图控件
    view=new QGraphicsView();
    //创建图像场景控件
    scene=new QGraphicsScene();
    //图像视图设置场景
    view->setScene(scene);
    //设置窗口布局
    vbox=new QVBoxLayout(this);
    vbox->addWidget(view);
    //创建图形控件
    widget=new QGraphicsWidget();
    widget->setFlags(QGraphicsWidget::ItemIsMovable|QGraphicsWidget::ItemIsSelectable);
    //向图像场景中添加图形控件
    scene->addItem(widget);
    //设置栅格布局
    grid=new QGraphicsGridLayout(widget);
    //创建9个按钮控件
    btn1=new QPushButton("按钮控件1");
    btn2=new QPushButton("按钮控件2");
    btn3=new QPushButton("按钮控件3");
    btn4=new QPushButton("按钮控件4");
    btn5=new QPushButton("按钮控件5");
    btn6=new QPushButton("按钮控件6");
    btn7=new QPushButton("按钮控件7");
    btn8=new QPushButton("按钮控件8");
    btn9=new QPushButton("按钮控件9");
    //创建代理控件，设置控件
    p1=new QGraphicsProxyWidget();p1->setWidget(btn1);
    p2=new QGraphicsProxyWidget();p2->setWidget(btn2);
    p3=new QGraphicsProxyWidget();p3->setWidget(btn3);
    p4=new QGraphicsProxyWidget();p4->setWidget(btn4);
    p5=new QGraphicsProxyWidget();p5->setWidget(btn5);
    p6=new QGraphicsProxyWidget();p6->setWidget(btn6);
    p7=new QGraphicsProxyWidget();p7->setWidget(btn7);
    p8=new QGraphicsProxyWidget();p8->setWidget(btn8);
    p9=new QGraphicsProxyWidget();p9->setWidget(btn9);
    //向线性布局中添加控件
    grid->addItem(p1,0,0);grid->addItem(p2,0,1);grid->addItem(p3,0,2);
    grid->addItem(p4,1,0);grid->addItem(p5,1,1);grid->addItem(p6,1,2);
    grid->addItem(p7,2,0);grid->addItem(p8,2,1);grid->addItem(p9,2,2);
    grid->setSpacing(10);
}

Widget::~Widget() {}
