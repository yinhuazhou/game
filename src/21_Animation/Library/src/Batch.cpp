﻿#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Library/Batch.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/Texture.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib;
using namespace GameLib::PseudoXml;

namespace {

double clamp( double a, double min, double max ){
	if ( a < min ){
		return min;
	}else if ( a > max ){
		return max;
	}else{
		return a;
	}
}

unsigned light(
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient,
const Vector3& diffuseColor,
const Vector3& position0,
const Vector3& position1,
const Vector3& position2 ){
	//计算法线。
	Vector3 n;
	Vector3 p01, p02;
	p01.setSub( position1, position0 );
	p02.setSub( position2, position0 );
	n.setCross( p01, p02 );
	//取该点和光向量的点积。
	double cosine = lightVector.dot( n );
	cosine /= n.length(); //除以法线长度
	if ( cosine < 0.0 ){
		cosine = 0.0; //
	}
	Vector3 c;
	c.x = lightColor.x * diffuseColor.x * cosine + ambient.x;
	c.y = lightColor.y * diffuseColor.y * cosine + ambient.y;
	c.z = lightColor.z * diffuseColor.z * cosine + ambient.z;
	c.x = clamp( c.x, 0.0, 1.0 );
	c.y = clamp( c.y, 0.0, 1.0 );
	c.z = clamp( c.z, 0.0, 1.0 );
	int r = static_cast< int >( c.x * 255.0 + 0.5 );
	int g = static_cast< int >( c.y * 255.0 + 0.5 );
	int b = static_cast< int >( c.z * 255.0 + 0.5 );
	return 0xff000000 | ( r << 16 ) | ( g << 8 ) | b;
}

} //namespace{}

Batch::Batch( Element& e, const GraphicsDatabase& db ) : 
mVertexBuffer( 0 ),
mIndexBuffer( 0 ),
mTexture( 0 ),
mBlendMode( Framework::BLEND_OPAQUE ){
	//
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		string value = a.value();
		if ( name == "name" ){ 
			mName = value;
		}else if ( name == "vertexBuffer" ){
			mVertexBuffer = db.vertexBuffer( value.c_str() );
		}else if ( name == "indexBuffer" ){
			mIndexBuffer = db.indexBuffer( value.c_str() );
		}else if ( name == "texture" ){
			mTexture = db.texture( value.c_str() );
		}else if ( name == "blend" ){
			if ( value == "opaque" ){
				mBlendMode = Framework::BLEND_OPAQUE;
			}else if ( value == "additive" ){
				mBlendMode = Framework::BLEND_ADDITIVE;
			}else if ( value == "linear" ){
				mBlendMode = Framework::BLEND_LINEAR;
			}
		}
	}
}

Batch::Batch(
const VertexBuffer* vb,
const IndexBuffer* ib,
const ::Texture* tex,
Framework::BlendMode blend ) :
mVertexBuffer( vb ),
mIndexBuffer( ib ),
mTexture( tex ),
mBlendMode( blend ){
}

Batch::~Batch(){
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	mTexture = 0;
}

void Batch::draw( 
const Matrix44& pvm, 
const Matrix34& wm, 
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient,
const Vector3& diffuseColor ) const {
	Framework f = Framework::instance();
	//纹理集
	if ( mTexture ){
		mTexture->set();
	}else{
		f.setTexture( 0 ); //空的纹理
	}
	//混合模式设定
	f.setBlendMode( mBlendMode );
	//根据混合模式打开/关闭Z缓冲区写入的标志
	if ( mBlendMode == Framework::BLEND_OPAQUE ){
		f.enableDepthWrite( true );
	}else{
		f.enableDepthWrite( false );
	}
	//Z测试始终处于打开状态
	f.enableDepthTest( true );
	//将顶点转换为世界坐标
	int vertexNumber = mVertexBuffer->size();
	Vector3* wv = new Vector3[ vertexNumber ];
	for ( int i = 0;i < vertexNumber; ++i ){
		wm.multiply( &wv[ i ], *mVertexBuffer->position( i ) );
	}
	//顶点最终变换
	double* fv = new double[ vertexNumber * 4 ]; //final vertices
	for ( int i = 0;i < vertexNumber; ++i ){
		pvm.multiply( &fv[ i * 4 ], wv[ i ] );
	}
	int triangleNumber = mIndexBuffer->size() / 3;
	for ( int i = 0; i < triangleNumber; ++i ){
		unsigned i0 = mIndexBuffer->index( i * 3 + 0 );
		unsigned i1 = mIndexBuffer->index( i * 3 + 1 );
		unsigned i2 = mIndexBuffer->index( i * 3 + 2 );
		//在局部顶点进行光照。结果为unsigned
		unsigned c = light(
			lightVector,
			lightColor,
			ambient,
			diffuseColor,
			wv[ i0 ],  //传递世界顶点
			wv[ i1 ], 
			wv[ i2 ] );
		f.drawTriangle3DH(
			&fv[ i0 * 4 ],
			&fv[ i1 * 4 ],
			&fv[ i2 * 4 ],
			&mVertexBuffer->uv( i0 )->x,
			&mVertexBuffer->uv( i1 )->x,
			&mVertexBuffer->uv( i2 )->x,
			c, c, c );
	}
	SAFE_DELETE_ARRAY( wv );
	SAFE_DELETE_ARRAY( fv );
}

const string* Batch::name() const {
	return &mName;
}

const IndexBuffer* Batch::indexBuffer() const {
	return mIndexBuffer;
}

const VertexBuffer* Batch::vertexBuffer() const {
	return mVertexBuffer;
}

