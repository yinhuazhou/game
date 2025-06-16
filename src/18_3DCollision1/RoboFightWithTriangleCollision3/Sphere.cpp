#include "Sphere.h"
#include "Library/Vector3.h"


void Sphere::setPosition( const Vector3& a ){
	mPosition = a;
}

void Sphere::setRadius( double a ){
	mRadius = a;
}

bool Sphere::isIntersect( const Sphere& b ) const {
	Vector3 difference;
	difference.setSub( mPosition, b.mPosition );
	double rSum = mRadius + b.mRadius;
	double squareDistance = difference.squareLength();
	rSum *= rSum;
	return ( squareDistance < rSum );
}

void Sphere::restrictMove( Vector3* v, const Sphere& a ) const {
	//复制生成
	Sphere t = *this;
	t.mPosition += *v; //移动
	//判断目的地，如果没有碰撞，那就没关系。
	bool r = t.isIntersect( a );
	if ( !r ){
		return;
	}
	//如果是这样，则修改向量。
	//b = (c0-c1)/|c0-c1|
	//a = v - dot( b, v ) * b
	//b是长度为1的碰撞垂直向量。a是所需的水平向量。
	//将上面的算式放到下面的算式中，
	//a = v - dot( (c0-c1)/|c0-c1|, v ) / |c0-c1| * b
	//把c0-c1变成为d
	//a = v - dot( d, v ) * d / |d|^2
	//计算变得更加容易。
	Vector3 d;
	d.setSub( mPosition, a.mPosition ); //c0-c1 = d
	double dotDV = d.dot( *v ); //dot(d, v)
	double dLength2 = d.squareLength(); //|d|^2
	d *= ( dotDV / dLength2 ); //dot(b,v) / |d|^2
	//最后减法
	*v -= d;
}
