#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("坐标转换");
    //创建图像视图
    graphicsView=new MyGraphicsView();
    setCentralWidget(graphicsView);
    //创建状态栏
    statusB=statusBar();
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
    //使用信号/槽
    connect(graphicsView,SIGNAL(sendPosition(QPointF)),this,SLOT(mousePosition(QPointF)));
}

MainWindow::~MainWindow() {}

void MainWindow::mousePosition(QPointF pt){
    QPoint point=pt.toPoint();
    QString str="视图坐标："+QString::number(point.x())+","+QString::number(point.y());
    //将视图中的点映射到场景中
    QPointF pointScene=graphicsView->mapToScene(point);
    str=str+"  场景坐标："+QString::number(pointScene.x())+","+QString::number(pointScene.y());
    //第1种获取视图控件中图形项的方法
    QGraphicsItem *item=graphicsView->itemAt(point);
    //第2种获取视图控件中图形项的方法
    //QGraphicsItem *item=graphicsScene->itemAt(pointScene,graphicsView->transform());
    QPointF pointItem;
    if(item->isVisible()){
        //把场景坐标转换为图形项坐标
        pointItem=item->mapFromScene(pointScene);
        str=str+"  图形项坐标："+QString::number(pointItem.x())+","+QString::number(pointItem.y());
    }
    statusB->showMessage(str);//在状态栏中显示坐标信息
}
