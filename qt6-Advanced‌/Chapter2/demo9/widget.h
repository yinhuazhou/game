#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QFont>
#include <QString>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QTreeWidget *treeWidget;
    QLabel *label;
private slots:
    void clicked_treeWidget(QTreeWidgetItem *item,int column);
};
#endif // WIDGET_H
