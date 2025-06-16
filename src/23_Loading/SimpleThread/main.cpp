#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
using namespace GameLib;
using namespace GameLib::Threading;

//线程派生
class MyThread : public Thread{
public:
	void operator()(){
		cout << "I am a thread." << endl;
	}
};

MyThread* gThread;

namespace GameLib{
	void Framework::update(){
		if ( !gThread ){
			//创建线程，开始
			gThread = new MyThread();
			gThread->start();
		}
		if ( isEndRequested() ){
			gThread->wait(); //完成后将其关闭。
			SAFE_DELETE( gThread );
		}
	}
} //namespace GameLib
