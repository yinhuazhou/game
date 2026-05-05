#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtPrintSupport>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QPoint>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QComboBox *comboBox;//对象指针
    QSpinBox *spinNum;
    QCheckBox *checkBox;
    QLineEdit *lineFile;
    QPushButton *btnPrinter;
    QFormLayout *formLayout;
    QPrinter *printer;
private slots:
    void do_currentText(QString text);
    void do_clicked(bool checked);
    void btn_printer();
};
#endif // WIDGET_H
