﻿#ifndef INCLUDED_MATRIX23_H
#define INCLUDED_MATRIX23_H

class Vector2;

class Matrix23{
public:
	//6元素构成
	Matrix23( 
		double e00, double e01, double e02,
		double e10, double e11, double e12 );
	//向量与乘法
	void multiply( Vector2* out, const Vector2& in ) const;
private:
	double m00, m01, m02;
	double m10, m11, m12;
};

#endif
