#include "GameLib/GameLib.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/Element.h"
#include "Library/File.h"

namespace PseudoXml{

Document::Document() : mRoot( 0 ){
	mRoot = new Element( "<ROOT>" );
}

Document::Document( const char* filename ) : mRoot( 0 ){
	//读取所有文件
	File file( filename );
	//为dummy根元素准备标签。名称为<ROOT>
	//由于<是一个不能在名称中使用的字符，因此很容易看出这是特殊的。
	Tag tag( "<ROOT>" );
	const char* begin = file.data();
	const char* end = begin + file.size();
	mRoot = new Element( &tag, &begin, end );
}

Document::~Document(){
	SAFE_DELETE( mRoot );
}

void Document::write( const char* filename ) const {
	string str;
	//忽略根元素并编写
	for ( int i = 0; i < mRoot->childNumber(); ++i ){
		mRoot->child( i )->convertToString( &str, 0 );
	}
	File::write( filename, str.c_str(), static_cast< int >( str.size() ) );
}

const Element* Document::root() const {
	return mRoot;
}

Element* Document::root(){
	return mRoot;
}

} //namespace PseudoXml
