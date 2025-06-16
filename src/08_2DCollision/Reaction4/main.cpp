#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	bool solvePenetration( const Square& b ){
		int al = mX - mHalfSize; //left A
		int ar = mX + mHalfSize; //right A
		int bl = b.mX - b.mHalfSize; //left B
		int br = b.mX + b.mHalfSize; //right B
		int at = mY - mHalfSize; //top A
		int ab = mY + mHalfSize; //bottom A
		int bt = b.mY - b.mHalfSize; //top B
		int bb = b.mY + b.mHalfSize; //bottom B

		bool hitX = ( al < br ) && ( ar > bl ); //
		bool hitY = ( at < bb ) && ( ab > bt ); //

		if ( hitX && hitY ){ //如果没有同时碰撞，则不会处理
			int pl = ar - bl; //从左插入
			int pr = br - al; //从右插入
			if ( pl < pr ){
				mX -= pl;
			}else{
				mX += pr;
			}
			int pt = ab - bt; //从上面插入
			int pb = bb - at; //从下面插入
			if ( pt < pb ){
				mY -= pt;
			}else{
				mY += pb;
			}
			return true;
		}else{
			return false;
		}
	}
	int mX;
	int mY;
	int mHalfSize;
};
bool gFirstFrame = true;
Square gPlayer; //
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
			dx = -1;
		}else if ( isKeyOn( 's' ) ){
			dx = 1;
		}
		if ( isKeyOn( 'w' ) ){
			dy = -1;
		}else if ( isKeyOn( 'z' ) ){
			dy = 1;
		}
		unsigned* vram = videoMemory();
		//移动
		gPlayer.mX += dx;
		gPlayer.mY += dy;
		//碰撞处理
		unsigned color = 0xffff0000;
		if ( gPlayer.solvePenetration( gWall ) ){
			color = 0xffffffff;
		}
		//绘制
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