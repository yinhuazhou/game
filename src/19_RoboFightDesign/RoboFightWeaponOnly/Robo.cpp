#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"

#include "Robo.h"
#include "Bullet.h"
#include "Pad.h"
#include "Library/Matrix34.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "GameLib/Math.h"
using namespace GameLib;

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mModel( 0 ),
mBullets( 0 ),
mBulletNumber( 100 ){ //质感
	mDatabase = new GraphicsDatabase( "robo.txt" );
	mModel = mDatabase->createModel( "robo" );
	mBullets = new Bullet[ mBulletNumber ];
}

Robo::~Robo(){
	SAFE_DELETE_ARRAY( mBullets );
	SAFE_DELETE( mModel ); //
	SAFE_DELETE( mDatabase );
}

void Robo::setPosition( const Vector3& a ){
	mPosition = a;
}

void Robo::setAngleY( double a ){
	mAngleY = a;
}

void Robo::update( const Vector3& enemyPos ){
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::TURN, mId ) ){
		if ( pad->isOn( Pad::LEFT, mId ) ){
			mAngleY += 1.0;
			if ( mAngleY > 180.0 ){ //落在-PI到PI中
				mAngleY -= 360.0;
			}
		}
		if ( pad->isOn( Pad::RIGHT, mId ) ){
			mAngleY -= 1.0;
			if ( mAngleY < -180.0 ){ //落在-PI到PI中
				mAngleY += 360.0;
			}
		}
	}else{
		//移动处理。首先，在不考虑视点的情况下进行加速
		Vector3 move( 0.0, 0.0, 0.0 );
		if ( pad->isOn( Pad::UP, mId ) ){
			move.z = -1.0;
		}
		if ( pad->isOn( Pad::DOWN, mId ) ){
			move.z = 1.0;
		}
		if ( pad->isOn( Pad::LEFT, mId ) ){
			move.x = -1.0;
		}
		if ( pad->isOn( Pad::RIGHT, mId ) ){
			move.x = 1.0;
		}
		//考虑旋转注视方向
		Matrix34 m;
		m.setRotationY( mAngleY + 180.0 );
		m.multiply( &move, move );
		mPosition += move;
	}

	//武器生成
	if ( pad->isOn( Pad::FIRE, mId ) ){
		//搜索武器
		for ( int i = 0; i < mBulletNumber; ++i ){
			if ( mBullets[ i ].isEmpty() ){
				mBullets[ i ].create(
					mDatabase, 
					"bullet", 
					mPosition,
					45.0,
					mAngleY );
				break;
			}
		}
	}
	//武器更新
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].update( enemyPos );
		}
	}
}

void Robo::draw( const Matrix44& pvm ) const {
	//在模型上设置位置信息
	mModel->setAngle( Vector3( 0.0, mAngleY, 0.0 ) );
	mModel->setPosition( mPosition );
	//绘制
	mModel->draw( pvm );
	//武器装备
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].draw( pvm );
		}
	}
}

const Vector3* Robo::position() const {
	return &mPosition;
}

void Robo::getViewMatrix( Matrix34* vm ) const {
	//首先创建正面方向矢量
	Vector3 d( 0.0, 0.0, 1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY );
	m.multiply( &d, d );
	//向前延伸mCameraTargetDistanceZ
	Vector3 t;
	t.setMul( d, 20.0 );
	//如果机器人高，会往下看一点。
	t.y -= mPosition.y * 0.12; //这种调整也是合适的
	//向后延伸mCameraDistacneZ
	Vector3 p;
	p.setMul( d, -20.0 );
	//将mCameraDistanceY加到Y
	p.y += 20.0;
	//如果机器人在高处，将其抬高一点，然后往下看。
	p.y += mPosition.y * 0.12; //这种调整也是合适的
	//增加机器人当前位置
	t += mPosition;
	p += mPosition;
	//创建视图矩阵
	vm->setViewTransform( p, t );
}