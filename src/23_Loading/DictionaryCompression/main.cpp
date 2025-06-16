#include <fstream>
#include <iostream>
#include <cassert> //使用标准断言。用法与GameLib中的ASSERT相同。
using namespace std;

void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );

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
	//本书描述的字典压缩最糟糕的是每127个字符
	//它只表示“从现在开始不再压缩127个字符”
	//文件大小仅增加到128/127。
	//在最坏的情况下可以确保这种容量。
	//但是，使用inSize * 128/127进行计算时，* 128可能会超过int的限制。
	//因此，通过添加inSize / 127来执行处理。另外，别忘了+1
	//如果感觉不对，请检查是否越界了。因此，最好将outMaxSize传递给compress（）。
	int outMaxSize = inSize + ( inSize / 127 ) + 1; //不要忘记+1。
	char* outData = new char[ outMaxSize ];

	//开始压缩
	int outSize;
	compress( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );

	//尺寸缩小
	cout << "FileSize: " << inSize << " -> " << outSize << endl;

	//确保对压缩数据进行解压后能够得到和原始数据相同
	char* outData2 = new char[ inSize ]; //确实是一样的吗
	int outSize2;
	decompress( 
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

//常用最大值和最小值
int min( int a, int b ){
	return ( a < b ) ? a : b;
}

int max( int a, int b ){
	return ( a > b ) ? a : b;
}

void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int oPos = 0; //写入侧的写入位置
	int i = 0;
	int unmatchBegin = 0; //不匹配区域的起始位置
	while ( i < sizeIn ){
		//从字典中搜索
		int matchLength = 0;
		int matchPos = 0;
		//从字典的开头搜索。j不超过i。
		//字典的开头。
		int dicBegin = max( i - 255, 0 ); //注意不要放在0的前面。所以使用max（）
		//最大搜寻长度
		int maxL = min( 127, sizeIn - i ); //由于无法搜索超出文件末尾的位置，因此请限制maxL。
		for ( int j = dicBegin; j < i; ++j ){ //该循环的内容占据了大多数计算。有必要在这里提升性能，但我认为这非常困难。
			//查找匹配长度
			int l = 0;
			while ( l < maxL ){ //j<iで、i+l<sizeIn。よって、j+l<sizeInで、範囲内に入る。ここでj+l>=iはありうる。つまり、辞書をはみ出して検索することはありうる。しかし、それでも正しく動くのだ。図を描いて調べよう。
				//如果下一个字符不匹配则结束
				if ( dataIn[ j + l ] != dataIn[ i + l ] ){
					break;
				}
				++l; //1文字成长
			}
			//记录是否比以前更长。匹配的方式很多，记录最大的一种。
			if ( matchLength < l ){
				matchPos = j;
				matchLength = l;
				if ( matchLength == maxL ){ //当达到最大匹配长度时，它就到此为止。
					break;
				}
			}
		}
		//如果匹配的字符超过3个，请以压缩模式进行记录。
		if ( matchLength >= 3 ){
			if ( unmatchBegin < i ){
				//写入未压缩的标头
				dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					dataOut[ oPos ] = dataIn[ j ];
					++oPos;
				}
			}
			//记录压缩部分
			dataOut[ oPos + 0 ] = static_cast< unsigned char >( 0x80 + matchLength ); //长度
			dataOut[ oPos + 1 ] = static_cast< unsigned char >( i - matchPos ); //位置
			oPos += 2;
			i += matchLength;
			unmatchBegin = i; //下一个开始是非一致位置
		}else{ //不匹配。
			++i;
			if ( i - unmatchBegin == 127 ){ //累积到一定程度。写入
				//写入未压缩的标头
				dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					dataOut[ oPos ] = dataIn[ j ];
					++oPos;
				}
				unmatchBegin = i;
			}
		}
	}

	//如果仍然存在不匹配位置，则最后写入
	if ( unmatchBegin < i ){
		//写入未压缩的标头
		dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
		++oPos;
		for ( int j = unmatchBegin; j < i; ++j ){
			dataOut[ oPos ] = dataIn[ j ];
			++oPos;
		}
	}
	*sizeOut = oPos; //写入大小
}

//部署非常容易。
//应该检查是否有错误。如果无法在中途读取损坏的文件，实用性就太差了
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int outPos = 0;
	for ( int i = 0; i < sizeIn; ++i ){
		int length;
		if ( dataIn[ i ] & 0x80 ){ //压缩方式
			length = dataIn[ i ] - 0x80;
			int position = dataIn[ i + 1 ];
			for ( int j = 0; j < length; ++j ){
				dataOut[ outPos + j ] = dataOut[ outPos - position + j ]; //
			}
			i += 1; //1多打工。
		}else{ //未压缩模式
			length = dataIn[ i ];
			for ( int j = 0; j < length; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 + j ];
			}
			i += length; //即使将其保留，也将添加1。通过rawLength推进原始数据并自然保留前​​一个字节
		}
		outPos += length;
	}
	*sizeOut = outPos;
}
