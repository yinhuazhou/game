#include "GameLib/GameLib.h"
#include "Library/IndexBuffer.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

IndexBuffer::IndexBuffer( Element& e ){
	//没有错误检查
	int n = e.childNumber();
	mSize = n * 3; //三角数×3
	if ( mSize > 0 ){
		mIndexBuffer = GameLib::Graphics::IndexBuffer::create( mSize );
		unsigned short* indices = mIndexBuffer.lock();
		for ( int i = 0; i < n; ++i ){
			Element triangle = e.child( i );
			Attribute a = triangle.attribute( 0 ); //由于只有一个索引
			//由于没有函数获取unsigned short，因此将其放入int并进行复制
			int tmp[ 3 ];
			a.getIntValues( tmp, 3 ); //是否必须检查错误？是的
			indices[ i * 3 + 0 ] = static_cast< unsigned short >( tmp[ 0 ] );
			indices[ i * 3 + 1 ] = static_cast< unsigned short >( tmp[ 1 ] );
			indices[ i * 3 + 2 ] = static_cast< unsigned short >( tmp[ 2 ] );
		}
		mIndexBuffer.unlock( &indices );
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

IndexBuffer::~IndexBuffer(){
	//当父消失时，索引缓冲区也将自行消失。
}

int IndexBuffer::size() const {
	return mSize;
}

const string* IndexBuffer::name() const {
	return &mName;
}

void IndexBuffer::set() const {
	GameLib::Graphics::Manager::instance().setIndexBuffer( mIndexBuffer );
}
