﻿#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace GameLib;

GraphicsDatabase* gDatabase;
Model* gSun;
Model* gEarth;
Model* gMoon;
int gCount;
Vector3 gLightVector;
Vector3 gLightColor( 0.8, 0.7, 0.6 );
Vector3 gAmbient( 0.2, 0.3, 0.4 );
Vector3 gEyePosition( 0.0, 10.0, 20.0 );

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "cube.txt" );
			gSun = gDatabase->createModel( "cube" );
			gEarth = gDatabase->createModel( "cube" );
			gMoon = gDatabase->createModel( "cube" );
		}
		//尝试移动相机和灯光
		gLightVector.x = sin( gCount ) * cos( gCount );
		gLightVector.y = sin( gCount ) * sin( gCount );
		gLightVector.z = cos( gCount );
		//视图矩阵，透视变换矩阵
		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, Vector3( 0.0, 0.0, 0.0 ) );
		pm *= vm;

		//开始生成矩阵
		Matrix34 w;
		w.setTranslation( Vector3( 0.0, 0.0, 0.0 ) ); //太阳系中心
		w.rotateY( gCount * 100.0 / 365.0 ); //整个太阳系的旋转=地球公转
		Matrix34 l1a;
		l1a.setTranslation( Vector3( 10.0, 0.0, 0.0 ) ); //太阳->地球
		Matrix34 l1b0;
		l1b0.setRotationY( gCount * 100.0 / 30.0 ); //月球公转
		Matrix34 l1b1;
		l1b1.setRotationY( gCount * 100.0 / 3.0 ); //地球自转
		Matrix34 l0;
		l0.setTranslation( Vector3( 4.0, 0.0, 0.0 ) ); //地球->月

		//太阳
		Matrix44 pvwm;
		pvwm.setMul( pm, w );
		gSun->draw( pvwm, gLightVector, gLightColor, gAmbient );

		Matrix34 wL1a = w;
		wL1a *= l1a;

		//地球
		Matrix34 t = wL1a;
		t *= l1b1;
		pvwm.setMul( pm, t );
		gEarth->draw( pvwm, gLightVector, gLightColor, gAmbient );
		
		//月
		t = wL1a;
		t *= l1b0;
		t *= l0;
		pvwm.setMul( pm, t );
		gMoon->draw( pvwm, gLightVector, gLightColor, gAmbient );

		++gCount;
		if ( isEndRequested() ){
			SAFE_DELETE( gSun );
			SAFE_DELETE( gEarth );
			SAFE_DELETE( gMoon );
			SAFE_DELETE( gDatabase );
		}
	}
}

