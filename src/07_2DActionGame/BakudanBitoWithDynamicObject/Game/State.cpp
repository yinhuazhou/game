#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"

#include "Image.h"

using namespace GameLib;

namespace{
//地图尺寸
const int WIDTH = 19;
const int HEIGHT = 15;

//合适的舞台数据
struct StageData{
	int mEnemyNumber; //敌人数
	int mBrickRate; //砖比率（百分比）
	int mItemPowerNumber; //冲击波道具数量
	int mItemBombNumber; //炸弹数量
};

StageData gStageData[] = {
	{ 2, 90, 4, 6, },
	{ 3, 80, 1, 0, },
	{ 6, 30, 0, 1, },
};

} //namespace 

State::State( int stageID ) : 
mImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //再用几次
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.dds" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //网格数

	//记录砖块。
	unsigned* brickList = new unsigned[ n ];
	int brickNumber = 0; //计算真正的砖块数量
	unsigned* floorList = new unsigned[ n ];
	int floorNumber = 0;

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( x == 0 || y == 0 || ( x == WIDTH-1 ) || ( y == HEIGHT-1 ) ){
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( ( x % 2 == 0 ) && ( y % 2 == 0 ) ){ //具体的
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( y + x < 4 ){
				//前三个网格是地面
			}else if ( ( stageID == 0 ) && ( y + x > ( WIDTH + HEIGHT - 6 ) ) ){
				; //如果有两个人，请在右下角留三个网格

			}else{ //其余的是砖或地板。随机决定
				if ( f.getRandom( 100 ) < stageData.mBrickRate  ){
					o.setFlag( StaticObject::FLAG_BRICK );
					//如果是砖头，请记录下来。
					brickList[ brickNumber ] = ( x << 16 ) + y;
					++brickNumber;
				}else{
					floorList[ floorNumber ] = ( x << 16 ) + y;
					++floorNumber;
				}
			}
		}
	}
	//将道具放在砖中
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;
	//这样做的方法是将砖列表中的第i个项目替换为合适的项目，然后将其放置在此处。
	for ( int i = 0; i < powerNumber + bombNumber; ++i ){
 		int swapped = f.getRandom( brickNumber - 1 - i ) + i; //
		unsigned t = brickList[ i ];
		brickList[ i ] = brickList[ swapped ];
		brickList[ swapped ] = t;

		int x = brickList[ i ] >> 16;
		int y = brickList[ i ] & 0xffff;
		StaticObject& o = mStaticObjects( x, y );
		if ( i < powerNumber ){
			o.setFlag( StaticObject::FLAG_ITEM_POWER );
		}else{
			o.setFlag( StaticObject::FLAG_ITEM_BOMB );
		}
	}
	SAFE_DELETE_ARRAY( brickList );

	//分配动态对象
	int playerNumber = ( mStageID == 0 ) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//玩家位置
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_1P );
	if ( mStageID == 0 ){
		mDynamicObjects[ 1 ].set( WIDTH-2, HEIGHT-2, DynamicObject::TYPE_2P );
	}
	//将敌人放到地板上。方式与放置道具几乎相同
	for ( int i = 0; i < enemyNumber; ++i ){
 		int swapped = f.getRandom( floorNumber - 1 - i ) + i;
		unsigned t = floorList[ i ];
		floorList[ i ] = floorList[ swapped ];
		floorList[ swapped ] = t;

		int x = floorList[ i ] >> 16;
		int y = floorList[ i ] & 0xffff;
		mDynamicObjects[ playerNumber + i ].set( x, y, DynamicObject::TYPE_ENEMY );
	}
	SAFE_DELETE_ARRAY( floorList );
}

State::~State(){
	SAFE_DELETE( mImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {
	//绘制背景
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//绘制前景
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//绘制冲击波
	//TODO:
}

void State::update(){
	//对其进行移动处理
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].update();
	}
}

bool State::hasCleared() const {
	//TODO:
	return false;
}

bool State::isAlive1P() const {
	//TODO:
	return true;
}

bool State::isAlive2P() const {
	//TODO:
	return true;
}

