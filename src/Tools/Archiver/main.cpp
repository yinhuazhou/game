#include <windows.h>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
using namespace std;
/*
本实例使用了ASCII版本的一些windows函数（按照普通char*处理）
正规的做法应当使用TCHAR等宏来处理，这样就不必在意是Unicode还是ASCII了，
不过那种代码读起来可能不容易理解。
此外，这里的重点不是探讨windows编程。
而且事实上，借助于perl和bash，C#等语言，能够比C++更方便地制作这类工具。
所以这里暂且忽略了windows的标准写法，直接调用ASCII版本的函数。

<用到的函数>
GetFirstFileA
GetNextFileA
<用到的结构体>
WIN32_FIND_DATAA
*/

//获取文件一览
void enumerateFiles( 
	list< string >& fileNameListOut, 
	const string& directoryName,
	bool verbose );

//创建档案文件
bool createArchive( 
	const string* fileNames, 
	int fileNumber, 
	const char* archiveName,
	int blockSize,
	bool verbose,
	double compressionThreshold );

//压缩处理
bool compress( 
	int* outSize, 
	int* inPlaceDecompressionBufferSize,
	bool* compressed,
	ofstream* oStream, 
	ifstream* iStream,
	double threshold );

//用于写入int数据的功能函数
void write( ofstream* o, unsigned a ){
   char str[ 4 ];
   str[ 0 ] = static_cast< char >( ( a & 0x000000ff ) >>  0 );
   str[ 1 ] = static_cast< char >( ( a & 0x0000ff00 ) >>  8 );
   str[ 2 ] = static_cast< char >( ( a & 0x00ff0000 ) >> 16 );
   str[ 3 ] = static_cast< char >( ( a & 0xff000000 ) >> 24 );
   o->write( str, 4 );
}

#include "GameLib/GameLib.h"

//第一个参数是文件名
int main( int argc, char** argv ){
	int blockSize = 4;
	string archiveDir;
	string archiveName;
	bool verbose = false;
	bool help = false;
	double threshold = 100.0;
	//解析命令行参数
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //由-开头的是一些选项参数
			char c = argv[ i ][ 1 ];
			if ( c == 'b' ){ //指定容量
				if ( ( i + 1 ) < argc ){
					istringstream iss( argv[ i + 1 ] );
					iss >> blockSize;
				}
				++i;
			}else if ( c == 'o' ){ //指定生成的文件名
				if ( ( i + 1 ) < argc ){
					archiveName = argv[ i + 1 ];
				}
				++i;
			}else if ( c == 'r' ){ //阈值
				if ( ( i + 1 ) < argc ){
					istringstream iss( argv[ i + 1 ] );
					iss >> threshold;
					if ( threshold > 100.0 ){
						threshold = 100.0; //不允许超过100
					}
				}
				++i;
			}else if ( c == 'v' ){ //详细模式
				verbose = true;
			}else if ( c == 'h' ){ //帮助
				help = true;
			}
		}else{
			archiveDir = argv[ i ];
		}
	}
	//必须指定档案文件夹
	if ( archiveDir.size() == 0 ){
		help = true;
	}
	//显示帮助内容然后结束
	if ( help ){
		cout << "Usage: Archiver.exe rootDir [ -o outfilename ][ -s blockSize ][ -r thresholdRatio ][ -v ][ -h ]" << endl;
		cout << "<Options>" << endl;
		cout << "\t-o outfilename : specify output archive name. default is \"rootDir\".bin" << endl;
		cout << "\t-s blocksize   : specify block size. \"4GB * blockSize\" is maximum archivesize. default is 4." << endl;
		cout << "\t-r percent     : specify threshold. if compressed size is less than this, non-compressed data is written." << endl;
		cout << "\t-v             : verbose mode." << endl;
		cout << "\t-h             : display this help." << endl;
#ifndef NDEBUG //只有在debug模式下才会运行
	while( true ){;} //为了让visual studio中运行命令行程序时不会一闪而过
#endif
		return 1;
	}
	//如果没有指定档案文件名则使用文件夹名字
	if ( archiveName.size() == 0 ){
		archiveName = archiveDir;
		archiveName += ".bin";
	}

	//对于频繁使用的类型创建别名
	typedef list< string > List;
	typedef List::iterator It;
	List fileNameList;
	//创建文件列表
	enumerateFiles( fileNameList, archiveDir.c_str(), verbose );
	//为了便于使用转换为数组
	int fileNumber = static_cast< int >( fileNameList.size() );
	string* fileNames = new string[ fileNumber ];
	int j = 0;
	for ( It i = fileNameList.begin(); i != fileNameList.end(); ++i ){
		fileNames[ j ] = *i;
		++j;
	}

	//生成档案文件
	bool succeeded = createArchive( 
		fileNames, 
		fileNumber, 
		archiveName.c_str(), 
		blockSize, 
		verbose, 
		threshold );
	if ( !succeeded ){ //失败时将不会退出
		cerr << "creating failed. delete output file manually." << endl;
#ifndef NDEBUG //只有debug模式下才会执行
	while( true ){;} //为了让visual studio中运行命令行程序时不会一闪而过
#endif
		return 1;
	}

#ifndef NDEBUG //只有debug模式下才会执行
	while( true ){;} //为了让visual studio中运行命令行程序时不会一闪而过
#endif
	return 0;
}

//------------------------函数----------------------------

void enumerateFiles( 
list< string >& fileNameList, 
const string& directoryName,
bool verbose ){
	HANDLE iterator; //涉及到遍历所以且命名为iterator
	WIN32_FIND_DATAA fileData; //这里记录文件名
	//获取第一个文件
	string searchPath = directoryName;
	searchPath += "\\*.*"; //\是特殊字符所以连续写两次。这样编译器会认为这里要表达的是一个\字符

	//只处理文件
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	if ( iterator == INVALID_HANDLE_VALUE ){
		//因为某种原因失败了
		return;
	}
	while ( true ){ //条件永远为真，后面合适的时机跳出循环
		if ( !( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ){
			//文件名必须包含路径
			string filename = directoryName;
			filename += '\\';
			filename += fileData.cFileName; 
			if ( verbose ){
				cout << "\tFile : " << filename << endl; //为了调试方便打印出来
			}
			fileNameList.push_back( filename ); //加入到列表
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //处理下一个。如果没有下一个了则结束。iterator不可变，但是HANDLE是个指针，所以没有问题
			break;
		}
	}
	//加上文件夹路径
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	if ( iterator == INVALID_HANDLE_VALUE ){
		//因为某种原因失败了
		return;
	}
	while ( true ){ //条件永远为真，后面合适的时机跳出循环
		string name = fileData.cFileName;
		//这里遇到.或者..时将特殊处理，否则会进入死循环
		if ( name == "." || name == ".." ){
			; //什么也不做
		}else if ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){ //是个目录
			//把路径添加上去
			string newDirectoryName = directoryName;
			newDirectoryName += '\\';
			newDirectoryName += name;
			if ( verbose ){
				cout << "Directory : " << newDirectoryName << endl;
			}
			enumerateFiles( fileNameList, newDirectoryName.c_str(), verbose ); //递归调用
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //处理下一个。如果没有下一个了则结束。iterator不可变，但是HANDLE是个指针，所以没有问题
			break;
		}
	}
}

bool createArchive( 
const string* fileNames, 
int fileNumber, 
const char* archiveName, 
int blockSize, 
bool verbose,
double threshold ){
	//打开写入位置
	ofstream out( archiveName, ofstream::binary );
	if ( !out ){
		cerr << "can't create output file." << endl;
		return false;
	}
	//确保数组容量
	int* originalSizes = new int[ fileNumber ];
	int* outputSizes = new int[ fileNumber ];
	int* inPlaceDecompressionBufferSizes = new int[ fileNumber ];

	//读取fileNames列表中的文件
	for ( int i = 0; i < fileNumber; ++i ){
		setlocale( LC_ALL, "" ); //不加这个将无法对文件名含有中文的文件处理
		ifstream in( fileNames[ i ].c_str(), ifstream::binary );
		if ( !in ){
			cerr << "can't open file : " << fileNames[ i ] << endl;
			return false;
		}
		in.seekg( 0, ifstream::end );
		streamsize sizeBig = in.tellg();
		if ( sizeBig >= 0x7fffffff ){
			cerr << "can't handle files larger than 2GB!" << endl;
			delete[] outputSizes;
			outputSizes = 0;
			delete[] originalSizes;
			originalSizes = 0;
			out.close();
			out.open( archiveName ); //再次打开清空文件
			return false;
		}
		in.seekg( 0, ifstream::beg );
		originalSizes[ i ] = static_cast< int >( sizeBig );
		bool compressed;
		compress( &outputSizes[ i ], &inPlaceDecompressionBufferSizes[ i ], &compressed, &out, &in, threshold );
		if ( verbose ){
			double ratio = ( sizeBig == 0 ) ? 100.0 : outputSizes[ i ] * 100.0 / sizeBig;
			cout << fileNames[ i ] << " ) : " << sizeBig << " -> " << outputSizes[ i ] << " ( " << ratio << "% )" << endl;
		}
		//为了填满区块，写入空数据
		int mod = outputSizes[ i ] % blockSize;
		int padding = ( mod > 0 ) ? ( blockSize - mod ) : 0;
		for ( int j = 0; j < padding; ++j ){
			out.put( '\0' );
		}
	}
	if ( verbose ){
		cout << "<Table>" << endl;
		cout << "FileNumber : " << fileNumber << endl;
		cout << "BlockSize : " << blockSize << endl;
	}
	//记录文件的结尾位置
	streamsize dataEnd = out.tellp(); //tellg是get的位置，tellp是put的位置
	//首先写入文件的数量
	out.write( reinterpret_cast< char* >( &fileNumber ), 4 );
	//为了创建表，重新调整偏移值，
	//把位置，容量，名称的长度，以及名称都存起来
	unsigned pos = 0; //区块单位的位置
	for ( int i = 0; i < fileNumber; ++i ){
		write( &out, pos );
		write( &out, outputSizes[ i ] );
		write( &out, originalSizes[ i ] );
		write( &out, inPlaceDecompressionBufferSizes[ i ] );
		string name = fileNames[ i ].c_str();
		int nameLength = static_cast< int >( name.size() );
		write( &out, nameLength );
		out.write( name.c_str(), nameLength );
		if ( verbose ){
			cout << pos << '\t' << outputSizes[ i ] << '\t' << originalSizes[ i ] << '\t' << inPlaceDecompressionBufferSizes[ i ] << '\t' << nameLength << '\t' << name << endl; //显示它进行调试
		}
		pos += ( outputSizes[ i ] + blockSize - 1 ) / blockSize;
	}
	//最终把区块的容量写入
	write( &out, blockSize );
	//在表头位置写入
	assert( dataEnd % blockSize == 0 );
	unsigned dataEndInBlock = static_cast< unsigned >( dataEnd / blockSize );
	write( &out, dataEndInBlock );
	//析构
	delete[] outputSizes;
	outputSizes = 0;
	delete[] originalSizes;
	originalSizes = 0;
	delete[] inPlaceDecompressionBufferSizes;
	inPlaceDecompressionBufferSizes = 0;

	return true;
}
