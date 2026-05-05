#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QBoxPlotSeries、QBoxSet");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图标视图中的图表
    chart->setTitle("箱形图");
    //创建数据
    QList<float> data1={21,22,30,25,26};
    QList<float> data2={11,12,15,17,18};
    QList<float> data3={21,22,23,28,29};
    QList<QList<float>> data={data1,data2,data3};
    //创建箱形图数据序列
    seriesBox=new QBoxPlotSeries();
    for(int i=0;i<data.size();i++){
        boxSet=new QBoxSet();
        for(int j=0;j<data[0].size();j++)
            boxSet->append(data[i][j]);
        seriesBox->append(boxSet);
    }
    //向图表中添加数据序列
    chart->addSeries(seriesBox);
    //创建坐标轴
    axisX=new QBarCategoryAxis();
    axisX->append({"1","2","3"});
    axisY=new QValueAxis();
    axisY->setRange(11,30);
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    //将数据序列与坐标轴关联
    seriesBox->attachAxis(axisX);
    seriesBox->attachAxis(axisY);
}

MainWindow::~MainWindow() {}
