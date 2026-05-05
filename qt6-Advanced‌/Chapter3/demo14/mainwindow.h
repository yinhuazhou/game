#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QHBoxLayout>
#include <QListView>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QString>
#include <QList>
#include <QAxObject>
#include <QFileDialog>
#include <QDir>
#include <mydelegate.h>
#include <QHeaderView>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *actionOpen;
    QListView *listView;
    QTableView *tableView;
    QFrame *frame;
    QSplitter *hSplitter;
    QHBoxLayout *hbox;
    QStandardItemModel *standardModel;
    MyDelegate *delegate;
private slots:
    void action_open();
    void listView_clicked(QModelIndex index);
};
#endif // MAINWINDOW_H
