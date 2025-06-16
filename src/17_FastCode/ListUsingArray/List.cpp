#include "List.h"

//使用dummy使代码非常容易！
static const int DUMMY_HEAD = 0;
static const int DUMMY_TAIL = 1;

//要做一些工作，以使开头和结尾不需要特别处理。
List::List( int capacity ) : 
mNodes( 0 ),
mAddPosition( 0 ),
mCapacity( capacity + 2 ){
	mNodes = new Node[ capacity + 2 ];
	//0特别节目。在第一个元素之前的虚拟元素。
	Node* e;
	e = &mNodes[ DUMMY_HEAD ];
	e->mNext = 1;
	e->mPrev = -1;
	e->mValue = 0.0;
	//1节目也很特别。在最后一个元素之后的虚拟元素。
	e = &mNodes[ DUMMY_TAIL ];
	e->mNext = -1;
	e->mPrev = 0;
	e->mValue = 0.0;

	mAddPosition = 2; //2开始添加
}

List::~List(){
	delete[] mNodes;
	mNodes = 0;
}

int List::addAfter( int position, double v ){
	//添加的地方是addPosition
	Node* e = &mNodes[ mAddPosition ];
	//取出指定的物体
	Node* cur = &mNodes[ position ];
	//取出下一个家伙
	Node* next = &mNodes[ cur->mNext ];
	//存值
	e->mValue = v;
	e->mPrev = position; //在指定地点之前
	e->mNext = cur->mNext; //
	//
	cur->mNext = mAddPosition;
	//
	next->mPrev = mAddPosition;
	++mAddPosition;

	return mAddPosition - 1;
}

int List::addBefore( int position, double v ){
	//添加的地方是addPosition
	Node* e = &mNodes[ mAddPosition ];
	//取出指定的物体
	Node* cur = &mNodes[ position ];
	//取出前一个
	Node* prev = &mNodes[ cur->mPrev ];
	//存值
	e->mValue = v;
	e->mPrev = cur->mPrev; //
	e->mNext = position; //下一个指定的
	//
	cur->mPrev = mAddPosition;
	//
	prev->mNext = mAddPosition;
	++mAddPosition;

	return mAddPosition - 1;
}

int List::addHead( double v ){
	return addBefore( first(), v ); //借助于dummy元素可以写
}

int List::addTail( double v ){
	return addAfter( last(), v ); //借助于dummy元素可以写
}

void List::remove( int position ){
	//指定
	Node* cur = &mNodes[ position ];
	//下一个
	Node* next = &mNodes[ cur->mNext ];
	//前
	Node* prev = &mNodes[ cur->mPrev ];
	//上一个，下一个。
	prev->mNext = cur->mNext;
	//
	next->mPrev = cur->mPrev;

	//如果删除了结尾，请回退addPosition。
	//也许可以添加更多。
	if ( position == mAddPosition - 1 ){
		--mAddPosition;
	}
}

void List::removeHead(){ //等同于别名。
	remove( first() );
}

void List::removeTail(){ //等同于别名。
	remove( last() );
}

double List::value( int position ) const {
	return mNodes[ position ].mValue;
}

int List::next( int position ) const {
	int r = mNodes[ position ].mNext;
	return ( r >= 2 ) ? r : -1; //如果指向dummy，则替换为-1
}

int List::previous( int position ) const {
	int r = mNodes[ position ].mPrev;
	return ( r >= 2 ) ? r : -1; //如果指向dummy，则替换为-1
}

int List::first() const {
	return mNodes[ DUMMY_HEAD ].mNext; //0第二个是虚假的开头，然后是真正的开头
}

int List::last() const {
	return mNodes[ DUMMY_TAIL ].mPrev; //1第二个是虚假的结尾，在那之前是真的结尾
}
