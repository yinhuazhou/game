#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>
#include <QTransform>
#include "pixmapwidget.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    PixmapWidget *pixWidget;//自定义窗口指针
    QVBoxLayout *vbox;
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QGraphicsProxyWidget *proxy;//代理控件指针
};
#endif // WIDGET_H
