#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QFont>
#include <QDir>
#include <QString>
#include <QContextMenuEvent>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QVBoxLayout *vbox;
    QListWidget *listWidget;
    QMenu *menu;
    QAction *actionAdd,*actionEdit,*actionRemove;
    QAction *actionSelectAll,*actionSelectNone,*actionInverse;
protected:
    void contextMenuEvent(QContextMenuEvent *e);
private slots:
    void add_item();
    void edit_item();
    void remove_item();
    void select_all();
    void inverse_select();
    void select_none();
};
#endif // WIDGET_H
