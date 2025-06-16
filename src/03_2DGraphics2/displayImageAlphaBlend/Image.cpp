#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image( const char* filename ) : 
mWidth( 0 ),
mHeight( 0 ),
mData( 0 ){
	File f( filename );
	mHeight = f.getUnsigned( 12 );
	mWidth = f.getUnsigned( 16 );
	mData = new unsigned[ mWidth * mHeight ];
	for ( int i = 0; i < mWidth * mHeight; ++i ){
		mData[ i ] = f.getUnsigned( 128 + i * 4 );
	}
}

Image::~Image(){
	delete[] mData;
	mData = 0;
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

//#define USE_FLOAT_VERSION
#define USE_INTEGER_VERSION
//#define USE_OPTIMIZED_VERSION_0
//#define USE_OPTIMIZED_VERSION_1

#ifdef USE_FLOAT_VERSION
//加入了混合处理（这个版本使用的是double类型，所以性能会差一些）
//优化的时候别忘记和这段代码的结果做一个对比
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ ( y + srcY ) * mWidth + ( x + srcX ) ];
			unsigned* dst = &vram[ ( y + dstY ) * windowWidth + ( x + dstX ) ];
			double srcA = static_cast< double >( ( src & 0xff000000 ) >> 24 ) / 255.f;
			double srcR = static_cast< double >( ( src & 0x00ff0000 ) >> 16 );
			double srcG = static_cast< double >( ( src & 0x0000ff00 ) >> 8 );
			double srcB = static_cast< double >( ( src & 0x000000ff ) >> 0 );

			double dstR = static_cast< double >( ( *dst & 0x00ff0000 ) >> 16 );
			double dstG = static_cast< double >( ( *dst & 0x0000ff00 ) >> 8 );
			double dstB = static_cast< double >( ( *dst & 0x000000ff ) >> 0 );

			double r = ( srcR - dstR ) * srcA + dstR;
			double g = ( srcG - dstG ) * srcA + dstG;
			double b = ( srcB - dstB ) * srcA + dstB;
			*dst = static_cast< unsigned >( r ) << 16;
			*dst |= static_cast< unsigned >( g ) << 8;
			*dst |= static_cast< unsigned >( b );
		}
	}
}
#endif

#ifdef USE_INTEGER_VERSION
//加入混合处理
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ ( y + srcY ) * mWidth + ( x + srcX ) ];
			unsigned* dst = &vram[ ( y + dstY ) * windowWidth + ( x + dstX ) ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			unsigned srcR = src & 0xff0000;
			unsigned srcG = src & 0x00ff00;
			unsigned srcB = src & 0x0000ff;
			unsigned dstR = *dst & 0xff0000;
			unsigned dstG = *dst & 0x00ff00;
			unsigned dstB = *dst & 0x0000ff;
			unsigned r = ( srcR - dstR ) * srcA / 255 + dstR;
			unsigned g = ( srcG - dstG ) * srcA / 255 + dstG;
			unsigned b = ( srcB - dstB ) * srcA / 255 + dstB;
			*dst = ( r & 0xff0000 ) | ( g & 0x00ff00 ) | b;
		}
	}
}
#endif

#ifdef USE_OPTIMIZED_VERSION_0
//加入了混合处理（并行处理的版本）

void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	int dx = srcY * mWidth + srcX;
	int dy = dstY * windowWidth + dstX;
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ sy + x ];
			unsigned* dst = &vram[ dy + x ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			unsigned srcRB = src & 0xff00ff;
			unsigned srcG = src & 0x00ff00;
			unsigned dstRB = *dst & 0xff00ff;
			unsigned dstG = *dst & 0x00ff00;
			unsigned rb = ( srcRB - dstRB ) * srcA  + dstRB * 255;
			unsigned r = ( rb & 0xffff0000 ) / 255;
			unsigned b = ( rb & 0x0000ffff ) / 255;
			unsigned g = ( srcG - dstG ) * srcA / 255 + dstG;
			*dst = ( r & 0xff0000 ) | ( g & 0x00ff00 ) | b;
		}
		sy += mWidth;
		dy += windowWidth;
	}
}
#endif

#ifdef USE_OPTIMIZED_VERSION_1
//带有Alpha混合（并行版本）

void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	int sy = srcY * mWidth + srcX;
	int dy = dstY * windowWidth + dstX;
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ sy + x ];
			unsigned* dst = &vram[ dy + x ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			srcA += ( srcA >= 1 ) ? 1 : 0; //超过1就按1来算
			unsigned srcRB = src & 0xff00ff;
			unsigned srcG = src & 0x00ff00;
			unsigned dstRB = *dst & 0xff00ff;
			unsigned dstG = *dst & 0x00ff00;
			unsigned rb = ( ( ( srcRB - dstRB ) * srcA ) >> 8 ) + dstRB;
			unsigned g = ( ( ( srcG - dstG ) * srcA ) >> 8 ) + dstG;
			*dst = ( rb & 0xff00ff ) | ( g & 0x00ff00 );
		}
		sy += mWidth;
		dy += windowWidth;
	}
}
#endif