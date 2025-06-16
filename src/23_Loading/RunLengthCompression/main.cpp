#include <fstream>
#include <iostream>
#include <cassert> //使用标准断言
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
	//本书所述的行程压缩最糟糕的是每127个字符
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

	assert( inSize == outSize2 );
	for ( int i = 0; i < inSize; ++i ){
		assert( inData[ i ] == outData2[ i ] );
	}
	cout << "succeeded." << endl;
#ifndef NDEBUG
	while ( true ){ ; }
#endif
}

//压缩

void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int runBegin = 0; //开始连接的地方
	int inPos = 0; //
	int outPos = 0; //写入侧的写入位置
	for ( int i = 0; i <= sizeIn; ++i ){ //如果考虑性能，则应在循环之后编写它，但这样代码比较冗长
		int runLength = i - runBegin; //连续长度。
		int rawLength = i - inPos; //不连续的长度。
		bool case5;
		bool match;
		if ( i == sizeIn ){
			match = false;
			case5 = ( rawLength > 0 ); //如果有些字符尚未写入，则必须将其写入。
		}else{
			match = ( dataIn[ i ] == dataIn[ runBegin ] );
			case5 = false;
		}
		bool case1 = ( runLength == 127 ); //情况一
		bool case2 = ( rawLength == 127 ); //情况二
		bool case3 = ( runLength >= 3 ) && ( !match ); //情况3
		bool case4 = ( runLength == 2 ) && match  && ( rawLength > 2 ); //ケース4 「3文字目が来たとき」というのは「2文字続いていて、前と同じ文字が来た」と読み替えられる。「その前に非連続領域がある」は「rawLength > 2」だ。3じゃなくて2。よく考えよ。
		if ( case1 || case3 ){ //上面的情况1和情况3
			dataOut[ outPos + 0 ] = static_cast< unsigned char >( 0x80 | runLength );
			dataOut[ outPos + 1 ] = dataIn[ runBegin ]; //连接的字符
			outPos += 2; //2打工写了
			inPos = i; //接下来，从当前位置写入。
			runBegin = i; //重设
		}else if ( case2 || case4 || case5 ){ //上述情况2,4,5
			if ( case4 ){ //4的情况下，写入的文字数要连续削减长度。另外，也有2且4的情况。125个字不连续3个字连接的情况。这个以4优先。
				rawLength -= 2; //-2,-1,0这样的情况下，写入字符数会减少两个字。注意这个很麻烦。
			}
			dataOut[ outPos ] = static_cast< unsigned char >( 0x00 | rawLength ); //未连接的长度
			++outPos;
			for ( int j = 0; j < rawLength; ++j ){ //写入未压缩区域
				dataOut[ outPos + j ] = dataIn[ inPos + j ];
			}
			outPos += rawLength; //仅到这里
			inPos += rawLength; //增加写入的字符数。
			if ( case2 ){ //在case2中，最后128个字符已被写入，因此需要复位。
				runBegin = i;
			}
		}
		if ( !match ){ //由于它不是连续的，因此连续性从此处开始。
			runBegin = i;
		}
	}
	*sizeOut = outPos;
}


//部署非常容易。
//应该检查是否有错误。如果无法在中途读取损坏的文件，实用性就太差了
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int outPos = 0;
	for ( int i = 0; i < sizeIn; ++i ){
		if ( dataIn[ i ] & 0x80 ){ //压缩方式
			int runLength = dataIn[ i ] - 0x80;
			for ( int j = 0; j < runLength; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 ];
			}
			outPos += runLength;
			i += 1; //1多打工。
		}else{ //未压缩模式
			int rawLength = dataIn[ i ];
			for ( int j = 0; j < rawLength; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 + j ];
			}
			outPos += rawLength;
			i += rawLength; //即使将其保留，也将添加1。通过rawLength推进原始数据并自然保留前​​一个字节
		}
	}
	*sizeOut = outPos;
}
