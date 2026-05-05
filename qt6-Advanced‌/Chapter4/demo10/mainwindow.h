#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>
#include <QtSql>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *actionOpen,*actionClose;
    QLabel *label;
    QComboBox *combox;
    QTableView *tableView;
    QFrame *frame;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
    QSqlQueryModel *queryModel;
    QSqlDatabase db;
private slots:
    void action_open();
    void action_close();
    void combox_changed();
};
#endif // MAINWINDOW_H
