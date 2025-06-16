#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Ending.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Image.h"

namespace Sequence{

Ending::Ending() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Ending::~Ending(){
	SAFE_DELETE( mImage );
}

Child* Ending::update( Parent* ){
	Child* next = this;
	if ( mCount == 120 ){ //2等待秒
		next = new Title;
	}
	//绘制
	mImage->draw();
	//输出文字
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "congratulations！" );
	f.drawDebugString( 0, 1, "creator L.U.O" );

	++mCount;

	return next;
}

} //namespace Sequence