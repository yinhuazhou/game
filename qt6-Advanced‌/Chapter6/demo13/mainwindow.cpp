#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QCategoryAxis");
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
    //向图表中添加数据序列
    chart->addSeries(barSeries);
    //创建条形图坐标轴
    axisX=new QBarCategoryAxis();
    axisX->append({"语文成绩","数学成绩","外语成绩"});
    //创建条目坐标轴
    axisY=new QCategoryAxis();
    axisY->setRange(0,101);
    axisY->append("不及格",59.9);
    axisY->append("及格",75);
    axisY->append("良好",90);
    axisY->append("优秀",100);
    axisY->setStartValue(10);
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignRight);
    //将数据序列与坐标轴关联
    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);
}

MainWindow::~MainWindow() {}
