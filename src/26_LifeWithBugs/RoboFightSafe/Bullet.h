#ifndef INCLUDED_BULLET_H
#define INCLUDED_BULLET_H

namespace GameLib{
	namespace Math{
		class Matrix44;
		class Vector3;
	}
}
using namespace GameLib::Math;

class Model;
class GraphicsDatabase;

class Bullet{
public:
	Bullet();
	~Bullet();
	void create( 
		AutoPtr< GraphicsDatabase >&,
		const char* batchName,
		const Vector3& position,
		float angleX, 
		float angleY,
		bool homing );
	const Vector3* getPosition() const;
	void update( const Vector3& enemyPos ); 
	void draw() const;
	bool isEmpty() const; //如果不使用则为true。由mCount <0确定
	void die(); //子弹将消失
private:
	AutoPtr< Model > mModel;
	int mCount; //-1就把子弹置空。
	bool mHoming; //跟随吗

	static const float mHomingX; //
	static const float mHomingY; //
	static const int mLife; //子弹寿命
	static const float mSpeed; //子弹速度
};

#endif