#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //具体
		FLAG_BRICK = ( 1 << 1 ), //
		FLAG_ITEM_BOMB = ( 1 << 2 ), //炸弹道具
		FLAG_ITEM_POWER = ( 1 << 3 ), //冲击波道具
	};
	StaticObject();

	//绘制地板，墙壁，砖块，炸弹和物品（如果有）
	void draw( int x, int y, const Image* ) const;

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );

private:
	unsigned mFlags;
};

#endif
