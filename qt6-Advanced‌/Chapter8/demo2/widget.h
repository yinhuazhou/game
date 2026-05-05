#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QHostInfo>
#include <QString>
#include <QHostAddress>
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QLabel *label;//标签指针
    QLineEdit *lineEdit;//单行输入控件指针
    QHBoxLayout *hbox;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnInfo;//按钮指针
    QPlainTextEdit *textEdit;//多行纯文本输入控件指针
private slots:
    void btn_info();
    void lookedUp(QHostInfo host);
};
#endif // WIDGET_H
