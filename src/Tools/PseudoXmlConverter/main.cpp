﻿#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;

int main( int argc, char** argv ){
	bool help = false;
	vector< string > inFileNames;

	//解析命令行参数
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //////////以-开头的是一些参数选项
			char c = argv[ i ][ 1 ];
			if ( c == 'h' ){ //帮助
				help = true;
			}
		}else{
			inFileNames.push_back( argv[ i ] );
		}
	}
	//一个文件也没有
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
		Document d = Document::create( src, size );

		size_t pos = inFileNames[ i ].rfind( '.' );
		inFileNames[ i ].erase( pos );
		inFileNames[ i ] += ".bin";

		Array< char > dst;
		d.convertToBinary( &dst );

		setlocale( LC_ALL, "" ); //为了确保能够对那些文件名包含中文字符的文件进行处理
		ofstream out( inFileNames[ i ].c_str(), ofstream::binary );
		out.write( &dst[ 0 ], dst.size() );
	}
#ifndef NDEBUG //只有在debug模式下才会执行
	while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
#endif
	return 0;
}

#include "GameLib/Framework.h"
namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){}
}
