#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QHeightMapSurfaceDataProxy");
    //创建三维曲面图表
    graph3D=new Q3DSurface();
    //创建三维图表容器
    container=createWindowContainer(graph3D);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);
    setCentralWidget(container);
    //读取图像文件、创建QImage对象
    QImage heightMapImage("D://Chapter7//mountain1.png");//灰度图片
    //创建数据代理
    dataProxy=new QHeightMapSurfaceDataProxy(heightMapImage);
    dataProxy->setValueRanges(-5000,5000,-5000,5000);
    //创建数据序列
    series=new QSurface3DSeries(dataProxy);
    series->setItemLabelFormat("(x,z,y)=(@xLabel,@zLabel,@yLabel)");
    series->setFlatShadingEnabled(false);//曲面更光滑
    series->setMeshSmooth(true);//使用预定义网格的平滑版本
    series->setDrawMode(QSurface3DSeries::DrawSurface);//只画曲面
    series->setMesh(QAbstract3DSeries::MeshSphere);//设置单点的样式
    graph3D->addSeries(series);
    //获取三维图表的坐标轴、设置坐标轴
    axisX=new QValue3DAxis();
    axisX->setTitle("AxisX:西--东");
    axisX->setTitleVisible(true);
    axisX->setLabelFormat("% .1f 米");
    axisX->setRange(-5000,5000);
    graph3D->setAxisX(axisX);

    axisY=new QValue3DAxis();
    axisY->setTitle("AxisY:高度");
    axisY->setTitleVisible(true);
    axisY->setRange(-10,10);
    axisY->setAutoAdjustRange(true);//垂直方向自动调整范围
    graph3D->setAxisY(axisY);

    axisZ=new QValue3DAxis();
    axisZ->setTitle("AxisZ:南--北");
    axisZ->setTitleVisible(true);
    axisZ->setRange(-5000,5000);
    axisZ->setAutoAdjustRange(true);
    graph3D->setAxisZ(axisZ);
}

MainWindow::~MainWindow() {}
