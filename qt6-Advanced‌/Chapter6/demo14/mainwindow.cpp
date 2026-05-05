#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QDateTimeAxis");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    //创建折线数据序列、添加数据
    lineSeries=new QLineSeries();
    lineSeries->setName("最高气温折线图");
    QList<float> high={29.1,26.1,31.5,34.6,35.4,38.8,42.3};
    for(int i=0;i<high.size();i++){
        lineSeries->append(i,high[i]);
    }
    //向图表中添加数据序列
    chart->addSeries(lineSeries);
    //创建时间坐标轴
    axisX=new QDateTimeAxis();
    QDateTime dtime1=QDateTime(QDate(2025,6,19),QTime(00,00,00));
    QDateTime dtime2=QDateTime(QDate(2025,6,26),QTime(00,00,00));
    //设置时间坐标轴的范围、格式、刻度数量
    axisX->setRange(dtime1,dtime2);
    axisX->setFormat("MM/dd/yyyy");
    axisX->setTickCount(7);
    //创建数值坐标轴
    axisY=new QValueAxis();
    axisY->setRange(25,43);//设置坐标轴的数值范围
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignRight);
}

MainWindow::~MainWindow() {}
