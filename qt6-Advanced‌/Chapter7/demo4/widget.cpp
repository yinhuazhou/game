#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,300);
    setWindowTitle("Q3DTheme");
    //创建按钮控件及其布局
    hbox=new QHBoxLayout();
    btnType1=new QPushButton("主题类型1");
    btnType2=new QPushButton("主题类型2");
    btnBack=new QPushButton("显示标签背景");
    btnGrid=new QPushButton("隐藏网格线");
    hbox->addWidget(btnType1);
    hbox->addWidget(btnType2);
    hbox->addWidget(btnBack);
    hbox->addWidget(btnGrid);
    //设置窗口的布局方式
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
    connect(btnType1,SIGNAL(clicked()),this,SLOT(change_type1()));
    connect(btnType2,SIGNAL(clicked()),this,SLOT(change_type2()));
    connect(btnBack,SIGNAL(clicked()),this,SLOT(show_back()));
    connect(btnGrid,SIGNAL(clicked()),this,SLOT(hide_grid()));
}

Widget::~Widget() {}

void Widget::change_type1(){
    graph3D->activeTheme()->setType(Q3DTheme::ThemeArmyBlue);
}

void Widget::change_type2(){
    graph3D->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
}

void Widget::show_back(){
    graph3D->activeTheme()->setLabelBackgroundEnabled(true);
    graph3D->activeTheme()->setBackgroundColor(Qt::red);
}

void Widget::hide_grid(){
    graph3D->activeTheme()->setGridEnabled(false);
}
