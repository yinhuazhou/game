#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QDir>
#include <QUrl>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMediaPlayer *player;//QMediaPlayer对象指针
    QMediaMetaData meta;//媒体元数据对象
    QPushButton *btnOpen;//按钮指针
    QPlainTextEdit *textEdit;//多行纯文本框指针
    QVBoxLayout *vbox;//垂直布局指针
private slots:
    void btn_open();
    void show_data();
};
#endif // WIDGET_H
