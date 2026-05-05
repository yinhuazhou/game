#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QLineSeries、QSplineSeries");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图标视图中的图表
    chart->setTitle("正弦、余弦");
    //创建折线数据序列、样条曲线数据序列
    seriesSin=new QLineSeries();
    seriesCos=new QSplineSeries();
    //设置数据序列的名称
    seriesSin->setName("sin");
    seriesCos->setName("cos");
    //向数据序列中添加数据
    for(int i=0;i<=720;i++){
        seriesSin->append(i,sin(i*pi/180));
        seriesCos->append(i,cos(i*pi/180));
    }
    //向图表中添加数据序列
    chart->addSeries(seriesSin);
    chart->addSeries(seriesCos);
    //创建坐标轴
    axisX=new QValueAxis();
    axisX->setRange(0,720);
    axisX->setTitleText("角度");
    axisY=new QValueAxis();
    axisY->setRange(-1,1);
    axisY->setTitleText("数值");
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
}

MainWindow::~MainWindow() {}
