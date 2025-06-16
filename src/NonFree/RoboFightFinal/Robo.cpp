#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Functions.h"

#include "Robo.h"
#include "Bullet.h"
#include "Pad.h"
#include <sstream>
using namespace std;
using namespace GameLib;
using namespace GameLib::Scene;

//真的生成文件了吗？
const int Robo::mJumpUpTime = 20; //上升的时间
const int Robo::mJumpStayTime = 60; //从上升到下降的时间
const int Robo::mJumpFallTime = 40; //下降的时间
const int Robo::mMoveAccelEndCount = 30; //从步行开始到加速结束的时间
const float Robo::mMaxMoveSpeed = 0.5; //最大移动速度
const float Robo::mJumpHeight = 20.0; //最大高度
const int Robo::mCameraDelayCount = 10; //跳跃开始后多少帧开始面对敌人
const float Robo::mCameraDistanceZ = 10.0; //从后面几米？
const float Robo::mCameraDistanceY = 4.0; //往下看
const float Robo::mCameraTargetDistanceZ = 20.0; //注视点是几米远？
const float Robo::mTurnSpeed = 1.0; //旋转速度
const int Robo::mMaxHitPoint = 100; //最大生命值
const int Robo::mMaxEnergy = 100; //最高武器点数
const int Robo::mEnergyPerBullet = 27; //武器能耗
const int Robo::mEnergyCharge = 1; //每帧存储的能量
const float Robo::mLockOnAngleIn = 10.0; //锁定角度
const float Robo::mLockOnAngleOut = 30.0; //锁定角度

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mBullets( 0 ),
mBulletNumber( 100 ), //质感
mCameraCount( 0 ),
mCount( 0 ),
mVelocity( 0.0, 0.0, 0.0 ),
mMode( MODE_ON_LAND ),
mHitPoint( mMaxHitPoint ),
mEnergy( mMaxEnergy ),
mLockOn( true ){ //最好一开始就锁定，因为它开始彼此面对。
	mSceneContainer = Container::create( "data/Sample_Robot.x.txt" );
	while ( !mSceneContainer.isReady() ){
		;
	}
	mModel = mSceneContainer.createModel( "batch0_0" );
	mModel.setScale( Vector3( 2.f ) );
	mBullets = new Bullet[ mBulletNumber ];
}

Robo::~Robo(){
	SAFE_DELETE_ARRAY( mBullets );
}

void Robo::setPosition( const Vector3& a ){
	mPosition = a;
}

void Robo::setAngleY( float a ){
	mAngleY = a;
}

const Vector3* Robo::getBulletPosition( int i ) const {
	if ( mBullets[ i ].isEmpty() ){
		return 0;
	}else{
		return mBullets[ i ].position();
	}
}

void Robo::update( Robo* enemy ){
	//死了
	if ( mHitPoint <= 0 ){
		return;
	}
	//AI思维。对于玩家来说，只需输入并返回
	bool iJump;
	bool iFire;
	bool iTurn;
	bool iLeft; 
	bool iRight; 
	bool iUp;
	bool iDown; 
	think( &iJump, &iFire, &iTurn, &iLeft, &iRight, &iUp, &iDown );
	//使用下面给出的输入进行操作
	const Vector3& enemyPos = *enemy->position();
	++mCount;
	//在跳吗
	float t;
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
			t = mJumpHeight / static_cast< float >( mJumpUpTime );
			mVelocity.y = t;
			if ( !iJump ){ //由于没有跳跃输入，因此更改为下降
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
			if ( !iJump ){ //由于没有跳跃输入，因此更改为下降
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
			t = mJumpHeight / static_cast< float >( mJumpFallTime );
			mVelocity.y = -t;
			//在此不进行着陆判断，因为最终是通过碰撞处理来确定的。
			break;
		case MODE_ON_LAND:
			if ( iJump ){
				mMode = MODE_JUMP_UP;
				mCount = 0;
				mCameraCount = 0;

				//转向敌人。
				Vector3 dir;
				dir.setSub( enemyPos, mPosition ); //从自己到敌人
				//Y轴角度为atan2（x，z）。但是，它必须是固定的。
				t = Math::atan2( dir.x, dir.z );
				//180度以上有差的话+-360度反转
				if ( t - mAngleY > 180.f ){
					t -= 360.f;
				}else if ( mAngleY - t > 180.f ){
					t += 360.f;
				}
				mAngleVelocityY = ( t - mAngleY ) / static_cast< float >( mCameraDelayCount );
			}else if ( iTurn ){
				turn( iLeft, iRight ); //代码变长，转入函数
			}else{
				move( iLeft, iRight, iUp, iDown ); //代码变长，转入函数
			}
			mVelocity.y = 0.f;
			break;
	}
	//下面是碰撞处理。
	mPosition += mVelocity;
	if ( mPosition.y < 0.f ){
		mPosition.y = 0.f;
		mMode = MODE_ON_LAND;
	}
	//武器生成
	if ( iFire ){
		//上升或下降时无法攻击
		if ( ( mMode != MODE_JUMP_FALL ) && ( mMode != MODE_JUMP_UP ) ){
			//有足够的能量吗？
			if ( mEnergy >= mEnergyPerBullet ){
				//搜索武器
				for ( int i = 0; i < mBulletNumber; ++i ){
					if ( mBullets[ i ].isEmpty() ){
						const char* name = ( mId == 0 ) ? "bulletB" : "bulletR";
						mBullets[ i ].create(
							mSceneContainer, 
							name, 
							mPosition,
							15.f,
							mAngleY,
							mLockOn );
						mEnergy -= mEnergyPerBullet;
						break;
					}
				}
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
			if ( t.squareLength() < 4.f ){
				enemy->setDamage( 1 ); //1试着减少了一点。
				mBullets[ i ].die(); //子弹将消失。
			}
		}
	}
	//武器能量点数
	mEnergy += mEnergyCharge;
	if ( mEnergy > mMaxEnergy ){
		mEnergy = mMaxEnergy;
	}
	//锁定处理
	//首先，测量角度。
	//测量的角度是多少？用内积计算
	Vector3 toEnemy;
	toEnemy.setSub( enemyPos, mPosition );
	Vector3 myDir( 0.f, 0.f, -1.f );
	Matrix34 m;
	m.setRotationY( mAngleY + 180.f );
	m.mul( &myDir, myDir );
	toEnemy *= 1.f / toEnemy.length(); //长度设为1
	float dotProduct = toEnemy.dot( myDir );
	if ( dotProduct > 1.f ){ //由于误差关系可能超过一。
		dotProduct = 1.f;
	}else if ( dotProduct < -1.f ){
		dotProduct = -1.f;
	}
	//转换为角度后，
	float angle = Math::acos( dotProduct );
	if ( mLockOn ){
		//找出是否被锁定
		if ( angle > mLockOnAngleOut ){
			mLockOn = false;
		}
	}else{
		//检查是否可以输入
		if ( angle < mLockOnAngleIn ){
			mLockOn = true;
		}
	}
	//在模型上设置位置信息
	mModel.setAngle( Vector3( 0.f, mAngleY, 0.f ) );
	mModel.setPosition( mPosition );
}

void Robo::think(
bool* jump,
bool* fire,
bool* turn,
bool* left,
bool* right,
bool* up,
bool* down ) const {
	Pad* pad = Pad::instance();
	if ( mId == 0 ){ //播放器
		*jump = pad->isOn( Pad::JUMP, mId );
		*fire = pad->isTriggered( Pad::FIRE, mId );
		*turn = pad->isOn( Pad::TURN, mId );
		*left = pad->isOn( Pad::LEFT, mId );
		*right = pad->isOn( Pad::RIGHT, mId );
		*up = pad->isOn( Pad::UP, mId );
		*down = pad->isOn( Pad::DOWN, mId );
	}else{ //AI
		//
		//如果未锁定则跳转
		*jump = !mLockOn;
		//尽可能多地发射子弹。
		*fire = true;
		//不要旋转
		*turn = false;
		//
		*left = *right = *up = *down = false;
	}
}

void Robo::turn( bool left, bool right ){
	if ( left ){
		mAngleY += mTurnSpeed;
		if ( mAngleY > 180.f ){ //落在-PI到PI中
			mAngleY -= 360.f;
		}
	}
	if ( right ){
		mAngleY -= mTurnSpeed;
		if ( mAngleY < -180.f ){ //落在-PI到PI中
			mAngleY += 360.0;
		}
	}
}

void Robo::move( bool left, bool right, bool up, bool down ){
	//移动处理。首先，在不考虑视点的情况下进行加速
	Vector3 move( 0.f, 0.f, 0.f );
	if ( up ){
		move.z = -1.f;
	}
	if ( down ){
		move.z = 1.f;
	}
	if ( left ){
		move.x = -1.f;
	}
	if ( right ){
		move.x = 1.f;
	}
	//考虑旋转注视方向
	Matrix34 m;
	m.setRotationY( mAngleY + 180.f );
	m.mul( &move, move );

	//只要适当地加速它
	if ( mVelocity.x == 0.f && mVelocity.z == 0.f ){
		//将最大速度除以加速所需的时间，即可得出每帧的加速度。
		float accel = mMaxMoveSpeed / static_cast< float >( mMoveAccelEndCount );
		mVelocity.setMul( move, accel );
	}else{ //如果已经在运行
		if ( move.x == 0.f && move.z == 0.f ){ //移动量为0
			mVelocity.set( 0.f, 0.f, 0.f ); //停止移动。
		}else{
			//如果已经在运行就比较麻烦
			//45只有一次转换方向的时候从零开始加速是压力。
			//因此，“和目前速度方向不符的分量都将清零”。

			//如果转弯为90度或以上暂时将速度降低为0。
			//在某些游戏中，最好使用惯性，但是如果想快速移动，惯性反而麻烦。
			//如果转弯为90度或以上，则当前速度和加速度的内积应为负
			float dp = mVelocity.dot( move );
			if ( dp <= 0.f ){
				mVelocity.set( 0.f, 0.f, 0.f );
			}else{ //小于90度
				//以当前移动速度仅提取水平分量
				//可以通过将移动方向单位向量的内积与移动方向单位向量相加来获得水平分量。
				//
				//V' = dot(V,E) * E
				//可以使用运动矢量M将E表示为E = M / | M |，
				//V' = dot(V,M) * M / ( |M|^2 )
				//编写和创建单位向量时，消除平方根。| M | ^ 2比| M |更快。
				float moveSquareLength = move.x * move.x + move.z * move.z;
				float dp = mVelocity.dot( move );
				mVelocity.setMul( move, dp / moveSquareLength );
			}
			//增加加速度。
			//移动速度等于最大速度/加速时间。
			float accel = mMaxMoveSpeed / static_cast< float >( mMoveAccelEndCount );
			mVelocity.madd( move, accel );
			//以最大速度停止
			float speed = mVelocity.length();
			if ( speed > mMaxMoveSpeed ){
				mVelocity *= mMaxMoveSpeed / speed;
			}
		}
	}
}

void Robo::draw() const {
	//绘制
	mModel.draw();
	//武器装备
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].draw();
		}
	}
}

const Vector3* Robo::position() const {
	return &mPosition;
}

void Robo::setDamage( int d ){
	mHitPoint -= d;
}

int Robo::getHitPoint() const {
	return mHitPoint;
}

int Robo::getEnergy() const {
	return mEnergy;
}

bool Robo::getLockOn() const {
	return mLockOn;
}

void Robo::getView( Matrix34* vm, Vector3* pos ) const {
	//首先创建正面方向矢量
	Vector3 d( 0.f, 0.f, 1.f );
	Matrix34 m;
	m.setRotationY( mAngleY );
	m.mul( &d, d );
	//向前延伸mCameraTargetDistanceZ
	Vector3 t;
	t.setMul( d, mCameraTargetDistanceZ );
	//如果机器人高，会往下看一点。
	t.y -= mPosition.y * 0.12f; //这种调整也是合适的
	//向后延伸mCameraDistacneZ
	Vector3 p;
	p.setMul( d, -mCameraDistanceZ );
	//将mCameraDistanceY加到Y
	p.y += mCameraDistanceY;
	//如果机器人在高处，将其抬高一点，然后往下看。
	p.y += mPosition.y * 0.12f; //这种调整也是合适的
	//增加机器人当前位置
	t += mPosition;
	p += mPosition;
	//创建视图矩阵
	vm->setViewTransform( p, t, Vector3( 0.f, 1.f, 0.f ) );
	*pos = p;
}
