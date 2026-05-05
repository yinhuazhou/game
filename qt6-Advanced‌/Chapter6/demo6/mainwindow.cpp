#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QBarSeries、QBarset");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    //创建条形图数据序列
    seriesBar=new QBarSeries();
    //创建数据项
    set1=new QBarSet("孙悟空的考试成绩");
    set1->append({70,80,90});
    set2=new QBarSet("猪八戒的考试成绩");
    set2->append({56,86,96});
    set3=new QBarSet("沙僧的考试成绩");
    set3->append({73,63,93});
    //创建横轴的坐标轴
    axisX=new QBarCategoryAxis();
    axisX->append({"语文","数学","英文"});
    //创建纵轴的坐标轴
    axisY=new QValueAxis();
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    //向数据序列中添加数据项
    seriesBar->append({set1,set2,set3});
    //向图表中添加数据序列
    chart->addSeries(seriesBar);
}

MainWindow::~MainWindow() {}
