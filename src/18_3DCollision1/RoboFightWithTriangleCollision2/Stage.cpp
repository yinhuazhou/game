#include "GameLib/Framework.h"

#include "Stage.h"
#include "Triangle.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Batch.h"
#include "Library/IndexBuffer.h"
#include "Library/VertexBuffer.h"
#include "Library/Model.h"
#include "Library/Matrix44.h"
#include "Library/Vector3.h"
#include <limits>
using namespace std;

Stage::Stage() : mDatabase( 0 ), mModel( 0 ), mTriangles( 0 ){
	mDatabase = new GraphicsDatabase( "stage.txt" );
	mModel = mDatabase->createModel( "draw" );

	//创建用于碰撞处理的数据
	const Batch* batch = mDatabase->batch( "collision" );
	const IndexBuffer* ib = batch->indexBuffer();
	const VertexBuffer* vb = batch->vertexBuffer();

	mTriangleNumber = ib->size() / 3;
	mTriangles = new Triangle[ mTriangleNumber ];
	for ( int i = 0; i < mTriangleNumber; ++i ){
		unsigned i0 = ib->index( i * 3 + 0 );
		unsigned i1 = ib->index( i * 3 + 1 );
		unsigned i2 = ib->index( i * 3 + 2 );
		mTriangles[ i ].setPosition( 
			*vb->position( i0 ),
			*vb->position( i1 ),
			*vb->position( i2 ) );
	}
}

Stage::~Stage(){
	SAFE_DELETE( mModel );
	SAFE_DELETE( mDatabase );
	SAFE_DELETE_ARRAY( mTriangles );
}

void Stage::draw( const Matrix44& pvm ) const {
	mModel->draw( pvm );
}

void Stage::restrictMove( Vector3* v, const Vector3& p ) const {
	//第一圈是正常的。
	Vector3 vCopy = *v;
	bool hit = false;
	for ( int i = 0; i < mTriangleNumber; ++i ){
		hit = mTriangles[ i ].restrictMove( &vCopy, p );
		if ( hit ){
			break;
		}
	}
	//没有碰撞。照原样
	if ( !hit ){
		return;
	}
	//如果被击中，则使用校正后的向量循环第二轮。
	//如果碰撞，则放弃修改后的移动
	hit = false;
	for ( int i = 0; i < mTriangleNumber; ++i ){
		hit = mTriangles[ i ].isIntersect( p, vCopy );
		if ( hit ){
			break;
		}
	}
	if ( hit ){
		v->set( 0.0, 0.0, 0.0 );
	}else{
		*v = vCopy;
	}
}
