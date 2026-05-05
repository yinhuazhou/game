#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QStackedBarSeries、QPercentBarSeries");
    hbox=new QHBoxLayout(this);
    //创建图表视图控件
    chartView1=new QChartView();
    chartView2=new QChartView();
    hbox->addWidget(chartView1);
    hbox->addWidget(chartView2);
    //创建图表
    chart1=new QChart();
    chart2=new QChart();
    chartView1->setChart(chart1);
    chartView2->setChart(chart2);
    //创建条形图数据序列
    seriesStacked=new QStackedBarSeries();
    seriesPercent=new QPercentBarSeries();
    //创建数据项
    set1=new QBarSet("孙悟空的考试成绩");
    set1->append({70,20,90});
    set2=new QBarSet("猪八戒的考试成绩");
    set2->append({36,86,96});
    set3=new QBarSet("沙僧的考试成绩");
    set3->append({73,63,63});
    //向数据序列中添加数据项
    seriesStacked->append({set1,set2,set3});
    seriesPercent->append({set1,set2,set3});
    //向图表中添加数据序列
    chart1->addSeries(seriesStacked);
    chart2->addSeries(seriesPercent);
}

Widget::~Widget() {}
