#ifndef INCLUDED_PSEUDO_XML_ELEMENT_H
#define INCLUDED_PSEUDO_XML_ELEMENT_H

#include <string>
#include <vector>
using namespace std;

namespace PseudoXml{

class Tag;
class Attribute;

class Element{
public:
	Element();
	Element( const char* name ); //从名称构造
	Element( Tag*, const char** readPointer, const char* end );
	~Element();
	//属性获取
	int attributeNumber() const;
	const Attribute* attribute( int i ) const;
	Attribute* attribute( int i );
	//属性设置
	void setAttributeNumber( int );
	//
	int childNumber() const;
	const Element* child( int i ) const;
	Element* child( int i );
	//
	void setChildNumber( int );
	//取得名字
	const string* name() const;
	//重命名
	void setName( const char* );
	//字符串化
	void convertToString( string* out, int indent ) const;
private:
	vector< Attribute* > mAttributes;
	vector< Element* > mChildren;
	string mName;
};

} //namespace PseudoXml

#endif
