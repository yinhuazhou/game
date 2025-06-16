#include "GameLib/GameLib.h"
#include "XFile/Template.h"
#include "XFile/Token.h"
#include "XFile/Element.h"
#include "GameLib/Base/Tank.h"
#include "XFile/Parser.h"
#include <sstream>

namespace XFile{

using namespace GameLib;
using namespace std;

Template::Template( RefString name, const RefString& uuid, int childNumber ) :
mName( name ),
mChildren( childNumber ), 
mUuid( uuid ){
}

Template::~Template(){
}

void Template::setChild( int i, TemplateLine** tl ){
	mChildren[ i ] = *tl;
	*tl = 0;
}

const RefString* Template::getType() const {
	return &mName;
}

const RefString* Template::getChildName( int index ) const {
	if ( index >= mChildren.size() ){ //有可能会请求更大的编号。返回0
		return 0;
	}else{
		return &mChildren[ index ]->mName;
	}
}

const RefString* Template::getChildType( int index ) const {
	if ( index >= mChildren.size() ){ //有可能会请求更大的编号。返回0
		return 0;
	}else if ( mChildren[ index ]->mTemplate ){ //复合类型
		return mChildren[ index ]->mTemplate->getType();
	}else{ //简单类型
		return &mChildren[ index ]->mPrimitiveType;
	}
}


int Template::createElement( 
Element** elementOut, 
const Array< Token >& t,
int p,
const Parser& parser,
const RefString& name,
map< RefString, Element* >* namedElements,
int* level ) const {

//if ( p % 1000 == 0 ){ cout << p << endl; }

	const int n = t.size();
	Tank< CompositeElement::Child > children;
	map< RefString, int > arrayIndices;
	//正规数据
	for ( int i = 0; i < mChildren.size(); ++i ){
		++( *level );
		CompositeElement::Child c; //
		if ( ( p < n ) && ( t[ p ] == Token::LEFT_BRACE ) ){ //引用
			p = parseReference( &c.mElement, t, p, namedElements );
		}else{
			const TemplateLine& line = *mChildren[ i ];
			if ( line.mIndices.size() > 0 ){ //数组
				p = createArray( &c.mElement, line, t, p, arrayIndices, parser, namedElements, level );
			}else if ( line.mTemplate ){ //复合类型
				RefString nullStr;
				p = line.mTemplate->createElement( &c.mElement, t, p, parser, nullStr, namedElements, level );
			}else{ //简单类型
				c.mPrimitive = *t[ p ].get();
				++p;
				int size = parser.toInt( c.mPrimitive ); //检测是否可以新增
				if ( size > 0 ){
					arrayIndices.insert( make_pair( line.mName, size ) );
				}
			}
		}
		children.add( c );
		Parser::dieIf( p >= n );
		if ( t[ p ] == Token::SEMICOLON ){
			++p;
		}else{ //不可能的情况
#if 0 //处理Metasequoia格式。去掉分号
			Parder::dieIf( true );
#endif
		}
		--( *level );
#if 1 //处理Metasequoia格式 去掉多余的分号
		if ( ( *level == 0 ) && ( p < n ) && ( t[ p ] == Token::SEMICOLON ) ){ //不可能的
			++p;
		}
#endif
	}
	//追加数据
	while ( p < n ){
		CompositeElement::Child c;
		if ( t[ p ] == Token::LEFT_BRACE ){ //引用
			p = parseReference( &c.mElement, t, p, namedElements );
		}else if ( t[ p ] == Token::IDENTIFIER ){
			//取出类型名
			const RefString* typeName = t[ p ].get();
			++p;
			//取出变量名
			RefString childName;
			if ( ( p < n ) && ( t[ p ] == Token::IDENTIFIER ) ){
				childName = *t[ p ].get();
				++p;
			}
			//{
			Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::LEFT_BRACE ) ); //不可能
			++p;
			//创建数据
			if ( parser.isPrimitiveType( *typeName ) ){ //简单类型
				c.mPrimitive = *t[ p ].get();
				++p;
			}else{ //复合类型
				const Template* tmpl = parser.getTemplate( *typeName );
				int childLevel = 0;
				p = tmpl->createElement( &c.mElement, t, p, parser, childName, namedElements, &childLevel );
				namedElements->insert( make_pair( childName, c.mElement ) );
			}
			//}
			Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::RIGHT_BRACE ) ); //不可能
			++p;
		}else{
			break; //没有引用也没有数据实体。可以结束了
		}
		children.add( c );
	}
	CompositeElement* e = NEW CompositeElement( this, children.size(), name );
	for ( int i = 0; !children.isEnd(); ++i ){
		CompositeElement::Child* c = children.get();
		e->setChild( i, *c );
		children.toNext();
	}
	*elementOut = e;

	return p;
}

int Template::createArray(
Element** elementOut, 
const TemplateLine& line,
const Array< Token >& t,
int p,
const map< RefString, int >& arrayIndices,
const Parser& parser,
map< RefString, Element* >* namedElements,
int* level ) const {
	//获取数组维度和各个维度的容量。这里有优化的空间
	const int n = t.size();
	const Array< RefString >& indices = line.mIndices;
	int dimension = indices.size();
	Array< int > arraySizes( dimension );
	int k = 0;
	int arraySize = 1;
	for ( int i = 0; i < indices.size(); ++i ){
		const RefString& s = indices[ i ];
		int size = parser.toInt( s );
		if ( size > 0 ){
			arraySizes[ k ] = size;
		}else{
			map< RefString, int >::const_iterator l = arrayIndices.find( s );
			Parser::dieIf( l == arrayIndices.end() );
			arraySizes[ k ] = l->second;
		}
		arraySize *= arraySizes[ k ];
	}
	//判断是简单类型还是复合类型然后分支处理
	if ( line.mTemplate ){ //复合类型
		CompositeArrayElement* a = NEW CompositeArrayElement( arraySizes );
		for ( int i = 0; i < arraySize; ++i ){
			Element* te = 0;
			RefString nullStr;
			p = line.mTemplate->createElement( &te, t, p, parser, nullStr, namedElements, level );
			if ( i < arraySize - 1 ){ //////////最后一个元素后面不带逗号
				Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::COMMA ) );
				++p;
			}
			a->setElement( i, &te );
		}
		*elementOut = a;
	}else{ //简单类型
		PrimitiveArrayElement* a = NEW PrimitiveArrayElement( arraySizes, line.mPrimitiveType );
		for ( int i = 0; i < arraySize; ++i ){
			a->setElement( i, *t[ p ].get() );
			++p;
			if ( i < arraySize - 1 ){ //最后一个元素后面不带逗号
				Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::COMMA ) );
				++p;
			}
		}
		*elementOut = a;
	}
	return p;
}

//引用处理 { name | UUID | name UUID }
int Template::parseReference(
Element** elementOut,
const Array< Token >& t,
int p,
map< RefString, Element* >* ) const {
	const int n = t.size();

	++p; //跳过左括号
	RefString refName;
	if ( ( p < n ) && ( t[ p ] == Token::IDENTIFIER ) ){
		refName = *t[ p ].get();
		++p;
	}
	if ( ( p < n ) && ( t[ p ] == Token::UUID ) ){ //UUID只有在名字不存在时才使用
		++p;
		if ( refName.size() == 0 ){
			refName = *t[ p ].get();
		}
	}
	Parser::dieIf( refName.size() == 0 );
	Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::RIGHT_BRACE ) ); //不可能
	++p;

	*elementOut = NEW ReferenceElement( refName );
/*
	//插入引用
	map< RefString, Element* >::iterator it = namedElements->find( refName );
	Parser::dieIf( it == namedElements->end() );
	*elementOut = it->second;
*/
	return p;
}


} //namespace XFile

