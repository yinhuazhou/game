#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
#include <QPointF>
//创建自定义视图控件
class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent=nullptr);
signals:
    void sendPoint(QPointF pt);//自定义信号，参数为鼠标被按下时鼠标在视图中的位置
    void movePoint(QPointF pt);//自定义信号，参数为移动鼠标时鼠标在视图中的位置
    void releasePoint(QPointF pt);//自定义信号，参数为鼠标被释放时鼠标在视图中的位置
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};
#endif // MYGRAPHICSVIEW_H
