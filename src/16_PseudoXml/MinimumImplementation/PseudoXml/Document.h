#ifndef INCLUDED_PSEUDO_XML_DOCUMENT_H
#define INCLUDED_PSEUDO_XML_DOCUMENT_H

namespace PseudoXml{

class Element;

class Document{
public:
	Document(); //
	Document( const char* filename );
	~Document();
	//写入
	void write( const char* filename ) const;
	//获取root
	const Element* root() const;
	Element* root(); //改写
private:
	Element* mRoot;
};

} //namespace PseudoXml

#endif
