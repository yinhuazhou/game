#ifndef INCLUDED_XFILE_DOCUMENT_H
#define INCLUDED_XFILE_DOCUMENT_H

#include <string>

namespace XFile{

class Parser;

//XFile类
/*!
根据XFile构造数据结构。不过无法直接访问构造出的数据结构，
只能转换为xml再用xml解析器去读取。
也就是说，XFile不是一种直接的建模格式，
只能作为一种中间格式
*/
class Document{
public:
	Document( const char* data, int size );
	~Document();
	bool isValid() const;
	///转成xml字串。
	void convertToString( std::string* ) const;
private:
	Parser* mParser;
};

} //namespace XFile

#endif
