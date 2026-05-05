#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("使用Graphics/View绘图");
    //设置窗口的布局为垂直布局
    vbox=new QVBoxLayout(this);
    //创建图像视图
    graphicsView=new QGraphicsView();
    graphicsView->setBackgroundBrush(Qt::gray);
    vbox->addWidget(graphicsView);
    //创建矩形范围
    QRectF rectf1(-20,-20,400,200);
    //创建图像场景
    graphicsScene=new QGraphicsScene(rectf1);
    //图像视图设置图像场景
    graphicsView->setScene(graphicsScene);
    //根据图像场景范围创建矩形图形项
    rectItem=new QGraphicsRectItem(rectf1);
    rectItem->setBrush(Qt::yellow);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    //向图像场景中添加矩形图形项
    graphicsScene->addItem(rectItem);
    //创建椭圆图形项
    QRectF rectf2(-40,-30,80,50);
    ellipseItem=new QGraphicsEllipseItem(rectf2);
    ellipseItem->setBrush(Qt::red);//设置画刷
    ellipseItem->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    //向图像场景中添加椭圆图形项
    graphicsScene->addItem(ellipseItem);
}

Widget::~Widget() {}
