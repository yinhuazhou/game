#ifndef INCLUDED_LIST_H
#define INCLUDED_LIST_H

//为便于理解，该值为double
class List{
public:
	struct Node{
		Node* mNext;
		Node* mPrev;
		double mValue;
	};
	List();
	~List();
	//追加 返回所有位置
	Node* addBefore( Node* , double value ); //在指定位置之前添加
	Node* addAfter( Node*, double value ); //在指定位置后添加
	Node* addHead( double value ); //添加头
	Node* addTail( double value ); //添加末尾
	//删掉
	void remove( Node* ); //删除指定位置
	void removeHead(); //删除开头
	void removeTail(); //去除尾巴
	//
	double value( Node* ) const;
	Node* next( Node* ) const;
	Node* previous( Node* ) const;
	Node* first() const;
	Node* last() const;
private:
	Node mDummyHead;
	Node mDummyTail;
};

#endif

