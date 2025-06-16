#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Library/Vector3.h"
#include "Library/Matrix34.h"
#include "Library/Matrix44.h"
#include "Robo.h"
#include "Stage.h"
#include "Pad.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

namespace { //匿名名称空间。在此处放置其他cpp不需要感受到的任何内容。

bool gFirstFrame = true;
Robo* gRobo[ 2 ]; //
Stage* gStage; //舞台全局可见
int gCount;
Vector3 gEyePosition( 0.0, 0.0, 1.0 );
Vector3 gEyeTarget( 0.0, 0.0, 0.0 );

} //namespace{}

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			
			Pad::create(); //初始化
			gRobo[ 0 ] = new Robo( 0 );
			gRobo[ 1 ] = new Robo( 1 );
			gStage = new Stage();
			//初始位置设定
			gRobo[ 0 ]->setPosition( Vector3( 0.0, 0.0, 50.0 ) );
			gRobo[ 1 ]->setPosition( Vector3( 0.0, 0.0, -50.0 ) );
			gRobo[ 0 ]->setAngleY( 180.0 );
			gRobo[ 1 ]->setAngleY( 0.0 );
		}

		//主循环
		//仅计算视点。因为我们将用于Robo::update()。但是视点数据是前一帧的
		Vector3 eyeVector;
		eyeVector.setSub( gEyeTarget, gEyePosition );

		//
		Vector3 move[ 2 ];
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->getMove( &move[ i ], eyeVector );
		}
		//球体碰撞检测
		const Sphere& spRobo0 = *gRobo[ 0 ]->sphere();
		const Sphere& spRobo1 = *gRobo[ 1 ]->sphere();
		const Sphere& spStage = *gStage->sphere();
		//限制运动方向
		//0番机器人
		spRobo0.restrictMove( &move[ 0 ], spRobo1 );
		spRobo0.restrictMove( &move[ 0 ], spStage );
		//1番机器人
		spRobo1.restrictMove( &move[ 1 ], spRobo0 );
		spRobo1.restrictMove( &move[ 1 ], spStage );

		//
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->update( move[ i ], *gRobo[ 1-i ]->position() );
		}

		//----绘制-
		//生成视图转换矩阵
		Vector3 dir;
		gRobo[ 0 ]->getDirection( &dir );
		gEyePosition = *( gRobo[ 0 ]->position() ); //机器人的位置
		//减去长度为6米的dir
		Vector3 t;
		t.setMul( dir, 6.0 );
		gEyePosition -= t;
		gEyePosition.y += 4.0; //6从米上看吧。
		//相反，注视点将增加8米。
		gEyeTarget = *( gRobo[ 0 ]->position() );
		t.setMul( dir, 8.0 );
		gEyeTarget += t;
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, gEyeTarget );

		//生成透视变换矩阵
		//透视变换矩阵
		Matrix44 pm; //60度就可以了吧。
		pm.setPerspectiveTransform( 60.0, 640.0, 480.0, 1.0, 1000.0 );
		//乘法运算
		pm *= vm;
		//绘制
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->draw( pm );
		}
		gStage->draw( pm );

		++gCount;
		//清理工作
		if ( isEndRequested() ){
			SAFE_DELETE( gRobo[ 0 ] );
			SAFE_DELETE( gRobo[ 1 ] );
			SAFE_DELETE( gStage );
			Pad::destroy();
		}
	}
}
