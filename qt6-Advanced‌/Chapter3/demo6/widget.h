#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLabel>
#include <QString>
#include <QModelIndex>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QTreeView *treeView;
    QFileSystemModel *fileModel;
    QLabel *label;
private slots:
    void treeView_clicked(QModelIndex index);
};
#endif // WIDGET_H
