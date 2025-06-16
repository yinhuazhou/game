#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"

#include "Library/TreeTemplate.h"
#include "Library/NodeTemplate.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "Library/Batch.h"

using namespace GameLib;
using namespace GameLib::PseudoXml;

TreeTemplate::TreeTemplate( Element& e, const GraphicsDatabase& db ) :
mNodes( 0 ),
mNodeNumber( 0 ){
	ASSERT( string( "Tree" ) == e.name() ); //tree？
	ASSERT( e.childNumber() == 1 ); //只有一个根节点，对吗
	//取得名字
	int an = e.attributeNumber();
	const string nameStr( "name" );
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		if ( nameStr == a.name() ){
			mName = a.value();
		}
	}
	//首先计算节点
	Element c = e.child( 0 );
	mNodeNumber = countNode( c, 0 );
	mNodes = new NodeTemplate[ mNodeNumber ];
	//构建层级
	build( c, db, 0 );
}

TreeTemplate::~TreeTemplate(){
	SAFE_DELETE_ARRAY( mNodes );
}

//递归
int TreeTemplate::countNode( Element& e, int n ){
	++n; //
	int cn = e.childNumber();
	for ( int i = 0; i < cn; ++i ){
		Element c = e.child( i );
		ASSERT( string( "Node" ) == c.name() ); //Node？
		n = countNode( c, n );
	}
	return n;
}

int TreeTemplate::build( Element& e, const GraphicsDatabase& db, int nodePos ){
	//设置自己的信息
	int an = e.attributeNumber();
	NodeTemplate& node = mNodes[ nodePos ];
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){
			node.mName = a.value();
		}else if ( name == "batch" ){
			node.mBatch = db.batch( a.value() );
		}else if ( name == "translation" ){
			a.getDoubleValues( &node.mTranslation.x, 3 );
		}else if ( name == "rotation" ){
			a.getDoubleValues( &node.mRotation.x, 3 );
		}else if ( name == "scale" ){
			a.getDoubleValues( &node.mScale.x, 3 );
		}
	}
	++nodePos;
	ASSERT( nodePos <= mNodeNumber );  //不可能的
	//插入孩子的信息
	int cn = e.childNumber();
	int elder = -1;
	for ( int i = 0; i < cn; ++i ){
		Element c = e.child( i );
		if ( i == 0 ){ //0如果是第一个孩子的话就给自己设定
			node.mChild = nodePos;
		}else{ //否则，到前一个子结点
			mNodes[ elder ].mBrother = nodePos;
		}
		elder = nodePos;
		nodePos = build( c, db, nodePos );
	}
	return nodePos;
}

const string* TreeTemplate::name() const {
	return &mName;
}

const NodeTemplate* TreeTemplate::node( int i ) const {
	return &mNodes[ i ];
}

int TreeTemplate::nodeNumber() const {
	return mNodeNumber;
}

