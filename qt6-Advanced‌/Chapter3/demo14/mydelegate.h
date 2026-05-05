#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QStyleOptionViewItem>
#include <QDoubleSpinBox>
#include <QString>

class MyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MyDelegate(QObject *parent=nullptr);
    ~MyDelegate();
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor,QModelIndex &index);
    void setModelData(QWidget *editor,QAbstractItemModel *model,QModelIndex &index);
    void updateEditorGeometry(QWidget *editor,QStyleOptionViewItem &option,QModelIndex &index);
};
#endif // MYDELEGATE_H
