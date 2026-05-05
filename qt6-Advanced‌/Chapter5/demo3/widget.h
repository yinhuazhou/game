#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QRectF>
#include "ellipse.h"
#include "cos.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QGraphicsItemGroup *group;//图形项组合指针
    Ellipse *item1;
    Cos *item2;
};
#endif // WIDGET_H
