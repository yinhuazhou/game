#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Vector3.h"
#include "Matrix34.h"

#include <sstream>
using namespace std;

bool gFirstFrame = true;
Vector3 gTranslation( 0.0, 0.0, 0.0 );
Vector3 gScale( 1.0, 1.0, 1.0 );
int gCount;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );

		//原始顶点数组
		Vector3 p[ 4 ];
		p[ 0 ].set( -1000.0, 0.0, -1000.0 );
		p[ 1 ].set( -1000.0, 0.0, 1000.0 );
		p[ 2 ].set( 1000.0, 0.0, -1000.0 );
		p[ 3 ].set( 1000.0, 0.0, 1000.0 );
		//创建矩阵
		//将其在三个轴上适当旋转，然后将其稍微向后移动。为便于查看。
		//矩阵创建的顺序相反，先移动。
		Matrix34 m;
		m.setTranslation( Vector3( 0.0, -100.0, -3000.0 ) );
		m.rotateZ( gCount );
		m.rotateY( gCount );
		m.rotateX( gCount );
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
		drawTriangle3DH( p4[ 0 ], p4[ 1 ], p4[ 2 ] );
		drawTriangle3DH( p4[ 3 ], p4[ 1 ], p4[ 2 ] );

		++gCount;
	}
}
