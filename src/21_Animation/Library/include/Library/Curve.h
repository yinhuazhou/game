#ifndef INCLUDED_CURVE_H
#define INCLUDED_CURVE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class Curve{
public:
	enum Type{
		TYPE_TRANSLATION_X,
		TYPE_TRANSLATION_Y,
		TYPE_TRANSLATION_Z,
		TYPE_ROTATION_X,
		TYPE_ROTATION_Y,
		TYPE_ROTATION_Z,
		TYPE_SCALE_X,
		TYPE_SCALE_Y,
		TYPE_SCALE_Z,
	};
	enum Interporation{
		INTERPORATION_NONE, //无插值。
		INTERPORATION_LINEAR, //1下一次插补
		INTERPORATION_CUBIC, //3下一次插补
	};
	Curve( GameLib::PseudoXml::Element& );
	~Curve();
	//在特定时间获取数据
	double get( double time ) const;
	//获取类型
	Type type() const;
private:
	struct Data{
		Data();
		double mTime;
		double mValue;
		double mLeftSlope;
		double mRightSlope;
	};
	Data* mData;
	int mDataNumber;
	Type mType;
	Interporation mInterporation;
};

#endif
