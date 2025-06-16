#ifndef INCLUDED_PSEUDO_XML_ATTRIBUTE_H
#define INCLUDED_PSEUDO_XML_ATTRIBUTE_H

#include <string>
using namespace std;

namespace PseudoXml{

class Attribute{
public:
	Attribute();
	Attribute( const char* name, const char* value );
	~Attribute();

	void set( const char* name, const char* value );
	void set( const char* name, int value ); //int値
	void set( const char* name, double value ); //double值
	void set( const char* name, const int* values, int number ); //整数数组
	void set( const char* name, const double* values, int number ); //double型数组
	const string* value() const;
	const string* name() const;
	//如果失败，则返回0，但与原始零没有区别。
	int getIntValue() const;
	//如果失败，则返回0.0，但与最初包含的0.0不能区分。
	double getDoubleValue() const;
	//返回值是实际读取的元素数
	int getIntValues( int* out, int number ) const;
	//返回值是实际读取的元素数
	int getDoubleValues( double* out, int number ) const;
private:
	string mName;
	string mValue;
};

} //namespace PseudoXml

#endif
