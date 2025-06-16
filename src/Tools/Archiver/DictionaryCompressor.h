#include <fstream>
#include <iostream>
#include <cassert>
#include "GameLib/Base/Compressor.h"
using namespace std;

class DictionaryCompressor{
public:
	static bool compress( 
		int* outSize, 
		int* necessaryBufferSize,
		bool* compressed,
		ofstream* oStream, 
		ifstream* iStream,
		double threshold );
private:
	static bool compress( 
		unsigned char* outData, 
		int* outSize, 
		int* necessaryBufferSize,
		int outBufferSize,
		const unsigned char* inData, 
		int inSize );
	static bool decompress( 
		unsigned char* outData, 
		int* outSize,
		int outBufferSize,
		const unsigned char* inData, 
		int inSize );
};

bool DictionaryCompressor::compress(
int* writtenSize, 
int* necessaryBufferSize,
bool* compressed,
ofstream* oStream, 
ifstream* iStream,
double threshold ){
	//如果不分割读取会很麻烦
	iStream->seekg( 0, ifstream::end );
	int inSize = static_cast< int >( iStream->tellg() );
	iStream->seekg( 0, ifstream::beg );
	char* inData = new char[ inSize ];
	iStream->read( inData, inSize );

	//准备好写入的位置
	int outBufferSize = inSize + ( inSize / 127 ) + 1; //不要忘记+1
	char* outData = new char[ outBufferSize ];
	if ( !outData ){
		cerr << "memory allocation failed : " << outBufferSize << endl;
		return false;
	}
	//开始压缩了
	int outSize;
	bool succeeded = compress( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		necessaryBufferSize,
		outBufferSize,
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );
	assert( succeeded );
#ifndef NDEBUG
	{
		//确保对压缩数据进行解压后能够得到和原始数据相同
		char* outData2 = new char[ inSize ]; //确实是一样的吗
		int outSize2;
		succeeded = decompress( 
			reinterpret_cast< unsigned char* >( outData2 ), 
			&outSize2, 
			inSize,
			reinterpret_cast< unsigned char* >( outData ), 
			outSize );
		assert( succeeded );
		for ( int i = 0; i < inSize; ++i ){
			assert( inData[ i ] == outData2[ i ] );
		}
		assert( inSize == outSize2 );
		delete[] outData2;
		outData2 = 0;
	}
#endif
	//写入
	double t = inSize;
	t *= threshold;
	t /= 100.0;
	if ( outSize < t ){ //写入压缩版本
		oStream->write( outData, outSize );
		*writtenSize = outSize;
		*compressed = true;
	}else{
		oStream->write( inData, inSize );
		*writtenSize = inSize;
		*necessaryBufferSize = inSize;
		*compressed = false;
	}
	delete[] inData;
	delete[] outData;
	inData = 0;
	outData = 0;
	return true;
}

//分配bit位
const int DIC_BITS = 10; //位置bit数
//下面是自动计算的常量
const int LENGTH_BITS = 16 - 1 - DIC_BITS; //长度
const int DIC_MASK = ( 1 << DIC_BITS ) - 1;
const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; 
const int DIC_MASK_SHIFTED = ( DIC_MASK >> 8 ) << LENGTH_BITS; 
const int LENGTH_MASK = ( 1 << LENGTH_BITS ) - 1;
const int DIC_SIZE = DIC_MASK + 3; //字典容量
const int MAX_LENGTH = LENGTH_MASK + 3; //最大匹配长度

//常用的比较数大小操作
inline int min( int a, int b ){
	return ( a < b ) ? a : b;
}

inline int max( int a, int b ){
	return ( a > b ) ? a : b;
}

//压缩处理

bool DictionaryCompressor::compress( 
unsigned char* oData, 
int* oSize, 
int* oNecessaryBufferSize,
int oBufferSize, 
const unsigned char* iData, 
int iSize ){
	int oPos = 0; //写入的位置
	int maxPosDiff = 0; //为了还原数据需要分配一定的缓冲区
	int i = 0;
	int unmatchBegin = 0; //非一致领域的开始位置
	while ( i < iSize ){
		//从字典开始寻找
		int matchLength = 0;
		int matchPos = 0;
		//字典开头
		int dicBegin = max( i - DIC_SIZE, 0 ); //不允许小于0，所以使用max()
		//最大查找长度
		int maxL = min( MAX_LENGTH, iSize - dicBegin ); //防止越过合理范围查找
		//从字典的开头开始寻找。
		int jEnd = i - 3; //从3个字符前开始查找。只要超过3个字不匹配就不压缩
		//这里和示例有所不同使用的是一层循环。这块可能不容易理解，但是对于某些特定文件（比如纯白的图片）来说处理速度会加快。
		int j = dicBegin;
		int l = 0;
		while ( ( j < jEnd ) && ( l < maxL ) ){
			if ( iData[ j ] == iData[ i + l ] ){
				++j;
				++l;
			}else{
				//如果和前面的长度一致则记录。 匹配的方法可能有很多，这里记录匹配最大的
				if ( matchLength < l ){
					matchPos = j - l;
					matchLength = l;
				}
				j -= l - 1; //l为0时也会前进一个字符。l增加后将返回。
				l = 0;
			}
		}
		if ( matchLength < l ){
			matchPos = j - l;
			matchLength = l;
		}
		//如果，匹配了3个字以上则按照压缩模式来记录
		if ( matchLength >= 3 ){
			if ( unmatchBegin < i ){
				if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //检测到写入缓冲区不够 最后的+1是针对头部的
					return false; //失败
				}
				//写入非压缩头部
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //至少保存1个，保存最后1个
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
			}
			if ( ( oPos + 2 ) > oBufferSize ){ //检测到缓冲区不足
				return false; //失败
			}
			//记录压缩部分
			int wl = matchLength - 3; //减3后存储
			int wp = i - matchPos - 3; //减3后存储
			int tmp = 0x80 | wl; //在长度值中加入压缩标记
			tmp |= ( wp & DIC_MASK_HIGH ) >> ( 8 - LENGTH_BITS ); //
			oData[ oPos + 0 ] = static_cast< unsigned char >( tmp );
			oData[ oPos + 1 ] = static_cast< unsigned char >( wp & 0xff );
			oPos += 2;
			i += matchLength;
			unmatchBegin = i; //下个开始就是非一致的位置
		}else{ //未匹配
			++i;
			if ( i - unmatchBegin == 128 ){ //攒到一定程度了，写入
				if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //检测到写入缓冲区不够 最后的+1是针对头部的
					return false; //失败
				}
				//写入非压缩头部
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //至少保存1个，保存最后1个
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
				unmatchBegin = i;
			}
		}
		//更新写入位置超过的量
		maxPosDiff = max( maxPosDiff, i - oPos );
	}

	//如果存在不一致的位置，则最后写入
	if ( unmatchBegin < i ){
		if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //检测到写入缓冲区不够 最后的+1是针对头部的
			return false; //失败
		}
		//写入非压缩头部
		oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //至少保存1个，保存最后1个
		++oPos;
		for ( int j = unmatchBegin; j < i; ++j ){
			oData[ oPos ] = iData[ j ];
			++oPos;
		}
	}

	*oNecessaryBufferSize = max( maxPosDiff + oPos, iSize );
	*oSize = oPos;
	return true;
}

//展开非常简单。但是因为错误检查所以有点长
bool DictionaryCompressor::decompress( 
unsigned char* oData, 
int* oSize,
int oBufferSize,
const unsigned char* iData, 
int iSize ){
	int oPos = 0;
	for ( int i = 0; i < iSize; ++i ){
		int length;
		if ( iData[ i ] & 0x80 ){ //压缩模式
			length = iData[ i ] & LENGTH_MASK;
			length += 3; //记录少了3个字
			int position = ( ( iData[ i ] & DIC_MASK_SHIFTED ) << ( 8 - LENGTH_BITS ) ) | iData[ i + 1 ]; //因为很复杂所以分解开
			position += 3; //记录少了3个字
			if ( oPos + length > oBufferSize ){ //检查写入缓冲区大小
				return false;
			}
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = oData[ oPos - position + j ]; //从out移动可能有点怪，之前的out是字典
			}
			i += 1; //多1个字节
		}else{ //非压缩模式
			length = iData[ i ] + 1; //记录少了1个字
			if ( oPos + length > oBufferSize ){ //检查写入缓冲区大小
				return false;
			}
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = iData[ i + 1 + j ];
			}
			i += length; 
		}
		oPos += length;
	}
	*oSize = oPos;
	return true;
}
