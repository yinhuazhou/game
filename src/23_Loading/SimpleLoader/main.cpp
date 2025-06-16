#include "GameLib/Framework.h"
#include "Loader.h"
#include <sstream>
using namespace std;
using namespace GameLib;

File* gFiles[ 10 ];

int gCount = 0;

namespace GameLib{
	void Framework::update(){
		if ( !Loader::instance() ){
			Loader::create();
		}
		//执行加载程序
		Loader::instance()->update();

		//如果加载完则删除
		for ( int i = 0; i < 10; ++i ){
			if ( gFiles[ i ] ){
				if ( gFiles[ i ]->isReady() ){
					Loader::instance()->destroyFile( &gFiles[ i ] );
					cout << i << " th File destroyed." << endl;
				}
			}
		}
		ostringstream oss;
 		//随机间隔
		if ( getRandom( 100 ) == 0 ){
			//随机数加载。
			int n = getRandom( 10 );
			for ( int i = 0; i < 10; ++i ){
				if ( n > 0 ){
					if ( gFiles[ i ] == 0 ){
						Loader::instance()->createFile( &gFiles[ i ], "../data/robo.dds" ); //在项目之间共享
						cout << i << " th File created." << endl;
					}
				}
				--n;
			}
		}
		oss.str( "" );
		int framerate = frameRate();
		oss << "Framerate: " << frameRate();
		int c = ( framerate > 255 ) ? 255 : framerate;
		drawDebugString( 0, 0, oss.str().c_str(), 0xffff0000 | ( c << 8 ) | c );
		int previousFrame = previousFrameInterval();
		oss.str( "" );
		oss << "PreviousFrame: " << previousFrameInterval();
		c = ( previousFrame > 63 ) ? 0 : ( 255 - previousFrame * 4 );
		drawDebugString( 0, 1, oss.str().c_str(), 0xffff0000 | ( c << 8 ) | c );

		//
		double p[ 4 ][ 4 ];
		p[ 0 ][ 2 ] = p[ 1 ][ 2 ] = p[ 2 ][ 2 ] = p[ 3 ][ 2 ] = 0.0;
		p[ 0 ][ 3 ] = p[ 1 ][ 3 ] = p[ 2 ][ 3 ] = p[ 3 ][ 3 ] = 1.0;
		p[ 0 ][ 0 ] = p[ 2 ][ 0 ] = ( gCount % 640 ) / 320.0 - 1.05;
		p[ 1 ][ 0 ] = p[ 3 ][ 0 ] = ( gCount % 640 ) / 320.0 - 0.95;
		p[ 0 ][ 1 ] = p[ 1 ][ 1 ] = -0.05;
		p[ 2 ][ 1 ] = p[ 3 ][ 1 ] = 0.05;
		drawTriangle3DH( p[ 0 ], p[ 1 ], p[ 2 ] );
		drawTriangle3DH( p[ 3 ], p[ 1 ], p[ 2 ] );
		++gCount;

		if ( isEndRequested() ){
			Loader::destroy();
		}
	}
} //namespace GameLib