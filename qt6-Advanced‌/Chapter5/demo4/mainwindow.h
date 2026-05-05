#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QMap>
#include <QRectF>
#include <QList>
#include <QPointF>
#include <QLineF>
#include <cmath>
#include "mygraphicsview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QGraphicsItem *temp=nullptr;//用于指向鼠标移动时产生的临时图形项
    QMap<QString,bool> shape;//用于记录哪个绘图按钮被选中
    MyGraphicsView *graphicsView;//图像视图指针
    QGraphicsScene *graphicsScene;//图像场景指针
    QMenuBar *menubar;
    QMenu *draw;
    QAction *actionLine,*actionRect,*actionEllipse,*actionCircle;
    QAction *actionStop,*actionDelete,*actionClear;
    QToolBar *toolbar;
    QPointF pressPos,movePos;//用于存放场景坐标
    void move_draw(QPointF pt1,QPointF pt2);
private slots:
    void line_triggered();
    void rect_triggered();
    void ellipse_triggered();
    void circle_triggered();
    void stop_triggered();
    void delete_triggered();
    void clear_triggered();
    void press_position(QPointF pt);
    void move_position(QPointF pt);
    void release_position(QPointF pt);
};
#endif // MAINWINDOW_H
