#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFont>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QHBoxLayout *hbox;
    QPushButton *btnAddColumn,*btnRemoveColumn;
    QPushButton *btnAddRow,*btnRemoveRow,*btnSelectAll;
    QTableWidget *tableWidget;
private slots:
    void add_column();
    void remove_column();
    void add_row();
    void remove_row();
    void select_all();
};
#endif // WIDGET_H
