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
int gDivision = 10;
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
	list< int >* boxList = new list< int >[ gDivision * gDivision ];
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
				boxList[ boxIndex ].push_back( i );
			}
		}
	}
	//
	typedef list< int >::iterator It; //太长了使用别名
	list< HitPair > hitList;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		for ( It j = boxList[ i ].begin(); j != boxList[ i ].end(); ++j ){
			It k = j; //在j之后
			++k;
			for ( ; k != boxList[ i ].end(); ++k ){
++( *test );
				if ( testCircles( *j, *k ) ){
++( *hit );
					HitPair hit;
					//同一pair必须相同，
					if ( *j < *k ){
						hit.mI0 = *j;
						hit.mI1 = *k;
					}else{
						hit.mI0 = *k;
						hit.mI1 = *j;
					}
					hitList.push_back( hit );
				}
			}
		}
	}
	//将pair复制到数组中
	HitPair* hitArray = new HitPair[ hitList.size() ];
	int j = 0;
	for ( list< HitPair >::iterator i = hitList.begin(); i != hitList.end(); ++i ){
		hitArray[ j ] = *i;
		++j;
	}
	//排序
	sort( hitArray, hitArray + j );
	//排除重复项
	size_t hitN = unique( hitArray, hitArray + j ) - hitArray;
	//施力
	for ( size_t i = 0; i < hitN; ++i ){
		addForce( hitArray[ i ].mI0, hitArray[ i ].mI1 );
	}
	SAFE_DELETE_ARRAY( hitArray );
	SAFE_DELETE_ARRAY( boxList );
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
			//通过对原点的引力来初始化速度
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
