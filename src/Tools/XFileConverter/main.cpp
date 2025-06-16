#include "XFile/Document.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "conversion.h"
#include "GameLib/GameLib.h"
using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;

bool convert( string* dst, const char* src, int size );

int main( int argc, char** argv ){
	bool verbose = false;
	bool help = false;
	vector< string > inFileNames;

	//解析命令行参数
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //////////以-开头的是一些参数选项
			char c = argv[ i ][ 1 ];
			if ( c == 'v' ){ //详细模式
				verbose = true;
			}else if ( c == 'h' ){ //帮助
				help = true;
			}
		}else{
			inFileNames.push_back( argv[ i ] );
		}
	}
	//一个文件都不存在
	if ( inFileNames.size() == 0 ){
		help = true;
	}
	//显示帮助后结束
	if ( help ){
		std::cout << "Usage: XFileConverter.exe [ -v ][ -h ] inputFiles..." << endl;
		std::cout << "<Options>" << endl;
		std::cout << "\t-v             : verbose mode." << endl;
		std::cout << "\t-h             : display this help." << endl;
#ifndef NDEBUG //只有在debug模式下才会执行
	while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
#endif
		return 1;
	}
	//开始处理了
	for ( unsigned i = 0; i < inFileNames.size(); ++i ){
		setlocale( LC_ALL, "" ); //为了确保能够对那些文件名包含中文字符的文件进行处理
		ifstream in( inFileNames[ i ].c_str(), ifstream::binary );
		in.seekg( 0, ifstream::end );
		int size = static_cast< int >( in.tellg() );
		in.seekg( 0, ifstream::beg );
		char* src = NEW char[ size + 1 ];
		in.read( src, size );
		src[ size ] = '\0'; //以NULL结尾
		string dst;
		if ( !convert( &dst, src, size ) ){
			std::cout << "Conversion Failed : " << inFileNames[ i ] << endl;
		}else{
			string outName = inFileNames[ i ] + ".txt";
			setlocale( LC_ALL, "" ); //为了确保能够对那些文件名包含中文字符的文件进行处理
			ofstream out( outName.c_str(), ofstream::binary );
			out.write( dst.c_str(), static_cast< streamsize >( dst.size() ) );
			if ( out ){
				std::cout << "Conversion Succeeded : " << inFileNames[ i ] << endl;
			}else{
				std::cout << "Output Failed : " << inFileNames[ i ] << endl;
			}
		}
	}
#ifndef NDEBUG //只有在debug模式下才会执行
	while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
#endif
	return 0;
}

#include <ctime>
bool convert( string* dst, const char* src, int size ){
	//构建X文件数据
	XFile::Document xFile( src, size );
	if ( !xFile.isValid() ){
		return false;
	}
	string xml;
	xFile.convertToString( &xml );
	//读取伪xml格式文件
	Document srcDoc = Document::create( xml.c_str(), static_cast< int >( xml.size() ) );
	Document dstDoc = convert( srcDoc );
	
	dstDoc.convertToString( dst );
	return true;
}

#include "GameLib/Framework.h"
namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){}
}


