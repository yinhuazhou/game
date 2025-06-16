#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include <sstream>
using namespace std;
using namespace GameLib::Threading;

int gX;
bool gWrite;
bool gRead;

//具有在执行线程的功能类
class MyThread : public Thread{
public:
	MyThread(){ start(); }
	~MyThread(){ wait(); }
	void operator()(){
		for ( int i = 0; i < 100; ++i ){
			while ( !gRead ){ //等待读取
				;
			}
			gX += 2;
			gRead = false;
			gWrite = true;
		}
	}
};

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		ostringstream o;

		gX = 0;
		gWrite = false;
		gRead = true;
		MyThread t;
		for ( int i = 0; i < 100; ++i ){
			while ( !gWrite ){ //等到写
				;
			}
			o.str( "" );
			o << gX;
			gWrite = false;
			gRead = true;
			drawDebugString( ( i % 16 ) * 4, i / 16, o.str().c_str() );
		}
	}
} //namespace GameLib
