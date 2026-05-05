#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QCategory3DAxis");
    //创建三维柱形图表
    graph3D=new Q3DBars();
    //创建三维图表容器
    container=createWindowContainer(graph3D);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);
    setCentralWidget(container);
    //创建三维柱状图数据序列
    series=new QBar3DSeries();
    series->setMesh(QAbstract3DSeries::MeshCylinder);//柱状样式
    //设置柱状标签显示格式
    series->setItemLabelFormat("(@rowLabel,@colLabel): % .1f");
    series->setName("三维柱状图数据序列");
    graph3D->addSeries(series);     //向图表中添加数据序列
    //设置视角
    Q3DCamera::CameraPreset camView=Q3DCamera::CameraPresetFrontHigh;
    graph3D->scene()->activeCamera()->setCameraPreset(camView);
    //创建列标签、行标签
    QStringList colLabs={"星期一","星期二","星期三","星期四","星期五"};
    QStringList rowLabs={"第1周","第2周","第3周","第4周"};
    //创建数据代理
    dataProxy=new QBarDataProxy();
    for(int j=0;j<4;j++){                   //4行
        QBarDataRow *row=new QBarDataRow(); //一行的QBarDataItem对象
        for(int i=0;i<5;i++){                   //7列
            float value=generator.bounded(8,16);//8～16之间的随机数字
            QBarDataItem item(value);
            row->append(item);
        }
        dataProxy->addRow(row);                 //添加行
    }
    dataProxy->setColumnLabels(colLabs);        //添加行标签
    dataProxy->setRowLabels(rowLabs);           //添加列标签
    series->setDataProxy(dataProxy);            //设置数据代理
}

MainWindow::~MainWindow() {}
