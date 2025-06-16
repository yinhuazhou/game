#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/Attribute.h"
#include <list>
using namespace std;

namespace PseudoXml{

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

Tag::Tag( const char* name ) : 
mName( name ),
mType( TYPE_BEGIN ){
}

//状态转换表。

Tag::Tag( const char** p, const char* e ) : 
mType( TYPE_BEGIN ){
	//临时存储属性名称和值
	string name;
	string value;

	int m = 0; //模式。使用模式会更好
	bool end = false; //标记退出循环
	while ( *p < e ){
		char c = **p;
		++( *p ); //
		switch ( m ){
			case 0: //初始状态
				switch ( c ){
					case '/': mType = TYPE_END; break; //结束标签
					default: mName += c; m = 1; break; //元素名
				}
				break;
			case 1: //元素名
				if ( c == '>' ){
					end = true;
				}else if ( isNormalChar( c ) ){
					mName += c;
				}else{
					m = 2; //去掉元素名称
				}
				break;
			case 2: //元素名称后的空格
				if ( c == '>' ){
					end = true;
				}else if ( isNormalChar( c ) ){
					name += c; //添加属性名称
					m = 3;
				}else{
					; //什么都不做
				}
				break;
			case 3: //属性名称
				switch ( c ){
					case '=': m = 4; break;
					default: name += c; break;
				}
				break;
			case 4: //=上
				switch ( c ){
					case '"': m = 5; break;
					default: break; //什么都不做
				}
				break;
			case 5: //属性值
				switch ( c ){
					case '"': 
						m = 2; //元素名称后的空格
						//添加属性
						mAttributes.push_back( new Attribute( name.c_str(), value.c_str() ) );
						//初始化名称和值
						name.clear();
						value.clear();
						break;
					default: value += c; break;
				}
				break;
		}
		if ( end ){ //完结
			break;
		}
	}
}

Tag::~Tag(){
	//创建的标签应始终是Element，
	//不要删除，因为传递了mAttributes的内容
}

const string* Tag::name() const {
	return &mName;
}

Tag::Type Tag::type() const {
	return mType;
}

//返回指针，因为拷贝性能太差
const vector< Attribute* >* Tag::attributes() const {
	return &mAttributes;
}

} //namespace PseudoXml