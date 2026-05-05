#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QStatusBar>
#include <QFileInfoList>
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
private slots:
    void action_open();
};
#endif // MAINWINDOW_H
