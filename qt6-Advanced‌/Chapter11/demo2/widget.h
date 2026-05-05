#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QAxWidget>
#include <QFileDialog>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//对象指针
    QPushButton *btn;
    QLabel *label;
    QAxWidget *mywidget;
private slots:
    void btn_open();
};
#endif // WIDGET_H
