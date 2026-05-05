#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QChartView、QChart、QLineSeries");
    //创建数据序列对象
    series=new QLineSeries();
    QList<QPointF> nums={QPointF(1,1),QPointF(2,4),QPointF(3,9),QPointF(4,16),QPointF(5,25)};
    series->setName("y=x^2");
    //向数据序列对象中添加数据
    series->append(nums);
    //创建图表对象
    chart=new QChart();
    //向图表对象中添加数据序列
    chart->addSeries(series);
    //创建默认的坐标轴
    chart->createDefaultAxes();
    //设置标题
    chart->setTitle("折线图");
    //创建图表视图控件，参数为图表指针
    chartView=new QChartView(chart);
    setCentralWidget(chartView);
}

MainWindow::~MainWindow() {}
