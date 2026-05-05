#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("QPieSeries、QPieSlice");
    //创建图表视图控件
    chartView=new QChartView();
    setCentralWidget(chartView);
    //创建图表
    chart=new QChart();
    chartView->setChart(chart);//设置图标视图中的图表
    //创建饼图数据序列
    seriesPie=new QPieSeries();
    seriesPie->setLabelsPosition(QPieSlice::LabelOutside);
    seriesPie->setPieStartAngle(90);
    seriesPie->setPieEndAngle(-270);
    //创建切片
    first=new QPieSlice("优秀",22);
    second=new QPieSlice("良好",32);
    //设置爆炸切片
    second->setExploded(true);
    //向饼图数据序列中添加切片
    seriesPie->append(first);
    seriesPie->append(second);
    seriesPie->append("及格",46);
    seriesPie->append("不及格",50);
    //设置标签可见
    seriesPie->setLabelsVisible(true);
    seriesPie->setHoleSize(0.4);//设置圆孔的尺寸
    chart->addSeries(seriesPie);//向图表中添加数据序列
}

MainWindow::~MainWindow() {}
