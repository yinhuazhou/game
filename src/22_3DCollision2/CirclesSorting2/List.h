#ifndef INCLUDED_GAMELIB_LIST_H
#define INCLUDED_GAMELIB_LIST_H

class List{
public:
	explicit List( int capacity );
	~List();
	//添加末尾。返回添加位置。
	int addTail( int value );
	//删除指定位置
	void remove( int position );
	///获取值
	int value( int position ) const;
	//通过isEnd（）检查是否在结尾
	int next( int position ) const;
	//获取头
	int first() const;
	//确定位置是否结束。
	bool isEnd( int position ) const;
private:
	struct Node{
		int mNext;
		int mPrev;
		int mValue;
	};
	List( const List& ); //禁止复制构造函数
	void operator=( const List& ); //禁止赋值

	Node* mNodes;
	int* mEmptyStack; //查找空元素是O（1）
	int mStackTop;
	const int mCapacity; //构造后不允许修改所以const
};

//编写cpp很麻烦所以在这里写。速度也比较快。

//要做一些工作，以使开头和结尾不需要特别处理。
//mNodes [mCapacity]是一个虚拟节点。因为在开始和结束时都使用了这个虚拟元素，处理才更加容易。
inline List::List( int capacity ) : 
mNodes( 0 ),
mEmptyStack( 0 ),
mCapacity( capacity ){
	mNodes = new Node[ mCapacity + 1 ]; //最后是一个dummy节点。所以+1
	mEmptyStack = new int[ mCapacity ];
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack[ i ] = i; //存储空位置的编号。因为刚开始所以全部都是
	}
	mStackTop = mCapacity - 1; //
	//虚拟节点设置。
	mNodes[ mCapacity ].mNext = mNodes[ mCapacity ].mPrev = mCapacity;
}

inline List::~List(){
	SAFE_DELETE_ARRAY( mNodes );
	SAFE_DELETE_ARRAY( mEmptyStack );
}

inline int List::addTail( int v ){
	//获得可用空间
	int newPos = mEmptyStack[ mStackTop ];
	--mStackTop;

	mNodes[ newPos ].mValue = v; //传入值
	mNodes[ newPos ].mNext = mCapacity; //接下来是一个虚拟节点
	int oldTail = mNodes[ mCapacity ].mPrev; //
	mNodes[ newPos ].mPrev = oldTail; //将旧尾巴放在新尾巴之前

	mNodes[ oldTail ].mNext = newPos; //
	mNodes[ mCapacity ].mPrev = newPos; //自己在dummy前
	return newPos; //返回添加它的位置
}

inline void List::remove( int position ){
	Node* cur = &mNodes[ position ]; //自己
	Node* nxt = &mNodes[ cur->mNext ]; //下一个
	Node* prev = &mNodes[ cur->mPrev ]; //前
	//上一个，下一个。
	prev->mNext = cur->mNext;
	//
	nxt->mPrev = cur->mPrev;

	//放入空编号列表
	++mStackTop;
	mEmptyStack[ mStackTop ] = position;
}

inline int List::value( int position ) const {
	return mNodes[ position ].mValue;
}

inline int List::next( int position ) const {
	return mNodes[ position ].mNext;
}

inline int List::first() const {
	//dummy之后是第一个
	return mNodes[ mCapacity ].mNext;
}

inline bool List::isEnd( int position ) const {
	//拥有虚拟容量mCapacity。
	return ( position == mCapacity );
}

#endif

