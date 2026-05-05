#include "mydelegate.h"

MyDelegate::MyDelegate(QObject *parent):QStyledItemDelegate(parent) {}

MyDelegate::~MyDelegate(){}
//创建代理控件的对象，并返回该对象
QWidget* MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QDoubleSpinBox *editor=new QDoubleSpinBox(parent);
    editor->setDecimals(2);//设置两位小数
    editor->setMinimum(0.00);
    editor->setMaximum(100.00);
    editor->setFrame(false);
    return editor;
}
//读取数据项的值，并设置代理控件中的数据
void MyDelegate::setEditorData(QWidget *editor, QModelIndex &index){
    const QAbstractItemModel *model=index.model();//获取模型
    QDoubleSpinBox *editor1=static_cast<QDoubleSpinBox*>(editor);
    QVariant value=model->data(index,Qt::DisplayRole);
    editor1->setValue(value.toFloat());
}
//把代理控件的数据写入数据模型中
void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, QModelIndex &index){
    QDoubleSpinBox *editor1=static_cast<QDoubleSpinBox*>(editor);
    if(editor1->isVisible()){
        float num=editor1->value();
        model->setData(index,QString::number(num),Qt::DisplayRole);
    }
}
//设置代理控件的位置
void MyDelegate::updateEditorGeometry(QWidget *editor, QStyleOptionViewItem &option, QModelIndex &index){
    editor->setGeometry(option.rect);
}


