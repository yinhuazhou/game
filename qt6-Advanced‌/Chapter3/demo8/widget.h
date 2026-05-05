#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;
    QSplitter *hSplitter;
    QListView *listView;
    QTreeView *treeView;
    QFileSystemModel *fileModel;
private slots:
    void view_clicked(QModelIndex index);
};
#endif // WIDGET_H
