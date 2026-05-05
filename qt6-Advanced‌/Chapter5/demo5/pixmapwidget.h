#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QDir>
//创建一个可以显示图像的窗口类
class PixmapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PixmapWidget(QWidget *parent = nullptr);
private:
    QVBoxLayout *vbox;
    QLabel *label;
    QPushButton *btn;
private slots:
    void btn_clicked();
};
#endif // PIXMAPWIDGET_H
