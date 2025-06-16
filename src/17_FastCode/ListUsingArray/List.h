#ifndef INCLUDED_LIST_H
#define INCLUDED_LIST_H

//为便于理解，该值为double。int是指示整个位置的下标。
class List{
public:
	List( int capacity );
	~List();
	//追加 返回所有位置
	int addBefore( int position, double value ); //在指定位置之前添加
	int addAfter( int position, double value ); //在指定位置后添加
	int addHead( double value ); //添加头
	int addTail( double value ); //添加末尾
	//删掉
	void remove( int position ); //删除指定位置
	void removeHead(); //删除开头
	void removeTail(); //去除尾巴
	//
	double value( int position ) const;
	int next( int position ) const;
	int previous( int position ) const;
	int first() const;
	int last() const;
private:
	//3因为有一个排列也很麻烦，所以对构造体
	struct Node{
		int mNext;
		int mPrev;
		double mValue;
	};
	Node* mNodes;
	int mAddPosition;
	int mCapacity;
};

#endif

