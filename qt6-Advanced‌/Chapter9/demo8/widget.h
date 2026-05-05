#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMetaEnum>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnGet;//按钮指针
    QPlainTextEdit *plainText;//多行纯文本框指针
private slots:
    void btn_get();
};
#endif // WIDGET_H
