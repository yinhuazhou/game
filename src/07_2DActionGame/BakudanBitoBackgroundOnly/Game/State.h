#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"

class Image; //只需要声明。不必包含它。
class StaticObject;

class State{
public:
	//给出阶段编号并构建。0是两个人
	static const int STAGE_ID_2PLAYERS = 0;
	State( int stageID );
	~State();
	//输入在里面
	void update();
	void draw() const;
	//从外部获取的信息
	bool hasCleared() const;
	bool isAlive1P() const; //1P还活着吗？
	bool isAlive2P() const; //2P还活着吗？
private:
	//不动的物体
	Array2D< StaticObject > mStaticObjects;
	int mStageID;

	Image* mImage; //图片
};

#endif
