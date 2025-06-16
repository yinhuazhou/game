#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"
#include <fstream>
using namespace std;
using namespace GameLib;

//压缩
bool compress(
int* writtenSize, 
int* inplaceDecompressionBufferSize,
bool* compressed,
ofstream* oStream, 
ifstream* iStream,
double threshold ){
	//正常来说应该要分批读取，不过那样太麻烦了这里且按一次处理来执行
	iStream->seekg( 0, ifstream::end );
	int inSize = static_cast< int >( iStream->tellg() );
	iStream->seekg( 0, ifstream::beg );
	Array< char > inData( inSize + 1 ); //为了确保0字节也能正常运行
	iStream->read( &inData[ 0 ], inSize );

	//开始压缩了
	Array< char > outData;
	Compressor::compress( 
		&outData,
		inplaceDecompressionBufferSize,
		&inData[ 0 ],
		inSize );
	int outSize = outData.size();
//ifndef NDEBUG/确保对压缩数据进行解压后能得到和原始数据相同
	if ( outSize > 0 ){
		{
			Array< char > decompressed( inSize );
			int decompressedSize;
			Compressor::decompress( 
				&decompressed[ 0 ],
				&decompressedSize,
				&outData[ 0 ],
				outSize );
			STRONG_ASSERT( inSize == decompressedSize );
			for ( int i = 0; i < inSize; ++i ){
				STRONG_ASSERT( inData[ i ] == decompressed[ i ] );
			}
		}
		{
			//解压还原测试
			Array< char > decompressed( *inplaceDecompressionBufferSize );
			int offset = *inplaceDecompressionBufferSize - outSize;
			for ( int i = 0; i < outSize; ++i ){
				decompressed[ offset + i ] = outData[ i ];
			}
			int decompressedSize;
			Compressor::decompress( 
				&decompressed[ 0 ],
				&decompressedSize,
				&decompressed[ 0 ] + offset,
				outSize );
			STRONG_ASSERT( inSize == decompressedSize );
			for ( int i = 0; i < inSize; ++i ){
				STRONG_ASSERT( inData[ i ] == decompressed[ i ] );
			}
		}
	}
//#endif
	//写入
	double t = inSize;
	t *= threshold;
	t /= 100.0;
	if ( outSize < t ){ //写入压缩版本
		oStream->write( &outData[ 0 ], outSize );
		*writtenSize = outSize;
		*compressed = true;
	}else{
		oStream->write( &inData[ 0 ], inSize );
		*writtenSize = inSize;
		*inplaceDecompressionBufferSize = inSize;
		*compressed = false;
	}
	return true;
}

