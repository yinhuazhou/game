#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("Q3DScatter、QScatter3DSeries、QScatterDataProxy");
    //创建三维散点图表
    graph3D=new Q3DScatter();
    //创建三维图表容器
    container=createWindowContainer(graph3D);
    setCentralWidget(container);
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
    series->setMesh(QAbstract3DSeries::MeshPyramid);//设置散点的形状
    series->setItemSize(0.15);//设置散点的大小，范围0.0~1.0
    //设置视角
    Q3DCamera::CameraPreset camView=Q3DCamera::CameraPresetFrontHigh;
    graph3D->scene()->activeCamera()->setCameraPreset(camView);
    //创建数据代理的代理项
    int N=440;      //数据点总数
    float x,y,z=0;
    itemArray=new QScatterDataArray();
    for(int i=0;i<N;i++){
        x=2*sin(20*z);
        y=2*cos(20*z);
        QVector3D vector3D(x,z,y);//三维坐标点
        QScatterDataItem item(vector3D);//空间中的一个散点数据项
        itemArray->append(item);
        z=z+0.005;
    }
    dataProxy->resetArray(itemArray);//重置数据代理的数组
}

MainWindow::~MainWindow() {}
