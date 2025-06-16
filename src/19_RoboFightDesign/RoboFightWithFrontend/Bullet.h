#ifndef INCLUDED_BULLET_H
#define INCLUDED_BULLET_H

class Model;
class GraphicsDatabase;
class Matrix44;
class Vector3;

class Bullet{
public:
	Bullet();
	~Bullet();
	void create( 
		GraphicsDatabase*,
		const char* batchName,
		const Vector3& position,
		double angleX, 
		double angleY,
		bool homing,
		unsigned color );
	const Vector3* position() const;
	//第二个参数是是否要跟踪
	void update( const Vector3& enemyPos ); 
	void draw( const Matrix44& projectionVewMatrix ) const;
	bool isEmpty() const; //如果不使用则为true。由mCount <0确定
	void die(); //子弹将消失
private:
	Model* mModel;
	int mCount; //-1就把子弹置空。
	bool mHoming; //跟随吗

	static const float mHomingX; //
	static const float mHomingY; //
	static const int mLife; //子弹寿命
	static const float mSpeed; //子弹速度
};

#endif