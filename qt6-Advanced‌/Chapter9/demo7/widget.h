#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QHBoxLayout>
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
    QHBoxLayout *hbox;//水平布局指针
    QVBoxLayout *vbox;//垂直布局指针
    QPushButton *btnCamera,*btnInputs,*btnOutputs;//按钮指针
    QPlainTextEdit *textEdit;//多行纯文本框指针
private slots:
    void btn_camera();
    void btn_inputs();
    void btn_outputs();
};
#endif // WIDGET_H
