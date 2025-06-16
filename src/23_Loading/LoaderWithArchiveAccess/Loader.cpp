#include "GameLib/GameLib.h"
#include "Loader.h"
#include <fstream>

#include <sstream>
#include "GameLib/Framework.h"
using namespace std;

namespace{ //习惯使用匿名名称空间。
	int getInt( ifstream& in ){
		unsigned char buffer[ 4 ];
		in.read( reinterpret_cast< char* >( buffer ), 4 );
		int r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}
} 

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

Archive::Archive( const char* name ){
	//打开文件并与成员放在一起。
	mStream = new ifstream( name, ifstream::binary );
	//在末尾前移4个字节
	mStream->seekg( -4, ifstream::end );
	//getInt（）是一个读取4个字节并返回int的函数。
	int tableBegin = getInt( *mStream );
	//移至table
	mStream->seekg( tableBegin, ifstream::beg );
	//4字节读取和文件数
	mFileNumber = getInt( *mStream );
	//循环读取
	for ( int i = 0; i < mFileNumber; ++i ){
		Entry e;
		e.mPosition = getInt( *mStream );
		e.mSize = getInt( *mStream );
		int nameLength = getInt( *mStream );
		//名称被临时放置在临时数组中。将尽快删除它。
		char* name = new char[ nameLength + 1 ]; //
		mStream->read( name, nameLength );
		name[ nameLength ] = '\0'; //
		//mEntries是map< char*, Entry >
		mEntries.insert( make_pair( name, e ) ); //储存在map
		//输出信息。对吗
//		GameLib::cout << e.mPosition << " " << e.mSize << " " << nameLength << " " << name << GameLib::endl;
		SAFE_DELETE_ARRAY( name );
	}
}

Archive::~Archive(){
	SAFE_DELETE( mStream );
}

void Archive::read( File* f )  {
	typedef map< string, Entry >::const_iterator It;
	It it = mEntries.find( f->mFilename );
	if ( it != mEntries.end() ){
		const Entry& e = it->second;
		f->mData = new char[ e.mSize ]; 
		f->mSize = e.mSize;
		//移动位置
		mStream->seekg( e.mPosition, ifstream::beg );
		//读取
		mStream->read( f->mData, e.mSize );
	}else{ //没有
		ASSERT( false );
	}
}

Loader* Loader::mInstance = 0;

Loader::Loader( const char* archiveName ) : mArchive( 0 ){
	if ( archiveName ){ //似乎有档案，打开它并做好准备。
		mArchive = new Archive( archiveName );
	}
}

Loader::~Loader(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		SAFE_DELETE( *i );
	}
	if ( mArchive ){
		SAFE_DELETE( mArchive );
	}
}

Loader* Loader::instance(){
	return mInstance;
}

void Loader::create( const char* archiveName ){
	ASSERT( !mInstance && "already created." );
	mInstance = new Loader( archiveName );
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
			if ( mArchive ){ //来自档案
				mArchive->read( f );
			}else{
				ifstream in( f->mFilename.c_str(), ifstream::binary );
				in.seekg( 0, ifstream::end );
				f->mSize = static_cast< int >( in.tellg() );
				in.seekg( 0, ifstream::beg );
				f->mData = new char[ f->mSize ];
				in.read( f->mData, f->mSize );
			}
		}
	}
}
