#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

Child* Failure::update( Parent* parent ){
	Child* next = this;
	if ( mCount == 60 ){ //1等待秒
		if ( parent->lifeNumber() == 0 ){
			parent->moveTo( Parent::NEXT_GAME_OVER ); 
		}else{
			next = new Ready;
		}
	}
	//绘制
	//首先绘制游戏画面
	parent->drawState();
	//在顶部绘制庆祝消息
	mImage->draw();
	//画清楚和字母
	Framework::instance().drawDebugString( 0, 0, "Ah!" );

	++mCount;

	return next;
}

} //namespace Game
} //namespace Sequence
