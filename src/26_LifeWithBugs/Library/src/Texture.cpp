#include "Library/Texture.h"
#include "GameLib/Graphics/Manager.h"
using namespace GameLib;
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Threading/Functions.h"
using namespace GameLib::PseudoXml;

Texture::Texture( Element& e ){
	//删除名称和文件名
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){
			mName = a.value();
		}else if ( name == "filename" ){
			mTexture = Graphics::Texture::create( a.value() );
			//等待加载完成
			while ( !mTexture.isReady() ){
				Threading::sleep( 1 );
			}
		}
	}
}

Texture::Texture( const char* filename ){
	mTexture = Graphics::Texture::create( filename );
	//等待加载完成
	while ( !mTexture.isReady() ){
		Threading::sleep( 1 );
	}
}

Texture::~Texture(){
	//当父级消失时，纹理会自行消失
}

void Texture::set() const {
	Graphics::Manager::instance().setTexture( mTexture );
}

const string* Texture::name() const {
	return &mName;
}
