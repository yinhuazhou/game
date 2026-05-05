#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    setGeometry(300,300,580,280);
    setWindowTitle("绘制图形");
    //创建图形视图控件
    graphicsView=new MyGraphicsView();
    setCentralWidget(graphicsView);
    QRectF rectf(width()/2,height()/2,width(),height());
    //创建图像场景
    graphicsScene=new QGraphicsScene(rectf);
    graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //图像视图设置图像场景
    graphicsView->setScene(graphicsScene);
    //shape用于记录哪个绘图按钮被选中
    shape["直线"]=false;shape["矩形"]=false;
    shape["椭圆"]=false;shape["圆"]=false;
    //使用信号/槽
    connect(graphicsView,SIGNAL(sendPoint(QPointF)),this,SLOT(press_position(QPointF)));
    connect(graphicsView,SIGNAL(movePoint(QPointF)),this,SLOT(move_position(QPointF)));
    connect(graphicsView,SIGNAL(releasePoint(QPointF)),this,SLOT(release_position(QPointF)));
    //创建菜单栏
    menubar=menuBar();
    //创建菜单
    draw=menubar->addMenu("绘图");
    //给菜单添加动作
    actionLine=draw->addAction("直线");
    actionRect=draw->addAction("矩形");
    actionEllipse=draw->addAction("椭圆");
    actionCircle=draw->addAction("圆");
    draw->addSeparator();//添加分隔符
    actionStop=draw->addAction("停止");
    actionDelete=draw->addAction("删除");
    actionClear=draw->addAction("清空");
    //创建工具栏
    toolbar=addToolBar("绘图");
    toolbar->addAction(actionLine);
    toolbar->addAction(actionRect);
    toolbar->addAction(actionEllipse);
    toolbar->addAction(actionCircle);
    toolbar->addSeparator();
    toolbar->addAction(actionStop);
    toolbar->addSeparator();
    toolbar->addAction(actionDelete);
    toolbar->addAction(actionClear);
    //使用信号/槽
    connect(actionLine,SIGNAL(triggered()),this,SLOT(line_triggered()));
    connect(actionRect,SIGNAL(triggered()),this,SLOT(rect_triggered()));
    connect(actionEllipse,SIGNAL(triggered()),this,SLOT(ellipse_triggered()));
    connect(actionCircle,SIGNAL(triggered()),this,SLOT(circle_triggered()));
    connect(actionStop,SIGNAL(triggered()),this,SLOT(stop_triggered()));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(delete_triggered()));
    connect(actionClear,SIGNAL(triggered()),this,SLOT(clear_triggered()));
}

MainWindow::~MainWindow() {}
//鼠标按下
void MainWindow::press_position(QPointF pt){
    QPoint point=pt.toPoint();
    pressPos=graphicsView->mapToScene(point);//映射成场景坐标
}
//鼠标移动
void MainWindow::move_position(QPointF pt){
    QPoint point=pt.toPoint();
    movePos=graphicsView->mapToScene(point);
    move_draw(pressPos,movePos);    //调用绘图函数
}
//鼠标释放
void MainWindow::release_position(QPointF pt){
    //QPoint point=pt.toPoint();
    if(temp!=nullptr){
        temp->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
        temp=nullptr;
    }
}
//绘制直线
void MainWindow::line_triggered(){
    shape["直线"]=true;shape["矩形"]=false;
    shape["椭圆"]=false;shape["圆"]=false;
}
//绘制矩形
void MainWindow::rect_triggered(){
    shape["直线"]=false;shape["矩形"]=true;
    shape["椭圆"]=false;shape["圆"]=false;
}
//绘制椭圆
void MainWindow::ellipse_triggered(){
    shape["直线"]=false;shape["矩形"]=false;
    shape["椭圆"]=true;shape["圆"]=false;
}
//绘制圆
void MainWindow::circle_triggered(){
    shape["直线"]=false;shape["矩形"]=false;
    shape["椭圆"]=false;shape["圆"]=true;
}
//停止绘图
void MainWindow::stop_triggered(){
    shape["直线"]=false;shape["矩形"]=false;
    shape["椭圆"]=false;shape["圆"]=false;
}
//删除图形项
void MainWindow::delete_triggered(){
    QList<QGraphicsItem *> items=graphicsScene->selectedItems();
    if(items.size()<=0)
        return;
    for(int i=0;i<items.size();i++)
        graphicsScene->removeItem(items[i]);
}
//清空图形项
void MainWindow::clear_triggered(){
    graphicsScene->clear();
    graphicsScene->update();
}
//当鼠标移动时绘制图形项
void MainWindow::move_draw(QPointF pt1, QPointF pt2){
    float x1=(pt1.x()<pt2.x())?pt1.x():pt2.x();//获取较小的横坐标
    float y1=(pt1.y()<pt2.y())?pt1.y():pt2.y();//获取较小的纵坐标
    float x2=(pt1.x()>pt2.x())?pt1.x():pt2.x();//获取较大的横坐标
    float y2=(pt1.y()>pt2.y())?pt1.y():pt2.y();//获取较大的纵坐标
    QPointF p1(x1,y1),p2(x2,y2);
    QRectF rect(p1,p2);//鼠标按下点与移动点点矩形区域
    if(temp!=nullptr)//在鼠标移动过程中，如果变量已经指向图形项，则需要移除该图形项
        graphicsScene->removeItem(temp);
    if(shape["直线"])
        temp=graphicsScene->addLine(QLineF(pt1,pt2));//添加直线
    if(shape["矩形"])
        temp=graphicsScene->addRect(rect);          //添加矩形
    if(shape["椭圆"])
        temp=graphicsScene->addEllipse(rect);       //添加椭圆
    if(shape["圆"]){
        //圆的半径
        float r=sqrt(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2));
        QPointF ptf1(p1.x()-r,p1.y()-r);
        QPointF ptf2(p1.x()+r,p1.y()+r);
        QRectF rectf1(ptf1,ptf2);//圆的矩形范围
        temp=graphicsScene->addEllipse(rectf1);     //添加圆
    }
}
