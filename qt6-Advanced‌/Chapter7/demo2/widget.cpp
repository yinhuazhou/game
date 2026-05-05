#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("Q3DScene、Q3DCamera");
    //创建设置视角的按钮和布局
    hbox1=new QHBoxLayout();
    btnFont=new QPushButton("正前方");
    btnFontLow=new QPushButton("前下方");
    btnFontHigh=new QPushButton("前上方");
    btnLeft=new QPushButton("正左方");
    btnLeftLow=new QPushButton("左下方");
    btnLeftHigh=new QPushButton("左上方");
    hbox1->addWidget(btnFont);
    hbox1->addWidget(btnFontLow);
    hbox1->addWidget(btnFontHigh);
    hbox1->addWidget(btnLeft);
    hbox1->addWidget(btnLeftLow);
    hbox1->addWidget(btnLeftHigh);
    //创建旋转、缩放的数字输入控件
    hbox2=new QHBoxLayout();
    xLabel=new QLabel("水平旋转角度：");
    xRot=new QDoubleSpinBox();
    xRot->setRange(-180,180);
    yLabel=new QLabel("垂直旋转角度：");
    yRot=new QDoubleSpinBox();
    yRot->setRange(-180,180);
    zoomLabel=new QLabel("缩放数值：");
    zoom=new QDoubleSpinBox();
    zoom->setRange(10,500);//默认值为100
    hbox2->addWidget(xLabel);
    hbox2->addWidget(xRot);
    hbox2->addWidget(yLabel);
    hbox2->addWidget(yRot);
    hbox2->addWidget(zoomLabel);
    hbox2->addWidget(zoom);
    //设置窗口的布局方式为垂直布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
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
    connect(btnFont,SIGNAL(clicked()),this,SLOT(preset_font()));
    connect(btnFontLow,SIGNAL(clicked()),this,SLOT(preset_fontLow()));
    connect(btnFontHigh,SIGNAL(clicked()),this,SLOT(preset_fontHigh()));
    connect(btnLeft,SIGNAL(clicked()),this,SLOT(preset_left()));
    connect(btnLeftLow,SIGNAL(clicked()),this,SLOT(preset_leftLow()));
    connect(btnLeftHigh,SIGNAL(clicked()),this,SLOT(preset_leftHigh()));

    connect(xRot,SIGNAL(valueChanged(double)),this,SLOT(x_rotation(double)));
    connect(yRot,SIGNAL(valueChanged(double)),this,SLOT(y_rotation(double)));
    connect(zoom,SIGNAL(valueChanged(double)),this,SLOT(zoom_changed(double)));
}

Widget::~Widget() {}

void Widget::preset_font(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetFront;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::preset_fontLow(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetFrontLow;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::preset_fontHigh(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetFrontHigh;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::preset_left(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetLeft;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::preset_leftLow(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetLeftLow;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::preset_leftHigh(){
    Q3DCamera::CameraPreset view=Q3DCamera::CameraPresetLeftHigh;
    graph3D->scene()->activeCamera()->setCameraPreset(view);
}

void Widget::x_rotation(double num){
    graph3D->scene()->activeCamera()->setXRotation(num);
}

void Widget::y_rotation(double num){
    graph3D->scene()->activeCamera()->setYRotation(num);
}

void Widget::zoom_changed(double num){
    graph3D->scene()->activeCamera()->setZoomLevel(num);
}
