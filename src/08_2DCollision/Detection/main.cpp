﻿#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	bool isIntersect( const Square& a ) const {
		int la = mX - mHalfSize; //left A
		int ra = mX + mHalfSize; //right A
		int lb = a.mX - a.mHalfSize; //left B
		int rb = a.mX + a.mHalfSize; //right B
		if ( ( la < rb ) && ( ra > lb ) ){
			int ta = mY - mHalfSize; //top A
			int ba = mY + mHalfSize; //bottom A
			int tb = a.mY - a.mHalfSize; //top B
			int bb = a.mY + a.mHalfSize; //bottom B
			if ( ( ta < bb ) && ( ba > tb ) ){
				return true;
			}
		}
		return false;
	}
	int mX;
	int mY;
	int mHalfSize;
};
bool gFirstFrame = true;
Square gPlayer; //玩家的意图
Square gWall; //墙

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			gPlayer.set( 16, 16, 16 );
			gWall.set( 160, 120, 16 );
		}
		//获取移动量
		int dx = 0;
		int dy = 0;
		if ( isKeyOn( 'a' ) ){
			dx -= 1;
		}else if ( isKeyOn( 's' ) ){
			dx += 1;
		}
		if ( isKeyOn( 'w' ) ){
			dy -= 1;
		}else if ( isKeyOn( 'z' ) ){
			dy += 1;
		}
		//移动
		gPlayer.mX += dx;
		gPlayer.mY += dy;
		//碰撞处理
		unsigned color;
		if ( gPlayer.isIntersect( gWall ) ){
			color = 0xffffffff;	 //碰撞了，给它上色
		}else{
			color = 0xffff0000;
		}
		//绘制
		unsigned* vram = videoMemory();
		//清除
		for ( int i = 0; i < width() * height(); ++i ){
			vram[ i ] = 0;
		}
		//
		for ( int y = 0; y < 32; ++y ){
			for ( int x = 0; x < 32; ++x ){
				int tx = x + gWall.mX - 16;
				int ty = y + gWall.mY - 16;
				vram[ ty * width() + tx ] = 0xff0000ff; 
			}
		}
		//
		for ( int y = 0; y < 32; ++y ){
			for ( int x = 0; x < 32; ++x ){
				int tx = x + gPlayer.mX - 16;
				int ty = y + gPlayer.mY - 16;
				vram[ ty * width() + tx ] = color; 
			}
		}
	}
}