#include "GameLib/GameLib.h"
#include "Library/Curve.h"

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Math.h"
#include <string>
using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;

Curve::Data::Data() :
mTime( 0.0 ),
mValue( 0.0 ),
mLeftSlope( 0.0 ),
mRightSlope( 0.0 ){
}

Curve::Curve( Element& e ) :
mData( 0 ),
mDataNumber( 0 ),
mInterporation( INTERPORATION_NONE ){
	ASSERT( string( "Curve" ) == e.name() ); //Curve？
	ASSERT( e.childNumber() > 0 ); //只有一个数据，对不对？
	//取得名字
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		string type = a.value();
		if ( name == "type" ){
			if ( type == "translationX" ){
				mType = TYPE_TRANSLATION_X;
			}else if ( type == "translationY" ){
				mType = TYPE_TRANSLATION_Y;
			}else if ( type == "translationZ" ){
				mType = TYPE_TRANSLATION_Z;
			}else if ( type == "rotationX" ){
				mType = TYPE_ROTATION_X;
			}else if ( type == "rotationY" ){
				mType = TYPE_ROTATION_Y;
			}else if ( type == "rotationZ" ){
				mType = TYPE_ROTATION_Z;
			}else if ( type == "scaleX" ){
				mType = TYPE_SCALE_X;
			}else if ( type == "scaleY" ){
				mType = TYPE_SCALE_Y;
			}else if ( type == "scaleZ" ){
				mType = TYPE_SCALE_Z;
			}
		}else if ( name == "interporation" ){
			if ( type == "none" ){
				mInterporation = INTERPORATION_NONE;
			}else if ( type == "linear" ){
				mInterporation = INTERPORATION_LINEAR;
			}else if ( type == "cubic" ){
				mInterporation = INTERPORATION_CUBIC;
			}
		}
	}
	mDataNumber = e.childNumber();
	mData = new Data[ mDataNumber ];
	for ( int i = 0; i < mDataNumber; ++i ){
		Element c = e.child( i );
		an = c.attributeNumber();
		for ( int j = 0; j < an; ++j ){
			Attribute a = c.attribute( j );
			string name = a.name();
			if ( name == "time" ){
				mData[ i ].mTime = a.getDoubleValue();
			}else if ( name == "value" ){
				mData[ i ].mValue = a.getDoubleValue();
			}else if ( name == "leftSlope" ){
				mData[ i ].mLeftSlope = a.getDoubleValue();
			}else if ( name == "rightSlope" ){
				mData[ i ].mRightSlope = a.getDoubleValue();
			}else{
				ASSERT( false ); //
			}
		}
	}
}

Curve::~Curve(){
	SAFE_DELETE_ARRAY( mData );
}

double Curve::get( double t ) const {
	//执行循环处理。最后的时间被视为一个周期

	//要计算可以从a中减去b的次数，请使用a -= toInt(a/b) * b计算。toInt为向下舍入为整数的函数。可以通过强制转换来实现。
	double quot = t / mData[ mDataNumber - 1 ].mTime; //
	int quotInt = static_cast< int >( quot ); 
	t -= static_cast< double >( quotInt ) * mData[ mDataNumber - 1 ].mTime;

	if ( mInterporation == INTERPORATION_NONE ){
		int last = 0;
		for ( int i = 0; i < mDataNumber; ++i ){
			if ( mData[ i ].mTime > t ){
				break;
			}
			last = i;
		}
		//使用最短时间不超过t的数据（无插值）
		return mData[ last ].mValue;
	}else if ( mInterporation == INTERPORATION_LINEAR ){
		int begin = 0;
		int end = 0;
		for ( end = 0; end < mDataNumber; ++end ){
			if ( mData[ end ].mTime > t ){
				break;
			}
			begin = end;
		}
		if ( end >= mDataNumber ){ //防止溢出。返回最后一个值
			return mData[ mDataNumber - 1 ].mValue;
		}
		double t0 = mData[ begin ].mTime;
		double t1 = mData[ end ].mTime;
		double p0 = mData[ begin ].mValue;
		double p1 = mData[ end ].mValue;
		//变量转换
		t = ( t - t0 ) / ( t1 - t0 );
		//线性补间
		return p0 + ( p1 - p0 ) * t;
	}else if ( mInterporation == INTERPORATION_CUBIC ){
		int begin = 0;
		int end = 0;
		for ( end = 0; end < mDataNumber; ++end ){
			if ( mData[ end ].mTime > t ){
				break;
			}
			begin = end;
		}
		if ( end >= mDataNumber ){ //防止溢出。返回最后一个值
			return mData[ mDataNumber - 1 ].mValue;
		}
		double t0 = mData[ begin ].mTime;
		double t1 = mData[ end ].mTime;
		double p0 = mData[ begin ].mValue;
		double p1 = mData[ end ].mValue;
		double v0 = mData[ begin ].mRightSlope; //起点的右斜率
		double v1 = mData[ end ].mLeftSlope; //终点向左倾斜
		//变量转换
		t = ( t - t0 ) / ( t1 - t0 );
		//以at^3 + bt^2 + c + d计算。c = v0，d = p0。
		double a = 2.0 * ( p0 - p1 ) + ( v0 + v1 );
		double b = 3.0 * ( p1 - p0 ) - ( 2.0 * v0 ) - v1;
		//线性补间
		double r = a; //a
		r *= t; //at
		r += b; //at+b
		r *= t; //at^2+bt
		r += v0; //at^2+bt+c
		r *= t; //at^3+bt^2+ct
		r += p0; //at^3+bt^2+ct+d
		return r;
	}else{
		ASSERT( false ); //
		return 0.0;
	}
}

Curve::Type Curve::type() const {
	return mType;
}
