#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
#include <sstream>
using namespace GameLib::Threading;

//要加的变量
int gCount;
//
Mutex gLock;

//函数类
class IncrementThread : public Thread{
public:
	IncrementThread(){
		start();
	}
	~IncrementThread(){
		wait();
	}
	//100万次递增
	void operator()(){
		for ( int i = 0; i < 1000*1000; ++i ){
			gLock.lock(); //加锁
			gCount += 2; //
			gLock.unlock(); //打开锁。
		}
	}
};

namespace GameLib{
	void Framework::update(){
		gCount = 0;
		gLock = Mutex::create(); //创建锁
		setFrameRate( 60 );
		{
			IncrementThread t0;
			IncrementThread t1;
		} //线程在哪里释放
		std::ostringstream o;
		o << gCount;
		drawDebugString( 0, 0, o.str().c_str() );
	}
} //namespace GameLib

