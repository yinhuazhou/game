#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,380);
    setWindowTitle("QPolarChart");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QPolarChart();
    chartView->setChart(chart);//设置图标视图中的图表
    chart->setTitle("极坐标图表");
    //创建折线图数据序列
    lineSeries=new QLineSeries();
    lineSeries->setName("折线图");
    //创建散点图数据序列
    scatterSeries=new QScatterSeries();
    scatterSeries->setName("散点图");
    //向折线数据序列中添加数据
    int r0=20;
    for(int angle=0;angle<360;angle++){
        float r=r0*cos(20*angle/180);
        lineSeries->append(angle,r);
    }
    //向散点图数据序列中添加数据
    int r1=5;
    for(int angle=0;angle<360;angle=angle+5){
        float s=pow(r1,2)+pow(pi*r1*angle/180,2);
        float r=pow(s,0.5);
        scatterSeries->append(angle,r);
    }
    //向图表中添加数据序列
    chart->addSeries(lineSeries);
    chart->addSeries(scatterSeries);
    //创建、设置坐标轴
    axisAngle=new QValueAxis();
    axisAngle->setTitleText("Angle");
    axisAngle->setRange(0,360);
    axisAngle->setLinePenColor(Qt::black);
    axisRadius=new QValueAxis();
    axisRadius->setTitleText("Distance");
    axisRadius->setRange(0,36);
    axisRadius->setGridLineColor(Qt::gray);
    //向极坐标图表中添加坐标轴
    chart->addAxis(axisAngle,QPolarChart::PolarOrientationAngular);
    chart->addAxis(axisRadius,QPolarChart::PolarOrientationRadial);
    //设置数据序列与坐标轴的关联
    lineSeries->attachAxis(axisAngle);
    lineSeries->attachAxis(axisRadius);
    scatterSeries->attachAxis(axisAngle);
    scatterSeries->attachAxis(axisRadius);
}

MainWindow::~MainWindow() {}
