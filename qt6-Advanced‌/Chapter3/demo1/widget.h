#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
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
    QVBoxLayout *vbox;
    QListView *listView;
    QStringListModel *listModel;
};
#endif // WIDGET_H
