#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include <sstream>
using namespace GameLib::Threading;

//要加的变量
int gCount;

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
			gCount += 2;
		}
	}
};

namespace GameLib{
	void Framework::update(){
		gCount = 0;
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

