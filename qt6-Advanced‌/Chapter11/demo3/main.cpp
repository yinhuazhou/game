#include "widget.h"
#include <QApplication>
#include <QTranslator>

QTranslator *trans=new QTranslator();//创建翻译器对象
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置显示中文或英文界面，EN表示英文界面，CN表示中文界面
    QString curLang="EN";
    bool isLoad=false;
    if(curLang=="EN")
        isLoad=trans->load("D:\\Chapter11\\demo3\\demo3_en.qm");
    else
        isLoad=trans->load("D:\\Chapter11\\demo3\\demo3_cn.qm");
    if(isLoad)
        a.installTranslator(trans);//加载翻译器
    Widget w;
    w.show();
    return a.exec();
}
