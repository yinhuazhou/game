#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QString>
#include <QHostInfo>
#include <QHostAddress>
#include <QList>
#include <QMetaEnum>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;//垂直布局指针
    QHBoxLayout *hbox;//水平布局指针
    QPushButton *btnName,*btnInfo;//按钮指针
    QLineEdit *lineEdit;//单行输入控件指针
    QPlainTextEdit *textEdit;//多行纯文本输入控件指针
private slots:
    void btn_name();
    void btn_info();
};
#endif // WIDGET_H
