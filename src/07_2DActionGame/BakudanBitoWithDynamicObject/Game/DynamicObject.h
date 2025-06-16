#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;

class DynamicObject{
public:
	enum Type{
		TYPE_1P,
		TYPE_2P,
		TYPE_ENEMY,

		TYPE_NONE, //非法值
	};
	DynamicObject();
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	void update();
private:
	Type mType;
	int mX;
	int mY;
	//仅限敌人
	int mDirectionX;
	int mDirectionY;
};

#endif
