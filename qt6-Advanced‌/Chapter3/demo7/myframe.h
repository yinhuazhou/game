#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QPaintEvent>
#include <QRect>
//因为要显示图片，所以重写paintEvent()事件
class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent=nullptr);
    ~MyFrame();
    void setPath(QString p);//设置图像文件
private:
    QString path;//用于记录图像文件
protected:
    void paintEvent(QPaintEvent *e);
};
#endif // MYFRAME_H
