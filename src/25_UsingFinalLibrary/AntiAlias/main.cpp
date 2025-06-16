//--------抗锯齿---
bool gMultiSampleAntiAlias = true; //重写一下

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
#include "GameLib/Graphics/Vertex.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/WindowCreator/WindowCreator.h"
using namespace GameLib::Math;
using namespace GameLib::Graphics;

VertexBuffer gVertexBuffer;
Texture gTexture;
int gCount;
GameLib::Graphics::TextureFilter gTextureFilter = GameLib::Graphics::TEXTURE_FILTER_POINT;

namespace GameLib{
	//使用Configuration类进行初始化。
	void Framework::configure( Configuration* conf ){
		conf->enableAntiAlias( gMultiSampleAntiAlias ); //是否减轻锯齿
		conf->enableVSync( true );
	}
	void Framework::update(){
		using namespace Graphics;
		//
		Graphics::Manager m = Graphics::Manager::instance();
		if ( gCount == 0 ){ 
			//--------------创建顶点缓冲区
			gVertexBuffer = VertexBuffer::create( 3 ); //3顶点缓冲
			Vertex* v = gVertexBuffer.lock(); //获取写指针
			//输入位置
			v[ 0 ].mPosition.set( -1.f, -1.f, 0.f );
			v[ 1 ].mPosition.set( 0.f, 1.f, 0.f );
			v[ 2 ].mPosition.set( 1.f, -0.8f, 0.f );
			for ( int i = 0; i < 3; ++i ){
				v[ i ].mColor = 0xffffffff;
			}
			//插入纹理坐标（适当）
			v[ 0 ].mUv.set( -2.f, 2.f );
			v[ 1 ].mUv.set( 0.f, -2.f );
			v[ 2 ].mUv.set( 2.f, -2.f );
			//写入后关闭
			gVertexBuffer.unlock( &v );

			//-------------创建纹理
			gTexture = Texture::create( "robo.tga" );
			while ( !gTexture.isReady() ){
				; //等待加载
			}
		}
		//指定要使用的数据
		m.setVertexBuffer( gVertexBuffer );
		m.setTexture( gTexture );

		//该矩阵可以是一个单位矩阵（需要透视变换矩阵和世界矩阵）
		Matrix44 pvm;
		pvm.setIdentity();
		m.setProjectionViewMatrix( pvm );
		Matrix34 wm;
		wm.setIdentity();
		m.setWorldMatrix( wm );
		//绘制
		m.draw( 0, 1, PRIMITIVE_TRIANGLE );

		//以下是使用空格键切换过滤器的代码
		if ( Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			if ( gTextureFilter == TEXTURE_FILTER_POINT ){
				gTextureFilter = TEXTURE_FILTER_LINEAR;
			}else if ( gTextureFilter == TEXTURE_FILTER_LINEAR ){
				gTextureFilter = TEXTURE_FILTER_POINT;
			}
		}
		//纹理滤镜
		m.setTextureFilter( gTextureFilter );

		DebugScreen ds = DebugScreen::instance();
		if ( gTextureFilter == TEXTURE_FILTER_POINT ){
			ds << "TextureFilter : POINT" << endl;
		}else if ( gTextureFilter == TEXTURE_FILTER_LINEAR ){
			ds <<"TextureFilter : LINEAR" << endl;
		}
		if ( gMultiSampleAntiAlias ){
			ds << "MultiSampleAntiAlias : TRUE" << endl;
		}else{
			ds << "MultiSampleAntiAlias : FALSE" << endl;
		}
		++gCount;

		//清理工作
		if ( isEndRequested() ){
			//全局的不会自行销毁所以需要释放
			gTexture.release();
			gVertexBuffer.release();
		}
	}
}
