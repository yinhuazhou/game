#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtPrintSupport>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMenuBar *menuBar;//对象指针
    QMenu *fileMenu;
    QAction *openAction,*previewAction,*printAction,*exitAction;
    QTextEdit *textEdit;
    QVBoxLayout *vbox;
    QPrinter *printer;
    QPrintPreviewDialog *previewDialog;
    QPrintDialog *printDialog;
private slots:
    void open_action();
    void preview_action();
    void print_action();
    void preview_paintRequested(QPrinter *printer);
    void printDialog_accepted(QPrinter *printer);
};
#endif // WIDGET_H
