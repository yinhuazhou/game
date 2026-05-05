#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QGraphicsItem");
    //设置布局
    vbox=new QVBoxLayout(this);
    //创建图像视图控件
    graphicsView=new QGraphicsView();
    vbox->addWidget(graphicsView);
    int w=500;                      //正弦曲线图形项的宽度
    int h=230;                      //正弦曲线图形项的高度
    QRectF rect(-10,-10-h/2,w,h);   //场景的范围
    //创建图像场景
    graphicsScene=new QGraphicsScene(rect);
    //图像视图设置图像场景
    graphicsView->setScene(graphicsScene);
    item1=new Ellipse(w,h);             //自定义椭圆图形项
    item2=new Cos(w,h);                 //自定义正弦曲线图形项
    item2->setParentItem(item1);        //设置图形项的父子关系
    graphicsScene->addItem(item1);      //添加自定义图形项
    //添加矩形边框
    QGraphicsRectItem *item3=graphicsScene->addRect(rect);
    //创建图形项组合
    group=graphicsScene->createItemGroup({item1,item3});
    //设置图形项组合可移动
    group->setFlag(QGraphicsItem::ItemIsMovable);
}

Widget::~Widget() {}
