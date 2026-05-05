#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtPrintSupport>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPen>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QPushButton *btnPrinter;//对象指针
    QVBoxLayout *vbox;
    QPrintDialog *printDialog;
private slots:
    void btn_printer();
    void printDialog_accepted(QPrinter *printer);
};
#endif // WIDGET_H
