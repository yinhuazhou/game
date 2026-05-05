#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
#include <QPointF>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent=nullptr);
    void drawBackground(QPainter *painter, const QRectF &rect);
signals:
    void sendPosition(QPointF pt);//自定义信号，参数是鼠标在视图中的位置
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};
#endif // MYGRAPHICSVIEW_H
