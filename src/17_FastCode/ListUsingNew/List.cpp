#include "List.h"

//要做一些工作，以使开头和结尾不需要特别处理。
List::List(){
	//开始之前的dummy元素
	mDummyHead.mNext = &mDummyTail;
	mDummyHead.mPrev = 0;
	mDummyHead.mValue = 0.0;
	//最后一个元素之后的虚拟元素。
	mDummyTail.mNext = 0;
	mDummyTail.mPrev = &mDummyHead;
	mDummyTail.mValue = 0.0;
}

List::~List(){
	//抹掉一切
	Node* p = first();
	while ( p ){
		Node* nxt = next( p );
		delete p;
		p = nxt;
	}
}

List::Node* List::addAfter( Node* cur, double v ){
	Node* e = new Node();
	//取出下一个家伙
	Node* nxt = cur->mNext;
	//存值
	e->mValue = v;
	e->mPrev = cur; //在指定地点之前
	e->mNext = nxt; //
	//
	cur->mNext = e;
	//
	nxt->mPrev = e;

	return e;
}

List::Node* List::addBefore( Node* cur, double v ){
	Node* e = new Node();
	//取出前一个
	Node* prev = cur->mPrev;
	//存值
	e->mValue = v;
	e->mPrev = prev; //
	e->mNext = cur; //下一个指定的
	//
	cur->mPrev = e;
	//
	prev->mNext = e;

	return e;
}

List::Node* List::addHead( double v ){
	return addBefore( first(), v ); //借助于dummy元素可以写
}

List::Node* List::addTail( double v ){
	return addAfter( last(), v ); //借助于dummy元素可以写
}

void List::remove( Node* cur ){
	//下一个
	Node* nxt = cur->mNext;
	//前
	Node* prev = cur->mPrev;
	//上一个，下一个。
	prev->mNext = nxt;
	//
	nxt->mPrev = prev;
	//扑灭
	delete cur;
	cur = 0;
}

void List::removeHead(){ //等同于别名。
	remove( first() );
}

void List::removeTail(){ //等同于别名。
	remove( last() );
}

double List::value( Node* p ) const {
	return p->mValue;
}

List::Node* List::next( Node* p ) const {
	Node* r = p->mNext;
	if ( r == &mDummyHead || r == &mDummyTail ){
		return 0;
	}else{
		return r;
	}
}

List::Node* List::previous( Node* p ) const {
	Node* r = p->mPrev;
	if ( r == &mDummyHead || r == &mDummyTail ){
		return 0;
	}else{
		return r;
	}
}

List::Node* List::first() const {
	return mDummyHead.mNext; //dummy之后的头真实的头
}

List::Node* List::last() const {
	return mDummyTail.mPrev; //在dummy末尾之前是真实的末尾
}
