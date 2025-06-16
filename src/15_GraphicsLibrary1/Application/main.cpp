#include "GameLib/Framework.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"
#include "Library/Matrix44.h"
#include "Library/Model.h"
#include "Library/Batch.h"
#include "Library/IndexBuffer.h"
#include "Library/VertexBuffer.h"
#include "Library/Camera.h"
#include "GameLib/Math.h"
using namespace GameLib;

const Vector3 gPositions[ 4 ] = {
	Vector3( -10.0, -10.0, 0.0 ),
	Vector3( -10.0, 10.0, 0.0 ),
	Vector3( 10.0, -10.0, 0.0 ),
	Vector3( 10.0, 10.0, 0.0 ),
};

const Vector2 gUVs[ 4 ] = {
	Vector2( 0.0, 0.0 ),
	Vector2( 0.0, 1.0 ),
	Vector2( 1.0, 0.0 ), 
	Vector2( 1.0, 1.0 ),
};

const unsigned gIndices[ 6 ] = {
	0, 1, 2,
	3, 1, 2,
};

bool gFirstFrame = true;
VertexBuffer* gVertexBuffer;
IndexBuffer* gIndexBuffer;
GameLib::Texture* gTexture;
Batch* gBatch;
Model* gModel;
Camera* gCamera;
int gCount;

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			//顶点缓冲区生成
			gVertexBuffer = new VertexBuffer( 4 );
			for ( int i = 0; i < 4; ++i ){
				gVertexBuffer->setPosition( i, gPositions[ i ] );
				gVertexBuffer->setUV( i, gUVs[ i ] );
			}
			//生成索引缓冲区
			gIndexBuffer = new IndexBuffer( 6 );
			for ( int i = 0; i < 6; ++i ){
				gIndexBuffer->setIndex( i, gIndices[ i ] );
			}
			//纹理生成
			createTexture( &gTexture, "grid.tga" );
			//生成批次
			gBatch = new Batch( gVertexBuffer, gIndexBuffer, gTexture, BLEND_OPAQUE );
			//生成模型
			gModel = new Model( gBatch );
			//相机生成
			gCamera = new Camera();
		}
		//更新相机
		gCamera->setPosition( Vector3(
			sin( gCount ) * 20.0,
			10.0,
			cos( gCount ) * 20.0 ) );

		Matrix44 pvm;
		gCamera->createPerspectiveViewMatrix( &pvm );
		//更新模型旋转
		gModel->setAngle( Vector3( gCount, gCount * 2, gCount * 3 ) );
		//绘制
		gModel->draw( pvm );

		++gCount;

		//清理。通常按照与创建时相反的顺序销毁它。
		//那些使用它的人应该首先被删除，而正在使用的那些人应该在以后被删除。
		if ( isEndRequested() ){
			SAFE_DELETE( gCamera );
			SAFE_DELETE( gModel );
			SAFE_DELETE( gBatch );
			SAFE_DELETE( gIndexBuffer );
			SAFE_DELETE( gVertexBuffer );
			destroyTexture( &gTexture );
		}
	}
}
