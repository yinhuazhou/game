#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/Vertex.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib::Math;
using namespace GameLib::Graphics;

VertexBuffer gVertexBuffer;
IndexBuffer gIndexBuffer;
Texture gTexture;
int gCount;
GameLib::Graphics::LightingMode gLightingMode = GameLib::Graphics::LIGHTING_NONE;

namespace GameLib{
	//使用Configuration类进行初始化。
	void Framework::configure( Configuration* conf ){
		conf->setWidth( 640 );
		conf->setHeight( 480 );
		conf->forbidFullScreen( true );
		conf->enableFullScreen( false ); //启动全屏模式
		conf->enableVSync( false ); //基于硬件固定帧率
		conf->enableAntiAlias( false ); //是否减轻锯齿
		conf->setTitle( "DrawCube Sample" ); //标题栏文字
	}
	void Framework::update(){
		using namespace Graphics;
		//
		Graphics::Manager m = Graphics::Manager::instance();
		if ( gCount == 0 ){ 
			m.setTextureFilter( TEXTURE_FILTER_POINT );
			//--------------创建顶点缓冲区
			gVertexBuffer = VertexBuffer::create( 8 ); //8个顶点的顶点缓冲区
			Vertex* v = gVertexBuffer.lock(); //获取写指针
			//输入位置
			v[ 0 ].mPosition.set( -1.f, -1.f, -1.f );
			v[ 1 ].mPosition.set( -1.f, -1.f, 1.f );
			v[ 2 ].mPosition.set( -1.f, 1.f, -1.f );
			v[ 3 ].mPosition.set( -1.f, 1.f, 1.f );
			v[ 4 ].mPosition.set( 1.f, -1.f, -1.f );
			v[ 5 ].mPosition.set( 1.f, -1.f, 1.f );
			v[ 6 ].mPosition.set( 1.f, 1.f, -1.f );
			v[ 7 ].mPosition.set( 1.f, 1.f, 1.f );
			//插入法线。尝试倾斜法线。长度约1
			v[ 0 ].mNormal.set( -0.57f, -0.57f, -0.57f );
			v[ 1 ].mNormal.set( -0.57f, -0.57f, 0.57f );
			v[ 2 ].mNormal.set( -0.57f, 0.57f, -0.57f );
			v[ 3 ].mNormal.set( -0.57f, 0.57f, 0.57f );
			v[ 4 ].mNormal.set( 0.57f, -0.57f, -0.57f );
			v[ 5 ].mNormal.set( 0.57f, -0.57f, 0.57f );
			v[ 6 ].mNormal.set( 0.57f, 0.57f, -0.57f );
			v[ 7 ].mNormal.set( 0.57f, 0.57f, 0.57f );
			//添加顶点颜色（我想查看光照效果，所以是纯白色
			for ( int i = 0; i < 8; ++i ){
				v[ i ].mColor = 0xffffffff;
			}
			//插入纹理坐标（适当）
			v[ 0 ].mUv.set( 0.f, 0.f );
			v[ 1 ].mUv.set( 0.f, 1.f );
			v[ 2 ].mUv.set( 1.f, 0.f );
			v[ 3 ].mUv.set( 1.f, 1.f );
			v[ 4 ].mUv.set( 0.f, 0.f );
			v[ 5 ].mUv.set( 0.f, 1.f );
			v[ 6 ].mUv.set( 1.f, 0.f );
			v[ 7 ].mUv.set( 1.f, 1.f );
			//写入后关闭
			gVertexBuffer.unlock( &v );

			//-------------创建索引缓冲区
			//3因为索引中只有一个三角形，所以索引缓冲器的大小通常是3的倍数
			gIndexBuffer = IndexBuffer::create( 36 ); //36顶点的下一步缓冲
			unsigned short indices[ 36 ] = {
				0, 1, 3, 3, 2, 0,
				4, 6, 7, 7, 5, 4,
				0, 4, 5, 5, 1, 0,
				2, 3, 7, 7, 6, 2,
				7, 3, 1, 1, 5, 7,
				2, 6, 4, 4, 0, 2,
			};
			unsigned short* iBuffer = gIndexBuffer.lock();
			for ( int i = 0; i < 36; ++i ){
				iBuffer[ i ] = indices[ i ];
			}
			gIndexBuffer.unlock( &iBuffer );

			//-------------创建纹理
			gTexture = Texture::create( "robo.tga" );
			while ( !gTexture.isReady() ){
				; //等待加载
			}
		}
		//指定要使用的数据
		m.setVertexBuffer( gVertexBuffer );
		m.setIndexBuffer( gIndexBuffer );
		m.setTexture( gTexture );

		//设置Alpha混合等。
		m.setBlendMode( BLEND_LINEAR );
		m.setCullMode( CULL_BACK ); //
		m.enableDepthTest( true );
		m.enableDepthWrite( true );
		m.setLightingMode( gLightingMode );

		//设置世界矩阵
		Matrix34 wm;
		wm.setRotationY( gCount * 0.09f );
		wm.rotateX( gCount * 0.08f );
		wm.rotateZ( gCount * 0.07f );
		//设置视图矩阵
		Matrix34 vm;
		Vector3 eyePosition( 
			4.f * sin( gCount * 0.01f ),
			0.f,
			4.f * cos( gCount * 0.01f ) );
		Vector3 eyeTarget( 0.f, 0.f, 0.f );
		Vector3 up( 0.f, 1.f, 0.f ); //指定“朝上”的向量。对其进行调整并将其倾斜到侧面。
		vm.setViewTransform( eyePosition, eyeTarget, up ); 

		//设定投影矩阵
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			45.f, //45度
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			1.f,
			1000.f );
		
		//将矩阵传递给硬件
		pm *= vm; //透视变换×视图变换
		m.setProjectionViewMatrix( pm ); //透视变换视图矩阵
		m.setWorldMatrix( wm ); //世界矩阵

		//进行光源设置。
		Vector3 lightPositions[ 4 ];
		float lightIntensities[ 4 ];
		for ( int i = 0; i < 4; ++i ){
			//第一名
			float phase = 90.f * static_cast< float >( i );
			float t = gCount * 0.025f + phase;
			lightPositions[ i ].x = 10.f * sin( t ) * sin( t );
			lightPositions[ i ].y = 10.f * sin( t ) * cos( t );
			lightPositions[ i ].z = 10.f * cos( t );
			lightIntensities[ i ] = 5.f;
		}
		//光源为白色，红色，绿色，红色
		Vector3 lightColors[ 4 ];
		lightColors[ 0 ].set( 1.f, 1.f, 1.f );
		lightColors[ 1 ].set( 1.f, 0.f, 0.f );
		lightColors[ 2 ].set( 0.f, 1.f, 0.f );
		lightColors[ 3 ].set( 0.f, 0.f, 1.f );
		//环境光约为0.3
		Vector3 ambient( 0.3f, 0.3f, 0.3f );
		//
		Vector3 diffuseColor( 1.f, 1.f, 1.f );
		Vector3 specularColor( 1.f, 1.f, 1.f );
		//更改此设置将更改高光的清晰度。图像越小，表面越粗糙
		float specularSharpness = 100.f;
		//相机位置设定
		m.setEyePosition( eyePosition );
		//设置光源信息
		m.setAmbientColor( ambient );
		m.setDiffuseColor( diffuseColor );
		m.setSpecularColor( specularColor );
		m.setSpecularSharpness( specularSharpness );
		for ( int i = 0; i < 4; ++i ){
			m.setLightPosition( i, lightPositions[ i ] );
			m.setLightColor( i, lightColors[ i ] );
			m.setLightIntensity( i, lightIntensities[ i ] );
		}
		//要做的就是绘制
		//从索引缓冲区开始依次使用参数，绘制多少以及绘制的内容。
		m.drawIndexed( 0, 12, PRIMITIVE_TRIANGLE );
		//在上述情况下，“从0开始绘制第12个三角形”。使用了36个索引。

		//以下是使用空格键切换光源的代码。
		if ( Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			if ( gLightingMode == LIGHTING_NONE ){
				gLightingMode = LIGHTING_PER_VERTEX;
			}else if ( gLightingMode == LIGHTING_PER_VERTEX ){
				gLightingMode = LIGHTING_PER_PIXEL;
			}else if ( gLightingMode == LIGHTING_PER_PIXEL ){
				gLightingMode = LIGHTING_NONE;
			}
		}
		DebugScreen ds = DebugScreen::instance();
		if ( gLightingMode == LIGHTING_NONE ){
			ds << "Lighting : NONE ( SPACE KEY TO CHANGE )";
		}else if ( gLightingMode == LIGHTING_PER_VERTEX ){
			ds << "Lighting : PER VERTEX ( SPACE KEY TO CHANGE )";
		}else if ( gLightingMode == LIGHTING_PER_PIXEL ){
			ds << "Lighting : PER PIXEL ( SPACE KEY TO CHANGE )";
		}
		++gCount;

		//清理工作
		if ( isEndRequested() ){
			//全局的不会自行销毁所以需要释放
			gTexture.release();
			gIndexBuffer.release();
			gVertexBuffer.release();
		}
	}
}
