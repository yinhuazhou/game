#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;

GraphicsDatabase* gDatabase;
Model* gUpperModel;
Model* gLowerModel;
int gCount;
Vector3 gLightVector;
Vector3 gLightColor( 0.8, 0.7, 0.6 );
Vector3 gAmbient( 0.2, 0.3, 0.4 );
Vector3 gEyePosition;

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "cube.txt" );
			gUpperModel = gDatabase->createModel( "cube" );
			gLowerModel = gDatabase->createModel( "cube" );
		}
		//尝试移动相机和灯光
		gLightVector.x = sin( gCount ) * cos( gCount );
		gLightVector.y = sin( gCount ) * sin( gCount );
		gLightVector.z = cos( gCount );
		gEyePosition.x = 20.0 * cos( gCount * 0.5 );
		gEyePosition.y = 10.0;
		gEyePosition.z = 20.0 * sin( gCount * 0.5 );

		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, Vector3( 0.0, 0.0, 0.0 ) );
		pm *= vm;

		//手动构建矩阵。
		//下半身倾斜45度
		Matrix34 w;
		w.setRotationZ( 45.0 );
		//上半身是下半身移至Y
		double y = 4.0 + sin( gCount * 5.0 );
		Matrix34 l;
		l.setTranslation( Vector3( 0.0, y, 0.0 ) );

		//画下半身
		Matrix44 pvwm;
		pvwm.setMul( pm, w );
		gUpperModel->draw( pvwm, gLightVector, gLightColor, gAmbient );

		//画上身
		Matrix34 t = w; 
		t *= l; //w * l
		pvwm.setMul( pm, t );
		gLowerModel->draw( pvwm, gLightVector, gLightColor, gAmbient );

		++gCount;
		if ( isEndRequested() ){
			SAFE_DELETE( gUpperModel );
			SAFE_DELETE( gLowerModel );
			SAFE_DELETE( gDatabase );
		}
	}
}
