#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"
#include "Robo.h"
#include "Bullet.h"
#include "Pad.h"
#include "Library/Matrix34.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
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
const int Robo::mMaxEnergy = 100; //最高武器点数
const int Robo::mEnergyPerBullet = 27; //武器能耗
const int Robo::mEnergyCharge = 1; //每帧存储的能量
const double Robo::mLockOnAngleIn = 10.0; //锁定角度
const double Robo::mLockOnAngleOut = 30.0; //锁定角度

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
mHitPoint( mMaxHitPoint ),
mEnergy( mMaxEnergy ),
mLockOn( true ){ //最好一开始就锁定，因为它开始彼此面对。
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
	//角度校正
	if ( mAngleY > 180.0 ){
		mAngleY -= 360.0;
	}else if ( mAngleY < -180.0 ){
		mAngleY += 360.0;
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
			t = mJumpHeight / static_cast< double >( mJumpFallTime );
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
				//Y轴角度为atan2（x，z）。
				t = GameLib::atan2( dir.x, dir.z );
				//180度以上有差的话+-360度反转
				if ( t - mAngleY > 180.0 ){
					t -= 360.0;
				}else if ( mAngleY - t > 180.0 ){
					t += 360.0;
				}
				mAngleVelocityY = ( t - mAngleY ) / static_cast< double >( mCameraDelayCount );
			}else if ( iTurn ){
				turn( iLeft, iRight ); //代码变长，转入函数
			}else{
				move( iLeft, iRight, iUp, iDown ); //代码变长，转入函数
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
	if ( iFire ){
		//上升或下降时无法攻击
		if ( ( mMode != MODE_JUMP_FALL ) && ( mMode != MODE_JUMP_UP ) ){
			//有足够的能量吗？
			if ( mEnergy >= mEnergyPerBullet ){
				//搜索武器
				for ( int i = 0; i < mBulletNumber; ++i ){
					if ( mBullets[ i ].isEmpty() ){
						Vector3 c = ( mId == 0 ) ? Vector3( 0.0, 0.35, 0.7 ) : Vector3( 0.7, 0.35, 0.0 );
						mBullets[ i ].create(
							mDatabase, 
							"bullet", 
							mPosition,
							15.0,
							mAngleY,
							mLockOn,
							c );
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
			if ( t.squareLength() < 4.0 ){
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
	Vector3 myDir( 0.0, 0.0, -1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY + 180.0 );
	m.multiply( &myDir, myDir );
	toEnemy *= 1.0 / toEnemy.length(); //长度设为1
	double dotProduct = toEnemy.dot( myDir );
	//转换为角度后，
	double angle = GameLib::acos( dotProduct );
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
		if ( mAngleY > 180.0 ){ //落在-PI到PI中
			mAngleY -= 360.0;
		}
	}
	if ( right ){
		mAngleY -= mTurnSpeed;
		if ( mAngleY < -180.0 ){ //落在-PI到PI中
			mAngleY += 360.0;
		}
	}
}

void Robo::move( bool left, bool right, bool up, bool down ){
	//移动处理。首先，在不考虑视点的情况下进行加速
	Vector3 move( 0.0, 0.0, 0.0 );
	if ( up ){
		move.z = -1.0;
	}
	if ( down ){
		move.z = 1.0;
	}
	if ( left ){
		move.x = -1.0;
	}
	if ( right ){
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

void Robo::draw( 
const Matrix44& pvm,
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient ) const {
	//在模型上设置位置信息
	mModel->setAngle( Vector3( 0.0, mAngleY, 0.0 ) );
	mModel->setPosition( mPosition );
	//绘制
	mModel->draw( pvm, lightVector, lightColor, ambient );
	//武器装备
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].draw( pvm, lightVector, lightColor, ambient );
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

int Robo::energy() const {
	return mEnergy;
}

bool Robo::isLockOn() const {
	return mLockOn;
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
