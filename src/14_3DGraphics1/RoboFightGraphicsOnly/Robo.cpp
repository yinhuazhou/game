#include "GameLib/Framework.h"
using namespace GameLib;

#include "Robo.h"
#include "Pad.h"
#include "Matrix34.h"
#include "Matrix44.h"
#include "GameLib/Math.h"
using namespace GameLib;

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mTexture( 0 ){
	Framework::instance().createTexture( &mTexture, "robo.tga" );
}

Robo::~Robo(){
	Framework::instance().destroyTexture( &mTexture );
}

const Vector3* Robo::position() const {
	return &mPosition;
}

void Robo::setPosition( const Vector3& a ){
	mPosition = a;
}

double Robo::angleY() const {
	return mAngleY;
}

void Robo::setAngleY( double a ){
	mAngleY = a;
}

void Robo::update( const Vector3& enemyPos, const Vector3& viewVector ){
	//移动
	Vector3 move( 0.0, 0.0, 0.0 );
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::UP, mId ) ){
		move.z = -1.0;
	}
	if ( pad->isOn( Pad::DOWN, mId ) ){
		move.z = 1.0;
	}
	if ( pad->isOn( Pad::LEFT, mId ) ){
		move.x = -1.0;
	}
	if ( pad->isOn( Pad::RIGHT, mId ) ){
		move.x = 1.0;
	}
	double ay = atan2( viewVector.x, viewVector.z ) + 180.0;
	Matrix34 m;
	m.setRotationY( ay );
	m.multiply( &move, move );
	mPosition += move;
	//跳！
	if ( pad->isOn( Pad::JUMP, mId ) ){
		mPosition.y += 1.0;
		//转向敌人。
		Vector3 dir;
		dir.setSub( enemyPos, mPosition ); //从自己到敌人
		//Y轴角度为atan2（x，z）。
		mAngleY = atan2( dir.x, dir.z );
	}else{
		mPosition.y -= 1.0;
		if ( mPosition.y < 0.0 ){
			mPosition.y = 0.0; //地下没有任何东西。
		}
	}
}

void Robo::getDirection( Vector3* v ) const {
	Matrix34 wm;
	wm.setRotationY( mAngleY );
	wm.multiply( v, Vector3( 0.0, 0.0, 1.0 ) );
}

void Robo::draw( const Matrix44& pvm ) const {
	Vector3 p[ 8 ];
	p[ 0 ].set( -0.5, 0.0, -0.5 );
	p[ 1 ].set( -0.5, 0.0, 0.5 );
	p[ 2 ].set( 0.5, 0.0, -0.5 );
	p[ 3 ].set( 0.5, 0.0, 0.5 );
	p[ 4 ].set( -0.5, 2.0, -0.5 );
	p[ 5 ].set( -0.5, 2.0, 0.5 );
	p[ 6 ].set( 0.5, 2.0, -0.5 );
	p[ 7 ].set( 0.5, 2.0, 0.5 );
	//UV
	const double uv[ 13 ][ 2 ] = {
		{ 0.0, 0.0 },//0
		{ 0.25, 0.0 },//1
		{ 0.5, 0.0 },//2 
		{ 0.75, 0.0 },//3
		{ 1.0, 0.0 },//4
		{ 0.0, 0.5 },//5 
		{ 0.25, 0.5 },//6
		{ 0.5, 0.5 },//7
		{ 0.75, 0.5 },//8
		{ 1.0, 0.5 },//9
		{ 0.0, 0.75 },//10
		{ 0.25, 0.75 },//11
		{ 0.5, 0.75 },//12
	};
	//记录哪个顶点处绘制三角形的表。可以试着绘制一下，否则不好理解
	const int vIdx[ 36 ] = {
		0,1,4,//-x
		5,1,4,
		2,3,6,//+x
		7,3,6,
		0,1,2,//-y
		3,1,2,
		4,5,6,//+y
		7,5,6,
		0,2,4,//-z
		6,2,4,
		1,3,5,//+z
		7,3,5,
	};
	//UV绘制三角形的表格。可以试着绘制一下，否则不好理解
	const int tIdx[ 36 ] = {
		8,9,3,
		4,9,3,
		8,7,3,
		2,7,3,
		6,11,7,
		12,11,7,
		5,10,6,
		11,10,6,
		7,6,2,
		1,6,2,
		5,6,0,
		1,6,0,
	};
	//世界矩阵
	Matrix34 wm;
	wm.setTranslation( mPosition );
	wm.rotateY( mAngleY );
	//透视变换视图矩阵和乘法
	Matrix44 pvwm;
	pvwm.setMul( pvm, wm );
	//矩阵向量乘法
	double p4[ 8 ][ 4 ];
	for ( int i = 0; i < 8; ++i ){
		pvwm.multiply( p4[ i ], p[ i ] );
	}

	Framework f = GameLib::Framework::instance();
	f.enableDepthTest( true );
	f.setTexture( mTexture ); //纹理集
	for ( int i = 0; i < 12; ++i ){
		int vi0 = vIdx[ 3 * i + 0 ];
		int vi1 = vIdx[ 3 * i + 1 ];
		int vi2 = vIdx[ 3 * i + 2 ];
		int ti0 = tIdx[ 3 * i + 0 ];
		int ti1 = tIdx[ 3 * i + 1 ];
		int ti2 = tIdx[ 3 * i + 2 ];
		f.drawTriangle3DH(
			p4[ vi0 ], p4[ vi1 ], p4[ vi2 ], 
			uv[ ti0 ], uv[ ti1 ], uv[ ti2 ] );
	}
}
