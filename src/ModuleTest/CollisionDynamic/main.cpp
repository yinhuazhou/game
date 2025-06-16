#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Random.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Model.h"
#include "GameLib/Scene/CollisionDetector.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Input/Mouse.h"

using namespace std;
using namespace GameLib;
using namespace GameLib::Math;
using namespace GameLib::Scene;

class Sphere{
public:
	float mRadius;
	Model mModel;
};

Sphere* gSpheres;
const int gSphereNumber = 1000;
Container gContainer;
CollisionDetector gDetector;

float gEyeDistance = 100.f;
float gAngleY = 0.f;
float gAngleX = 0.f;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( false );
	}
	void Framework::update(){
		if ( !gSpheres ){
			gDetector = CollisionDetector::create( gSphereNumber );
			gContainer = Container::create( "cube.txt" );
			while ( !gContainer.isReady() ){ ; } //等待加载
			gSpheres = new Sphere[ gSphereNumber ];
			//初始放置
			Random ran = Random::create();
			for ( int i = 0; i < gSphereNumber; ++i ){
				Sphere& s = gSpheres[ i ];
				s.mModel = gContainer.createModel( 0 ); //创建模型
				Vector3 p(
					ran.getFloat( -100.f, 100.f ),
					ran.getFloat( -100.f, 100.f ),
					ran.getFloat( -100.f, 100.f ) );
				s.mModel.setPosition( p );
				s.mRadius = 5.f;
			}
		}
		//速度初始化
		Array< Vector3 > velocities( gSphereNumber );
		for ( int i = 0; i < gSphereNumber; ++i ){
			//通过对原点的引力来初始化速度
			velocities[ i ].setMul( *gSpheres[ i ].mModel.position(), -0.01f );
		}
		//碰撞检测
		//注册。返回值是一个ID，因此保存它。无法保证顺序从0返回。
		Array< int > idTable( gSphereNumber );
		for ( int i = 0; i < gSphereNumber; ++i ){
			idTable[ i ] = gDetector.add( *gSpheres[ i ].mModel.position(), gSpheres[ i ].mRadius );
		}
		//执行检测
		Array< CollisionDetector::Pair > results;

		unsigned t0 = time(); //计算时间
		gDetector.detect( &results );
		unsigned t1 = time();

		//根据碰撞结果修改速度
		for ( int i = 0; i < results.size(); ++i ){
			const CollisionDetector::Pair& pair = results[ i ];
			int i0 = idTable[ pair.mId0 ];
			int i1 = idTable[ pair.mId1 ];
			Sphere& s0 = gSpheres[ i0 ];
			Sphere& s1 = gSpheres[ i1 ];
			const Vector3& p0 = *s0.mModel.position();
			const Vector3& p1 = *s1.mModel.position();
			Vector3 n; //碰撞法线
			n.setSub( p1, p0 );
			float distance = n.length();
			if ( distance != 0.f ){
				n *= 1.f / distance;
			}else{
				n.set( 0.f, 1.f, 0.f ); //完全符合时，沿Y = 1方向反弹
			}
			n *= 1.f; //弹跳强度
			velocities[ i0 ] -= n;
			velocities[ i1 ] += n;
		}
		//更新位置
		for ( int i = 0; i < gSphereNumber; ++i ){
			Sphere& s = gSpheres[ i ];
			Vector3 p = *s.mModel.position();
			p += velocities[ i ];
			s.mModel.setPosition( p );
		}
		//屏幕显示
		DebugScreen sc;
		sc << "FRAMERATE: " << frameRate() << " HIT:" << results.size() << " TIME:" << t1 - t0;
//------以下与主要内容无关的代码

		//透视变换
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			60.f, 
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			gEyeDistance * 0.01f, gEyeDistance * 10.f );
		//创建一个视图矩阵
		Matrix34 rm;
		rm.setRotationY( gAngleY );
		rm.rotateX( gAngleX );
		Vector3 tv( 0.f, 0.f, 1.f );
		rm.mul( &tv, tv );
		Vector3 eyePosition;
		eyePosition.setMul( tv, gEyeDistance );
		pm.multiplyViewTransform( eyePosition, Vector3( 0.f ), Vector3( 0.f, 1.f, 0.f ) );

		//反映相机输入
		Input::Mouse mouse; //
		float x = static_cast< float >( mouse.velocityX() );
		float y = static_cast< float >( mouse.velocityY() );
		if ( mouse.isOn( Input::Mouse::BUTTON_LEFT ) ){ //左键旋转
				gAngleX -= 0.1f * y;
				if ( gAngleX > 89.f ){
					gAngleX = 89.f;
				}else if ( gAngleX < -89.f ){
					gAngleX = -89.f;
				}
				gAngleY -= 0.1f * x;
		}
		int w = mouse.wheel();
		if ( w < 0 ){
			gEyeDistance *= 0.9f;
		}else if ( w > 0 ){
			gEyeDistance *= 1.1f;
		}

		//绘制
		Graphics::Manager gm;
		gm.setProjectionViewMatrix( pm );
		gm.setLightingMode( Graphics::LIGHTING_NONE );
		gm.enableDepthWrite( true );
		gm.enableDepthTest( true );
		gm.setAmbientColor( Vector3( 0.0f, 0.0f, 0.0f ) );

		gm.setLightColor( 0, Vector3( 0.1f, 0.5f, 0.1f ) );
		gm.setLightPosition( 0, Vector3( 0.f, 0.f, 0.f ) );
		gm.setLightIntensity( 0, 100.f );

		gm.setLightColor( 1, Vector3( 0.6f, 0.4f, 0.2f ) );
		gm.setLightPosition( 1, Vector3( 5777.f, 5777.f, 5777.f ) );
		gm.setLightIntensity( 1, 10000.f );

		gm.setLightColor( 2, Vector3( 0.2f, 0.4f, 0.6f ) );
		gm.setLightPosition( 2, Vector3( -5777.f, -5777.f, -5777.f ) );
		gm.setLightIntensity( 2, 10000.f );

		gm.setEyePosition( eyePosition );

		for ( int i = 0;i < gSphereNumber; ++i ){
			gSpheres[ i ].mModel.draw();
		}

		if ( isEndRequested() ){
			gDetector.release();
			gContainer.release();
			SAFE_DELETE_ARRAY( gSpheres );
		}
	}
}
