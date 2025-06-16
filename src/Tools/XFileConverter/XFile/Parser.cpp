#include "XFile/Parser.h"
#include "XFile/Element.h"
#include "XFile/Template.h"
#include "XFile/StandardTemplates.h"

namespace XFile{

using namespace std;

namespace {
	bool isNormalChar( char c ){
		unsigned char u = static_cast< unsigned char >( c );
		static const char table[ 0x100 ] = {
			0, 0, 0, 0, 0, 0, 0, 0, //0x00-0x07
			0, 0, 0, 0, 0, 0, 0, 0, //0x08-0x0f
			0, 0, 0, 0, 0, 0, 0, 0, //0x10-0x17
			0, 0, 0, 0, 0, 0, 0, 0, //0x18-0x1f
			0, 0, 0, 0, 0, 0, 0, 0, //0x20-0x27  !"#$%&'
			0, 0, 0, 1, 0, 1, 1, 0, //0x28-0x2f ()*+,-./
			1, 1, 1, 1, 1, 1, 1, 1, //0x30-0x37 01234567
			1, 1, 0, 0, 0, 0, 0, 0, //0x38-0x3f 89:;<=>?
			0, 1, 1, 1, 1, 1, 1, 1, //0x40-0x47 @ABCDEFG
			1, 1, 1, 1, 1, 1, 1, 1, //0x48-0x4f HIJKLMNO
			1, 1, 1, 1, 1, 1, 1, 1, //0x50-0x57 PQRSTUVW
			1, 1, 1, 0, 0, 0, 0, 1, //0x58-0x5f XYZ[\]^_
			0, 1, 1, 1, 1, 1, 1, 1, //0x60-0x67 `abcdefg
			1, 1, 1, 1, 1, 1, 1, 1, //0x68-0x6f hijklmno
			1, 1, 1, 1, 1, 1, 1, 1, //0x70-0x77 pqrstuvw
			1, 1, 1, 0, 0, 0, 0, 0, //0x78-0x7f xyz{|}~
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0x80-0x8f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0x90-0x9f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xa0-0xaf
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xb0-0xbf
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xc0-0xcf
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xd0-0xdf
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xe0-0xef
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0xf0-0xff
		};
		return ( table[ u ] != 0 );
	}
	bool isSpace( char c ){
		return ( c >= 0 && c <= ' ' );
	}
} //namespace {}

Parser::Parser( const char* data, int size ) : mIsValid( false ), mRootElement( 0 ){
	//检测头部
	const char* end = data + size;
	if ( !checkHeader( data, end ) ){
		return;
	}
	data += 16; //去掉头部
	size -= 16;
	//预处理（除掉注释，以及连续的空格等等）
	mPreprocessed.setSize( size + 1 );
	int preprocessedSize = preprocess( &mPreprocessed[ 0 ], data, size );
	if ( preprocessedSize == 0 ){
		return;
	}

	data = &mPreprocessed[ 0 ];
	size = preprocessedSize;
	end = data + size;

	//语法解析
	Tank< Token > tokensTank;
	tokenize( &tokensTank, data, size );
	//为方便使用移动到数组中
	Array< Token > tokens( tokensTank.size() );
	tokensTank.copyTo( &tokens[ 0 ] );

	//语法解析
	bool failed = false;
	try{
		analyze( tokens );
	}catch( ... ){
		failed = true;
	}

	mIsValid = !failed;
}

Parser::~Parser(){
	int p = mTemplates.first();
	while ( !mTemplates.isEnd( p ) ){
		Template* t = *mTemplates.value( p );
		SAFE_DELETE( t );
		p = mTemplates.next( p );
	}
	SAFE_DELETE( mRootElement );
}

bool Parser::isValid() const {
	return mIsValid;
}

void Parser::toString( string* s ) const {
	OStringStream o;
	o.precision( 8 ); 
	o << "<?xml version=\"1.0\"?>" << endl;
	o << "<XFile>" << endl;
	//依次文本化
	RefString nullString;
	CompositeElement* e = dynamic_cast< CompositeElement* >( mRootElement );
	for ( int i = 0; i < e->childNumber(); ++i ){
		e->child( i )->mElement->toString( &o, 1, nullString );
	}
	o << "</XFile>" << endl;
	Array< char > t;
	o.get( &t );
	*s = &t[ 0 ];
}

bool Parser::isPrimitiveType( const RefString& name ) const {
	return mPrimitiveTypes.isIn( name );
}

const Template* Parser::getTemplate( const RefString& name ) const {
	const Template* t = *mTemplates[ name ];
	dieIf( !t );
	return t;
}

void Parser::dieIf( bool f ){
	if ( f ){
		throw SyntaxErrorException();
	}
}

int Parser::toInt( const RefString& s ){
	char* endPtr;
	int r = strtol( s.get(), &endPtr, 10 ); //istringstream性能较差所以用stdtol()。
	int convN = static_cast< int >( endPtr - s.get() ); //转换成功后就会按合适的尺寸往后走。不使用istringstream主要是因为性能问题
	if ( convN != s.size() ){ //尺寸错误
		r = 0;
	}
	return r;
}


/*语法解析
FILE : TEMPLATE* DATA*
TEMPLATE : template Identifier { Uuid TEMPLATE_LINE* Bracket?  }
TEMPLATE_ENTRY : ( SINGLE_TEMPLATE | ARRAY_TEMPLATE )
SINGLE_TEMPLATE : Identifier Identifier ;
ARRAY_TEMPLATE : Array Identifier Identifier Brackets+ ;
DATA : Identifier { DATA_ENTRY* DATA* }
DATA_ENTRY : ( Identifier | DATA_ENTRY | ARRAY ) ; //下面用模板类递归处理
ARRAY : ( ( Identifier | DATA_ENTRY ), )+
*/

void Parser::analyze( const Array< Token >& tokens ){
	mPrimitiveTypes.setCapacity( 10 );
	RefString s;
	s = "CHAR";
	mPrimitiveTypes.add( s );
	s = "UCHAR";
	mPrimitiveTypes.add( s );
	s = "BYTE";
	mPrimitiveTypes.add( s );
	s = "WORD";
	mPrimitiveTypes.add( s );
	s = "DWORD";
	mPrimitiveTypes.add( s );
	s = "FLOAT";
	mPrimitiveTypes.add( s );
	s = "DOUBLE";
	mPrimitiveTypes.add( s );
	s = "STRING";
	mPrimitiveTypes.add( s );
	s = "CSTRING";
	mPrimitiveTypes.add( s );
	s = "UNICODE"; //不支持
	mPrimitiveTypes.add( s );
	//语法解析
	analyzeFile( tokens );
}

//FILE : TEMPLATE* DATA*
void Parser::analyzeFile( const Array< Token >& tokens ){
	map< RefString, Template* > templates;

	//用于处理XSI输出的.x。读取追加的模板
	{
		Tank< Token > templateTokenTank;
		tokenize( &templateTokenTank, gStandardTemplates, sizeof( gStandardTemplates ) );
		Array< Token > templateTokens;
		templateTokens.setSize( templateTokenTank.size() );
		templateTokenTank.copyTo( &templateTokens[ 0 ] );
		const int n2 = templateTokens.size();
		int p2 = 0; //位置
		while ( p2 < n2 ){
			if ( templateTokens[ p2 ] != Token::TEMPLATE ){ //如果读取到Identifier表示实体
				break;
			}
			p2 = analyzeTemplate( templateTokens, p2, &templates );
		}
	}
	//TEMPLATE*
	const int n = tokens.size();
	int p = 0; //位置
	while ( p < n ){
		if ( tokens[ p ] != Token::TEMPLATE ){ //如果读取到Identifier表示实体
			break;
		}
		p = analyzeTemplate( tokens, p, &templates );
	}
	//为性能考虑，把std::map转移到HashMap
	mTemplates.setCapacity( static_cast< int >( templates.size() ) );
	for ( map< RefString, Template* >::iterator i = templates.begin(); i != templates.end(); ++i ){
		mTemplates.add( i->first, i->second );
	}
	templates.clear();

	//DATA*
	Template rootTemplate( RefString(), RefString(), 0 );
	int level = 0;
	map< RefString, Element* > namedElements;
	rootTemplate.createElement( &mRootElement, tokens, p, *this, RefString(), &namedElements, &level );
}

//TEMPLATE : template Identifier { Uuid TEMPLATE_LINE* Bracket?  }
int Parser::analyzeTemplate( const Array< Token >& t, int p, map< RefString, Template* >* templates ){
	const int n = t.size();
	//template
	++p;
	//Identifier
	dieIf( ( p >= n ) || ( t[ p ] != Token::IDENTIFIER ) ); //不可能
	const Token& name = t[ p ];
	++p;
	//{
	dieIf( ( p >= n ) || ( t[ p ] != Token::LEFT_BRACE ) ); //不可能
	++p;
	//Uuid
	dieIf( ( p >= n ) || ( t[ p ] != Token::UUID ) ); //不可能
	const Token& uuid = t[ p ];
	++p;
	//TEMPLATE_LINE*
	Tank< TemplateLine* > templateLines;
	while ( ( p < n ) && ( ( t[ p ] == Token::IDENTIFIER ) || ( t[ p ] == Token::ARRAY ) ) ){
		TemplateLine* templateLine = NEW TemplateLine;
		p = analyzeTemplateEntry( templateLine, t, p, templates );
		templateLines.add( templateLine );
	}
	//Bracket?
	const Token* restriction = 0;
	dieIf( p >= n );
	if ( t[ p ] == Token::BRACKETS ){
		restriction = &t[ p ];
		++p;
	}
	//}
	dieIf( ( p >= n ) || ( t[ p ] != Token::RIGHT_BRACE ) ); //不可能
	++p;
	//如果已经存在同名的模板则不必处理
	if ( templates->find( *name.get() ) == templates->end() ){
		//取出数据
		int lineNumber = templateLines.size();
		Template* tmpl = NEW Template( 
			*name.get(),
			*uuid.get(), 
			lineNumber );
		for ( int i = 0; i < lineNumber; ++i ){
			tmpl->setChild( i, templateLines.get() );
			templateLines.toNext();
		}
		//放入map
		templates->insert( make_pair( *name.get(), tmpl ) );
	}
	return p;
}

//TEMPLATE_ENTRY : ( SINGLE_TEMPLATE | ARRAY_TEMPLATE )
int Parser::analyzeTemplateEntry( TemplateLine* templateLine, const Array< Token >& t, int p, map< RefString, Template* >* templates ){
	if ( t[ p ] == Token::IDENTIFIER ){
		p = analyzeSingleTemplate( templateLine, t, p, templates );
	}else if ( t[ p ] == Token::ARRAY ){
		p = analyzeArrayTemplate( templateLine, t, p, templates );
	}else{ //错误
		dieIf( true );
	}
	return p;
}

//SINGLE_TEMPLATE : Identifier Identifier ;
int Parser::analyzeSingleTemplate( TemplateLine* templateLine, const Array< Token >& t, int p, map< RefString, Template* >* templates ){
	const int n = t.size();
	//类型名
	const Token& typeName = t[ p ];
	++p;
	//变量名
	dieIf( ( p >= n ) || ( t[ p ] != Token::IDENTIFIER ) );
	templateLine->mName = *( t[ p ].get() );
	++p;
	//;
	dieIf( ( p >= n ) || ( t[ p ] != Token::SEMICOLON ) );
	++p;
	//取出模板
	if ( isPrimitiveType( *typeName.get() ) ){
		templateLine->mTemplate = 0;
		templateLine->mPrimitiveType = *typeName.get();
	}else{
		map< RefString, Template* >::const_iterator it = templates->find( *typeName.get() );
		dieIf( it == templates->end() );
		templateLine->mTemplate = &*it->second;
	}
	return p;
}

//ARRAY_TEMPLATE : Array Identifier Identifier Brackets+ ;
int Parser::analyzeArrayTemplate( TemplateLine* templateLine, const Array< Token >& t, int p, map< RefString, Template* >* templates ){
	const int n = t.size();
	//array
	++p;
	//类型名
	const Token& typeName = t[ p ];
	++p;
	//变量名
	dieIf( ( p >= n ) || ( t[ p ] != Token::IDENTIFIER ) );
	templateLine->mName = *( t[ p ].get() );
	++p;
	//[]
	dieIf( ( p >= n ) || ( t[ p ] != Token::BRACKETS ) );
	Tank< RefString > indices;
	do{
		indices.add( *( t[ p ].get() ) );
		++p;
	}while ( ( p < n ) && ( t[ p ] == Token::BRACKETS ) );

	templateLine->mIndices.setSize( indices.size() );
	indices.copyTo( &templateLine->mIndices[ 0 ] );
	//;
	dieIf( ( p >= n ) || ( t[ p ] != Token::SEMICOLON ) );
	++p;
	//取出模板
	if ( isPrimitiveType( *typeName.get() ) ){
		templateLine->mTemplate = 0;
		templateLine->mPrimitiveType = *typeName.get();
	}else{
		map< RefString, Template* >::const_iterator it = templates->find( *typeName.get() );
		dieIf( it == templates->end() );
		templateLine->mTemplate = &*it->second;
	}
	return p;
}

/*
//1.去掉注释
//2.去掉换行
//3.连续不断的空格

0:初始状态
1:空格上
2:注释
3:/上
4:""中

0,s,1 ->输出空格
0,#,2
0,/,3
0,",4
0,*,0
1,s,1 ->忽略
1,#,2
1,/,3
1,",4
1,*,0
2,\n,0
2,*,2
3,/,2
3,*,E
4,",0
4,*,4

*/
int Parser::preprocess( char* dst, const char* src, int size ){
	int m = 0; //型号
	const char* end = src + size;
	const char* origDst = dst;
	while ( src < end ){
		char c = *src;
		++src;
		switch ( m ){
			case 0:
				if ( isSpace( c ) ){
					m = 1;
					*dst = ' ';
					++dst;
				}else if ( c == '#' ){
					m = 2;
				}else if ( c == '/' ){
					m = 3;
				}else if ( c == '"' ){
					m = 4;
					*dst = c;
					++dst;
				}else{
					*dst = c;
					++dst;
				}
				break;
			case 1:
				if ( isSpace( c ) ){
					; //忽略
				}else if ( c == '#' ){
					m = 2;
				}else if ( c == '/' ){
					m = 3;
				}else if ( c == '"' ){
					m = 4;
					*dst = c;
					++dst;
				}else{
					m = 0;
					*dst = c;
					++dst;
				}
				break;
			case 2:
				if ( c == '\n' ){
					m = 0;
				}else{ 
					; //注释中。跳过
				}
				break;
			case 3:
				if ( c == '/' ){
					m = 2;
				}else{
					return 0; //错误
				}
				break;
			case 4:
				if ( c == '"' ){
					m = 0;
					*dst = c;
					++dst;
				}else{
					*dst = c;
					++dst;
				}
				break;
		}
	}
	*dst = '\0';
	return static_cast< int >( dst - origDst );
}

bool Parser::checkHeader( const char* data, const char* end ){
	//头部尺寸不够
	if ( end - data < 16 ){
		return false;
	}
	string src( data + 0 , 4 );
	if ( src != "xof " ){
		return false;
	}
	src = string( data + 8, 4 );
	if ( src != "txt " ){
		return false;
	}
	src = string( data + 12 , 4 );
	if ( src != "0032" && src != "0064" ){
		return false;
	}
	return true;
}

/*
词法解析

0:初始状态
1:字符串内
2:UUID中
3:[]中
4:[]中的字符串
5:""中

0,{,0 out(LEFT_BRACE)
0,},0 out(RIGHT_BRACE)
0,,,0 out(COMMA)
0,;,0 out(SEMI_COLON)
0,<,2 UUIDへ s清空
0,[,3 []へ s清空
0,",5 字符串常量
0,c,1 字符串 s清空
0,*,0

1,{,0 out(LEFT_BRACE)
1,},0 out(RIGHT_BRACE)
1,,,0 out(COMMA)
1,;,0 out(SEMI_COLON)
1,<,2 UUIDへ s清空
1,[,3 []へ s清空
1,c,1 字符串继续
1,*,0 out(IDENTIFIER | TEMPLATE)

2,>,0 out(UUID)
2,*,2 UUID继续

3,],0 out(BRACKETS)
3,c,4 []中的字符串
3,*,3

4,],0 out(BRACKETS)
4,c,4 继续
4,*,3 跳出

5,",0 out(STRING)
5,*,5 继续

*/
void Parser::tokenize( Tank< Token >* tokens, const char* p, int size ){
	const char* end = p + size;
	const char* s = 0; //字符串指针
	int l = 0; //字符串长
	int m = 0;
	while ( p < end ){
		char c = *p;
		switch ( m ){
			case 0:
				if ( c == '{' ){
					tokens->add( Token( Token::LEFT_BRACE ) );
				}else if ( c == '}' ){
					tokens->add( Token( Token::RIGHT_BRACE ) );
				}else if ( c == ',' ){
					tokens->add( Token( Token::COMMA ) );
				}else if ( c == ';' ){
					tokens->add( Token( Token::SEMICOLON ) );
				}else if ( c == '<' ){
					m = 2;
					s = p + 1;
					l = 0;
				}else if ( c == '[' ){
					m = 3;
					s = p + 1;
					l = 0;
				}else if ( c == '"' ){
					m = 5;
					s = p + 1;
					l = 0;
				}else if ( isNormalChar( c ) ){
					m = 1;
					s = p;
					l = 1;
				}else{
					; //保持不变
				}
				break;
			case 1:
				if ( isNormalChar( c ) ){
					++l;
				}else{
					//输出字符串
					string str( s, l );
					if ( str == "template" ){
						tokens->add( Token( Token::TEMPLATE ) );
					}else if ( str == "array" ){
						tokens->add( Token( Token::ARRAY ) );
					}else{
						tokens->add( Token( Token::IDENTIFIER, s, l ) );
					}
					//后处理
					if ( c == '{' ){
						m = 0;
						tokens->add( Token( Token::LEFT_BRACE ) );
					}else if ( c == '}' ){
						m = 0;
						tokens->add( Token( Token::RIGHT_BRACE ) );
					}else if ( c == ',' ){
						m = 0;
						tokens->add( Token( Token::COMMA ) );
					}else if ( c == ';' ){
						m = 0;
						tokens->add( Token( Token::SEMICOLON ) );
					}else if ( c == '<' ){
						m = 2;
						s = p + 1;
						l = 0;
					}else if ( c == '[' ){
						m = 3;
						s = p + 1;
						l = 0;
					}else{
						m = 0;
					}
				}
				break;
			case 2:
				if ( c == '>' ){
					m = 0;
					tokens->add( Token( Token::UUID, s, l ) );
				}else{
					++l;
				}
				break;
			case 3:
				if ( c == ']' ){
					m = 0;
					tokens->add( Token( Token::BRACKETS, s, l ) );
				}else if ( isNormalChar( c ) ){
					++l;
					m = 4;
				}else{
					; //什么也不做
				}
				break;
			case 4:
				if ( c == ']' ){
					m = 0;
					tokens->add( Token( Token::BRACKETS, s, l ) );
				}else if ( isNormalChar( c ) ){
					++l;
				}else{
					m = 3;
				}
				break;
			case 5:
				if ( c == '"' ){
					tokens->add( Token( Token::STRING, s, l ) );
					m = 0;
				}else{
					++l;
				}
				break;
		}
		++p;
	}
}

} //namespace XFile