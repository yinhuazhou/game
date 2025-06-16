#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Image.h"
#include "State.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Menu::Menu() : mImage( 0 ), mMenuCursor( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Menu::~Menu(){
	SAFE_DELETE( mImage );
}

void Menu::update( Parent* parent ){
	Framework f = Framework::instance();
	//0：重做
	//1：面对面硒
	//2:进入标题画面
	//3：返回
	if ( f.isKeyTriggered( 'w' ) ){
		--mMenuCursor;
		if ( mMenuCursor < 0 ){ //
 			mMenuCursor = 3;
		}
	}else if ( f.isKeyTriggered( 'z' ) ){
		++mMenuCursor;
		if ( mMenuCursor > 3 ){ //3越过0就循环成0
			mMenuCursor = 0;
		}
	}else if ( f.isKeyTriggered( ' ' ) ){
		switch ( mMenuCursor ){
			case 0: //重来
				parent->state()->reset();
				parent->moveTo( Parent::SEQ_PLAY );
				break;
			case 1: //
				parent->moveTo( Parent::SEQ_STAGE_SELECT );
				break;
			case 2: //到主题
				parent->moveTo( Parent::SEQ_TITLE );
				break;
			case 3: //照原样返回
				parent->moveTo( Parent::SEQ_PLAY );
				break;
			default: //忽略其他
				break;
		}
	}
	//绘制
	//首先是游戏画面
	parent->state()->draw();
	//
	mImage->draw();
	//绘制文字
	f.drawDebugString( 0, 0, "[menu]" );
	f.drawDebugString( 1, 1, "one again" );
	f.drawDebugString( 1, 2, "select level" );
	f.drawDebugString( 1, 3, "return title" );
	f.drawDebugString( 1, 4, "return game" );
	//写光标
	f.drawDebugString( 0, mMenuCursor + 1, ">" );
}



} //namespace Game
} //namespace Sequence