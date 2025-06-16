#include <sstream>
#include "GameLib/Framework.h"
#include "Vector3.h"
#include "Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

Vector3 gEyePosition( 0.0, 0.0, 1.0 );
Vector3 gEyeTarget( 0.0, 0.0, 0.0 );
int gCount;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		//原始顶点数组
		Vector3 p[ 4 ];
		unsigned c[ 4 ]; //易于识别的颜色
		p[ 0 ].set( -1000.0, 0.0, -1000.0 );
		p[ 1 ].set( -1000.0, 0.0, 1000.0 );
		p[ 2 ].set( 1000.0, 0.0, -1000.0 );
		p[ 3 ].set( 1000.0, 0.0, 1000.0 );
		c[ 0 ] = 0xffff0000; //红色的
		c[ 1 ] = 0xff00ff00; //绿
		c[ 2 ] = 0xff0000ff; //蓝色的
		c[ 3 ] = 0xffffffff; //白

		//创建一个视图矩阵
		Matrix34 m;
		m.setViewTransform( gEyePosition, gEyeTarget ); //这是主体

		//向量乘以矩阵
		for ( int i = 0; i < 4; ++i ){
			m.multiply( &p[ i ], p[ i ] );
		}

		//远裁面和近裁面
		const double nearClip = 1.0;
		const double farClip = 10000.0;
		//从远面和近面创建Z范围转换公式
		const double zConvA = 1.0 / ( nearClip - farClip ); //1/(n-f)
		const double zConvB = nearClip * zConvA; //n/(n-f)

		//准备移交给硬件。4维
		double p4[ 4 ][ 4 ];
		for ( int i = 0; i < 4; ++i ){
			p4[ i ][ 0 ] = p[ i ].x;
			//将y乘上640/480以调整宽高比
			p4[ i ][ 1 ] = p[ i ].y * 640.0 / 480.0;
			//转换前将z存储为w的负数。如果Z轴朝前，则此负号是必需的。
			p4[ i ][ 3 ] = -p[ i ].z;
			//z范围转换
			p4[ i ][ 2 ] = zConvA * p[ i ].z + zConvB; //
			//将z乘以w。
			p4[ i ][ 2 ] *= p4[ i ][ 3 ];
		}
		//画一个矩形。
		drawTriangle3DH( p4[ 0 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 0 ], c[ 1 ], c[ 2 ] );
		drawTriangle3DH( p4[ 3 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 3 ], c[ 1 ], c[ 2 ] );

		++gCount;

		//调整视点和注视点
		gEyePosition.x = sin( gCount ) * 2000;
		gEyePosition.z = cos( gCount ) * 2000;
		gEyePosition.y = 1000.f;
		gEyeTarget.x = gCount % 100;
		gEyeTarget.y = gCount % 200;
		gEyeTarget.z = gCount % 300;
	}
}
