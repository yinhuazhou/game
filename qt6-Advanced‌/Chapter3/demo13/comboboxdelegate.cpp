#include "comboboxdelegate.h"

comboBoxDelegate::comboBoxDelegate(QObject *parent):QStyledItemDelegate(parent)
{}

comboBoxDelegate::~comboBoxDelegate(){}
//创建代理控件的对象，并返回该对象
QWidget * comboBoxDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index)const{
    QComboBox *comBox=new QComboBox(parent);
    QIcon male("D:\\Chapter3\\male.png");
    QIcon female("D:\\Chapter3\\female.png");
    comBox->addItem(male,"男");
    comBox->addItem(female,"女");
    comBox->setEditable(false);
    return comBox;//返回代理控件
}
//读取数据项的值，并设置代理控件中的数据
void comboBoxDelegate::setEditorData(QWidget *editor, QModelIndex &index){
    const QAbstractItemModel *model=index.model();//获取模型
    QComboBox *editor1=static_cast<QComboBox*>(editor);
    QVariant value=model->data(index,Qt::DisplayRole);
    if(value.toString()=="男")
        editor1->setCurrentIndex(0);
    else
        editor1->setCurrentIndex(1);
}
//把代理控件的数据写入数据模型中
void comboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, QModelIndex &index){
    QComboBox *editor1=static_cast<QComboBox*>(editor);
    if(editor1->isVisible()){
        int current=editor1->currentIndex();
        QString text=editor1->itemText(current);
        QIcon icon=editor1->itemIcon(current);
        model->setData(index,text,Qt::DisplayRole);
        model->setData(index,icon,Qt::DecorationRole);
    }

}
//设置代理控件的位置
void comboBoxDelegate::updateEditorGeometry(QWidget *editor, QStyleOptionViewItem &option, QModelIndex &index){
    editor->setGeometry(option.rect);
}

