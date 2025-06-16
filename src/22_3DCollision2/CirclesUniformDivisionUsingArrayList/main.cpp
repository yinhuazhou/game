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
const int gBoxListBlockSize = 100; //即将来临
const int gHitListBlockSize = 300; //即将来临
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
	list< int* >* boxList = new list< int* >[ gDivision * gDivision ];
	int* boxListBlockPos = new int[ gDivision * gDivision ];
	for ( int i = 0; i < gDivision * gDivision; ++i ){ //初始化列表位置。
		boxListBlockPos[ i ] = gBoxListBlockSize;
	}
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
				if ( boxListBlockPos[ boxIndex ] == gBoxListBlockSize ){
					boxList[ boxIndex ].push_back( new int[ gBoxListBlockSize ] );
					boxListBlockPos[ boxIndex ] = 0;
				}
				int* a = boxList[ boxIndex ].back();
				a[ boxListBlockPos[ boxIndex ] ] = i;
				++boxListBlockPos[ boxIndex ];
			}
		}
	}
	//这里的代码非常复杂，因此我们使用许多别名来使其简短。
	//理想情况下，应该以可以与列表相同的方式使用的形式创建列表<int *>。
	//
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		//别名准备
		typedef list< int* >::const_iterator It; //别名
		const list< int* >& l = boxList[ i ]; //别名
		int blockPos = boxListBlockPos[ i ];
		int blockNumber = static_cast< int >( l.size() );
		int lastBlock = blockNumber - 1;
		It it0 = l.begin();
		int blockProcessed0 = 0;
		while ( it0 != l.end() ){
			//如果是最后一个块则取在块中的位置
			int n0 = ( blockProcessed0 != lastBlock ) ? gBoxListBlockSize : blockPos;
			for ( int j = 0; j < n0; ++j ){
				It it1 = it0;
				int blockProcessed1 = blockProcessed0; //1从第一块开始。之前不要。
				while ( it1 != l.end() ){
					//如果是最后一个块则取在块中的位置
					int n1 = ( blockProcessed1 != lastBlock ) ? gBoxListBlockSize : blockPos;
					int kBegin = ( it0 == it1 ) ? ( j + 1 ) : 0; //同一块之间的K = j + 1
					for ( int k = kBegin; k < n1; ++k ){
						int i0 = ( *it0 )[ j ];
						int i1 = ( *it1 )[ k ];
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
					++blockProcessed1;
					++it1; //不知道到底是什么括号。
 				}
			}
			++blockProcessed0;
			++it0;
		}
	}
	//将命中列表复制到数组。排序。
	int blockNumber = static_cast< int >( hitList.size() );
	HitPair* hitArray = 0;
	if ( blockNumber > 0 ){
		int hitArraySize = ( blockNumber - 1 ) * gHitListBlockSize + hitListBlockPos;
		hitArray = new HitPair[ hitArraySize ];
		int blockCopied = 0;
		list< HitPair* >::iterator hitListIt = hitList.begin();
		while ( blockCopied < ( blockNumber - 1 ) ){
			for ( int i = 0; i < gHitListBlockSize; ++i ){
				hitArray[ blockCopied * gHitListBlockSize + i ] = ( *hitListIt )[ i ];
			}
			++blockCopied;
			++hitListIt;
		}
		//最后一块
		for ( int i = 0; i < hitListBlockPos; ++i ){
			hitArray[ blockCopied * gHitListBlockSize + i ] = hitList.back()[ i ];
		}
		//排序
		sort( hitArray, hitArray + hitArraySize );
		//排除重复项
		size_t hitN = unique( hitArray, hitArray + hitArraySize ) - hitArray;
		//施力
		for ( size_t i = 0; i < hitN; ++i ){
			addForce( hitArray[ i ].mI0, hitArray[ i ].mI1 );
		}
		//清理工作
		for ( list< HitPair* >::iterator i = hitList.begin(); i != hitList.end(); ++i ){
			SAFE_DELETE_ARRAY( *i );
		}
		SAFE_DELETE_ARRAY( hitArray );
	}
	//不再需要列表。删除是很麻烦的。
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		for ( list< int* >::iterator j = boxList[ i ].begin(); j != boxList[ i ].end(); ++j ){
			SAFE_DELETE_ARRAY( *j );
		}
	}
	SAFE_DELETE_ARRAY( boxList );
	SAFE_DELETE_ARRAY( boxListBlockPos );

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
		//初始化加速度
		for ( int i = 0;i < N*N; ++i ){
			//
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
