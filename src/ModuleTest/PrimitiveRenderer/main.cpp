#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
using namespace std;
using namespace GameLib::Math;
using namespace GameLib::Graphics;
using namespace GameLib::Scene;

int gCount;
PrimitiveRenderer gPrimitiveRenderer;
Texture gTexture;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( true );
	}
	void Framework::update(){
		if ( gCount == 0 ){
			//初始化具有1000个顶点和100个批处理的图形类
			gPrimitiveRenderer = PrimitiveRenderer::create( 1000, 100 );
			//纹理加载
			gTexture = Texture::create( "test.tga" );
			while ( !gTexture.isReady() ){
				; //等待加载
			}
		}

		//原始顶点数组
		Vector3 p[ 4 ];
		Vector2 t[ 4 ];
		unsigned c[ 4 ]; //易于识别的颜色
		p[ 0 ].set( -1.f, -1.f, 0.f );
		p[ 1 ].set( -1.f, 1.f, 0.f );
		p[ 2 ].set( 1.f, -1.f, 0.f );
		p[ 3 ].set( 1.f, 1.f, 0.f );
		t[ 0 ].set( 0.f, 0.f );
		t[ 1 ].set( 0.f, 1.f );
		t[ 2 ].set( 1.f, 0.f );
		t[ 3 ].set( 1.f, 1.f );

		c[ 0 ] = 0xffff0000; //红色的
		c[ 1 ] = 0xff00ff00; //绿
		c[ 2 ] = 0xff0000ff; //蓝色的
		c[ 3 ] = 0xffffffff; //白

		//世界变换（Z旋转）
		Matrix34 wm;
		wm.setRotationZ( gCount * 2.f );
		//创建一个视图矩阵
		Vector3 eyePosition;
		eyePosition.x = sin( gCount / 2.f ) * 4.f;
		eyePosition.z = cos( gCount / 2.f ) * 4.f;
		eyePosition.y = 1.f;
		Vector3 eyeTarget( 0.f, 0.f, 0.f );
		Matrix34 vm;
		vm.setViewTransform( eyePosition, eyeTarget, Vector3( 0.f, 1.f, 0.f ) );
		//透视变换矩阵
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			60.f, 
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			1.f, 100.f );
		//首先创建VW
		vm *= wm;
		//接下来，创建PVW
		pm *= vm;
		//矩阵集
		gPrimitiveRenderer.setTransform( pm );
		//纹理集
		gPrimitiveRenderer.setTexture( gTexture );

		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );

		//用数数来改变画什么
		if ( gCount % 300 > 200 ){ //三角形
			gPrimitiveRenderer.addTriangle( p[ 0 ], p[ 1 ], p[ 2 ], t[ 0 ], t[ 1 ], t[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ] );
			gPrimitiveRenderer.addTriangle( p[ 3 ], p[ 1 ], p[ 2 ], t[ 3 ], t[ 1 ], t[ 2 ], c[ 3 ], c[ 1 ], c[ 2 ] );
		}else if ( gCount % 300 > 100 ){ //线
			//添加线
			for ( int i = 0; i < 4; ++i ){
				for ( int j = i + 1; j < 4; ++j ){
					gPrimitiveRenderer.addLine( p[ i ], p[ j ], t[ i ], t[ j ], c[ i ], c[ j ] );
				}
			}
		}else{ //点数
			gPrimitiveRenderer.addPoint( p[ 0 ], t[ 0 ], c[ 0 ] );
			gPrimitiveRenderer.addPoint( p[ 1 ], t[ 1 ], c[ 1 ] );
			gPrimitiveRenderer.addPoint( p[ 2 ], t[ 2 ], c[ 2 ] );
			gPrimitiveRenderer.addPoint( p[ 3 ], t[ 3 ], c[ 3 ] );
		}
		//绘制一个精灵。
		//启用Alpha混合
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );
		Vector2 p2[ 2 ];
		p2[ 0 ].set( 0.f, 0.f ); //左上方
		p2[ 1 ].set( 128.f, 256.f ); //右下
		gPrimitiveRenderer.addRectangle( p2[ 0 ], p2[ 1 ], t[ 0 ], t[ 3 ], ( ( gCount % 256 ) << 24 ) | 0xffffff );

		//
		gPrimitiveRenderer.draw();

		DebugScreen() << frameRate();
		++gCount;

		if ( isEndRequested() ){
			gPrimitiveRenderer.release(); //释放处理是必需的，因为它是全局的
			gTexture.release();
		}
	}
}
