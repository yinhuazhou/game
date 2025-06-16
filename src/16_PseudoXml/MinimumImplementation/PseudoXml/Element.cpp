#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
using namespace std;

namespace PseudoXml{

Element::Element(){
}

Element::Element( const char* name ) :
mName( name ){
}

Element::Element( Tag* beginTag, const char** p, const char* e ){
	//移植名称
	mName = *( beginTag->name() );
	//属性移植
	mAttributes = *beginTag->attributes(); //整个复制

	//查找标签
	while ( *p < e ){
		if ( **p == '<' ){ //发现
			++( *p ); //移至<下一个
			Tag tag( p, e ); //解释标签
			Tag::Type type = tag.type();
			if ( type == Tag::TYPE_BEGIN ){ //开始标签
				mChildren.push_back( new Element( &tag, p, e ) ); //
			}else if ( type == Tag::TYPE_END ){ //结束标签
				break; //完结
			}
		}else{
			++( *p ); //忽略直到<出现
		}
	}
}

Element::~Element(){
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		SAFE_DELETE( mAttributes[ i ] );
	}
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		SAFE_DELETE( mChildren[ i ] );
	}
}

int Element::attributeNumber() const {
	return static_cast< int >( mAttributes.size() );
}

const Attribute* Element::attribute( int i ) const {
	return mAttributes[ i ];
}

Attribute* Element::attribute( int i ){
	return mAttributes[ i ];
}

void Element::setAttributeNumber( int n ){
	//舍弃现在的
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		SAFE_DELETE( mAttributes[ i ] );
	}
	mAttributes.resize( n );
	for ( int i = 0; i < n; ++i ){
		mAttributes[ i ] = new Attribute();
	}
}

int Element::childNumber() const {
	return static_cast< int >( mChildren.size() );
}

const Element* Element::child( int i ) const {
	return mChildren[ i ];
}

Element* Element::child( int i ){
	return mChildren[ i ];
}

void Element::setChildNumber( int n ){
	//舍弃现在的
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		SAFE_DELETE( mChildren[ i ] );
	}
	mChildren.resize( n );
	for ( int i = 0; i < n; ++i ){
		mChildren[ i ] = new Element();
	}
}

const string* Element::name() const {
	return &mName;
}

void Element::setName( const char* name ){
	mName = name;
}

void Element::convertToString( string* out, int indent ) const {
	//用tab做缩进
	for ( int i = 0; i < indent; ++i ){
		*out += '\t';
	}
	//标签开始和元素名称
	*out += '<';
	*out += mName;
	//写入属性
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		if ( mAttributes[ i ] ){
			*out += ' '; //空格
			*out += *( mAttributes[ i ]->name() );
			*out += "=\"";
			*out += *( mAttributes[ i ]->value() );
			*out += '"';
		}
	}
	*out += ">\r\n"; //开始标签结束
	//
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		if ( mChildren[ i ] ){
			mChildren[ i ]->convertToString( out, indent + 1 );
		}
	}
	//用tab做缩进
	for ( int i = 0; i < indent; ++i ){
		*out += '\t';
	}
	//结束标签
	*out += "</";
	*out += mName;
	*out += ">\r\n";
}

} //namespace PseudoXml
