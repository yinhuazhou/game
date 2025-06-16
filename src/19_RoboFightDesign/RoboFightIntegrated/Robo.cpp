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

//真的生成文件了吗？
const int Robo::mJumpUpTime = 20; //上升的时间
const int Robo::mJumpStayTime = 60; //从上升到下降的时间
const int Robo::mJumpFallTime = 40; //下降的时间
const int Robo::mMoveAccelEndCount = 30; //从步行开始到加速结束的时间
const double Robo::mMaxMoveSpeed = 0.5; //最大移动速度
const double Robo::mJumpHeight = 20.0; //最大高度
const int Robo::mCameraDelayCount = 10; //跳跃开始后多少帧开始面对敌人
const double Robo::mCameraDistanceZ = 10.0; //从后面几米？
const double Robo::mCameraDistanceY = 4.0; //往下看
const double Robo::mCameraTargetDistanceZ = 20.0; //注视点是几米远？
const double Robo::mTurnSpeed = 1.0; //旋转速度
const int Robo::mMaxHitPoint = 100; //最大生命值

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mModel( 0 ),
mBullets( 0 ),
mBulletNumber( 100 ), //质感
mCameraCount( 0 ),
mCount( 0 ),
mVelocity( 0.0, 0.0, 0.0 ),
mMode( MODE_ON_LAND ),
mHitPoint( mMaxHitPoint ){
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

void Robo::update( Robo* enemy ){
	//死了
	if ( mHitPoint <= 0 ){
		return;
	}
	//角度范围校正
	if ( mAngleY > 180.0 ){
		mAngleY -= 360.0;
	}else if ( mAngleY < -180.0 ){
		mAngleY += 360.0;
	}

	const Vector3& enemyPos = *enemy->position();
	++mCount;
	//在跳吗
	Pad* pad = Pad::instance();
	bool inputJump = pad->isOn( Pad::JUMP, mId );
	double t;
	//写一些类似于词法分析的东西。代码重复增加，但是以块为单位查看时，它变得更简单。
	//将其与普通写法进行比较。
	switch ( mMode ){
		case MODE_JUMP_UP:
			//如果相机未完全旋转，请继续旋转相机
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			//上升
			t = mJumpHeight / static_cast< double >( mJumpUpTime );
			mVelocity.y = t;
			if ( !inputJump ){ //由于没有跳跃输入，因此更改为下降
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}else if ( mCount >= mJumpUpTime ){ //上升结束
				mMode = MODE_JUMP_STAY; 
				mCount = 0;
			}
			mVelocity.x = mVelocity.z = 0.0; //取消X，Z移动
			break;
		case MODE_JUMP_STAY:
			//如果相机未完全旋转，请继续旋转相机
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			mVelocity.y = 0.0;
			if ( !inputJump ){ //由于没有跳跃输入，因此更改为下降
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}else if ( mCount >= mJumpStayTime ){ //往下
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}
			break;
		case MODE_JUMP_FALL:
			//如果相机未完全旋转，请继续旋转相机
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			//下降
			t = mJumpHeight / static_cast< double >( mJumpFallTime );
			mVelocity.y = -t;
			//在此不进行着陆判断，因为最终是通过碰撞处理来确定的。
			break;
		case MODE_ON_LAND:
			if ( inputJump ){
				mMode = MODE_JUMP_UP;
				mCount = 0;
				mCameraCount = 0;

				//转向敌人。
				Vector3 dir;
				dir.setSub( enemyPos, mPosition ); //从自己到敌人
				//Y轴角度为atan2（x，z）
				t = GameLib::atan2( dir.x, dir.z );
				//180度以上有差的话+-360度反转
				if ( t - mAngleY > 180.0 ){
					t -= 360.0;
				}else if ( mAngleY - t > 180.0 ){
					t += 360.0;
				}
				mAngleVelocityY = ( t - mAngleY ) / static_cast< double >( mCameraDelayCount );
			}else if ( pad->isOn( Pad::TURN, mId ) ){
				turn(); //代码变长，转入函数
			}else{
				move(); //代码变长，转入函数
			}
			mVelocity.y = 0.0;
			break;
	}
	//下面是碰撞处理。
	mPosition += mVelocity;
	if ( mPosition.y < 0.0 ){
		mPosition.y = 0.0;
		mMode = MODE_ON_LAND;
	}
	//武器生成
	if ( pad->isTriggered( Pad::FIRE, mId ) ){
		//搜索武器
		for ( int i = 0; i < mBulletNumber; ++i ){
			if ( mBullets[ i ].isEmpty() ){
				mBullets[ i ].create(
					mDatabase, 
					"bullet", 
					mPosition,
					15.0,
					mAngleY );
				break;
			}
		}
	}
	//武器更新
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].update( enemyPos );
			//碰撞处理
			Vector3 t;
			t.setSub( *mBullets[ i ].position(), enemyPos );
			if ( t.squareLength() < 4.0 ){
				enemy->setDamage( 1 ); //1试着减少了一点。
				mBullets[ i ].die(); //子弹将消失。
			}
		}
	}
}

void Robo::turn(){
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::LEFT, mId ) ){
		mAngleY += mTurnSpeed;
		if ( mAngleY > 180.0 ){ //落在-PI到PI中
			mAngleY -= 360.0;
		}
	}
	if ( pad->isOn( Pad::RIGHT, mId ) ){
		mAngleY -= mTurnSpeed;
		if ( mAngleY < -180.0 ){ //落在-PI到PI中
			mAngleY += 360.0;
		}
	}
}

void Robo::move(){
	//移动处理。首先，在不考虑视点的情况下进行加速
	Vector3 move( 0.0, 0.0, 0.0 );
	Pad* pad = Pad::instance();
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

	//只要适当地加速它
	if ( mVelocity.x == 0.0 && mVelocity.z == 0.0 ){
		//将最大速度除以加速所需的时间，即可得出每帧的加速度。
		double accel = mMaxMoveSpeed / static_cast< double >( mMoveAccelEndCount );
		mVelocity.setMul( move, accel );
	}else{ //如果已经在运行
		if ( move.x == 0.0 && move.z == 0.0 ){ //移动量为0
			mVelocity.set( 0.0, 0.0, 0.0 ); //停止移动。
		}else{
			//如果已经在运行就比较麻烦
			//45只有一次转换方向的时候从零开始加速是压力。
			//因此，“和目前速度方向不符的分量都将清零”。

			//如果转弯为90度或以上暂时将速度降低为0。
			//在某些游戏中，最好使用惯性，但是如果想快速移动，惯性反而麻烦。
			//如果转弯为90度或以上，则当前速度和加速度的内积应为负
			double dp = mVelocity.dot( move );
			if ( dp <= 0.0 ){
				mVelocity.set( 0.0, 0.0, 0.0 );
			}else{ //小于90度
				//以当前移动速度仅提取水平分量
				//可以通过将移动方向单位向量的内积与移动方向单位向量相加来获得水平分量。
				//
				//V' = dot(V,E) * E
				//可以使用运动矢量M将E表示为E = M / | M |，
				//V' = dot(V,M) * M / ( |M|^2 )
				//编写和创建单位向量时，消除平方根。| M | ^ 2比| M |更快。
				double moveSquareLength = move.x * move.x + move.z * move.z;
				double dp = mVelocity.dot( move );
				mVelocity.setMul( move, dp / moveSquareLength );
			}
			//增加加速度。
			//移动速度等于最大速度/加速时间。
			double accel = mMaxMoveSpeed / static_cast< double >( mMoveAccelEndCount );
			mVelocity.madd( move, accel );
			//以最大速度停止
			double speed = mVelocity.length();
			if ( speed > mMaxMoveSpeed ){
				mVelocity *= mMaxMoveSpeed / speed;
			}
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

void Robo::setDamage( int d ){
	mHitPoint -= d;
}

int Robo::hitPoint() const {
	return mHitPoint;
}

void Robo::getViewMatrix( Matrix34* vm ) const {
	//首先创建正面方向矢量
	Vector3 d( 0.0, 0.0, 1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY );
	m.multiply( &d, d );
	//向前延伸mCameraTargetDistanceZ
	Vector3 t;
	t.setMul( d, mCameraTargetDistanceZ );
	//如果机器人高，会往下看一点。
	t.y -= mPosition.y * 0.12; //这种调整也是合适的
	//向后延伸mCameraDistacneZ
	Vector3 p;
	p.setMul( d, -mCameraDistanceZ );
	//将mCameraDistanceY加到Y
	p.y += mCameraDistanceY;
	//如果机器人在高处，将其抬高一点，然后往下看。
	p.y += mPosition.y * 0.12; //这种调整也是合适的
	//增加机器人当前位置
	t += mPosition;
	p += mPosition;
	//创建视图矩阵
	vm->setViewTransform( p, t );
}