#include <windows.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
using namespace std;


//获取文件列表
void enumerateFiles( 
list< string >& fileNameListOut, 
const string& directoryName );

//用于写入int的便捷函数
void write( ofstream* o, int a ){
   char str[ 4 ];
   str[ 0 ] = static_cast< char >( ( a & 0x000000ff ) >>  0 );
   str[ 1 ] = static_cast< char >( ( a & 0x0000ff00 ) >>  8 );
   str[ 2 ] = static_cast< char >( ( a & 0x00ff0000 ) >> 16 );
   str[ 3 ] = static_cast< char >( ( a & 0xff000000 ) >> 24 );
   o->write( str, 4 );
}


//建立档案
void createArchive( 
const string* fileNames, 
int fileNumber, 
const char* archiveName );

//第一个参数是输出文件名
int main( int, char** argv ){
	//常用类型的别名
	typedef list< string > List;
	typedef List::iterator It;

	//第一个参数是存档名称
	string archiveName = argv[1];
	archiveName += ".bin"; //创建的二进制文件通常以bin作为扩展名。

	List fileNameList;
	//创建文件列表
	enumerateFiles( fileNameList, argv[1] );
	//转换为数组，为方便使用
	int fileNumber = static_cast< int >( fileNameList.size() );
	string* fileNames = new string[ fileNumber ];
	int j = 0;
	for ( It i = fileNameList.begin(); i != fileNameList.end(); ++i ){
		fileNames[ j ] = *i;
		++j;
	}

	//存档生成
	createArchive( fileNames, fileNumber, archiveName.c_str() );

#ifndef NDEBUG //如果这样做，它将仅在调试时出现
	while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
#endif
	return 0;
}

//------------------------函数---------

void enumerateFiles( 
list< string >& fileNameList, 
const string& directoryName ){
	HANDLE iterator; //它就像一个迭代器，因此我们将其命名为iterator。
	WIN32_FIND_DATAA fileData; //在此处输入文件名。
	//获取第一个文件
	string searchPath = directoryName;
	searchPath += "\\*.*"; //\是特殊字符所以连续写两次。这样编译器会认为这里要表达的是一个\字符
	//原始文件。目录稍后处理。
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	while ( true ){ //暂时没有条件。退出循环
		const char* name = fileData.cFileName;
		if ( !( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ){ //忽略目录
			//文件名中必须包含目录名
			string filename = directoryName;
			filename += '\\';
			filename += name; 
			cout << "\tFile : " << filename << endl; //显示它。调试。
			fileNameList.push_back( filename ); //加到列表
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //如果没有下一步，请退出。迭代器不太可能更改，但是HANDLE类型是指针，所以没问题。
			break;
		}
	}
	//这次只是目录
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	while ( true ){ //暂时没有条件。退出循环
		string name = fileData.cFileName;
		//
		if ( name == "." || name == ".." ){
			; //什么都不做
		}else if ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){ //目录
			//添加目录名称。
			string newDirectoryName = directoryName;
			newDirectoryName += '\\';
			newDirectoryName += name;
			cout << "Directory : " << newDirectoryName << endl; //显示它。调试。
			enumerateFiles( fileNameList, newDirectoryName.c_str() ); //递归调用
		} //如果不是目录，则不执行任何操作
		if ( !FindNextFileA( iterator, &fileData ) ){ //如果没有下一步，请退出。迭代器不太可能更改，但是HANDLE类型是指针，所以没问题。
			break;
		}
	}
}

void createArchive( const string* fileNames, int fileNumber, const char* archiveName ){
	//打开写入目的地
	ofstream out( archiveName, ofstream::binary );
	//分配文件大小数组
	int* fileSizes = new int[ fileNumber ];

	//打开，读取和复制列表fileNames
	for ( int i = 0; i < fileNumber; ++i ){
		ifstream in( fileNames[ i ].c_str(), ifstream::binary );
		in.seekg( 0, ifstream::end );
		fileSizes[ i ] = in.tellg();
		in.seekg( 0, ifstream::beg );
		char* data = new char[ fileSizes[ i ] ];
		in.read( data, fileSizes[ i ] ); //读取
		out.write( data, fileSizes[ i ] ); //写入
	}
	//记录文件结尾
	int dataEnd = out.tellp(); //tellg是get的位置，tellp是put的位置
	//首先写出文件数量
	write( &out, fileNumber );
	//
	//存储名称的位置，大小，字符数和名称。
	int pos = 0;
	for ( int i = 0; i < fileNumber; ++i ){
		write( &out, pos );
		write( &out, fileSizes[ i ] );
		string name = fileNames[ i ].c_str();
		int nameLength = static_cast< int >( name.size() );
		write( &out, nameLength );
		out.write( name.c_str(), nameLength );
		cout << pos << '\t' << fileSizes[ i ] << '\t' << nameLength << '\t' << name << endl; //显示它进行调试
		pos += fileSizes[ i ];
	}
	//最后，写出表格的起始位置。
	write( &out, dataEnd );
	//清理工作
	delete[] fileSizes; //没有SAFE_DELETE_ARRAY。因为这不是笔者的类库。
	fileSizes = 0;
}
