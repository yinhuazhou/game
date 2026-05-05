#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtPrintSupport>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//对象指针
    QPushButton *btnShow;
    QPlainTextEdit *textEdit;
private slots:
    void btn_show();
};
#endif // WIDGET_H
