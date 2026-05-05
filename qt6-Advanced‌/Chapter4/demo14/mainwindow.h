#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *queryModel1,*queryModel2;
    QSqlRelationalTableModel *relationModel;
private slots:
    void action_open();
    void action_close();
};
#endif // MAINWINDOW_H
