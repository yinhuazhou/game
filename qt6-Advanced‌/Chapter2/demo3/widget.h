#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QFont>
#include <QDir>
#include <QString>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QHBoxLayout *hbox;
    QPushButton *btnAdd,*btnEdit,*btnRemove,*btnSort;
    QListWidget *listWidget;
private slots:
    void add_item();
    void edit_item();
    void remove_item();
    void sort_item();
};
#endif // WIDGET_H
