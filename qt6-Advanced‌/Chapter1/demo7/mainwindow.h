#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QString>
#include <QFileInfo>
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
    QAction *actionOpen,*actionClose;
    QStatusBar *status;
    QPlainTextEdit *plainText;
    QFileSystemWatcher *watcher;
private slots:
    void action_open();
    void file_changed(QString fileName);
};
#endif // MAINWINDOW_H
