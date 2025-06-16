#include "GameLib/GameLib.h"
#include "Library/IndexBuffer.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

IndexBuffer::IndexBuffer( Element& e ){
	//没有错误检查
	int n = e.childNumber();
	mSize = n * 3; //三角数×3
	if ( mSize > 0 ){
		mIndices = new unsigned[ mSize ];
		for ( int i = 0; i < n; ++i ){
			Element triangle = e.child( i );
			Attribute a = triangle.attribute( 0 ); //由于只有一个索引
			//由于没有获取unsigned的函数，因此放入int并复制
			int tmp[ 3 ];
			a.getIntValues( tmp, 3 ); //是否必须检查错误？是的
			mIndices[ i * 3 + 0 ] = tmp[ 0 ];
			mIndices[ i * 3 + 1 ] = tmp[ 1 ];
			mIndices[ i * 3 + 2 ] = tmp[ 2 ];
		}
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

IndexBuffer::IndexBuffer( int size ) :
mIndices( 0 ),
mSize( size ){
	mIndices = new unsigned[ mSize ];
}

IndexBuffer::~IndexBuffer(){
	SAFE_DELETE_ARRAY( mIndices );
}

unsigned IndexBuffer::index( int i ) const {
	ASSERT( i >= 0 && i < mSize );
	return mIndices[ i ];
}

void IndexBuffer::setIndex( int i, unsigned v ){
	ASSERT( i >= 0 && i < mSize );
	mIndices[ i ] = v;
}

int IndexBuffer::size() const {
	return mSize;
}

const string* IndexBuffer::name() const {
	return &mName;
}