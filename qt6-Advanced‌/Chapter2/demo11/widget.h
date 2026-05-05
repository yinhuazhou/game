#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QList>
#include <QAxObject>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QHBoxLayout *hbox;
    QPushButton *btnOpen,*btnSave;
    QTableWidget *tableWidget;
private slots:
    void open_excel();
    void save_excel();
};
#endif // WIDGET_H
