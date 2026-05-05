#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setWindowTitle("代理控件");
    pixWidget=new PixmapWidget();//创建自定义窗口
    graphicsView=new QGraphicsView();//创建图像视图控件
    graphicsScene=new QGraphicsScene();//创建图像场景
    graphicsView->setScene(graphicsScene);//设置图像视图中的场景
    proxy=new QGraphicsProxyWidget(nullptr,Qt::Window);//创建代理控件
    proxy->setWidget(pixWidget);//代理控件中设置代理控件
    QTransform matrix;
    proxy->setTransform(matrix.shear(-0.8,-0.1));//错切变换
    graphicsScene->addItem(proxy);//在图像场景中添加代理控件
    vbox=new QVBoxLayout(this);//设置布局
    vbox->addWidget(graphicsView);
}

Widget::~Widget() {}
