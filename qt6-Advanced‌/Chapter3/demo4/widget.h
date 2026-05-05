#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QAxObject>

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
    void open_xlsx();
};
#endif // WIDGET_H
