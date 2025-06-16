#ifndef INCLUDED_ANIMATION_NODE_H
#define INCLUDED_ANIMATION_NODE_H

class Vector3;
class Curve;

#include <string>
using namespace std;

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class AnimationNode{
public:
	AnimationNode( GameLib::PseudoXml::Element& );
	~AnimationNode();
	//获取数据。如果没有动画，就没有办法处理。
	void data( 
		Vector3* translationInOut,
		Vector3* rotationInOut,
		Vector3* scaleInOut,
		double time ) const;
	const string* name() const;
private:
	Curve** mCurves;
	int mCurveNumber;
	string mName;
};

#endif
