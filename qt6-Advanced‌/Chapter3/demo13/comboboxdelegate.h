#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QStyleOptionViewItem>
#include <QComboBox>
#include <QIcon>
#include <QString>

class comboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    comboBoxDelegate(QObject *parent=nullptr);
    ~comboBoxDelegate();
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor,QModelIndex &index);
    void setModelData(QWidget *editor,QAbstractItemModel *model,QModelIndex &index);
    void updateEditorGeometry(QWidget *editor,QStyleOptionViewItem &option,QModelIndex &index);
};
#endif // COMBOBOXDELEGATE_H
