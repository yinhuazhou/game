#include "GameLib/GameLib.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Vertex.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "Library/VertexBuffer.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

VertexBuffer::VertexBuffer( Element& e ){
	//没有错误检查
	mSize = e.childNumber();
	if ( mSize > 0 ){
		mVertexBuffer = GameLib::Graphics::VertexBuffer::create( mSize );
		GameLib::Graphics::Vertex* v = mVertexBuffer.lock();
		for ( int i = 0; i < mSize; ++i ){
			v[ i ].mNormal.set( 0.f, 0.f, 0.f );
			v[ i ].mPosition.set( 0.f, 0.f, 0.f );
			v[ i ].mUv.set( 0.f, 0.f );
			v[ i ].mColor = 0xffffffff;

			Element vertex = e.child( i );
			//从属性中提取
			int an = vertex.attributeNumber();
			for ( int j = 0; j < an; ++j ){
				Attribute a = vertex.attribute( j );
				string name = a.name();
				if ( name == "position" ){
					a.getFloatValues( &v[ i ].mPosition.x, 3 );
				}else if ( name == "normal" ){
					a.getFloatValues( &v[ i ].mNormal.x, 3 );
				}else if ( name == "uv" ){
					a.getFloatValues( &v[ i ].mUv.x, 2 );
				}
			}
		}
		mVertexBuffer.unlock( &v );
	}
	//
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){ 
			mName = a.value();
		}
	}
}

VertexBuffer::~VertexBuffer(){
	//当父消失时，顶点缓冲区也消失
}

int VertexBuffer::size() const {
	return mSize;
}

const string* VertexBuffer::name() const {
	return &mName;
}

void VertexBuffer::set() const {
	GameLib::Graphics::Manager::instance().setVertexBuffer( mVertexBuffer );
}
