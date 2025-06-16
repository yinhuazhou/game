#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;

GraphicsDatabase* gDatabase;
Model* gModel;
double gT; //时间

const int N = 9; //点数

//插值点
Vector3 gPoints[ N ] = {
	Vector3( -10.0, 0.0, 3.0 ),
	Vector3( -10.0, 0.0, -3.0 ),
	Vector3( 10.0, 0.0, -3.0 ),
	Vector3( 10.0, 0.0, 3.0 ),
	Vector3( 5.0, -2.0, 3.0 ),
	Vector3( 5.0, -4.0, 0.0 ),
	Vector3( -5.0, 4.0, 0.0 ),
	Vector3( -5.0, 2.0, 3.0 ),
	Vector3( -10.0, 0.0, 3.0 ), //起点
};

//通过时刻
double gTimes[ N ] = {
	0.0,
	100.0,
	300.0,
	400.0,
	500.0,
	550.0,
	600.0,
	650.0,
	700.0,
};

//2下一个、1次、0次的顺序
Vector3 gEquations[ N - 1 ][ 3 ];

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "cube.txt" );
			gModel = gDatabase->createModel( "cube" );

			Vector3 v( 0.0, 0.0, 0.0 ); //第一区间的起点的斜率是否为0？因为是向量，因为计算三个值。
			//在XYZ中执行单独的计算，但是不使用vector类计算功能。
			for ( int i = 0; i < N-1; ++i ){ //由于点数为N，因此区间数为N-1
				//创建常量以便于查看。
				Vector3 p0 = gPoints[ i ];
				Vector3 p1 = gPoints[ i + 1 ];
				double t0 = gTimes[ i ];
				double t02 = t0 * t0;
				double t1 = gTimes[ i + 1 ];
				double t10 = t1 - t0;
				double t102 = t10 * t10;

				Vector3 a,b,c; //所有向量均由xyz计算
				//首先算出a。
				a.x = ( ( p1.x - p0.x ) - v.x*t10 ) / t102;
				a.y = ( ( p1.y - p0.y ) - v.y*t10 ) / t102;
				a.z = ( ( p1.z - p0.z ) - v.z*t10 ) / t102;
				//接下来是b
				b.x = v.x - 2.0 * a.x * t0;
				b.y = v.y - 2.0 * a.y * t0;
				b.z = v.z - 2.0 * a.z * t0;
				//
				c.x = p0.x - a.x*t02 - b.x*t0;
				c.y = p0.y - a.y*t02 - b.y*t0;
				c.z = p0.z - a.z*t02 - b.z*t0;

				//保存公式
				gEquations[ i ][ 0 ] = a;
				gEquations[ i ][ 1 ] = b;
				gEquations[ i ][ 2 ] = c;

				//计算终点斜率
				v.x = 2.0 * a.x * t1 + b.x;
				v.y = 2.0 * a.y * t1 + b.y;
				v.z = 2.0 * a.z * t1 + b.z;
			}
		}

		//执行循环处理。
		double period = gTimes[ N - 1 ];
		double quot = gT / period;
		int quotInt = static_cast< int >( quot ); 
		gT -= static_cast< double >( quotInt ) * period;

		//查找区间
		int last = 0;
		for ( int i = 0; i < N; ++i ){
			if ( gTimes[ i ] > gT ){
				break;
			}
			last = i;
		}

		//
		Vector3 p;
		Vector3 a = gEquations[ last ][ 0 ];
		Vector3 b = gEquations[ last ][ 1 ];
		Vector3 c = gEquations[ last ][ 2 ];
		Vector3 p0 = gPoints[ last ];
		Vector3 p1 = gPoints[ last + 1 ];
		p.x = ( ( a.x * gT ) + b.x ) * gT + c.x;
		p.y = ( ( a.y * gT ) + b.y ) * gT + c.y;
		p.z = ( ( a.z * gT ) + b.z ) * gT + c.z;
		//该值已确定。设置模型
		gModel->setPosition( p );

		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( Vector3( 0.0, 30.0, 30.0 ), Vector3( 0.0, 0.0, 0.0 ) );
		pm *= vm;
		gModel->draw( pm, Vector3( 0.0, 1.0, 0.0 ), Vector3( 1.0, 1.0, 1.0 ), Vector3( 0.2, 0.2, 0.2 ) );

		gT += 1.0;
		if ( isEndRequested() ){
			SAFE_DELETE( gModel );
			SAFE_DELETE( gDatabase );
		}
	}
}
