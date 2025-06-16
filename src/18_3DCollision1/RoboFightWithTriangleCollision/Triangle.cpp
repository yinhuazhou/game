#include "Triangle.h"
#include "Library/Vector3.h"


void Triangle::setPosition( const Vector3& p0, const Vector3& p1, const Vector3& p2 ){
	mPosition[ 0 ] = p0;
	mPosition[ 1 ] = p1;
	mPosition[ 2 ] = p2;
}

bool Triangle::isIntersect( const Vector3& lineBegin, const Vector3& lineVector ) const {
	//替换为与书本相同的符号
	const Vector3& a = lineBegin;
	const Vector3& b = lineVector;
	const Vector3& c = mPosition[ 0 ];
	Vector3 d, e;
	d.setSub( mPosition[ 1 ], c );
	e.setSub( mPosition[ 2 ], c );
	Vector3 f;
	f.setSub( c, a );
	//计算法线
	Vector3 n;
	n.setCross( d, e );
	//计算t。
	double nf = n.dot( f );
	double nb = n.dot( b );
	if ( nb == 0.f ){ //平行
	  return false; //确保离开了。
	}
	double t = nf / nb;
	if ( t < 0 || t > 1 ){ 
	  return false;
	}
	//计算v。
	Vector3 p;
	p.setMadd( b, t, a );
	Vector3 g;
	g.setSub( p, c );
	double gd = g.dot( d );
	double ge = g.dot( e );
	double dd = d.dot( d );
	double ee = e.dot( e );
	double de = d.dot( e );
	//分母不能为0。所以不要检查。
	double u = ( gd*de - ge*dd ) / ( de*de - ee*dd );
	if ( u < 0 || u > 1 ){
	  return false;
	}
	double v = ( ge*de - gd*ee ) / ( de*de - ee*dd );
	if ( v < 0 || ( u + v > 1 ) ){
	  return false;
	}
	return true;
}

void Triangle::restrictMove( Vector3* v, const Vector3& p ) const {
	//初步判断
	bool r = isIntersect( p, *v );
	if ( !r ){
		return; //它不会碰撞所以就结束了
	}
	//碰撞了就应该修正vector
	//法线n的使用如下
	//a = v - dot( n, v ) * n / |n|^2
	Vector3 d, e, n;
	d.setSub( mPosition[ 1 ], mPosition[ 0 ] );
	e.setSub( mPosition[ 2 ], mPosition[ 0 ] );
	n.setCross( d, e );

	n *= n.dot( *v ) / n.squareLength();
	*v -= n;
}
