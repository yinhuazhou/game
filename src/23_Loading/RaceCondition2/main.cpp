#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
#include <sstream>
using namespace std;
using namespace GameLib::Threading;

class Bool{
public:
	Bool() : mBool( false ){
		mMutex = Mutex::create();
	}
	void operator=( bool f ){
		mMutex.lock();
		mBool = f;
		mMutex.unlock();
	}
	bool get(){
		//乍一看，它看起来很奇怪。这是为了减少缓存和编译器问题
		mMutex.lock();
		bool r = mBool;
		mMutex.unlock();
		return r;
	}
private:
	bool mBool;
	Mutex mMutex;
};

int gX;
Bool gWrite;
Bool gRead;

//具有在执行线程的功能类
class MyThread : public Thread{
public:
	MyThread(){ start(); }
	~MyThread(){ wait(); }
	void operator()(){
		for ( int i = 0; i < 100; ++i ){
			while ( !gRead.get() ){ //等待读取
				;
			}
			gX += 2;
			gRead = false; //还没读
			gWrite = true; //写了
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
			while ( !gWrite.get() ){ //等待写
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
