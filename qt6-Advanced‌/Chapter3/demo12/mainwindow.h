#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QFrame>
#include <QHBoxLayout>
#include <QListView>
#include <QTreeView>
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
    QTreeView *treeView;
    QFrame *frame;
    QSplitter *hSplitter;
    QHBoxLayout *hbox;
    QStandardItemModel *standardModel;
private slots:
    void action_open();
    void listView_clicked(QModelIndex index);
};
#endif // MAINWINDOW_H
