#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; //只需要声明。不必包含它。

//状态类
class State{
public:
	State( const char* stageData, int size );
	~State();
	void update( int moveX, int moveY, int frameIntervalInMilliSecond );
	void draw() const;
	bool hasCleared() const;
private:
	class Object;
	void setSize( const char* stageData, int size );

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Image* mImage; //图片
	int mMoveCount;
	static const int MAX_MOVE_COUNT = 500; //0.5秒数
};

#endif
