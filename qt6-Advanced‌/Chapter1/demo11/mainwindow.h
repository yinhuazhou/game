#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QStatusBar>
#include <QBuffer>
#include <QDataStream>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *actionCreate,*actionShow;
    QStatusBar *status;
    QPlainTextEdit *plainText;
    QBuffer *buffer;
private slots:
    void action_create();
    void action_show();
};
#endif // MAINWINDOW_H
