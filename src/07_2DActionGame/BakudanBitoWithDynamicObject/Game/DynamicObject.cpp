#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"

#include "Game/DynamicObject.h"
#include "Image.h"

using namespace GameLib;

namespace{

//参数组
//速度 单位是内部单位/帧
const int PLAYER_SPEED = 1000;
const int ENEMY_SPEED = 500;

} //namespace 

//尽可能输入错误的值。如果不调用set就会中断
DynamicObject::DynamicObject() : 
mType( TYPE_NONE ),
mX( 0xffffffff ), 
mY( 0xffffffff ){
}

void DynamicObject::set( int x, int y, Type type ){
	//转换为内部坐标
	mX = x * 16000 + 8000;
	mY = y * 16000 + 8000;
	mType = type;
	//仅限敌人。移动方向初始化
	if ( mType == TYPE_ENEMY ){
		mDirectionX = mDirectionY = 0;
		Framework f = Framework::instance();
		switch ( f.getRandom( 4 ) ){
			case 0 : mDirectionX = 1; break;
			case 1 : mDirectionX = -1; break;
			case 2 : mDirectionY = 1; break;
			case 3 : mDirectionY = -1; break;
		}
	}
}

void DynamicObject::draw( const Image* image ) const {
	//将内部坐标转换为像素坐标（+500四舍五入）
	int dstX = ( mX - 8000 + 500 ) / 1000;
	int dstY = ( mY - 8000 + 500 ) / 1000;
	//
	int srcX, srcY;
	srcX = srcY = -1;
	switch ( mType ){
		case TYPE_1P: srcX = 0; srcY = 0; break;
		case TYPE_2P: srcX = 16; srcY = 0; break;
		case TYPE_ENEMY: srcX = 32; srcY = 16; break;
		default: HALT( "arienai" ); break;
	}
	image->draw( dstX, dstY, srcX, srcY, 16, 16 );
}

void DynamicObject::update(){
	Framework f = Framework::instance();
	//敌方行为
	if ( mType == TYPE_ENEMY ){
		//
		//mDirectionX没有单位。dt是毫秒。ENEMY_SPEED是像素/秒。
		//目前，毫秒*像素/秒
		//分母乘以1000会消除分母和分子（以毫秒为单位）。
		//要将像素更改为内部坐标，需要乘以1000。1000由分子和分母偏移。
		//根据以上所述，如下。
		mX += mDirectionX * ENEMY_SPEED;
		mY += mDirectionY * ENEMY_SPEED;
	}else if ( mType == TYPE_1P ){ //玩家行为
		int dx, dy;
		dx = dy = 0;
		if ( f.isKeyOn( 'w' ) ){
			dy = -1;
		}else if ( f.isKeyOn( 'z' ) ){
			dy = 1;
		}else if ( f.isKeyOn( 'a' ) ){
			dx = -1;
		}else if ( f.isKeyOn( 's' ) ){
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}else if ( mType == TYPE_2P ){ //玩家行为
		int dx, dy;
		dx = dy = 0;
		if ( f.isKeyOn( 'i' ) ){
			dy = -1;
		}else if ( f.isKeyOn( 'm' ) ){
			dy = 1;
		}else if ( f.isKeyOn( 'j' ) ){
			dx = -1;
		}else if ( f.isKeyOn( 'k' ) ){
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}
	//边界处理
	const int X_MIN = 8000;
	const int X_MAX = 320 * 1000 - 8000;
	const int Y_MIN = 8000;
	const int Y_MAX = 240 * 1000 - 8000;
	bool hit = false;
	if ( mX < X_MIN ){
		mX = X_MIN;
		hit = true;
	}else if ( mX > X_MAX ){
		mX = X_MAX;
		hit = true;
	}
	if ( mY < Y_MIN ){
		mY = Y_MIN;
		hit = true;
	}else if ( mY > Y_MAX ){
		mY = Y_MAX;
		hit = true;
	}
	//如果敌人转身
	if ( hit && mType == TYPE_ENEMY ){
		mDirectionX = mDirectionY = 0;
		switch ( f.getRandom( 4 ) ){
			case 0 : mDirectionX = 1; break;
			case 1 : mDirectionX = -1; break;
			case 2 : mDirectionY = 1; break;
			case 3 : mDirectionY = -1; break;
		}
	}
}