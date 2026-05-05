#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QBarCategoryAxis");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    //创建条形图数据序列
    barSeries=new QBarSeries();
    //创建数据项
    set1=new QBarSet("孙悟空的考试成绩");
    set1->append({60,80,70});
    set2=new QBarSet("猪八戒的考试成绩");
    set2->append({63,72,86});
    set3=new QBarSet("沙僧的考试成绩");
    set3->append({95,62,75});
    //向条形图数据序列中添加数据项
    barSeries->append({set1,set2,set3});
    //创建折线图数据序列、添加数据
    lineSeries=new QLineSeries();
    lineSeries->setName("各科成绩的最高分");
    lineSeries->append(0,95);
    lineSeries->append(1,80);
    lineSeries->append(2,86);
    //向图表中添加数据序列
    chart->addSeries(barSeries);
    chart->addSeries(lineSeries);
    //创建条形图坐标轴
    axisX=new QBarCategoryAxis();
    axisX->append({"语文成绩","数学成绩","外语成绩"});
    //创建数值坐标轴
    axisY=new QValueAxis();
    axisY->setRange(0,100);//设置坐标轴的数值范围
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignRight);
    //将数据序列与坐标轴关联
    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
}

MainWindow::~MainWindow() {}
