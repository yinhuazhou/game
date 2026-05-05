#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QVector3D");
    //创建设置图表平移的按钮和布局
    hbox=new QHBoxLayout();
    btnLeft=new QPushButton("左移");
    btnRight=new QPushButton("右移");
    btnUp=new QPushButton("上移");
    btnDown=new QPushButton("下移");
    btnFar=new QPushButton("远移");
    btnNear=new QPushButton("近移");
    hbox->addWidget(btnLeft);
    hbox->addWidget(btnRight);
    hbox->addWidget(btnUp);
    hbox->addWidget(btnDown);
    hbox->addWidget(btnFar);
    hbox->addWidget(btnNear);
    //设置窗口的布局方式为垂直布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox);
    //创建三维散点图表
    graph3D=new Q3DScatter();
    //创建三维图表容器
    container=createWindowContainer(graph3D);
    vbox->addWidget(container);
    dataProxy=new QScatterDataProxy();//创建三维散点图的数据代理
    series=new QScatter3DSeries(dataProxy);//根据数据代理创建数据序列
    series->setItemLabelFormat("(x,z,y)=(@xLabel,@zLabel,@yLabel)");
    series->setMeshSmooth(true);//使用预定义网格的平滑版本
    graph3D->addSeries(series);
    //获取三维图表的坐标轴、设置坐标轴
    graph3D->axisX()->setTitle("axis X");
    graph3D->axisX()->setTitleVisible(true);
    graph3D->axisY()->setTitle("axis Y");
    graph3D->axisY()->setTitleVisible(true);
    graph3D->axisZ()->setTitle("axis Z");
    graph3D->axisZ()->setTitleVisible(true);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);
    series->setMesh(QAbstract3DSeries::MeshSphere);//设置散点的形状
    series->setItemSize(0.15);//设置散点的大小，范围0.0~1.0
    //创建数据代理的代理项
    int N=200;      //数据点总数
    float x,y,z=0;
    itemArray=new QScatterDataArray();
    for(int i=0;i<N;i++){
        x=z*sin(20*z);
        y=z*cos(20*z);
        QVector3D vector3D(x,z,y);//三维坐标点
        QScatterDataItem item(vector3D);//空间中的一个散点数据项
        itemArray->append(item);
        z=z+0.006;
    }
    dataProxy->resetArray(itemArray);//重置数据代理的数组
    //使用信号/槽
    connect(btnLeft,SIGNAL(clicked()),this,SLOT(move_left()));
    connect(btnRight,SIGNAL(clicked()),this,SLOT(move_right()));
    connect(btnUp,SIGNAL(clicked()),this,SLOT(move_up()));
    connect(btnDown,SIGNAL(clicked()),this,SLOT(move_down()));
    connect(btnFar,SIGNAL(clicked()),this,SLOT(move_far()));
    connect(btnNear,SIGNAL(clicked()),this,SLOT(move_near()));
}

Widget::~Widget() {}

void Widget::move_left(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float x=target3D.x();
    target3D.setX(x+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void Widget::move_right(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float x=target3D.x();
    target3D.setX(x-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void Widget::move_up(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float y=target3D.y();
    target3D.setY(y-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void Widget::move_down(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float y=target3D.y();
    target3D.setY(y+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void Widget::move_far(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float z=target3D.z();
    target3D.setZ(z-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void Widget::move_near(){
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    float z=target3D.z();
    target3D.setZ(z+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}
