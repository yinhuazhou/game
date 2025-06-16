#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//函数原型
void mainLoop();

//全局变量
State* gState = 0;

//用户封装函数。内容被抛出给mainLoop（）
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	Framework f = Framework::instance();
	//初始化第一帧
	if ( !gState ){ 
		File file( "stageData.txt" );
		if ( !( file.getData() ) ){ //没有数据！
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.getData(), file.getSize() );
	}
	bool cleared = false;
	//主循环
	//清除检测
	if ( gState->hasCleared() ){
		cleared = true;
	}
	int dx = 0;
	int dy = 0;
	if ( f.isKeyOn( 'a' ) ){
		dx -= 1;
	}else if ( f.isKeyOn( 's' ) ){
		dx += 1;
	}else if ( f.isKeyOn( 'w' ) ){
		dy -= 1;
	}else if ( f.isKeyOn( 'z' ) ){
		dy += 1;
	}
	//更新
	gState->update( dx, dy );
	//绘制
	gState->draw();

	if ( cleared ){
		//庆祝消息
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
	//结束判断
	if ( f.isKeyOn( 'q' ) ){
		f.requestEnd();
	}
	if ( f.isEndRequested() ){
		if ( gState ){
			delete gState;
			gState = 0;
		}
	}
}


