#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QRectF>
#include <QLinearGradient>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QGraphicsPixmapItem *pixmapItem=nullptr;//图像文件项指针
    QGraphicsView *view;//图像视图指针
    QGraphicsScene *scene;//图像场景指针
    QPushButton *btnOpen,*btnBlur,*btnColor,*btnShadow,*btnOpacity;//按钮指针
    QHBoxLayout *hbox;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
private slots:
    void btn_open();
    void btn_blur();
    void btn_color();
    void btn_shadow();
    void btn_opacity();
};
#endif // WIDGET_H
