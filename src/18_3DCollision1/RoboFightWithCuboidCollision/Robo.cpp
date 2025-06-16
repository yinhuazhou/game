#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"

#include "Robo.h"
#include "Pad.h"
#include "Library/Matrix34.h"
#include "Library/Matrix44.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "GameLib/Math.h"
using namespace GameLib;

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mModel( 0 ){
	mDatabase = new GraphicsDatabase( "robo.txt" );
	mModel = mDatabase->createModel( "robo" );

	Vector3 c = mPosition;
	c.y += 1.0; //中心往上移
	mCuboid.setPosition( c );
	mCuboid.setHalfSize( Vector3( 0.5, 1.0, 0.5 ) );
}

Robo::~Robo(){
	SAFE_DELETE( mModel ); //
	SAFE_DELETE( mDatabase );
}

const Vector3* Robo::position() const {
	return &mPosition;
}

void Robo::setPosition( const Vector3& a ){
	mPosition = a;
	Vector3 c = mPosition;
	c.y += 1.0; //中心往上移
	mCuboid.setPosition( c );
}

double Robo::angleY() const {
	return mAngleY;
}

void Robo::setAngleY( double a ){
	mAngleY = a;
}

void Robo::getMove( 
Vector3* move,
const Vector3& viewVector ) const {
	//移动
	move->set( 0.0, 0.0, 0.0 );
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::UP, mId ) ){
		move->z = -0.2;
	}
	if ( pad->isOn( Pad::DOWN, mId ) ){
		move->z = 0.2;
	}
	if ( pad->isOn( Pad::LEFT, mId ) ){
		move->x = -0.2;
	}
	if ( pad->isOn( Pad::RIGHT, mId ) ){
		move->x = 0.2;
	}
	double ay = atan2( viewVector.x, viewVector.z ) + 180.0;
	Matrix34 m;
	m.setRotationY( ay );
	m.multiply( move, *move );

	//跳！
	if ( pad->isOn( Pad::JUMP, mId ) ){
		move->y += 0.1;
	}else{
		move->y -= 0.1;
	}
}

void Robo::update( 
const Vector3& move,
const Vector3& enemyPos ){
	mPosition += move;
	Vector3 c = mPosition;
	c.y += 1.0; //中心往上移
	mCuboid.setPosition( c );
	//跳跃。
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::JUMP, mId ) ){
		//转向敌人。
		Vector3 dir;
		dir.setSub( enemyPos, mPosition ); //从自己到敌人
		//Y轴角度为atan2（x，z）。
		mAngleY = atan2( dir.x, dir.z );
	}
}

void Robo::getDirection( Vector3* v ) const {
	Matrix34 wm;
	wm.setRotationY( mAngleY );
	wm.multiply( v, Vector3( 0.0, 0.0, 1.0 ) );
}

void Robo::draw( const Matrix44& pvm ) const {
	//在模型上设置位置信息
	mModel->setAngle( Vector3( 0.0, mAngleY, 0.0 ) );
	mModel->setPosition( mPosition );
	//绘制
	mModel->draw( pvm );
}

const Cuboid* Robo::cuboid() const {
	return &mCuboid;
}