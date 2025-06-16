//最近流行的RangeCoder。但是，请注意，该专利是否可行值得确认。如果担心，请使用霍夫曼。

//为了使代码在保持易读性，性能被忽略。性能非常慢
//考虑如何更快。

#include <fstream>
#include <iostream>
#include <cassert> //使用标准断言。用法与GameLib中的ASSERT相同。
#include "BitStream.h" //以位为单位的读写类
using namespace std;

//经常使用unsigned，
typedef unsigned U4; //4这是U 4字节的Unsigned的意思

//宽度精度。要压缩大于此大小的文件，需要一些技巧。
const U4 RANGE_MAX = 0xffffffff;

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
	//RangeCoder最坏的情况是所有代码均为8位，并且添加了1024字节的频率表和4字节的原始大小。
	int outMaxSize = inSize + 256 * 4 + 4;
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

void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	OBitStream stream( dataOut ); //目标缓冲区。实际上不需要一点一点地输出，这样是为了容易理解
	U4 n = sizeIn; //

	//首先写入原始文件大小
	stream.write( n );

	//首先计算字符数
	U4 range[ 256 ];
	for ( U4 i = 0; i < 256; ++i ){
		range[ i ] = 0;
	}
	for ( U4 i = 0; i < n; ++i ){
		++range[ dataIn[ i ] ];
	}
	//
	for ( int i = 0; i < 256; ++i ){
		stream.write( range[ i ] );
	}
	//现在将其转换为起点和范围。
	U4 begin[ 256 ];
	begin[ 0 ] = 0;
	for ( U4 i = 1; i < 256; ++i ){
		begin[ i ] = begin[ i - 1 ] + range[ i - 1 ];
	}
	//
	U4 b = 0; //起点
	U4 r = RANGE_MAX; //
	for ( U4 i = 0; i < n; ++i ){
		U4 c = dataIn[ i ]; //字母c
		//计算宽度/数据量。用unit的缩写u
		U4 u = r / n;
		//更新起点
		U4 add = u * begin[ c ];
		if ( b > ( b + add ) ){ //处理这种不规则非常重要。在这里很难说出为什么有必要。
			stream.add(); //继续。最后一位加1。
		}
		b += u * begin[ c ];
		//更新宽度
		r = u * range[ c ];
		//如果宽度小于0x40000000，将其移位1位。
		while ( r <= RANGE_MAX/2 ){
			//输出最高位。
			stream.write( ( b >= 0x80000000 ) ? true : false );
			//将宽度和起点都加倍。
			r <<= 1;
			b <<= 1;
		}
	}
	//输出剩余的32位
	for ( int i = 0; i < 32; ++i ){
		stream.write( ( b >= 0x80000000 ) ? true : false );
		b <<= 1;
	}

	*sizeOut = stream.sizeInByte() + 4; //获取以字节为单位的大小，并在头部字符数上加4。
}

//
//这是因为加入了范围查找。但是通过二分法查找，它的速度提高了约10倍。
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	IBitStream stream( dataIn );
	//首先，取出32位。这是文件的大小。
	U4 n = stream.readU4();

	//提取每个字符的频率
	U4 range[ 256 ];
	for ( U4 i = 0; i < 256; ++i ){
		range[ i ] = stream.readU4();
	}
	//现在将其转换为起点和范围。
	U4 begin[ 256 ];
	begin[ 0 ] = 0;
	for ( U4 i = 1; i < 256; ++i ){
		begin[ i ] = begin[ i - 1 ] + range[ i - 1 ];
	}
	//开始展开
	U4 b = 0;
	//前32位是起点。读取它
	for ( int i = 0; i < 32; ++i ){
		b <<= 1;
		b |= ( stream.read() ) ? 1 : 0;
	}
	U4 r = RANGE_MAX; //
	U4 pos = 0; //写位置
	for ( U4 i = 0; i < n; ++i ){
		//计算宽度/数据量。用unit的缩写u
		U4 u = r / n;
		//首先要做的是搜索。找出当前b在哪里。
		int c = -1; //文字
		for ( U4 j = 0; j < 256; ++j ){
			if ( b < ( ( begin[ j ] + range[ j ] ) * u ) ){ //如果它小于起点+宽度，则在该区域中。
				c = j; //发现
				break;
			}
		}
		assert( c != -1 ); //找到了什么？
		//找到的字符是c！
		dataOut[ pos ] = static_cast< unsigned char >( c );
//cout<static cast<char>（c）；//绘制出现的文字。
		++pos;
		//从起点减去该位置。
		b -= u * begin[ c ];
		//更新宽度
		r = u * range[ c ];
		//如果宽度小于0x40000000，将其移位1位。
		while ( r <= RANGE_MAX/2 ){
			//将宽度和起点都加倍。
			r <<= 1;
			b <<= 1;
			//从b的最低位bit获取输出信息
			b |= ( stream.read() ) ? 1 : 0;
		}
	}
//	cout << endl;
	assert( pos == n ); //解码正确吗？
	assert( stream.positionInByte() <= sizeIn ); //读取太多了？
	*sizeOut = pos;
}
