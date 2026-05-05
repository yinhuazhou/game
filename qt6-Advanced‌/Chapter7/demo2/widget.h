#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtDataVisualization>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox1,*hbox2;//水平布局指针
    QPushButton *btnFont,*btnFontLow,*btnFontHigh;//按钮指针
    QPushButton *btnLeft,*btnLeftLow,*btnLeftHigh;//按钮指针
    QLabel *xLabel,*yLabel,*zoomLabel;//标签指针
    QDoubleSpinBox *xRot,*yRot,*zoom;//数字输入框指针
    QWidget *container;//三维图表容器指针
    Q3DScatter *graph3D;//三维散点图表指针
    QScatterDataProxy *dataProxy;//三维散点图的数据代理指针
    QScatter3DSeries *series;//数据序列指针
    QScatterDataArray *itemArray;//散点数组指针
private slots:
    void preset_font();
    void preset_fontLow();
    void preset_fontHigh();
    void preset_left();
    void preset_leftLow();
    void preset_leftHigh();
    void x_rotation(double num);
    void y_rotation(double num);
    void zoom_changed(double num);
};
#endif // WIDGET_H
