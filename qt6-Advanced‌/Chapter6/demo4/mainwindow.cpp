#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QAreaSeries");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图标视图中的图表
    chart->setTitle("面积图");
    //创建折线数据序列
    seriesSin=new QLineSeries();
    seriesCos=new QLineSeries();
    //向折线数据序列中添加数据
    for(int i=0;i<=720;i++){
        seriesSin->append(i,sin(i*pi/180));
        seriesCos->append(i,cos(i*pi/180));
    }
    //创建面积图数据序列
    seriesArea=new QAreaSeries();
    seriesArea->setUpperSeries(seriesSin);
    seriesArea->setLowerSeries(seriesCos);
    seriesArea->setName("sin-cos");
    //向图表中添加面积数据序列
    chart->addSeries(seriesArea);
    //设置边框颜色
    QColor color1=seriesArea->borderColor();
    color1.setRgb(255,0,0);
    seriesArea->setBorderColor(color1);
    //设置填充颜色
    QColor color2=seriesArea->color();
    color2.setRgb(0,255,0);
    seriesArea->setColor(color2);
    //创建坐标轴
    axisX=new QValueAxis();
    axisX->setRange(0,360);
    axisX->setTitleText("角度");
    axisY=new QValueAxis();
    axisY->setRange(-1,1);
    axisY->setTitleText("数值");
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
}

MainWindow::~MainWindow() {}
