#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Vector2.h"
#include <sstream>
#include <list>
#include <algorithm>
using namespace std;
using namespace GameLib;

struct Circle{
	Vector2 mPosition;
	Vector2 mVelocity;
};
Circle* gCircles = 0;
const int N = 40; //
const double R = 2.0; //半径2
const double RSUM2 = ( R + R ) * ( R + R ); //半径平方和
const Vector2 gMinimum( -160.0, -160.0 );
const Vector2 gMaximum( 160.0, 160.0 );
const int gHitListBlockSize = 10000; //即将来临
bool testCircles( int index0, int index1 ); //1逐个判定函数（注：不给力）
void addForce( int i0, int i1 ); //这是增加力
const int MAX_LEVEL = 13; //最大分割段数

//----------------------这是这一章的技巧

//胜利pair
struct HitPair{ 
	bool operator<( const HitPair& a ) const { //
		if ( mI0 < a.mI0 ){
			return true;
		}else if ( mI0 > a.mI0 ){
			return false;
		}else{
			return ( mI1 < a.mI1 );
		}
	}
	bool operator==( const HitPair& a ) const {//需要unique
		return ( ( mI0 == a.mI0 ) && ( mI1 == a.mI1 ) );
	}
	int mI0;
	int mI1;
};

struct Node{
	enum Direction{
		DIR_X,
		DIR_Y,
		DIR_NONE,
	};
	Node() : 
	mDirection( DIR_NONE ),
	mLeft( 0 ), 
	mRight( 0 ), 
	mIndices( 0 ),
	mIndexNumber ( 0 ){
	}
	//递归构造（参数在x，y范围内）
	void build( 
		double x0, 
		double x1,
		double y0,
		double y1,
		int* indices,
		int* indexPos,
		Node* nodes,
		int* nodePos,
		int restLevel, //还能分几次？
		bool drawLine );
	//递归判断
	void detect( list< HitPair* >& hitList, int* hitListBlockPos, int* test, int* hit ) const; 

	Direction mDirection;
	Node* mLeft;
	Node* mRight;
	int* mIndices;
	int mIndexNumber;
};

//箱数和圆的编号pair
struct ObjPair{
	void set( int box, int circle ){
		mBox = box;
		mCircle = circle;
	}
	int mBox;
	int mCircle;
};


void processCollision( int* test, int* hit ){
	*test = 0;
	*hit = 0;
#if 1
	int n = N*N; //

	//准备节点数组。
	//最大节点数为MAX_LEVEL，所以节点数为2 ^ MAX_LEVEL-1
	//1如果是1段、2段的话是1+2段，3段的话是1+2+4段，这样考虑就可以了。
	int nodePosMax = 1;
	for ( int i = 0; i < MAX_LEVEL; ++i ){
		nodePosMax *= 2;
	}
	nodePosMax -= 1;
	//索引的最大数量是多少？
	//假设它们重叠了100次。
	int indexPosMax = n * 100;
	Node* nodes = new Node[ nodePosMax ];
	int* indices = new int[ indexPosMax ];
	//准备第一个节点。
	Node root;
	root.mIndices = indices;
	for ( int i = 0; i < n; ++i ){
		root.mIndices[ i ] = i;
	}
	root.mIndexNumber = n;
	int indexPos = n; //切割位置
	int nodePos = 1; //切割位置
	//递归构造
	bool draw = true;
	if ( Input::Manager::instance().keyboard().isOn( ' ' ) ){
		draw = false;
	}
	root.build( 
		gMinimum.x, 
		gMaximum.x, 
		gMinimum.y,
		gMaximum.y, 
		indices, 
		&indexPos, 
		nodes, 
		&nodePos, 
		MAX_LEVEL - 1, //经过MAX_LEVEL-1次分割。
		draw );
	//递归判断（在此函数中不是很困难）
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	root.detect( hitList, &hitListBlockPos, test, hit ); //第二个参数已更新，因此有一个指针

	//
	//为了使代码简单
	//将数组列表转换为普通数组
	HitPair* hitListArray = new HitPair[ hitList.size() * gHitListBlockSize ];
	int hitListArraySize = 0;
	typedef list< HitPair* >::iterator HIt; //别名
	int blockPos = 0;
	int blockNumber = static_cast< int >( hitList.size() );
	for ( HIt i = hitList.begin(); i != hitList.end(); ++i ){
		//查找当前块的大小
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? hitListBlockPos : gHitListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			hitListArray[ hitListArraySize ] = ( *i )[ j ];
			++hitListArraySize;
		}
		SAFE_DELETE_ARRAY( *i ); //不再需要了
		++blockPos;
	}

	//首先计算有多少pair的一号元素是i
	int* hitListSize = new int[ n ];
	//初始化
	for ( int i = 0; i < n; ++i ){
		hitListSize[ i ] = 0;
	}
	//循环计数
	for ( int i = 0; i < hitListArraySize; ++i ){
		++hitListSize[ hitListArray[ i ].mI0 ];
	}
	//hitListSize是一个数字，它将转换为从开头开始的偏移量。
	int* hitListOffset = new int[ n ];
	int offset = 0;
	for ( int i = 0; i < n; ++i ){
		hitListOffset[ i ] = offset;
		offset += hitListSize[ i ];
		hitListSize[ i ] = 0; //大小数组设置为0。
		//
		//
	}
	//分配对象数组。
	int* hitArray = new int[ offset ];
	for ( int i = 0; i < hitListArraySize; ++i ){
		const HitPair& o = hitListArray[ i ];
		hitArray[ hitListOffset[ o.mI0 ] + hitListSize[ o.mI0 ] ] = o.mI1;
		++hitListSize[ o.mI0 ];//大小+1。退出此循环时，状态与第一次计数时的状态相同。
	}
	SAFE_DELETE_ARRAY( hitListArray );

	//
	for ( int i = 0; i < n; ++i ){
		int* box = &hitArray[ hitListOffset[ i ] ];
		int boxSize = hitListSize[ i ];
		for ( int j = 0; j < boxSize; ++j ){
			if ( box[ j ] >= 0 ){
				addForce( i, box[ j ] );
				//
				for ( int k = j + 1; k < boxSize; ++k ){
					if ( box[ k ] == box[ j ] ){
						box[ k ] = -1;
					}
				}
			}
		}
	}
	//清理工作
	SAFE_DELETE_ARRAY( hitArray );
	SAFE_DELETE_ARRAY( hitListSize );
	SAFE_DELETE_ARRAY( hitListOffset );
	SAFE_DELETE_ARRAY( indices );
	SAFE_DELETE_ARRAY( nodes );
#else //粗糙版本进行调试
	for ( int i = 0; i < N*N; ++i ){
		for ( int j = i + 1; j < N*N; ++j ){
			++( *test );
			if ( testCircles( i, j ) ){
				++( *hit );
			}
		}
	}
#endif
}

//小技巧
void Node::build( 
double x0, 
double x1, 
double y0, 
double y1,
int* indices,
int* indexPos,
Node* nodes,
int* nodePos,
int restLevel, //经过多少次
bool draw ){
	//这里以最简单的方式进行划分。
	//除以x和y中的较长者。
	double div; //分界线
	if ( ( x1 - x0 ) > ( y1 - y0 ) ){
		mDirection = DIR_X;
		div = ( x0 + x1 ) * 0.5;
	}else{
		mDirection = DIR_Y;
		div = ( y0 + y1 ) * 0.5;
	}
	//准备计数
	int c0, c1;
	c0 = c1 = 0; //分配给左右节点的三角形数量
	//分配子节点
	mLeft = &nodes[ *nodePos + 0 ];
	mRight = &nodes[ *nodePos + 1 ];
	*nodePos += 2;
	//之后根据方向执行分支
	if ( mDirection == DIR_X ){
		for ( int i = 0; i < mIndexNumber; ++i ){
			const Circle& c = gCircles[ mIndices[ i ] ]; //不喜欢全局变量
			const Vector2& p = c.mPosition;
			if ( p.x - R <= div ){ ++c0; } //考虑到数值计算错误，此处的代码可能不合适。可以再考虑
			if ( p.x + R >= div ){ ++c1; } //它应该是<= divX + e，> = divX-e。e是误差范围。
		}
		//
		mLeft->mIndices = indices + *indexPos;
		*indexPos += c0;
		mRight->mIndices = indices + *indexPos;
		*indexPos += c1;
		//分配
		for ( int i = 0; i < mIndexNumber; ++i ){
			int idx = mIndices[ i ];
			const Circle& c = gCircles[ idx ]; //不喜欢全局变量
			const Vector2& p = c.mPosition;
			if ( p.x - R <= div ){
				mLeft->mIndices[ mLeft->mIndexNumber ] = idx;
				++mLeft->mIndexNumber;
			}
			if ( p.x + R >= div ){
				mRight->mIndices[ mRight->mIndexNumber ] = idx;
				++mRight->mIndexNumber;
			}
		}
		if ( restLevel > 1 ){ //
			if ( c0 > 1 ){ //2没有一个以上就没有除掉的意义。
				mLeft->build( x0, div, y0, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
			if ( c1 > 1 ){
				mRight->build( div, x1, y0, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
		}
		if ( draw ){ //绘制分割线
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = div - 0.25 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = div + 0.25 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = y0 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = y1 + 120.0;
			Framework::instance().drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
			Framework::instance().drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
		}
	}else{
		for ( int i = 0; i < mIndexNumber; ++i ){
			const Circle& c = gCircles[ mIndices[ i ] ]; //不喜欢全局变量
			const Vector2& p = c.mPosition;
			if ( p.y - R <= div ){ ++c0; } //考虑到数值计算错误，此处的代码可能不合适。可以再考虑
			if ( p.y + R >= div ){ ++c1; } //它应该是<= divX + e，> = divX-e。e是误差范围。
		}
		//
		mLeft->mIndices = indices + *indexPos;
		*indexPos += c0;
		mRight->mIndices = indices + *indexPos;
		*indexPos += c1;
		//分配
		for ( int i = 0; i < mIndexNumber; ++i ){
			int idx = mIndices[ i ];
			const Circle& c = gCircles[ idx ]; //不喜欢全局变量
			const Vector2& p = c.mPosition;
			if ( p.y - R <= div ){
				mLeft->mIndices[ mLeft->mIndexNumber ] = idx;
				++mLeft->mIndexNumber;
			}
			if ( p.y + R >= div ){
				mRight->mIndices[ mRight->mIndexNumber ] = idx;
				++mRight->mIndexNumber;
			}
		}
		if ( restLevel > 1 ){ //
			if ( c0 > 1 ){ //2没有一个以上就没有除掉的意义。
				mLeft->build( x0, x1, y0, div, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
			if ( c1 > 1 ){
				mRight->build( x0, x1, div, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
		}
		if ( draw ){ //绘制分割线
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = x0 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = x1 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = div - 0.25 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = div + 0.25 + 120.0;
			Framework::instance().drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
			Framework::instance().drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
		}
	}
	//不需要自己的数组。但是，由于无法释放，因此将其设置为0，这样就不会发生错误。
	//可以编写代码以便以后重用此部分，
	//预分配的数组可以更短，但变得更加复杂。
	mIndices = 0;
	mIndexNumber = 0;
}

//使用方便
void Node::detect( list< HitPair* >& hitList, int* hitListBlockPos, int* test, int* hit ) const{
	if ( mDirection == DIR_NONE ){ //
		for ( int i = 0; i < mIndexNumber; ++i ){
			int i0 = mIndices[ i ];
			for ( int j = i + 1; j < mIndexNumber; ++j ){
				int i1 = mIndices[ j ];
				++( *test );
				if ( testCircles( i0, i1 ) ){
					++( *hit );
					HitPair hit;
					//同一pair必须相同，
					if ( i0 < i1 ){
						hit.mI0 = i0;
						hit.mI1 = i1;
					}else{
						hit.mI0 = i0;
						hit.mI1 = i1;
					}
					if ( *hitListBlockPos == gHitListBlockSize ){
						hitList.push_back( new HitPair[ gHitListBlockSize ] );
						*hitListBlockPos = 0;
					}
					hitList.back()[ *hitListBlockPos ] = hit;
					++( *hitListBlockPos );
				}
			}
		}
	}else{ //
		mLeft->detect( hitList, hitListBlockPos, test, hit );
		mRight->detect( hitList, hitListBlockPos, test, hit );
	}
}


//----------------------下面是本章的主线代码

//2处理单个circle的内容。中了的话真的
bool testCircles( int i0, int i1 ){
	Circle& c0 = gCircles[ i0 ];
	const Vector2& p0 = c0.mPosition;
	Circle& c1 = gCircles[ i1 ];
	const Vector2& p1 = c1.mPosition;
	//距离是多少？
	Vector2 t;
	t.setSub( p1, p0 );
	double sql = t.squareLength();
	if ( sql < RSUM2 ){ //半径应为4
		return true;
	}else{
		return false;
	}
}

void addForce( int i0, int i1 ){
	Vector2 t;
	t.setSub( gCircles[ i0 ].mPosition, gCircles[ i1 ].mPosition );
	double l = t.length();
	t *= 0.25 / l; //适当调整长度
	//弹回。由于t是p0-> p1的向量，因此将其添加到c1并从c0中减去。
	gCircles[ i0 ].mVelocity += t;
	gCircles[ i1 ].mVelocity -= t;
}

namespace GameLib{
	void Framework::update(){
		if ( !gCircles ){
			gCircles = new Circle[ N*N ];
			//初始放置
			for ( int i = 0; i < N*N; ++i ){
				gCircles[ i ].mPosition.set( 
					static_cast< double >( ( ( i % N ) - N/2 ) * 4 ) + 0.001 * i, //错开
					static_cast< double >( ( ( i / N ) - N/2 ) * 4 ) );
			}
		}
		//速度初始化
		for ( int i = 0;i < N*N; ++i ){
			//沿原点方向初始化速度
			gCircles[ i ].mVelocity.setMul( gCircles[ i ].mPosition, -0.001 );
		}
		int test;
		int hit;
		processCollision( &test, &hit ); //碰撞检测函数

		//更新
		for ( int i = 0;i < N*N; ++i ){
			gCircles[ i ].mPosition += gCircles[ i ].mVelocity;

			//绘制
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = gCircles[ i ].mPosition.x - 0.5 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = gCircles[ i ].mPosition.x + 0.5 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = gCircles[ i ].mPosition.y - 0.5 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = gCircles[ i ].mPosition.y + 0.5 + 120.0;
			drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ] );
			drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ] );
		}
		ostringstream oss;
		oss << frameRate() << " " << test << " " << hit;
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}
