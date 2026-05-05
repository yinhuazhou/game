#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QStringList>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *hbox;
    QListView *listView1,*listView2;
    QStringListModel *listModel;
};
#endif // WIDGET_H
