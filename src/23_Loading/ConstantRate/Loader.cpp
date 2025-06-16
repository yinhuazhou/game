#include "GameLib/GameLib.h"
#include "Loader.h"
#include <fstream>
using namespace std;

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

Loader* Loader::mInstance = 0;

Loader::Loader() : 
mCurrentFile( 0 ),
mStream( 0 ),
mFileSize( 0 ),
mFilePosition( 0 ),
mData( 0 ){
}

Loader::~Loader(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		SAFE_DELETE( *i );
	}
	//如果正在读取文件，则销毁
	if ( mStream ){
		SAFE_DELETE( mStream );
	}
	if ( mData ){
		SAFE_DELETE( mData );
	}
	mCurrentFile = 0; //不要delete。应该在列表中
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
	*f = new File( filename );
	mFiles.push_back( *f );
}

void Loader::destroyFile( File** f ){
	if ( !( *f ) ){ //已经为0。
		return;
	}
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		if ( *i == *f ){ //找到了。
			SAFE_DELETE( *f ); //消除东西
			mFiles.erase( i ); //也将其从列表中删除
			*f = 0; //将接收指针设置为0
			break;
		}
	}
	ASSERT( !( *f ) && "can't find this pointer in the list. there must be a BUG!" );
}

void Loader::update(){
	typedef list< File* >::iterator It;
	//如果已经在加载，请继续加载
	if ( mCurrentFile ){
		//真的在列表中吗？
		bool found = false;
		for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
			if ( *i == mCurrentFile ){
				found = true; //找到了
				break;
			}
		}
		if ( !found ){ //不再了吗？似乎被销毁了。
			if ( mData ){
				SAFE_DELETE_ARRAY( mData );
			}
			if ( mStream ){
				SAFE_DELETE( mStream );
			}
			mFileSize = mFilePosition = 0;
			mCurrentFile = 0;
		}else{ //有一个文件。继续加载吗？
			int rest = mFileSize - mFilePosition;
			//如果这次读取的大小大于READ_UNIT，则为READ_UNIT，其余大小如下
			int size = ( rest > READ_UNIT ) ? READ_UNIT : rest;
			//读取
			mStream->read( mData + mFilePosition, size );
			mFilePosition += size;
			//完成后进行设置。
			if ( size == rest ){ //读取量与其余部分相等=已完成
				mCurrentFile->mData = mData;
				mCurrentFile->mSize = mFileSize;
				mData = 0; //
				mFileSize = mFilePosition = 0;
				SAFE_DELETE( mStream );
				mCurrentFile = 0;
			}
		}
	}else{ //没有正在读取的内容，必须打开文件。
		for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
			File* f = *i;
			if ( !f->isReady() ){ //还没结束，会加载
				mStream = new ifstream( f->mFilename.c_str(), ifstream::binary );
				mStream->seekg( 0, ifstream::end );
				mFileSize = static_cast< int >( mStream->tellg() );
				mData = new char[ mFileSize ];
				mStream->seekg( 0, ifstream::beg ); //
				mFilePosition = 0;
				mCurrentFile = f;
				break; //退出
			}
		}
	}
}
