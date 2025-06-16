#include "GameLib/GameLib.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
#include "PseudoXml/Parser.h"

namespace PseudoXml{

class ElementData{
public:
	~ElementData(){
		//应该为空
		assert( mAttributes.size() == 0 && mChildren.size() == 0 );
	}
	string mName; //元素名
	list< Attribute* > mAttributes; //属性
	list< ElementData* > mChildren; //子数据
};

Parser::Parser( const char* data, int size ) :
mData( data ),
mDataSize( size ),
mMode( 0 ),
mInEndTag( false ){
}

Parser::~Parser(){
	//这里应该是空的
	assert( mElements.size() == 0 );
}

namespace {
//函数用于检查是否属于a-z,A-Z,0-9,_中的某个字符
//易于理解，但速度较慢。考虑一种高性能的方法。
bool static isNormalChar( char c ){
	if ( c >= '0' && c <= '9' ){
		return true;
	}
	if ( c >= 'a' && c <= 'z' ){
		return true;
	}
	if ( c >= 'A' && c <= 'Z' ){
		return true;
	}
	if ( c == '_' ){
		return true;
	}
	return false;
}

} //namespace {}

//状态转换表。

Element* Parser::build(){
	//临时存储属性名称和值
	string name;
	string value;
	
	//dummy父
	ElementData* parent = new ElementData;
	parent->mName = "<ROOT>"; //特殊处理的根元素
	elements.push_back( parent ); //

	//要创建的元素
	ElementData* child = new ElementData;

	//结束标签？
	bool isEndTag = false;

	for ( int i = 0; i < mDataSize; ++i ){
		char c = mData[ i ];
		//按模式分支
		switch ( mMode ){
			case 0: //初始状态
				switch ( c ){
					case '<': mMode = 1; break;
					default: break; //什么都不做
				}
				break;
			case 1: //<上
				switch ( c ){
					case '/': mInEndTag = true; break; //结束标签
					default: child->mName += c; mMode = 2; break; //元素名
				}
				break;
			case 2: //元素名
				if ( c == '>' ){
					mMode = 7;
				}else if ( isNormalChar( c ) ){
					child->mName += c;
				}else{
					mMode = 3; //去掉元素名称
				}
				break;
			case 3: //元素名称后的空格
				if ( c == '>' ){
					mMode = 7;
				}else if ( isNormalChar( c ) ){
					name += c; //添加属性名称
					mMode = 4;
				}else{
					; //什么都不做
				}
				break;
			case 4: //属性名称
				switch ( c ){
					case '=': mMode = 5; break;
					default: name += c; break;
				}
				break;
			case 5: //=上
				switch ( c ){
					case '"': mMode = 6; break;
					default: break; //什么都不做
				}
				break;
			case 6: //属性值
				switch ( c ){
					case '"': 
						mMode = 3; //元素名称后的空格
						//添加属性
						child->mAttributes.push_back( new Attribute( name.c_str(), value.c_str() ) );
						//初始化名称和值
						name.clear();
						value.clear();
						break;
					default: value += c; break;
				}
				break;
			case 7: //>上方。标签结束
				if ( mInEndTag ){ //由于它是结束标签
					elements.pop_back();
					parent = elements.back(); //
					mInEndTag = false;
				}else{ //将元素添加到当前子项作为开始标签
					parent->mChildren.push_back( child );
					parent = child; //
					child = new ElementData; //制作一个新的并准备
				}
				mMode = 0; //没问题，转到0
				break;
		}
	}
	//现在所有东西都准备好了，可以制作元素。
	Element* root = new Element;
	ElementData* rootData = elements.front();
	root->setName( rootData->mName );
	root->setAttributeNumber( rootData->mAttributes.size() );
	int j = 0;
	for ( list< Attributes* >::iterator i = rootData->mAttributes.begin(); i != rootData->mAttributes.end(); ++i ){
		root->setAttribute( j, *i );
		++j;
		*i = 0; //转移控制
	}
	root->setChildNumber( rootData->mChildren.size() );

}

} //namespace PseudoXml
