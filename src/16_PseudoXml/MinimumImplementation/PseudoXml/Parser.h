#ifndef INCLUDED_PSEUDO_XML_PARSER_H
#define INCLUDED_PSEUDO_XML_PARSER_H

#include <string>
#include <list>
using namespace std;

namespace PseudoXml{

class ElementData;

class Parser{
public:
	Parser( const char* data, int size );
	Element* build();
	~Parser();
private:
	int mMode; //模式变量
	bool mInEndTag; //在结尾标签吗？
	list< ElementData* > mElementStack; //存放元素
};

} //namespace PseudoXml

#endif