#include "GameLib/GameLib.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Functions.h"
using namespace GameLib;

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
using namespace std;

#include "Bullet.h"

//参数
const int Bullet::mLife = 120;
const float Bullet::mHomingX = 1.0;
const float Bullet::mHomingY = 1.0;
const float Bullet::mSpeed = 1.0;

Bullet::Bullet() : mCount( -1 ){
}

Bullet::~Bullet(){
}

bool Bullet::isEmpty() const {
	return ( mCount < 0 );
}

void Bullet::die(){
	mCount = -1;
}

void Bullet::create(
AutoPtr< GraphicsDatabase >& db,
const char* batchName,
const Vector3& p,
float angleX,
float angleY,
bool homing ){
	//如果有模型，请将其删除
	if ( mModel ){
		mModel = 0; //代入0手动删除。
	}
	mModel = db->createModel( batchName );
	mModel->setPosition( p );
	mModel->setAngle( Vector3( angleX, angleY, 0.f ) );
	mHoming = homing;
	mCount = 0;
}

const Vector3* Bullet::getPosition() const {
	return mModel->getPosition();
}

void Bullet::update( const Vector3& enemyPos ){
	//转向敌人。
	Vector3 dir;
	Vector3 p = *mModel->getPosition();
	Vector3 a = *mModel->getAngle();
	if ( mHoming ){
		dir.setSub( enemyPos, *mModel->getPosition() ); //从自己到敌人
		//Y轴角度为atan2（x，z）。
		float ty = atan2( dir.x, dir.z );
		//180度以上有差的话+-360度反转
		if ( ty - a.y > 180.f ){
			ty -= 2.f * 180.f;
		}else if ( a.y - ty > 180.f ){
			ty += 2.f * 180.f;
		}
		//X轴角度为Y /（X，Z）。
		float zxLength = sqrt( dir.x * dir.x + dir.z * dir.z );
		float tx = atan2( dir.y, zxLength );
		//X轴角度的范围是（-90,90），并且偏差不超过180度。
		float hx = mHomingX;
		float hy = mHomingY; 
		//在归位范围内
		if ( tx - a.x < hx && a.x - tx < hx ){
			a.x = tx;
		}else if ( tx < a.x ){
			a.x -= hx;
		}else{
			a.x += hx;
		}
		if ( ty - a.y < hy && a.y - ty < hy ){
			a.y = ty;
		}else if ( ty < a.y ){
			a.y -= hy;
		}else{
			a.y += hy;
		}
	}
	//很有意思，它是z旋转
	a.z += 20.0;
	//角度更新
	mModel->setAngle( a );
	//使用该方向的旋转矩阵将位置转换为（0,0,1）并相加
	Vector3 v( 0.0, 0.0, mSpeed );
	Matrix34 m;
	m.setRotationY( a.y );
	m.rotateX( -a.x );
	m.mul( &v, v );
	p += v;
	mModel->setPosition( p );
	++mCount;
	if ( mCount >= mLife ){
		mCount = -1;
	}
}

void Bullet::draw() const {
	mModel->draw();
}
