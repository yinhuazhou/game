#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringListModel>
#include <QVariant>
#include <QModelIndex>
#include <QModelIndexList>
#include <QString>
#include <QStringList>
#include <QListView>
#include <mylistview.h>
#include <QAxObject>
#include <QFileDialog>
#include <QDir>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox1,*vbox2;
    QHBoxLayout *hbox;
    QPushButton *btnOpen,*btnAdd;
    MyListView *listView1,*listView2;
    QStringListModel *model1,*model2;
private slots:
    void btnAdd_clicked();
    void btnOpen_clicked();
};
#endif // WIDGET_H
