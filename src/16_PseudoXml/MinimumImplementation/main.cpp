#include "GameLib/Framework.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"
#include "Library/Matrix44.h"
#include "Library/Model.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Camera.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Math.h"
using namespace GameLib;

bool gFirstFrame = true;
GraphicsDatabase* gGraphicsDatabase;
Model* gModel;
Camera* gCamera;
int gCount;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){
			gFirstFrame = false;
			//读取数据库
			gGraphicsDatabase = new GraphicsDatabase( "data.txt" );
			//制作模型
			gModel = gGraphicsDatabase->createModel( "batch" );
			//相机生成
			gCamera = new Camera();
		}
		//更新相机
		gCamera->setPosition( Vector3(
			sin( gCount ) * 20.0,
			10.0,
			cos( gCount ) * 20.0 ) );

		Matrix44 pvm;
		gCamera->createPerspectiveViewMatrix( &pvm );
		//更新模型旋转
		gModel->setAngle( Vector3( gCount, gCount * 2, gCount * 3 ) );
		//绘制
		gModel->draw( pvm );

		//伪XML导出测试
		if ( GameLib::Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			using namespace PseudoXml;
			Document document;
			Element* root = document.root();
			//在根上添加一个
			root->setChildNumber( 1 );
			Element* cameraData = root->child( 0 );
			cameraData->setName( "CameraData" );
			//插入两个数据
			cameraData->setChildNumber( 2 );
			Element* e = cameraData->child( 0 );
			e->setName( "Position" );
			//位置数据
			e->setAttributeNumber( 3 );
			e->attribute( 0 )->set( "x", gCamera->position()->x );
			e->attribute( 1 )->set( "y", gCamera->position()->y );
			e->attribute( 2 )->set( "z", gCamera->position()->z );
			//注释点数据
			e = cameraData->child( 1 );
			e->setName( "Target" );
			e->setAttributeNumber( 3 );
			e->attribute( 0 )->set( "x", gCamera->target()->x );
			e->attribute( 1 )->set( "y", gCamera->target()->y );
			e->attribute( 2 )->set( "z", gCamera->target()->z );
			//写
			document.write( "camera.txt" );
		}
		++gCount;

		//清理。通常按照与创建时相反的顺序销毁它。
		//那些使用它的人应该首先被删除，而正在使用的那些人应该在以后被删除。
		if ( isEndRequested() ){
			SAFE_DELETE( gCamera );
			SAFE_DELETE( gModel );
			SAFE_DELETE( gGraphicsDatabase );
		}
	}
}
