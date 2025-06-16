#ifndef INCLUDED_MATRIX34_H
#define INCLUDED_MATRIX34_H

class Vector3;

class Matrix34{
public:
	Matrix34(); //默认构造
	//移动矩阵生成
	void setTranslation( const Vector3& );
	//生成缩放矩阵
	void setScaling( const Vector3& );
	//旋转矩阵生成X
	void setRotationX( double radian );
	//旋转矩阵生成Y
	void setRotationY( double radian );
	//旋转矩阵生成Z
	void setRotationZ( double radian );
	//创建视图矩阵
	void setViewTransform( const Vector3& position, const Vector3& target );
	//向量与乘法
	void multiply( Vector3* out, const Vector3& in ) const;
	//矩阵与乘法
	void operator*=( const Matrix34& );
	//和移动矩阵相乘
	void translate( const Vector3& );
	//和缩放矩阵做乘法
	void scale( const Vector3& );
	//X旋转矩阵和乘法
	void rotateX( double radian );
	//与Y旋转矩阵相乘
	void rotateY( double radian );
	//Z旋转矩阵和乘法
	void rotateZ( double radian );

	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
};

#endif
