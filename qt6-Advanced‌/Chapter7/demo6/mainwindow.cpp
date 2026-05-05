#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("Q3DSurface、QSurface3DSeries、QSurfaceDataProxy");
    //创建三维曲面图表
    graph3D=new Q3DSurface();
    //创建三维图表容器
    container=createWindowContainer(graph3D);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);
    setCentralWidget(container);

    dataProxy=new QSurfaceDataProxy();//创建三维曲面图的数据代理
    series=new QSurface3DSeries(dataProxy);//根据数据代理创建数据序列
    series->setItemLabelFormat("(x,z,y)=(@xLabel,@zLabel,@yLabel)");
    series->setMeshSmooth(true);//使用预定义网格的平滑版本
    series->setMesh(QAbstract3DSeries::MeshSphere);//设置单点的样式
    graph3D->addSeries(series);
    //设置视角
    Q3DCamera::CameraPreset camView=Q3DCamera::CameraPresetFrontHigh;
    graph3D->scene()->activeCamera()->setCameraPreset(camView);

    //获取三维图表的坐标轴、设置坐标轴
    axisX=new QValue3DAxis();
    axisX->setTitle("Axis X");
    axisX->setTitleVisible(true);
    axisX->setRange(-10,10);
    graph3D->setAxisX(axisX);

    axisY=new QValue3DAxis();   //垂直方向的坐标轴
    axisY->setTitle("Axis Y");
    axisY->setTitleVisible(true);
    axisY->setAutoAdjustRange(true);//垂直方向自动调整范围
    graph3D->setAxisY(axisY);

    axisZ=new QValue3DAxis();
    axisZ->setTitle("Axis Z");
    axisZ->setTitleVisible(true);
    axisZ->setRange(-8,8);
    //axisZ->setAutoAdjustRange(true);
    graph3D->setAxisZ(axisZ);

    //创建数据代理的代理项
    int N=60;      //单重循环的次数
    float x=-6,y,z;
    itemArray=new QSurfaceDataArray();
    for(int i=0;i<N;i++){
        QSurfaceDataRow *row=new QSurfaceDataRow();
        y=-6;
        for(int j=0;j<N;j++){
            float z0=qSqrt(pow(x,2)+pow(y,2));
            z=cos(z0);
            QVector3D vector3D(x,z,y);//三维坐标点
            QSurfaceDataItem item(vector3D);//三维曲面的数据项
            row->append(item);            //向row中添加一个数据项
            y=y+0.2;
        }
        dataProxy->addRow(row);//在末尾添加一行数据项
        x=x+0.2;
    }
}

MainWindow::~MainWindow() {}
