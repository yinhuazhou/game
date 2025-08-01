﻿#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include <sstream>
using namespace std;

double gZMove = 5000.0;
double gXMove = 0.0;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );

		double p[ 4 ][ 3 ] = { 
			{ 1500.0, 500.0, 1000.0 },
			{ 500.0, 1500.0, 1000.0 },
			{ 1500.0, 500.0, 2000.0 },
			{ 500.0, 1500.0, 2000.0 },
		};

		//将x和y乘以640，除以z，然后加上（320,240）。将z从[1,10000]转换为[0,1]。
		for ( int i = 0; i < 4; ++i ){
			//反映移动量
			p[ i ][ 0 ] += gXMove;
			p[ i ][ 2 ] += gZMove;
			//放大，除以z
			p[ i ][ 0 ] *= 640.0 / p[ i ][ 2 ];
			p[ i ][ 1 ] *= 640.0 / p[ i ][ 2 ];
			//中心偏移
			p[ i ][ 0 ] += 320.f;
			p[ i ][ 1 ] += 240.f;
			//z范围转换
			p[ i ][ 2 ] = ( 1.0/9999.0 ) * p[ i ][ 2 ] - ( 1.0/9999.0 );
		}
		//画一个矩形。
		drawTriangle3D( p[ 0 ], p[ 1 ], p[ 2 ] );
		drawTriangle3D( p[ 3 ], p[ 1 ], p[ 2 ] );

		if ( Input::Manager::instance().keyboard().isOn( 'z' ) ){
			gZMove -= 25.f;
		}else if ( Input::Manager::instance().keyboard().isOn( 'w' ) ){
			gZMove += 25.f; 
		}
		if ( Input::Manager::instance().keyboard().isOn( 'a' ) ){
			gXMove -= 25.f;
		}else if ( Input::Manager::instance().keyboard().isOn( 's' ) ){
			gXMove += 25.f; 
		}
		//显示
		ostringstream oss;
		oss << "[w-z] Z MOVEMENT: " << gZMove;
		drawDebugString( 10, 0, oss.str().c_str() );
		oss.str( "" );
		oss << "[a-s] X MOVEMENT: " << gXMove;
		drawDebugString( 10, 1, oss.str().c_str() );
	}
}
