#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QChart>
#include <QChartView>
#include <QStackedBarSeries>
#include <QBarSet>
#include <QPercentBarSeries>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;//水平布局指针
    QChartView *chartView1,*chartView2;//图表视图指针
    QChart *chart1,*chart2;//图表指针
    QStackedBarSeries *seriesStacked;//条形图数据序列指针
    QPercentBarSeries *seriesPercent;//条形图数据序列指针
    QBarSet *set1,*set2,*set3;//数据项指针
};
#endif // WIDGET_H
