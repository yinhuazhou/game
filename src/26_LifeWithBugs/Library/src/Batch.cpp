#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix44.h"
using namespace GameLib;
#include "Library/Batch.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/Texture.h"
#include "Library/GraphicsDatabase.h"

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

Batch::Batch( Element& e, const GraphicsDatabase& db ) : 
mBlendMode( Graphics::BLEND_OPAQUE ),
mDiffuseColor( 1.f, 1.f, 1.f ),
mSpecularColor( 1.f, 1.f, 1.f ),
mTransparency( 1.f ),
mSpecularSharpness( 1.f ){
	//
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		string value = a.value();
		if ( name == "name" ){ 
			mName = value;
		}else if ( name == "vertexBuffer" ){
			mVertexBuffer = db.getVertexBuffer( value.c_str() );
		}else if ( name == "indexBuffer" ){
			mIndexBuffer = db.getIndexBuffer( value.c_str() );
		}else if ( name == "texture" ){
			mTexture = db.getTexture( value.c_str() );
		}else if ( name == "diffuseColor" ){
			a.getFloatValues( &mDiffuseColor.x, 3 );
		}else if ( name == "specularColor" ){
			a.getFloatValues( &mSpecularColor.x, 3 );
		}else if ( name == "specularSharpness" ){
			mSpecularSharpness = a.getFloatValue();
		}else if ( name == "transparency" ){
			mTransparency = a.getFloatValue();
		}else if ( name == "blend" ){
			if ( value == "opaque" ){
				mBlendMode = Graphics::BLEND_OPAQUE;
			}else if ( value == "additive" ){
				mBlendMode = Graphics::BLEND_ADDITIVE;
			}else if ( value == "linear" ){
				mBlendMode = Graphics::BLEND_LINEAR;
			}
		}
	}
}

//不用做任何事
Batch::~Batch(){
}

void Batch::draw() const {
	Graphics::Manager m = Graphics::Manager::instance();
	//纹理集
	if ( mTexture ){
		mTexture->set();
	}else{
		m.setTexture( 0 ); //空的纹理
	}
	//混合模式设定
	m.setBlendMode( mBlendMode );
	//根据混合模式打开/关闭Z缓冲区写入的标志
	if ( mBlendMode == Graphics::BLEND_OPAQUE ){
		m.enableDepthWrite( true );
	}else{
		m.enableDepthWrite( false );
	}
	//Z测试始终处于打开状态
	m.enableDepthTest( true );
	//暂时取消剔除
	m.setCullMode( Graphics::CULL_NONE );
	//颜色参数
	m.setDiffuseColor( mDiffuseColor );
	m.setSpecularColor( mSpecularColor );
	m.setSpecularSharpness( mSpecularSharpness );
	m.setTransparency( mTransparency );

	//数据集
	mVertexBuffer->set();
	mIndexBuffer->set();
	//开始绘制
	m.drawIndexed( 0, mIndexBuffer->size() / 3, Graphics::PRIMITIVE_TRIANGLE );
}

const string* Batch::name() const {
	return &mName;
}
