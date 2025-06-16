#ifndef INCLUDED_SPHERE_H
#define INCLUDED_SPHERE_H

#include "Library/Vector3.h"

class Sphere{
public:
	bool isIntersect( const Sphere& ) const;

	void setPosition( const Vector3& );
	void setRadius( double );
	//在接收到的运动矢量的各个分量中，仅剩下可以移动而不撞到对手的分量
	void restrictMove( Vector3* moveVectorInOut, const Sphere& ) const;
private:
	Vector3 mPosition;
	double mRadius;
};

#endif

