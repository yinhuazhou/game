#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPen>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//对象指针
    QPushButton *btn;
    QPdfWriter *pdfWriter;
private slots:
    void btn_clicked();
};
#endif // WIDGET_H
