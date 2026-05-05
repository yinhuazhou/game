#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFont>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QTableWidget *tableWidget;
    void open_database();
};
#endif // WIDGET_H
