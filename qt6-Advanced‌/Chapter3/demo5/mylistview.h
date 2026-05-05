#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
#include <QWidget>

class MyListView : public QListView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent=nullptr);
    ~MyListView();
    //继承QListView类的selectedIndexes()，使该方法可以在派生类的对象中可以使用
    using QListView::selectedIndexes;
};
#endif // MYLISTVIEW_H
