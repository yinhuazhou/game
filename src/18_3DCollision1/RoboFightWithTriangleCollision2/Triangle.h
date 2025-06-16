#ifndef INCLUDED_TRIANGLE_H
#define INCLUDED_TRIANGLE_H

#include "Library/Vector3.h"

class Triangle{
public:
	//计算碰撞的时刻
	double getIntersectionTime( const Vector3& lineBegin, const Vector3& lineVector ) const;
	//起点和线段向量
	bool isIntersect( const Vector3& lineBegin, const Vector3& lineVector ) const;
	//在接收到的运动矢量的各个分量中，仅剩下可以移动而不撞到对手的分量
	bool restrictMove( Vector3* moveVectorInOut, const Vector3& position ) const;
	//从接收到的运动矢量中删除法线分量
	void modifyMove( Vector3* moveVectorInOut ) const;
	//位置设定
	void setPosition( const Vector3& p0, const Vector3& p1, const Vector3& p2 );
private:
	Vector3 mPosition[ 3 ]; //3顶点
};

#endif

