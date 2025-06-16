#ifndef INCLUDED_VECTOR3_H
#define INCLUDED_VECTOR3_H

class Vector3{
public:
	//一个什么都不做的构造函数
	Vector3();
	//根据double构造
	Vector3( double x, double y, double z );
	//通过Vector2构造
	Vector3( const Vector3& );
	//稍后设置数字
	void set( double x, double y, double z );
	//加法
	void operator+=( const Vector3& );
	//减法
	void operator-=( const Vector3& );
	//复制
	void operator=( const Vector3& );
	//设置加法
	void setAdd( const Vector3&, const Vector3& );
	//设置减法
	void setSub( const Vector3&, const Vector3& );
	//乘积（this+ = a * b）。方便且经常使用
	void madd( const Vector3& a, double b );
	//xyz乘以a倍
	void operator*=( double );
	//设置乘法
	void setMul( const Vector3&, double );
	//求出长度的平方
	double squareLength() const;
	//得到长度
	double length() const;
	//点积计算
	double dot( const Vector3& ) const;
	///this = crossProduct( a, b )
	void setCross( const Vector3& a, const Vector3& b );
	/// this = a * b + c
	void setMadd( const Vector3& a, double b, const Vector3& c );

	double x, y, z;
};

#endif
