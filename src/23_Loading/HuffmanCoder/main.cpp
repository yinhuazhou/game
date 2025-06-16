//为了使代码在保持易读性，性能被忽略。性能非常慢
//考虑如何更快。

#include <fstream>
#include <iostream>
#include <cassert> //使用标准断言。用法与GameLib中的ASSERT相同。
#include "BitStream.h" //以位为单位的读写类
using namespace std;

void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
//以位为单位比较的函数
bool isEqual( unsigned char* a, unsigned char* b, int sizeInBit );

//命令行上的第一个参数是文件名
int main( int, char** argv ){
	//全部读取
	ifstream in( argv[ 1 ], ifstream::binary ); 
	//argv [1]必须习惯于第一个参数。
	//可以在项目属性的“调试”下设置命令行参数。设置为test.txt。
	in.seekg( 0, ifstream::end );
	int inSize = static_cast< int >( in.tellg() );
	in.seekg( 0, ifstream::beg );
	char* inData = new char[ inSize ];
	in.read( inData, inSize );

	//分配整个写缓冲区。
	//霍夫曼代码的最坏情况是将代码表添加到所有8bit中。
	//
	int outMaxSize = inSize + 9 * 256 + 4;
	char* outData = new char[ outMaxSize ];

	//开始压缩
	int outSize;
	encode( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );

	//尺寸缩小
	cout << "FileSize: " << inSize << " -> " << outSize << endl;

	//确保对压缩数据进行解压后能够得到和原始数据相同
	char* outData2 = new char[ inSize ]; //确实是一样的吗
	int outSize2;
	decode( 
		reinterpret_cast< unsigned char* >( outData2 ), 
		&outSize2, 
		reinterpret_cast< unsigned char* >( outData ), 
		outSize );

	for ( int i = 0; i < inSize; ++i ){
		assert( inData[ i ] == outData2[ i ] );
	}
	assert( inSize == outSize2 );
	cout << "succeeded." << endl;
#ifndef NDEBUG
	while ( true ){ ; }
#endif
}

//压缩


//哈夫曼树中的一个节点。由于它是二叉树，因此具有左右指针，计数和字符。如果没有字符，则为-1。
class Node{
public:
	Node() : mLeft( 0 ), mRight( 0 ), mCount( 0xffffffff ), mLetter( -1 ){} //计数是无限的

	//
	void makeCodeTable( int* lengths, unsigned char* codes, unsigned char* currentCode, int currentCodeLength ){
		//由于8个位包含在一个无符号字符中，因此除以8可以确定字节，其余部分可以确定移位。
		int index = currentCodeLength / 8;
		int shift = currentCodeLength % 8;
		//
		if ( mLeft ){
			//
			currentCode[ index ] &= ~( 1 << shift ); //位设置为0。
			mLeft->makeCodeTable( lengths, codes, currentCode, currentCodeLength + 1);
		}
		if ( mRight ){
			//
			currentCode[ index ] |= ( 1 << shift ); //位设置1。
			mRight->makeCodeTable( lengths, codes, currentCode, currentCodeLength + 1 );
		}
		//如果这是一个字符，则必须输出该字符。
		if ( mLetter != -1 ){
			lengths[ mLetter ] = currentCodeLength;
			for ( int i = 0; i < 8; ++i ){
				codes[ mLetter * 8 + i ] = currentCode[ i ];
			}
		}
	}


	Node* mLeft;
	Node* mRight;
	unsigned mCount; //此节的计数（类似0xffffffff这种unsigned）
	int mLetter; //
};


void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	//数
	int counts[ 256 ];
	for ( int i = 0; i < 256; ++i ){
		counts[ i ] = 0;
	}
	for ( int i = 0; i < sizeIn; ++i ){
		++counts[ dataIn[ i ] ];
	}
	//
	//该树通过排列256个节点开始。
	//

	//归纳思考。
	//一个字符时，只有一个子句。
	//当添加一个字符时，最多增加两个节
	//左右两边的枝都存放起来。由于重复255次，所以最多有511个子句。但准备512个。
	Node nodes[ 512 ];
	//首先，初始化前256个节点。
	for ( int i = 0; i < 256; ++i ){
		nodes[ i ].mCount = counts[ i ];
		nodes[ i ].mLetter = i;
	}
	int nodeNumber = 256; //现在使用256个。

	//创建一棵树！
	//255旋转的话，所有的节都会粘在一起。这个可以证明，所以试试看。
	//
	//无论树的形状如何，具有255个末端节点的树肯定拥有511个节点。
	for ( int j = 0; j < 255; ++j ){
		//首先要做什么。找到数量最少的一个，第二个。
		//不相关的节点将被跳过，因为它们的计数为0xffffffff。
		//第511个是虚拟的。因为数量庞大，
		int merged0 = 511; //计数最小的部分
		int merged1 = 511; //
		for ( int i = 0; i < nodeNumber; ++i ){ //有更好的方法可以更快地完成此搜索，但是代码太长了。
			if ( nodes[ i ].mCount < nodes[ merged1 ].mCount ){
				if ( nodes[ i ].mCount < nodes[ merged0 ].mCount ){
					merged1 = merged0; //2把位上升到目前为止的第一位
					merged0 = i; //1换位
				}else{
					merged1 = i; //2只换位
				}
			}
		}
		//1把第二位连在一起组成一个节。
		Node* newNode = &nodes[ nodeNumber ];
		++nodeNumber;
		newNode->mLeft = &nodes[ merged0 ];
		newNode->mRight = &nodes[ merged1 ];
		newNode->mCount = nodes[ merged0 ].mCount + nodes[ merged1 ].mCount; //计数总和
		nodes[ merged0 ].mCount = nodes[ merged1 ].mCount = 0xffffffff; //不需要附加的数量，因此将其恢复为无穷大。
	}
	//树准备好了！
	//开始压缩，
	//将结果代码放入数组中会更容易，因为以后的处理会更容易。
	int lengths[ 256 ]; //符号长度
	unsigned char codes[ 256 * 8 ]; //符号（最多8个字节）
	//初始化标志
	for ( int i = 0; i < 256; ++i ){
		for ( int j = 0; j < 8; ++j ){
			codes[ i * 8 + j ] = 0;
		}
	}
	//根是我们制作的最后一个部分，编号为510。从编号510开始递归制作一个代码表。
	int currentCodeLength = 0;
	unsigned char currentCode[ 8 ];
	for ( int i = 0; i < 8; ++i ){
		currentCode[ i ] = 0;
	}
	nodes[ 510 ].makeCodeTable( lengths, codes, currentCode, currentCodeLength ); //最后一个参数是当前深度

	//显示结果符号表
	for ( int i = 0; i < 256; ++i ){
		cout << i << "\t" << lengths[ i ] << "\t";
		for ( int j = 0; j < lengths[ i ]; ++j ){
			cout << ( ( codes[ i * 8 + ( j / 8 ) ] & ( 1 << ( j % 8 ) ) ) >> ( j % 8 ) );
		}
		cout << endl;
	}

	//符号表已准备就绪！开始写作。
	OBitStream stream( dataOut ); //输出缓冲区
	
	//首先写入原始文件大小
	stream.write( static_cast< unsigned >( sizeIn ) );

	//写入符号表
	for ( int i = 0; i < 256; ++i ){
		unsigned char l = static_cast< unsigned char >( lengths[ i ] );
		stream.write( &l, 8 ); //符号长度固定为8位
		stream.write( &codes[ i * 8 ], lengths[ i ] ); //长度可变
	}
	
	//开始压缩！编写非常简单。只需替换并编写。
	for ( int i = 0; i < sizeIn; ++i ){
		int c = dataIn[ i ];
		stream.write( &codes[ c * 8 ], lengths[ c ] );
	}
	*sizeOut = stream.sizeInByte() + 4; //获取以字节为单位的大小，并在头部字符数上加4。
}

//
//1因为每次读比特表都会陷入调查符号表中有没有要找的东西的窘境。
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	IBitStream stream( dataIn );
	//首先，取出32位。这是文件的大小。
	int origSize = stream.readS4();

	//制作一个符号表。
	int lengths[ 256 ]; //符号长度
	unsigned char codes[ 256 * 8 ]; //符号（最多8个字节）
	for ( int i = 0; i < 256 * 8; ++i ){
		codes[ i ] = 0; //代码0初始化
	}
	for ( int i = 0; i < 256; ++i ){
		unsigned char buf[ 8 ];
		stream.read( buf, 8 ); //符号长度
		lengths[ i ] = buf[ 0 ];
		stream.read( &codes[ i * 8 ], lengths[ i ] ); //读取符号
	}
	//显示读取的代码表
	for ( int i = 0; i < 256; ++i ){
		cout << i << "\t" << lengths[ i ] << "\t";
		for ( int j = 0; j < lengths[ i ]; ++j ){
			cout << ( ( codes[ i * 8 + ( j / 8 ) ] & ( 1 << ( j % 8 ) ) ) >> ( j % 8 ) );
		}
		cout << endl;
	}

	//读取内容。
	int pos = 0; //写位置
	for ( int i = 0; i < origSize; ++i ){
		//0创建初始化区域，
		unsigned char code[ 8 ];
		for ( int j = 0; j < 8; ++j ){
			code[ j ] = 0;
		}
		//1位读取时检查是否与编码表相匹配
		bool found = false;
		for ( int j = 0; j < 255; ++j ){ //最多255位。
			int index = j / 8;
			int shift = j % 8;
			if ( stream.read() ){ //阅读1bit，1则写入1
				code[ index ] |= ( 1 << shift );
			}
			//从符合表中搜索。
			int length = j + 1;
			for ( int k = 0; k < 256; ++k ){
				if ( lengths[ k ] == length ){ //一样长
					if ( isEqual( &codes[ k * 8 ], code, length ) ){ //
						dataOut[ pos ] = static_cast< unsigned char >( k ); //输出字符
//cout<static cast<char>（k）；//显示解码后的字符
						++pos;
						found = true;
						break;
					}
				}
			}
			if ( found ){
				break;
			}
		}
		assert( found ); //找不到
	}
	cout << endl;
	assert( pos == origSize ); //解码正确吗？
	assert( stream.positionInByte() <= sizeIn );
	*sizeOut = pos;
}


//位比较函数
bool isEqual( unsigned char* a, unsigned char* b, int sizeInBit ){
	//首先以字节为单位
	int sizeInByte = sizeInBit / 8;
	for ( int i = 0; i < sizeInByte; ++i ){
		if ( a[ i ] != b[ i ] ){
			return false;
		}
	}
	sizeInBit = sizeInBit % 8; //剩余位数
	if ( sizeInBit > 0 ){ //如果有，比较其余部分
		//进行一系列的置零，以将所有相关位清零。
		unsigned char mask = static_cast< unsigned char >( 0xff >> ( 8 - sizeInBit ) ); //例如，如果有6位，则要输入00111111。移位并制作11111111。
		return ( ( a[ sizeInByte ] & mask ) == ( b[ sizeInByte ] & mask ) );
	}else{ //
		return true;
	}
}
