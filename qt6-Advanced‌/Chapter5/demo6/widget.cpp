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
    //设置线性布局
    linear=new QGraphicsLinearLayout(Qt::Vertical,widget);
    //创建标签控件并显示图像
    label=new QLabel("标签控件");
    QPixmap pix1("D:\\Chapter5\\images\\cat1.png");
    QPixmap pix2=pix1.scaled(380,220);//缩放图像文件
    label->setPixmap(pix2);
    //创建2个按钮控件
    btn1=new QPushButton("按钮控件1");
    btn2=new QPushButton("按钮控件2");
    //创建代理控件，设置控件
    p1=new QGraphicsProxyWidget();p1->setWidget(label);
    p2=new QGraphicsProxyWidget();p2->setWidget(btn1);
    p3=new QGraphicsProxyWidget();p3->setWidget(btn2);
    //向线性布局中添加控件
    linear->addItem(p1);linear->addItem(p2);linear->addItem(p3);
    linear->setSpacing(5);
    linear->setStretchFactor(p1,1);
    linear->setStretchFactor(p2,2);
    linear->setStretchFactor(p3,2);
}

Widget::~Widget() {}
