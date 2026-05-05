#include "widget.h"
#include <QApplication>

QTranslator trans;//全局变量
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings;
    //设置读取注册表，EN表示英文界面，CN表示中文界面
    QString curLang=settings.value("Language","CN").toString();
    bool isLoad=false;
    if(curLang=="EN")
        isLoad=trans.load("D:\\Chapter21\\demo4\\demo4_en.qm");
    else
        isLoad=trans.load("D:\\Chapter21\\demo4\\demo4_cn.qm");
    if(isLoad)
        a.installTranslator(&trans);//加载翻译器
    Widget w;
    w.show();
    return a.exec();
}
