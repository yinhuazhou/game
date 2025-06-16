#include "GameLib/Framework.h"
#include "GameLib/Math.h"
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
const int gDivision = 30;
const int gBoxListBlockSize = 10000; //即将来临
const int gHitListBlockSize = 10000; //即将来临
bool testCircles( int index0, int index1 ); //1逐个判定函数（注：不给力）
void addForce( int i0, int i1 ); //这是增加力

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
	//计算箱子大小
	Vector2 boxSize; //箱体尺寸
	boxSize.setSub( gMaximum, gMinimum );
	boxSize *= 1.0 / static_cast< float >( gDivision );
	//准备列表
	list< ObjPair* > boxList;
	int boxListBlockPos = gBoxListBlockSize;

	//第一阶段。将箱子编号和物体编号存储在ArrayList中
	for ( int i = 0; i < n; ++i ){
		Vector2 t;
		t.setSub( gCircles[ i ].mPosition, gMinimum );
		double minX = t.x - R;
		double maxX = t.x + R;
		double minY = t.y - R;
		double maxY = t.y + R;
		//转换为箱子编号（可以更快地进行除法计算？）
		int minXBox = static_cast< int >( minX / boxSize.x );
		int maxXBox = static_cast< int >( maxX / boxSize.x );
		int minYBox = static_cast< int >( minY / boxSize.y );
		int maxYBox = static_cast< int >( maxY / boxSize.y );
		ASSERT( minXBox >= 0 && maxXBox < gDivision && minYBox >=0 && maxYBox < gDivision );
		for ( int j = minXBox; j <= maxXBox; ++j ){
			for ( int k = minYBox; k <= maxYBox; ++k ){
				int boxIndex = k * gDivision + j; //箱子号为“ Y *（分割数）+ X”
				if ( boxListBlockPos == gBoxListBlockSize ){
					boxList.push_back( new ObjPair[ gBoxListBlockSize ] );
					boxListBlockPos = 0;
				}
				boxList.back()[ boxListBlockPos ].set( boxIndex, i ) ;
				++boxListBlockPos;
			}
		}
	}
	//第二阶段。
	int* boxListSize = new int[ gDivision * gDivision ];
	//初始化
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		boxListSize[ i ] = 0;
	}
	//循环计数
	typedef list< ObjPair* >::iterator It; //别名
	int blockPos = 0;
	int blockNumber = static_cast< int >( boxList.size() );
	for ( It i = boxList.begin(); i != boxList.end(); ++i ){
		//查找当前块的大小
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? boxListBlockPos : gBoxListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			const ObjPair& o = ( *i )[ j ];
			++boxListSize[ o.mBox ];
		}
		++blockPos;
	}
	//boxListSize是一个数字，该数字将转换为从开头开始的偏移量。
	int* boxListOffset = new int[ gDivision * gDivision ];
	int offset = 0;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		boxListOffset[ i ] = offset;
		offset += boxListSize[ i ];
		boxListSize[ i ] = 0; //大小数组设置为0。
		//
		//
	}
	//第三阶段。实际上为每个箱子生成一个列表。

	//分配对象数组。
	int* boxArray = new int[ offset ];
	//输入。
	blockPos = 0;
	blockNumber = static_cast< int >( boxList.size() );
	for ( It i = boxList.begin(); i != boxList.end(); ++i ){
		//查找当前块的大小
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? boxListBlockPos : gBoxListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			const ObjPair& o = ( *i )[ j ];
			boxArray[ boxListOffset[ o.mBox ] + boxListSize[ o.mBox ] ] = o.mCircle;
			++boxListSize[ o.mBox ]; //大小+1。退出此循环时，状态与第一次计数时的状态相同。
		}
		SAFE_DELETE_ARRAY( *i );
		++blockPos;
	}
	//由于这种安排，它非常简单。
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	for ( int i = 0; i < gDivision * gDivision; ++i ){ //在箱子里循环
		//准备箱子中的数量
		int boxSize = boxListSize[ i ];
		const int* box = &boxArray[ boxListOffset[ i ] ]; //别名
		for ( int j = 0; j < boxSize; ++j ){ //第一个对象循环
			for ( int k = j + 1; k < boxSize; ++k ){ //第二对象循环
				int i0 = box[ j ];
				int i1 = box[ k ];
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
					if ( hitListBlockPos == gHitListBlockSize ){
						hitList.push_back( new HitPair[ gHitListBlockSize ] );
						hitListBlockPos = 0;
					}
					hitList.back()[ hitListBlockPos ] = hit;
					++hitListBlockPos;
				}
			}
		}
	}
	//调用addForce（），删除匹配列表的重复项。

	//为了使代码简单
	//将数组列表转换为普通数组
	HitPair* hitListArray = new HitPair[ hitList.size() * gHitListBlockSize ];
	int hitListArraySize = 0;
	typedef list< HitPair* >::iterator HIt; //别名
	blockPos = 0;
	blockNumber = static_cast< int >( hitList.size() );
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
	offset = 0;
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

	SAFE_DELETE_ARRAY( boxArray );
	SAFE_DELETE_ARRAY( boxListOffset );
	SAFE_DELETE_ARRAY( boxListSize );

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
