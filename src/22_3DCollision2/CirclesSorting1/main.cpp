#include "GameLib/Framework.h"
#include "Vector2.h"
#include "GameLib/Math.h"
#include <sstream>
#include <algorithm>
using namespace std;
using namespace GameLib;

struct Circle{
	Vector2 mPosition;
	Vector2 mVelocity;
};
Circle* gCircles = 0;
const int N = 40; //
const double R = 2.0; //半径4
const double RSUM2 = ( R + R ) * ( R + R ); //半径平方和
bool testCircles( int index0, int index1 ); //1逐个判定函数

//----------------------这是这一章的技巧

//用于排序的结构体
struct Key{
	//定义键之间的大小关系
	bool operator<( const Key& a ){
		if ( mX < a.mX ){ //小则为真
			return true;
		}else if ( mX > a.mX ){ //如果大则为假
			return false;
		}else if ( mIsLeft && !a.mIsLeft ){ //一样 在这种情况下，优先处理左侧
			return true;
		}else if ( !mIsLeft && a.mIsLeft ){
			return false;
		}else{ //根据index确定
			return ( mIndex < a.mIndex ); 
		}
	}

	double mX; //X坐标
	int mIndex; //圆的编号
	bool mIsLeft; //如果为true，则为左端；如果为false，则为右端
};

void processCollision( int* test, int* hit ){
	*test = 0;
	*hit = 0;
#if 1
	int n = N*N; //
	//创建一个数组进行排序。
	Key* keys = new Key[ n*2 ];
	//填写key内容
	for ( int i = 0; i < n; ++ i){
		const Circle& c = gCircles[ i ];
		keys[ 2 * i + 0 ].mX = c.mPosition.x - R; //左端
		keys[ 2 * i + 1 ].mX = c.mPosition.x + R; //右端
		keys[ 2 * i + 0 ].mIndex = keys[ 2 * i + 1 ].mIndex = i; //自己的编号
		keys[ 2 * i + 0 ].mIsLeft = true; //向左
		keys[ 2 * i + 1 ].mIsLeft = false; //右
	}
	//排序
	sort( keys, keys + n*2 );
	//
	int* leftPos = new int[ n ];
	for ( int i = 0; i < n*2; ++i ){
		if ( keys[ i ].mIsLeft ){ //
			leftPos[ keys[ i ].mIndex ] = i; //mIndex号的左端是第i个键，依此类推。
		}
	}

	//分配相遇列表
	int* metList = new int[ n ];
	//一一搜寻。
	for ( int i = 0; i < n; ++i ){
		//对于搜索的内容，首先从表中找到最左边的位置。
		int pos = leftPos[ i ];
		++pos; //接下来
		int listPos = 0; //列表个数
		while ( keys[ pos ].mIndex != i ){ //循环直到找到右端
			int idx = keys[ pos ].mIndex; //对手编号
			if ( keys[ pos ].mIsLeft ){ //左边找到
				++( *test );
				if ( testCircles( i, idx ) ){ //判断这个
					++( *hit );
				}
				metList[ listPos ] = idx; //添加到相遇列表
				++listPos;
			}else{ //右侧发现
				int foundPos = 0; //找出是否在列表中
				for ( int foundPos = 0; foundPos < listPos; ++foundPos ){
					if ( idx == metList[ foundPos ] ){
						break; //发现！
					}
				}
				if ( foundPos < listPos ){ //
					//删除。
					for ( int j = foundPos; j < listPos-1; ++j ){
						metList[ j ] = metList[ j + 1 ];
					}
					--listPos;
				}else{ //找不到 首次对面，判定。
					++( *test );
					if ( testCircles( i, idx ) ){
						++( *hit );
					}
				}
			}
			++pos; //下一个
		}
	}
	SAFE_DELETE_ARRAY( metList );
	SAFE_DELETE_ARRAY( leftPos );
	SAFE_DELETE_ARRAY( keys );
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
	if ( sql < RSUM2 ){ 
		double l = GameLib::sqrt( sql ) + 0.0000001; //防止0作为除数
		t *= 0.25 / l; //适当调整长度
		//弹回。由于t是p0-> p1的向量，因此将其添加到c1并从c0中减去。
		c1.mVelocity += t;
		c0.mVelocity -= t;
		return true;
	}else{
		return false;
	}
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
