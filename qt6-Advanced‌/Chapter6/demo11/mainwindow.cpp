#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QValueAxis、QLogValueAxis");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图表视图中的图表
    chart->setTitle("随机数据");
    //创建折线数据序列
    lineSeries=new QLineSeries();
    lineSeries->setName("折线数据序列");
    //向折线数据序列中添加随机数
    for(int i=0;i<200;i++){
        lineSeries->append(i,10000*random.generateDouble());
    }
    chart->addSeries(lineSeries);//向图表中添加数据序列
    //创建数值坐标轴
    axisX=new QValueAxis();
    axisX->setTitleText("数值坐标轴");//设置坐标轴的标题
    axisX->setTitleBrush(Qt::black);//设置画刷颜色
    axisX->setLabelsColor(Qt::black);//设置标签颜色
    axisX->setRange(0,100);         //设置坐标轴的范围
    axisX->setTickCount(10);        //设置刻度的数量
    axisX->applyNiceNumbers();      //应用智能刻度标签
    axisX->setLinePenColor(Qt::black);//设置坐标轴的颜色
    QPen pen1=axisX->linePen();     //获取坐标轴的钢笔
    pen1.setWidth(2);               //设置钢笔的宽度
    axisX->setLinePen(pen1);        //设置坐标轴的钢笔
    axisX->setGridLineColor(Qt::gray);//设置网格线的颜色
    QPen pen2=axisX->gridLinePen();   //获取网格线的钢笔
    pen2.setWidth(2);               //设置钢笔宽度
    axisX->setGridLinePen(pen2);    //设置网格线的钢笔
    axisX->setMinorTickCount(3);    //设置次刻度的数量
    axisX->setLabelFormat("%5.1f"); //设置标签的格式
    //创建对数坐标轴
    axisY=new QLogValueAxis();
    axisY->setBase(10.0);           //设置对数的底
    axisY->setMax(10000.0);         //设置最大值
    axisY->setMin(10.0);            //设置最小值
    axisY->setTitleText("对数坐标轴");//设置标题
    axisY->setMinorTickCount(9);    //设置次网格线的数量
    axisY->setLabelFormat("%6d");   //设置标签的格式
    //向图表中添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
}

MainWindow::~MainWindow() {}
