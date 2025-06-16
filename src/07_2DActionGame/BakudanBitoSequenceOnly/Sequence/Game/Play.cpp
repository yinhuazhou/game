#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "State.h"

namespace Sequence{
namespace Game{

Play::Play(){ //什么都不做
}

Play::~Play(){ //什么都不做
}

//游戏本体
void Play::update( Parent* parent ){
	Framework f = Framework::instance();;

	State* state = parent->state();

	bool cleared = state->hasCleared();
	bool die1P = false;
	bool die2P = false;
	//使用debug命令行测试。
	if ( f.isKeyTriggered( '1' ) ){ //1P杀人
		die2P = true;
	}else if ( f.isKeyTriggered( '2' ) || f.isKeyTriggered( 'x' ) ){ //2P杀人
		die1P = true;
	}else if ( f.isKeyTriggered( 'c' ) ){
		cleared = true;
	}
	//如果按空格键，则暂停
	//清除后报告
	if ( parent->mode() == Parent::MODE_1P ){
		if ( cleared && !die1P ){
			parent->moveTo( Parent::NEXT_CLEAR );
		}else if ( die1P ){
			parent->moveTo( Parent::NEXT_FAILURE );
		}
	}else{ //双人
		if ( die1P || die2P ){ //如果任何一方死亡，则判断胜负
			parent->moveTo( Parent::NEXT_JUDGE );
		}
	}
	if ( f.isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//更新
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
	state->update( dx, dy );
	//绘制
	state->draw();
}

} //namespace Game
} //namespace Sequence
