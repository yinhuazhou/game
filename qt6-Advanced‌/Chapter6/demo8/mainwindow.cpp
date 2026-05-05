#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QCandlestickSeries、QCandlestickset");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    chart->setTitle("蜡烛图");
    //创建数据
    QList<float> begin={23.0,23.3,22.8,23.5,24.5};  //开始值
    QList<float> high={25.3,24.3,24.7,24.3,25.1};   //最高值
    QList<float> low={22.1,21.8,21.7,23.1,23.4};    //最低值
    QList<float> close={24.0,22.7,22.5,23.9,24.7};   //结束值
    //创建蜡烛图数据序列
    candlestickSeries=new QCandlestickSeries();
    candlestickSeries->setMaximumColumnWidth(20);//设置最大列宽
    candlestickSeries->setIncreasingColor(Qt::red);//设置上涨颜色
    candlestickSeries->setDecreasingColor(Qt::green);//设置下跌颜色
    candlestickSeries->setCapsVisible(true);        //显示帽线
    for(int i=0;i<begin.size();i++){
        candleSet=new QCandlestickSet();//蜡烛数据项
        candleSet->setOpen(begin[i]);   //设置初始值
        candleSet->setHigh(high[i]);    //设置最高值
        candleSet->setLow(low[i]);      //设置最低值
        candleSet->setClose(close[i]);  //设置结束值
        candlestickSeries->append(candleSet);//添加蜡烛数据项
    }
    //向图表中添加数据序列
    chart->addSeries(candlestickSeries);
    //创建坐标轴
    axisX=new QBarCategoryAxis();
    axisX->append({"1","2","3","4","5"});
    axisY=new QValueAxis();
    axisY->setRange(21,26);
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    //将数据序列与坐标轴关联
    candlestickSeries->attachAxis(axisX);
    candlestickSeries->attachAxis(axisY);
}

MainWindow::~MainWindow() {}
