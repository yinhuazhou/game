#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ),mCursorPosistion( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
}

void Pause::update( Parent* parent ){
	//0：重做
	//1：面对面硒
	//2:进入标题画面
	//3：返回
	if ( Pad::isTriggered( Pad::U) ){
		--mCursorPosistion;
		if ( mCursorPosistion < 0 ){ //
 			mCursorPosistion = 1;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosistion;
		if ( mCursorPosistion > 1 ){ //1越过0就循环成0
			mCursorPosistion = 0;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosistion == 0 ){ //返回游戏
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosistion == 1 ){ //返回主题
			parent->moveTo( Parent::NEXT_TITLE );
		}
		//决定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//绘制
	//首先是游戏画面
	parent->drawState();
	//
	mImage->draw();
	//绘制文字
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[pause]" );
	f.drawDebugString( 1, 2, "game continue" );
	f.drawDebugString( 1, 3, "return title" );
	//写光标
	f.drawDebugString( 0, mCursorPosistion + 2, ">" );
}



} //namespace Game
} //namespace Sequence