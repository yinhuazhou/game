#ifndef INCLUDED_PSEUDO_XML_TAG_H
#define INCLUDED_PSEUDO_XML_TAG_H

#include <vector>
#include <string>
using namespace std;

namespace PseudoXml{

class Attribute;

class Tag{
public:
	enum Type{
		TYPE_BEGIN,
		TYPE_END,
	};
	Tag( const char* name ); //仅使用名称创建标签
	Tag( const char** readPointer, const char* end );
	~Tag();
	const string* name() const;
	//获取属性数组。返回指针，拷贝数据量太大所以返回指针
	const vector< Attribute* >* attributes() const;
	Type type() const;
private:
	vector< Attribute* > mAttributes;
	string mName;
	Type mType;
};

} //namespace PseudoXml

#endif
