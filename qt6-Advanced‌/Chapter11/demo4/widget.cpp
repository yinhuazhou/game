#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //使用信号/槽
    connect(ui->btnChinese,SIGNAL(clicked()),this,SLOT(btn_chinese()));
    connect(ui->btnEnglish,SIGNAL(clicked()),this,SLOT(btn_english()));
}

Widget::~Widget()
{
    delete ui;
}
extern QTranslator trans;//声明外部变量
void Widget::btn_chinese(){
    if(trans.load("D:\\Chapter11\\demo4\\demo4_cn.qm")){
        ui->retranslateUi(this);//重新翻译界面文字
        QSettings settings;
        settings.setValue("Language","CN");
    }
}

void Widget::btn_english(){
    if(trans.load("D:\\Chapter11\\demo4\\demo4_en.qm")){
        ui->retranslateUi(this);//重新翻译界面文字
        QSettings settings;
        settings.setValue("Language","EN");
    }
}
