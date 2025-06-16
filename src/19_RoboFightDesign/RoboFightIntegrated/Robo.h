#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "Library/Vector3.h"

class Matrix34;
class Matrix44;
class GraphicsDatabase;
class Model;
class Bullet;

class Robo{
public:
	Robo( int id ); //
	~Robo();
	void draw( const Matrix44& perspectiveViewMatrix ) const;
	void update( Robo* robo );
	void setPosition( const Vector3& );
	void setAngleY( double );
	const Vector3* position() const;
	void getViewMatrix( Matrix34* ) const;
	void setDamage( int damage );
	int hitPoint() const;
private:
	void move();
	void turn();

	Vector3 mPosition;
	double mAngleY;
	int mId;
	GraphicsDatabase* mDatabase;
	Model* mModel;
	
	Bullet* mBullets;
	int mBulletNumber;

	int mCameraCount;
	int mCount; //自开始移动以来已经经过了几帧？
	Vector3 mVelocity; //当前平面速度
	double mAngleVelocityY; //转弯速度

	static const int mJumpUpTime; //上升的时间
	static const int mJumpStayTime; //从上升到下降的时间
	static const int mJumpFallTime; //下降的时间
	static const int mMoveAccelEndCount; //从步行开始到加速结束的时间
	static const double mMaxMoveSpeed; //最大移动速度
	static const double mJumpHeight; //最大高度
	static const int mCameraDelayCount; //跳跃开始后多少帧开始面对敌人
	static const double mCameraDistanceZ; //从后面几米？
	static const double mCameraDistanceY; //往下看
	static const double mCameraTargetDistanceZ; //注视点是几米远？
	static const double mTurnSpeed; //旋转速度
	static const int mMaxHitPoint; //最大生命值

	enum Mode{
		MODE_JUMP_UP, //跳起来
		MODE_JUMP_STAY, //跳起来停在空中
		MODE_JUMP_FALL, //跳下时
		MODE_ON_LAND, //着陆
	};
	Mode mMode;

	int mHitPoint; //体力
};

#endif
