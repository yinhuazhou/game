#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QFileSystemModel>
#include <QTreeView>
#include <QModelIndex>
#include <myframe.h>
#include <QString>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QFileSystemModel *fileModel;
    QTreeView *treeView;
    MyFrame *frame;
    QSplitter *hSplitter;
    QHBoxLayout *hbox;
private slots:
    void view_clicked(QModelIndex index);
};
#endif // WIDGET_H
