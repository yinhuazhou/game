#include "GameLib/GameLib.h"
#include "GameLib/Threading/Functions.h"
#include "Loader.h"
#include <fstream>
using namespace std;
using namespace GameLib;
using namespace GameLib::Threading;

File::File( const char* filename ) : 
mFilename( filename ),
mData( 0 ),
mSize( 0 ){
}

File::~File(){
	if ( mData ){
		SAFE_DELETE_ARRAY( mData );
	}
	mSize = 0;
}

bool File::isReady() const {
	return ( mData != 0 ); //由于在加载完成之前数据不包含值，
}

int File::getSize() const {
	ASSERT( mData && "Loading is not finished. " );
	return mSize;
}

const char* File::getData() const {
	ASSERT( mData && "Loading is not finished. " );
	return mData;
}

void LoadingThread::operator()(){
	bool end = false;
	while ( !end ){ //无限循环
		Threading::sleep( 1 ); //每圈休眠一会儿。防止它一直旋转。
		Loader::instance()->update( &end );
	}
}

Loader* Loader::mInstance = 0;

Loader::Loader() : mThread( 0 ), mEndRequest( false ){
	mThread = new LoadingThread;
	mLock = Mutex::create();

	//写入0
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		mFiles[ i ] = 0;
	}
}

Loader::~Loader(){
	//停止线程
	mLock.lock();
	mEndRequest = true;
	mLock.unlock();
	//删除线程。等待结束
	SAFE_DELETE( mThread );

	//
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( mFiles[ i ] ){
			SAFE_DELETE( mFiles[ i ] );
		}
	}
}

Loader* Loader::instance(){
	return mInstance;
}

void Loader::create(){
	ASSERT( !mInstance && "already created." );
	mInstance = new Loader();
}

void Loader::destroy(){
	SAFE_DELETE( mInstance );
}

void Loader::createFile( File** f, const char* filename ){
	ASSERT( !( *f ) && "pointer must be NUL. " ); 
	//寻找空缺的地方
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( !mFiles[ i ] ){
			*f = new File( filename );
			mLock.lock(); //
			mFiles[ i ] = *f;
			mLock.unlock();
			break;
		}
	}
}

void Loader::destroyFile( File** f ){
	if ( !( *f ) ){ //已经为0。
		return;
	}
	//查找
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( mFiles[ i ] == *f ){
			mLock.lock(); //
			SAFE_DELETE( mFiles[ i ] );
			mLock.unlock();
			*f = 0; //将接收指针设置为0
			break;
		}
	}
	ASSERT( !( *f ) && "can't find this pointer in the list. there must be a BUG!" );
}

void Loader::update( bool* endOut ){
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		string filename; //获取文件名。

		mLock.lock();
		if ( mEndRequest ){ //结束了。在下一个循环中退出。
			*endOut = true;
		}
		if ( mFiles[ i ] && !mFiles[ i ]->isReady() ){
			filename = mFiles[ i ]->mFilename;
		}
		mLock.unlock(); //只看一次加载是否完成

		if ( filename.size() > 0 ){ //包含文件名。加载它。
			int size;
			char* data = 0;
			//载入局部变量。
			ifstream in( filename.c_str(), ifstream::binary );
			in.seekg( 0, ifstream::end );
			size = static_cast< int >( in.tellg() ); 
			in.seekg( 0, ifstream::beg );
			data = new char[ size ];
			in.read( data, size );
			//加载后输入关键部分。否则性能将急剧下降。
			mLock.lock();
			if ( mFiles[ i ] ){ //也许不再。检查一下
				mFiles[ i ]->mData = data;
				data = 0; //
				mFiles[ i ]->mSize = size;
			}
			mLock.unlock();
			if ( data ){ //如果仍有数据，说明没有进入上面的if处理，因此将其丢弃。
				//delete速度很慢
				SAFE_DELETE_ARRAY( data );
			}
		}
	}
}
