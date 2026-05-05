#include "widget.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("图形效果");
    view=new QGraphicsView();//创建图像视图控件
    scene=new QGraphicsScene();//创建图像场景
    view->setScene(scene);//在图像视图中设置场景
    //创建5个按钮
    btnOpen=new QPushButton("打开图像");
    btnBlur=new QPushButton("模糊效果");
    btnColor=new QPushButton("变色效果");
    btnShadow=new QPushButton("阴影效果");
    btnOpacity=new QPushButton("透明效果");
    //创建水平布局，并将5个按钮添加到该水平布局下
    hbox=new QHBoxLayout();
    hbox->addWidget(btnOpen);hbox->addWidget(btnBlur);
    hbox->addWidget(btnColor);hbox->addWidget(btnShadow);
    hbox->addWidget(btnOpacity);
    //设置窗口布局
    vbox=new QVBoxLayout(this);
    vbox->addLayout(hbox);vbox->addWidget(view);
    //使用信号/槽
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(btn_open()));
    connect(btnBlur,SIGNAL(clicked()),this,SLOT(btn_blur()));
    connect(btnColor,SIGNAL(clicked()),this,SLOT(btn_color()));
    connect(btnShadow,SIGNAL(clicked()),this,SLOT(btn_shadow()));
    connect(btnOpacity,SIGNAL(clicked()),this,SLOT(btn_opacity()));
    //设置按钮处于失效状态
    btnBlur->setEnabled(false);btnColor->setEnabled(false);
    btnShadow->setEnabled(false);btnOpacity->setEnabled(false);
}

Widget::~Widget() {}

void Widget::btn_open(){
    QString curPath=QDir::currentPath();//获取程序当前目录
    QString filter="图像(*.png *.bmp *.jpg *.jpeg);;所有文件(*.*)";
    QString title="打开图像文件";//文件对话框的标题
    QString fileName=QFileDialog::getOpenFileName(this,title,curPath,filter);
    if(fileName.isEmpty())
        return;
    if(pixmapItem!=nullptr){
        scene->removeItem(pixmapItem);
    }
    else{
        btnBlur->setEnabled(false);btnColor->setEnabled(false);
        btnShadow->setEnabled(false);btnOpacity->setEnabled(false);
    }
    QPixmap pix(fileName);
    pixmapItem=new QGraphicsPixmapItem(pix);
    scene->addItem(pixmapItem);
    btnBlur->setEnabled(true);btnColor->setEnabled(true);
    btnShadow->setEnabled(true);btnOpacity->setEnabled(true);
}

void Widget::btn_blur(){
    QGraphicsBlurEffect *effect=new QGraphicsBlurEffect();
    effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    pixmapItem->setGraphicsEffect(effect);
}

void Widget::btn_color(){
    QGraphicsColorizeEffect *effect=new QGraphicsColorizeEffect();
    effect->setColor(Qt::blue);
    effect->setStrength(10);
    pixmapItem->setGraphicsEffect(effect);
}

void Widget::btn_shadow(){
    QGraphicsDropShadowEffect *effect=new QGraphicsDropShadowEffect();
    pixmapItem->setGraphicsEffect(effect);
}

void Widget::btn_opacity(){
    QRectF rect=pixmapItem->boundingRect();
    QLinearGradient linear(rect.topLeft(),rect.bottomLeft());
    linear.setColorAt(0.11,Qt::transparent);
    linear.setColorAt(0.49,Qt::black);
    linear.setColorAt(0.88,Qt::white);
    QGraphicsOpacityEffect *effect=new QGraphicsOpacityEffect();
    effect->setOpacityMask(linear);
    pixmapItem->setGraphicsEffect(effect);
}
