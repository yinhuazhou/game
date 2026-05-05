#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;
    QTableView *tableView;
    QStandardItemModel *standardModel;
    void open_csv();
};
#endif // WIDGET_H
