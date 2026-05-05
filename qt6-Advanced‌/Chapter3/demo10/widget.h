#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QList>
#include <QAxObject>

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
    void open_xlsx();
};
#endif // WIDGET_H
