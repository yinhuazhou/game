#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,600,280);
    setWindowTitle("QLegend、QLegendMarker");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    //创建条形图数据序列
    barSeries=new QBarSeries();
    //创建数据项
    set1=new QBarSet("孙悟空的成绩");
    set1->append({60,80,70});
    set2=new QBarSet("猪八戒的成绩");
    set2->append({63,72,86});
    set3=new QBarSet("沙僧的成绩");
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
    //获取图例
    legend=chart->legend();
    //设置图例
    legend->setAlignment(Qt::AlignBottom);
    legend->setBackgroundVisible(true);//设置背景可见
    legend->setBorderColor(Qt::black);//设置边框颜色
    legend->setColor(Qt::white);
    QPen pen=legend->pen();
    pen.setWidth(4);
    legend->setPen(pen);
    legend->setToolTip("图例");//设置提示信息
    legend->setMarkerShape(QLegend::MarkerShapeFromSeries);
    //获取图例标志、设置图例标志
    QList<QLegendMarker *> markers=legend->markers();
    QLegendMarker *i;
    foreach(i,markers){
        QFont font=i->font();
        font.setPointSize(12);
        i->setFont(font);
        if(i->type()==QLegendMarker::LegendMarkerTypeBar)
            i->setShape(QLegend::MarkerShapeRotatedRectangle);
        else
            i->setShape(QLegend::MarkerShapeFromSeries);
    }
}

MainWindow::~MainWindow() {}
