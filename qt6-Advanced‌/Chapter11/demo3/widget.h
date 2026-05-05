#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QSettings>
#include <QTranslator>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QPushButton *btnChinese,*btnEnglish;//对象指针
    QPushButton *btnOne,*btnTwo,*btnThree,*btnFile;
    QPushButton *btnEdit,*btnCopy,*btnCut,*btnPaste;
    QGridLayout *layout;
};
#endif // WIDGET_H
