﻿#include "GameLib/GameLib.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Sound/Manager.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Random.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/Threading/Manager.h"

//示例类库
//#include "GameLib/Input/Keyboard.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector4.h"

#include "src/Framework.h"
#include "src/FontTextureGenerated.h"

#include <string>
using namespace std;

namespace GameLib{

namespace { //匿名

class Impl{
public:
	Impl() :
	mWidth( 640 ),
	mHeight( 480 ),
	mFullScreen( false ),
	mFullScreenForbidden( true ),
	mVSync( false ),
	mAntiAlias( false ),
	mTitle( "A GameLib Framework Application" ),
	mArchiveNames( 0 ),
	mArchiveNumber( 0 ),
	mLoadMode( FileIO::Manager::MODE_DIRECT_FIRST ),
	mPreviousFrameInterval( 0 ),
	mPreviousFrameTime( 0 ),
	mFrameRate( 0 ),
	mIdealFrameInterval( 0 ),
	mEndRequested( false ),
	mStarted( false ){
		//随机数初始化
		mRandom = Random::create();
		//cout初始化
		cout.begin();
		//帧历史记录重置
		unsigned t = time();
		for ( int i = 0; i < TIME_HISTORY_SIZE; ++i ){
			mTimeHistory[ i ] = t;
		}
/*
		mVideoMemoryWithPadding.setSize( mWidth * ( mHeight + 2 ) );
		//0初始化
		for ( int i = 0; i < mWidth * ( mHeight + 2 ); ++i ){
			mVideoMemoryWithPadding[ i ] = 0;
		}
		for ( int i = 0; i < mWidth; ++i ){
			mVideoMemoryWithPadding[ i ] = MAGIC_NUMBER;
			mVideoMemoryWithPadding[ mWidth * ( mHeight + 1 ) + i ] = MAGIC_NUMBER;
		}
*/
	}
	~Impl(){
//
		mPrimitiveRenderer.release();
		for ( int i = 0; i < TEXTURE_NUMBER; ++i ){
			mTextures[ i ].release();
		}

		if ( mArchiveNames ){
			SAFE_DELETE_ARRAY( mArchiveNames );
		}
		mDebugStringRenderer.release();
		mDebugFont.release();
		Graphics::Manager::destroy();
		Input::Manager::destroy();
		Sound::Manager::destroy();
		FileIO::Manager::destroy();
		Threading::Manager::destroy();
		//cout结束
		cout.end();
	}
	void start( void* windowHandle ){
		//线程系统初始化
		Threading::Manager::create( 0 );
		//初始化文件加载器
		if ( mArchiveNumber > 0 ){
			Array< const char* > names( mArchiveNumber );
			for ( int i = 0; i < mArchiveNumber; ++i ){
				names[ i ] = mArchiveNames[ i ].c_str();
			}
			FileIO::Manager::create( &names[ 0 ], mArchiveNumber, mLoadMode );
			//丢弃，因为不需要归档名称数组
			SAFE_DELETE_ARRAY( mArchiveNames );
			mArchiveNumber = 0;
		}else{
			FileIO::Manager::create();
		}
		//声音初始化
		Sound::Manager::create( windowHandle );
		//输入初始化
		Input::Manager::create( windowHandle );
		//初始化绘制
		Graphics::Manager::create( windowHandle, mWidth, mHeight, mFullScreen, mVSync, mAntiAlias );
		//绘制文字
		//ASCII表（ASCII（126-32 + 1 = 95个字符+ 63个字符，158个字符
		Array< Scene::Font::BitmapChar > chars( 158 );
		//ASCII部分填充
		int pos = 0;
		for ( int i = 0x20; i <= 0x7E; ++i ){
			Scene::Font::BitmapChar& c = chars[ pos ];
			c.mCode = static_cast< unsigned short >( i );
			c.mX = static_cast< unsigned short >( ( i % 16 ) * 8 );
			c.mY = static_cast< unsigned short >( ( i / 16 ) * 16 );
			c.mTexturePage = 0;
			c.mWidth = 8;
			c.mHeight = 16;
			++pos;
		}
		//半角假名部分
		for ( int i = 0xA1; i <= 0xDF; ++i ){
			Scene::Font::BitmapChar& c = chars[ pos ];
			c.mCode = static_cast< unsigned short >( i );
			c.mX = static_cast< unsigned short >( ( i % 16 ) * 8 );
			c.mY = static_cast< unsigned short >( ( i / 16 ) * 16 );
			c.mTexturePage = 0;
			c.mWidth = 8;
			c.mHeight = 16;
			++pos;
		}

		ASSERT( pos == 158 );
		int textureSize = sizeof( gFontTga );
		const char* fontDataArray[ 1 ];
		fontDataArray[ 0 ] = gFontTga;
		mDebugFont = Scene::Font::create( fontDataArray, &textureSize, 1, &( chars[ 0 ] ), 158 );
		mDebugStringRenderer = Scene::StringRenderer::create( 2048, 128 ); //这样够了吗
		mDebugStringRenderer.setFont( mDebugFont );
//2D图层
//		m2dTexture = Graphics::Texture( mWidth, mHeight, false );
		Graphics::Manager().setTextureFilter( Graphics::TEXTURE_FILTER_POINT );
		mPrimitiveRenderer = Scene::PrimitiveRenderer::create( 100000, 10000 ); //大量过剩

		
		mStarted = true;
	}
	unsigned time() const {
		return WindowCreator().time();
	}
	void preUpdate(){
		unsigned currentTime = time();
		while ( ( currentTime - mPreviousFrameTime ) < mIdealFrameInterval ){
			Threading::sleep( 1 );
			currentTime = time();
		}
		mPreviousFrameTime = currentTime;

		//帧时间更新
		mPreviousFrameInterval = currentTime - mTimeHistory[ TIME_HISTORY_SIZE - 1 ];
		unsigned frameIntervalSum = currentTime - mTimeHistory[ 0 ];
		mFrameRate = TIME_HISTORY_SIZE * 1000 / frameIntervalSum;
		//更新历史
		for ( int i = 0; i < TIME_HISTORY_SIZE - 1; ++i ){
			mTimeHistory[ i ] = mTimeHistory[ i + 1 ]; 
		}
		mTimeHistory[ TIME_HISTORY_SIZE - 1 ] = currentTime;

		Graphics::Manager().beginDraw();
		float pointerScale;
		Vector2 pointerOffset;
		Graphics::Manager().getPointerModifier( &pointerScale, &pointerOffset );
		Input::Manager().update( pointerScale, pointerOffset );
//样例
		mPrimitiveRenderer.enableDepthTest( false );
		mPrimitiveRenderer.enableDepthWrite( true );
		mPrimitiveRenderer.setCullMode( Graphics::CULL_NONE );
		mPrimitiveRenderer.setBlendMode( Graphics::BLEND_OPAQUE );
	}
	void postUpdate(){
		//----2D处理
/*
		//检查销毁
		for ( int i = 0; i < mWidth; ++i ){
			ASSERT( mVideoMemoryWithPadding[ i ] == MAGIC_NUMBER && "VRAM RANGE ERROR!" );
			ASSERT( mVideoMemoryWithPadding[ mWidth * ( mHeight + 1 ) + i ] == MAGIC_NUMBER && "VRAM RANGE ERROR!" );
		}
		//复制VRAM
		unsigned* dst;
		int pitch;
		m2dTexture.lock( &dst, &pitch );
		const unsigned* src = &mVideoMemoryWithPadding[ mWidth ];
		for ( int y = 0; y < mHeight; ++y ){			
			for ( int x = 0; x < mWidth; ++x ){
				dst[ x ] = 0xff000000 | src[ x ]; //强制alpha ff
			}
			src += mWidth;
			dst += pitch / 4;
		}
		src = 0; //使用终止
		m2dTexture.unlock( &dst );
		Graphics::Manager::getInstance().blendToScreen( m2dTexture );
*/
		mPrimitiveRenderer.draw();

		//合成文字
		mDebugStringRenderer.draw();
		Graphics::Manager().endDraw();
	}
//示例类库
	void setBlendMode( Framework::BlendMode a ){
		Graphics::BlendMode b = Graphics::BLEND_OPAQUE;
		switch ( a ){
			case Framework::BLEND_LINEAR: b = Graphics::BLEND_LINEAR; break;
			case Framework::BLEND_ADDITIVE: b = Graphics::BLEND_ADDITIVE; break;
			case Framework::BLEND_OPAQUE: b = Graphics::BLEND_OPAQUE; break;
			default: ASSERT( false ); break;
		}
		mPrimitiveRenderer.setBlendMode( b );
	}
	void drawTriangle2D(
	const double* p0,
	const double* p1,
	const double* p2,
	const double* t0,
	const double* t1,
	const double* t2,
	unsigned c0,
	unsigned c1,
	unsigned c2 ){
		//转换为浮点数
		Vector3 p[ 3 ];
		Vector2 t[ 3 ];
		
		p[ 0 ].x = static_cast< float >( p0[ 0 ] ); 
		p[ 0 ].y = static_cast< float >( p0[ 1 ] ); 
		p[ 1 ].x = static_cast< float >( p1[ 0 ] );
		p[ 1 ].y = static_cast< float >( p1[ 1 ] );
		p[ 2 ].x = static_cast< float >( p2[ 0 ] );
		p[ 2 ].y = static_cast< float >( p2[ 1 ] );
		p[ 0 ].z = p[ 1 ].z = p[ 2 ].z = 0.f;

		if ( t0 ){
			t[ 0 ].x = static_cast< float >( t0[ 0 ] );
			t[ 0 ].y = static_cast< float >( t0[ 1 ] );
		}
		if ( t1 ){
			t[ 1 ].x = static_cast< float >( t1[ 0 ] );
			t[ 1 ].y = static_cast< float >( t1[ 1 ] );
		}
		if ( t2 ){
			t[ 2 ].x = static_cast< float >( t2[ 0 ] );
			t[ 2 ].y = static_cast< float >( t2[ 1 ] );
		}
		//转换为clip坐标系
		//[0, w ] -> [ -1, 1 ]
		//[0，h]-> [1，-1]
		float sx = 2.f / static_cast< float >( mWidth );
		float sy = -2.f / static_cast< float >( mHeight );
		for ( int i = 0; i < 3; ++i ){
			p[ i ].x *= sx;
			p[ i ].y *= sy;
			p[ i ].x -= 1.f;
			p[ i ].y += 1.f;
		}
		mPrimitiveRenderer.addTriangle( p[ 0 ], p[ 1 ], p[ 2 ], t[ 0 ], t[ 1 ], t[ 2 ], c0, c1, c2 );
	}
	void drawTriangle3D(
	const double* p0,
	const double* p1,
	const double* p2,
	const double* t0,
	const double* t1,
	const double* t2,
	unsigned c0,
	unsigned c1,
	unsigned c2 ){
		//转换为浮点数
		Vector3 p[ 3 ];
		Vector2 t[ 3 ];
		
		for ( int i = 0; i < 3; ++i ){
			p[ 0 ][ i ] = static_cast< float >( p0[ i ] );
			p[ 1 ][ i ] = static_cast< float >( p1[ i ] );
			p[ 2 ][ i ] = static_cast< float >( p2[ i ] );
		}

		if ( t0 ){
			t[ 0 ].x = static_cast< float >( t0[ 0 ] );
			t[ 0 ].y = static_cast< float >( t0[ 1 ] );
		}
		if ( t1 ){
			t[ 1 ].x = static_cast< float >( t1[ 0 ] );
			t[ 1 ].y = static_cast< float >( t1[ 1 ] );
		}
		if ( t2 ){
			t[ 2 ].x = static_cast< float >( t2[ 0 ] );
			t[ 2 ].y = static_cast< float >( t2[ 1 ] );
		}
		//转换为clip坐标系
		//[0, w ] -> [ -1, 1 ]
		//[0，h]-> [1，-1]
		float sx = 2.f / static_cast< float >( mWidth );
		float sy = -2.f / static_cast< float >( mHeight );
		for ( int i = 0; i < 3; ++i ){
			p[ i ].x *= sx;
			p[ i ].y *= sy;
			p[ i ].x -= 1.f;
			p[ i ].y += 1.f;
		}
		mPrimitiveRenderer.addTriangle( p[ 0 ], p[ 1 ], p[ 2 ], t[ 0 ], t[ 1 ], t[ 2 ], c0, c1, c2 );
	}
	void drawTriangle3DH(
	const double* p0,
	const double* p1,
	const double* p2,
	const double* t0,
	const double* t1,
	const double* t2,
	unsigned c0,
	unsigned c1,
	unsigned c2 ){
		//转换为浮点数
		Vector4 p[ 3 ];
		Vector2 t[ 3 ];
		
		for ( int i = 0; i < 4; ++i ){
			p[ 0 ][ i ] = static_cast< float >( p0[ i ] );
			p[ 1 ][ i ] = static_cast< float >( p1[ i ] );
			p[ 2 ][ i ] = static_cast< float >( p2[ i ] );
		}

		if ( t0 ){
			t[ 0 ].x = static_cast< float >( t0[ 0 ] );
			t[ 0 ].y = static_cast< float >( t0[ 1 ] );
		}
		if ( t1 ){
			t[ 1 ].x = static_cast< float >( t1[ 0 ] );
			t[ 1 ].y = static_cast< float >( t1[ 1 ] );
		}
		if ( t2 ){
			t[ 2 ].x = static_cast< float >( t2[ 0 ] );
			t[ 2 ].y = static_cast< float >( t2[ 1 ] );
		}
		mPrimitiveRenderer.addTransformedTriangle( p[ 0 ], p[ 1 ], p[ 2 ], t[ 0 ], t[ 1 ], t[ 2 ], c0, c1, c2 );
	}
	Graphics::Texture createTexture(
	int dw,
	int dh,
	const unsigned* src,
	int sw,
	int sh ){
		//2乘幂检查
		bool dwOk = false;
		bool dhOk = false;
		for ( int i = 0; i < 32; ++i ){
			if ( dw == ( 1 << i ) ){
				dwOk = true;
			}
			if ( dh == ( 1 << i ) ){
				dhOk = true;
			}
		}
		ASSERT( dwOk && dhOk && "Texture size must be POWER OF 2" );
		//寻找可用空间
		int newPos = -1;
		for ( int i = 0; i < TEXTURE_NUMBER; ++i ){
			if ( !mTextures[ i ] ){
				newPos = i;
				break;
			}
		}
		ASSERT( newPos >= 0 && "Texture Full! can't create!" );
		mTextures[ newPos ] = Graphics::Texture::create( dw, dh, false );
		unsigned* dst;
		int pitch;
		mTextures[ newPos ].lock( &dst, &pitch, 0 );
		int yEnd = ( dh < sh ) ? dh : sh;
		int xEnd = ( dw < sw ) ? dw : sw;
		for ( int y = 0; y < yEnd; ++y ){
			for ( int x = 0; x < xEnd; ++x ){
				dst[ x ] = src[ x ];
			}
			for ( int x = xEnd; x < pitch / 4; ++x ){ //X太黑
				dst[ x ] = 0;
			}
			dst += pitch / 4;
			src += sw;
		}
		//Y
		for ( int y = yEnd; y < dh; ++y ){
			for ( int x = 0; x < pitch / 4; ++x ){
				dst[ x ] = 0;
			}
			dst += pitch / 4;
		}
		mTextures[ newPos ].unlock( &dst );
		return mTextures[ newPos ];
	}
	void destroyTexture( Graphics::Texture& t ){
		for ( int i = 0; i < TEXTURE_NUMBER; ++i ){
			if ( mTextures[ i ] == t ){
				mTextures[ i ].release();
				break;
			}
		}
	}
//	Array< unsigned > mVideoMemoryWithPadding;
//	Graphics::Texture m2dTexture;
//	static const unsigned MAGIC_NUMBER = 0x12345678;
	Scene::PrimitiveRenderer mPrimitiveRenderer;
	static const int TEXTURE_NUMBER = 1024; //已经够了。
	Graphics::Texture mTextures[ TEXTURE_NUMBER ];

	int mWidth;
	int mHeight;
	bool mFullScreen;
	bool mFullScreenForbidden;
	bool mVSync;
	bool mAntiAlias;
	string mTitle;
	string* mArchiveNames;
	int mArchiveNumber;
	FileIO::Manager::AccessMode mLoadMode;
	static const int TIME_HISTORY_SIZE = 60;
	unsigned mTimeHistory[ TIME_HISTORY_SIZE ];
	int mPreviousFrameInterval;
	unsigned mPreviousFrameTime;
	int mFrameRate;
	unsigned mIdealFrameInterval;
	bool mEndRequested;
	bool mStarted;
	Scene::StringRenderer mDebugStringRenderer;
	Scene::Font mDebugFont;
	Random mRandom;
};

Impl* gImpl = 0;

} //namespace{}

Framework::Framework(){
	//不允许来自其他线程的调用
	ASSERT( WindowCreator::isMainThread() && "you must call from MAIN thread" );
}

void Framework::create(){
	ASSERT( !gImpl );
	gImpl = NEW Impl();
}

void Framework::destroy(){
	SAFE_DELETE( gImpl );
}

Framework Framework::instance(){
	return Framework();
}

void Framework::start( void* windowHandle ){
	gImpl->start( windowHandle );
}

void Framework::preUpdate(){
	gImpl->preUpdate();
}

void Framework::postUpdate(){
	gImpl->postUpdate();
}

int Framework::width() const {
	return gImpl->mWidth;
}

int Framework::height() const {
	return gImpl->mHeight;
}
/*
const char* Framework::getTitle() const {
	return gImpl->mTitle.c_str();
}

bool Framework::isFullScreen() const {
	return gImpl->mFullScreen;
}

bool Framework::isFullScreenForbidden() const {
	return gImpl->mFullScreenForbidden;
}
*/
void Framework::requestEnd(){
	gImpl->mEndRequested = true;
}

bool Framework::isEndRequested() const {
	return gImpl->mEndRequested;
}

unsigned Framework::time() const {
	return gImpl->time();
}

int Framework::previousFrameInterval() const {
	return gImpl->mPreviousFrameInterval;
}

int Framework::frameRate() const {
	return gImpl->mFrameRate;
}

void Framework::setFrameRate( int fr ){
	gImpl->mIdealFrameInterval = 1000 / fr;
}

void Framework::drawDebugString( int c, int r, const char* s, unsigned col ){
	gImpl->mDebugStringRenderer.add( c * 8, r * 16, s, col );
}
/*
void Framework::enableFullScreen( bool f ){ 
	if ( gImpl->mFullScreenForbidden ){ //被禁止
		return;
	}
	WindowCreator wc = WindowCreator::getInstance();
	Graphics::Manager gm = Graphics::Manager::getInstance();
	if ( gImpl->mStarted ){
		//中途更改
		if ( gImpl->mFullScreen != f ){
			//上游
			wc.enableFullScreen( f );
			//下游
			gm.enableFullScreen( f );
		}
	}
	gImpl->mFullScreen = f;
}

//Configuration
void Framework::Configuration::setWidth( int width ){
	gImpl->mWidth = width;
}

void Framework::Configuration::setHeight( int height ){
	gImpl->mHeight = height;
}

void Framework::Configuration::setTitle( const char* title ){
	gImpl->mTitle = title;
}

void Framework::Configuration::setArchiveNumber( int n ){
	ASSERT( !gImpl->mArchiveNames && "you can't call twice." );
	gImpl->mArchiveNumber = n;
	gImpl->mArchiveNames = NEW string[ n ];
}

void Framework::Configuration::setArchiveName( int index, const char* name ){
	ASSERT( gImpl->mArchiveNames && "you must call setArchiveNumber() before this call." );
	ASSERT( index >= 0 && index < gImpl->mArchiveNumber );
	gImpl->mArchiveNames[ index ] = name;
}

void Framework::Configuration::setLoadMode( LoadMode lm ){
	using namespace FileIO;
	Manager::AccessMode am = Manager::MODE_DIRECT_FIRST;
	switch ( lm ){
		case LOAD_ARCHIVE_FIRST: am = Manager::MODE_ARCHIVE_FIRST; break;
		case LOAD_DIRECT_FIRST: am = Manager::MODE_DIRECT_FIRST; break;
		case LOAD_ARCHIVE_ONLY: am = Manager::MODE_ARCHIVE_ONLY; break;
		default: ASSERT( false ); break;
	}
	gImpl->mLoadMode = am;
}

void Framework::Configuration::enableFullScreen( bool f ){
	gImpl->mFullScreen = f;
}

void Framework::Configuration::enableVSync( bool f ){
	gImpl->mVSync = f;
}

void Framework::Configuration::enableAntiAlias( bool f ){
	gImpl->mAntiAlias = f;
}

void Framework::Configuration::forbidFullScreen( bool f ){
	gImpl->mFullScreenForbidden = f;
}
*/

//示例类库使用的函数
/*
unsigned* Framework::getVideoMemory(){
	return &gImpl->mVideoMemoryWithPadding[ gImpl->mWidth ];
}
bool Framework::isKeyOn( int c ) const {
	return Input::Manager::getInstance().getKeyboard().isOn( c );
}

bool Framework::isKeyTriggered( int c ) const {
	return Input::Manager::getInstance().getKeyboard().isTriggered( c );
}
*/
void Framework::sleep( int ms ) const {
	Threading::sleep( ms );
}

int Framework::getRandom( int m ){
	if ( m <= 0 ){
		return gImpl->mRandom.getInt();
	}else{
		return gImpl->mRandom.getInt( 0, m );
	}
}

/*
void Framework::drawTriangle2D(
const double* p0,
const double* p1,
const double* p2,
const double* t0,
const double* t1,
const double* t2,
unsigned c0,
unsigned c1,
unsigned c2 ){
	gImpl->drawTriangle2D( p0, p1, p2, t0, t1,t2, c0, c1, c2 );
}

void Framework::drawTriangle3D(
const double* p0,
const double* p1,
const double* p2,
const double* t0,
const double* t1,
const double* t2,
unsigned c0,
unsigned c1,
unsigned c2 ){
	gImpl->drawTriangle3D( p0, p1, p2, t0, t1,t2, c0, c1, c2 );
}
*/

void Framework::drawTriangle3DH(
const double* p0,
const double* p1,
const double* p2,
const double* t0,
const double* t1,
const double* t2,
unsigned c0,
unsigned c1,
unsigned c2 ){
	gImpl->drawTriangle3DH( p0, p1, p2, t0, t1,t2, c0, c1, c2 );
}

class Texture{ //纹理dummy类
public:
	Graphics::Texture mTexture;
};
/*
void Framework::createTexture(
Texture** tex,
int tw,
int th,
const unsigned* data,
int iw,
int ih ){
	ASSERT( !( *tex ) && "Non Null Pointer! it might be alrady created." );
	*tex = NEW Texture();
	( *tex )->mTexture = gImpl->createTexture( tw, th, data, iw, ih );
}
*/

void Framework::createTexture(
Texture** tex,
const char* filename ){
	ASSERT( !( *tex ) && "Non Null Pointer! it might be alrady created." );
	*tex = NEW Texture();
	( *tex )->mTexture = Graphics::Texture::create( filename );
	while ( !( *tex )->mTexture.isReady() ){
		; //等待加载
	}
}

void Framework::getTextureSizes( const Texture* tex, int* width, int* height, int* originalWidth, int* originalHeight ){
	if ( width ){
		*width = tex->mTexture.width();
	}
	if ( height ){
		*height = tex->mTexture.height();
	}
	if ( originalWidth ){
		*originalWidth = tex->mTexture.originalWidth();
	}
	if ( originalHeight ){
		*originalHeight = tex->mTexture.originalHeight();
	}
}

void Framework::destroyTexture( Texture** tex ){
	ASSERT( tex && "NULL Pointer!" );
	if ( *tex ){
		gImpl->destroyTexture( ( *tex )->mTexture );
	}
	SAFE_DELETE( *tex );
}

void Framework::setTexture( const Texture* tex ){
	if ( tex ){
		gImpl->mPrimitiveRenderer.setTexture( tex->mTexture );
	}else{
		gImpl->mPrimitiveRenderer.setTexture( 0 );
	}
}

void Framework::setBlendMode( BlendMode a ){
	gImpl->setBlendMode( a );
}

void Framework::enableDepthTest( bool f ){
	gImpl->mPrimitiveRenderer.enableDepthTest( f );
}

void Framework::enableDepthWrite( bool f ){
	gImpl->mPrimitiveRenderer.enableDepthWrite( f );
}

//WindowProcedure的用户封装函数
void WindowCreator::configure( Configuration* config ){
	Framework::create();
	//用户设定
	Framework f;
//	Framework::Configuration fwConfig;
//	f.configure( &fwConfig );
	config->setWidth( f.width() );
	config->setHeight( f.height() );
	config->setTitle( "3D Graphics1 samples" );
	config->enableFullScreen( false );
	config->forbidFullScreen( true );
}

void WindowCreator::update(){
	WindowCreator wc;
	Graphics::Manager gm;
	Framework f;
	//根据WindowCreator的状态执行各种操作
	bool sleepFlag = false;
	//最小化后则休眠
	if ( wc.isMinimized() ){
		sleepFlag = true;
	}
	//基本上会进入睡眠状态。
/*
	if ( !gm.canRender() ){
		gm.restore();
		sleepFlag = true;
	}
	//来自Window的模式切换信号
	bool wcFullScreen = wc.isFullScreen();
	if ( f.isFullScreen() != wcFullScreen ){
		f.enableFullScreen( wcFullScreen );
	}
*/
	if ( !sleepFlag ){
		f.preUpdate();
	}
	//检查结束命令是否来自上游（例如x按钮）
	if ( isEndRequested() ){ //收到后则将终止命令发送到Framework
		f.requestEnd();
	}
	if ( sleepFlag ){
		Threading::sleep( 50 ); //睡50ms
	}else{
		f.update();
		f.postUpdate();
	}
	//结束判断
	if ( f.isEndRequested() ){
		Framework::destroy();
		end(); //让上游知道
	}
}

void WindowCreator::start( void* windowHandle ){
	Framework().start( windowHandle );
}

/*
//StandardInput
StandardInput cin;

StandardInput::StandardInput() : mPrevEnterOn( false ){
}

StandardInput& StandardInput::operator>>( char& a ){
	a = '\0'; //初始值
	Input::Keyboard kb = Input::Manager::getInstance().getKeyboard( 0 );
	bool entered = false;
	while ( true ){ //无限循环
		for ( int i = 0; i < 0x80; ++i ){ //查看所有键
			if ( kb.isOn( i ) ){
				a = static_cast< char >( i );
				entered = true;
			}
		}
		bool enterOn = kb.isOn( Input::Keyboard::KEY_RETURN );
		if ( entered && enterOn && !mPrevEnterOn ){
			mPrevEnterOn = enterOn;
			break;
		}
		float pointerScale;
		Vector2 pointerOffset;
		Graphics::Manager::getInstance().getPointerModifier( &pointerScale, &pointerOffset );
		Input::Manager::getInstance().update( pointerScale, pointerOffset );
		//如果收到终止请求则退出
		bool altF4 = kb.isOn( Input::Keyboard::KEY_ALT ) && kb.isOn( Input::Keyboard::KEY_F4 );
		if ( altF4 ){
			Framework::getInstance().requestEnd();
			break;
		}
		mPrevEnterOn = enterOn;
	}
	return *this;
}
*/

} //namespace GameLib
