﻿#include "GameLib/Framework.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/FileIO/InFile.h"
#include "GameLib/FileIO/OutFile.h"
#include "GameLib/Math/Random.h"
using namespace GameLib;
#include <sstream>
using namespace std;

static const int N = 30;
FileIO::InFile gIn[ N ];
FileIO::OutFile gOut[ N ];
int gSize[ N ];
Math::Random gRandom;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		//存档访问。测试多个档案
		c->setArchiveNumber( 2 );
		c->setArchiveName( 0, "data1.bin" );
		c->setArchiveName( 1, "data2.bin" ); //它不必存在。
		c->setLoadMode( LOAD_ARCHIVE_FIRST );
	}
	void Framework::update(){
		if ( !gRandom ){
			gRandom = Math::Random::create();
			setFrameRate( 60 );
		}
		FileIO::Manager m = FileIO::Manager::instance();
		for ( int i = 0; i < N; ++i ){
			if ( gIn[ i ] ){
				if ( gIn[ i ].isFinished() ){
					for ( int j = 0; j < ( i % 10 ); ++j ){
						ASSERT( gIn[ i ].data()[ j ] == '0'+ ( i % 10 ) );
					}
					gIn[ i ].release();
				}
			}else{
				ostringstream oss;
				oss << "data\\" << i << ".txt";
				gIn[ i ] = FileIO::InFile::create( oss.str().c_str() );
				cout << "READ:" << oss.str().c_str() << endl;
			}
			//检查输出句柄是否还在。完成后删除
			if ( gOut[ i ] ){
				if ( gOut[ i ].isFinished() ){
					gOut[ i ].release();
				}
			}else{
				ostringstream oss;
				oss << "wrote" << i << ".txt";
				gSize[ i ] = gRandom.getInt( 0, N );
				char* buffer = NEW char[ gSize[ i ] ];
				for ( int j = 0; j < gSize[ i ]; ++j ){
					buffer[ j ] = '0' + static_cast< char >( i );
				}
				gOut[ i ] = FileIO::OutFile::create( oss.str().c_str(), buffer, gSize[ i ] );
				delete[] buffer;
				cout << "WRITE: " << oss.str().c_str() << endl;
			}
		}
		//有时没有handler写入
		if ( gRandom.getInt( 0, 1000 ) == 0 ){
			int size = gRandom.getInt( 0, N );
			char* buffer = NEW char[ size ];
			for ( int j = 0; j < size; ++j ){
				buffer[ j ] = '0' + static_cast< char >( j );
			}
			m.write( "handleLessWrite.txt", buffer, size );
			cout << "write request without handle." << endl;
			delete[] buffer;
		}
		//结束处理（检查内存泄漏，销毁等）
		if ( isEndRequested() ){
			for ( int i = 0; i < N; ++i ){
				gIn[ i ].release();
				while ( gOut[ i ] && !gOut[ i ].isFinished() ){
					; //等待写入
				}
				gOut[ i ].release();
			}
			gRandom.release();
		}
	}
}
