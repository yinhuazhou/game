#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QAxObject>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;//对象指针
    QVBoxLayout *vbox;
    QPushButton *btnOpen,*btnSave;
    QPlainTextEdit *textEdit;
    QAxObject *wordApp;//Word应用程序对象指针
    QAxObject *docs;//文档集指针
    QAxObject *doc;//文档指针
private slots:
    void btn_open();
    void btn_save();
};
#endif // WIDGET_H
