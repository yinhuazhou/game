#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicsview.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QStatusBar>
#include <QRectF>
#include <QPoint>
#include <QString>
#include <QGraphicsItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QStatusBar *statusB;  //状态栏指针
    MyGraphicsView *graphicsView;//图像视图指针
    QGraphicsScene *graphicsScene;//图像场景指针
    QGraphicsRectItem *rectItem;//矩形图形项指针
    QGraphicsEllipseItem *ellipseItem;//椭圆图形项指针
private slots:
    void mousePosition(QPointF pt);
};
#endif // MAINWINDOW_H
