#ifndef INCLUDED_CUBOID_H
#define INCLUDED_CUBOID_H

class Vector3;

class Cuboid{
public:
	bool isIntersect( const Cuboid& ) const;

	void setPosition( const Vector3& );
	void setHalfSize( const Vector3& );
	//在接收到的运动矢量的各个分量中，仅剩下可以移动而不撞到对手的分量
	void restrictMove( Vector3* moveVectorInOut, const Cuboid& ) const;
private:
	int mX;
	int mY;
	int mZ;
	int mHalfSizeX;
	int mHalfSizeY;
	int mHalfSizeZ;
};

#endif

