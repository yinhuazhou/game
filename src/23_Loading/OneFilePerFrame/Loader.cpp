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

Loader::Loader(){
}

Loader::~Loader(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		SAFE_DELETE( *i );
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
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		File* f = *i;
		if ( !f->isReady() ){ //还没结束，会加载
			ifstream in( f->mFilename.c_str(), ifstream::binary );
			in.seekg( 0, ifstream::end );
			f->mSize = static_cast< int >( in.tellg() );
			in.seekg( 0, ifstream::beg );
			f->mData = new char[ f->mSize ];
			in.read( f->mData, f->mSize );
			break; //该示例只有一行。
		}
	}
}
