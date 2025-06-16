#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Model.h"
#include "GameLib/Math/Vector3.h"
namespace GameLib{
	namespace Math{
		class Matrix34;
		class Matrix44;
	}
}
using namespace GameLib::Math;

class Bullet;

class Robo{
public:
	Robo( int id ); //设置编号。程序运行时需要知道自己的编号
	~Robo();
	void draw() const;
	void update( Robo* robo );
	void setPosition( const Vector3& );
	void setAngleY( float );
	const Vector3* position() const;
	void getView( Matrix34* matrix, Vector3* position ) const;
	void setDamage( int damage );
	int getHitPoint() const;
	int getEnergy() const;
	bool getLockOn() const;
	const Vector3* getBulletPosition( int i ) const;

	static const int mJumpUpTime; //上升的时间
	static const int mJumpStayTime; //从上升到下降的时间
	static const int mJumpFallTime; //下降的时间
	static const int mMoveAccelEndCount; //从步行开始到加速结束的时间
	static const float mMaxMoveSpeed; //最大移动速度
	static const float mJumpHeight; //最大高度
	static const int mCameraDelayCount; //跳跃开始后多少帧开始面对敌人
	static const float mCameraDistanceZ; //从后面几米？
	static const float mCameraDistanceY; //往下看
	static const float mCameraTargetDistanceZ; //注视点是几米远？
	static const float mTurnSpeed; //旋转速度
	static const int mMaxHitPoint; //最大生命值z
	static const int mMaxEnergy; //武器点数最大值
	static const int mEnergyPerBullet; //单发能耗
	static const int mEnergyCharge; //每帧存储的能量
	static const float mLockOnAngleIn; //锁定角度
	static const float mLockOnAngleOut; //锁定角度
private:
	void move( bool left, bool right, bool up, bool down );
	void turn( bool left, bool right );
	//返回按钮的输入。玩家控制角色
	void think( bool* jump, bool* fire, bool* turn, bool* left, bool* right, bool* up, bool* down ) const;

	Vector3 mPosition;
	float mAngleY;
	int mId;
	GameLib::Scene::Container mSceneContainer;
	GameLib::Scene::Model mModel;
	
	Bullet* mBullets;
	int mBulletNumber;

	int mCameraCount;
	int mCount; //自开始移动以来已经经过了几帧？
	Vector3 mVelocity; //当前平面速度
	float mAngleVelocityY; //转弯速度

	enum Mode{
		MODE_JUMP_UP, //跳起来
		MODE_JUMP_STAY, //跳起来停在空中
		MODE_JUMP_FALL, //跳下时
		MODE_ON_LAND, //着陆
	};
	Mode mMode;

	int mHitPoint; //体力
	int mEnergy; //射击武器所需的能量
	bool mLockOn; //被锁住了吗？
};

#endif
