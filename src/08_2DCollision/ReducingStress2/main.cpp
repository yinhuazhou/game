#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	bool isIntersect( const Square& b ) const {
		int al = mX - mHalfSize; //left A
		int ar = mX + mHalfSize; //right A
		int bl = b.mX - b.mHalfSize; //left B
		int br = b.mX + b.mHalfSize; //right B
		if ( ( al < br ) && ( ar > bl ) ){
			int at = mY - mHalfSize; //top A
			int ab = mY + mHalfSize; //bottom A
			int bt = b.mY - b.mHalfSize; //top B
			int bb = b.mY + b.mHalfSize; //bottom B
			if ( ( at < bb ) && ( ab > bt ) ){
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
Square gPlayer; //
Square gWall[ 5 ][ 5 ];

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			for ( int i = 0; i < 5; ++i ){
				for ( int j = 0; j < 4; ++j ){
					gWall[ i ][ j ].set( 
						48 + i * 64,
						16 + j * 64,
						16 );
				}
				gWall[ i ][ 4 ].set( 48 + i * 64, 16 + 2 * 64 + 32, 16 );
			}
			gPlayer.set( 16, 16, 16 );
		}
		int dx = 0;
		int dy = 0;
		if ( isKeyOn( 'a' ) ){
			dx -= 1;
		}else if ( isKeyOn( 's' ) ){
			dx += 1;
		}else if ( isKeyOn( 'w' ) ){ //对角线
			dy -= 1;
		}else if ( isKeyOn( 'z' ) ){
			dy += 1;
		}
		//碰撞处理
		unsigned color = 0xffff0000;
		Square movedPlayer;
		movedPlayer.set( gPlayer.mX + dx, gPlayer.mY + dy, gPlayer.mHalfSize );
		bool hit = false;
		int dx2 = 0; //移位
		int dy2 = 0;
		for ( int i = 0; i < 5; ++i ){
			for ( int j = 0; j < 5; ++j ){
				if ( movedPlayer.isIntersect( gWall[ i ][ j ] ) ){
					//尝试将其挤到一侧。
					Square slided0 = movedPlayer;
					Square slided1 = movedPlayer;
					if ( dx != 0 ){ //横向运动。垂直做
						slided0.mY += gPlayer.mHalfSize; //一半
						slided1.mY -= gPlayer.mHalfSize; //一半
					}else{ //垂直运动。在水平方向上做
						slided0.mX += gPlayer.mHalfSize; //一半
						slided1.mX -= gPlayer.mHalfSize; //一半
					}
					bool hit0 = false;
					bool hit1 = false;
					if ( slided0.isIntersect( gWall[ i ][ j ] ) ){
						hit0 = true;
					}
					if ( slided1.isIntersect( gWall[ i ][ j ] ) ){
						hit1 = true;
					}
					if ( dx != 0 ){
						if ( hit0 && !hit1 ){ //当它向上移动时不会撞到。向上移动。
							dy2 -= 1;
						}else if ( hit1 && !hit0 ){ //如果它向下移动，它不会碰撞。往下移
							dy2 += 1;
						}
					}else{
						if ( hit0 && !hit1 ){ //向左移动时不会命中。将其向左移动。
							dx2 -= 1;
						}else if ( hit1 && !hit0 ){ //向右移动时不会命中。向右移动
							dx2 += 1;
						}
					}
					hit = true;
					color = 0xffffffff;
				}
			}
		}
		if ( hit ){ //如果命中，请再次用dx2进行判断
			movedPlayer.set( gPlayer.mX + dx2, gPlayer.mY + dy2, gPlayer.mHalfSize );
			hit = false;
			for ( int i = 0; i < 5; ++i ){
				for ( int j = 0; j < 5; ++j ){
					if ( movedPlayer.isIntersect( gWall[ i ][ j ] ) ){
						hit = true;
					}
				}
			}
			if ( !hit ){ //命中，但可以移动
				gPlayer.mX += dx2;
				gPlayer.mY += dy2;
			}
		}else{ //如果未碰撞，则可以按计划移动
			gPlayer.mX += dx;
			gPlayer.mY += dy;
		}
		//绘制
		unsigned* vram = videoMemory();
		//清除
		for ( int i = 0; i < width() * height(); ++i ){
			vram[ i ] = 0;
		}
		//
		for ( int i = 0; i < 5; ++i ){
			for ( int j = 0; j < 5; ++j ){
				for ( int y = 0; y < 32; ++y ){
					for ( int x = 0; x < 32; ++x ){
						int tx = x + gWall[ i ][ j ].mX - 16;
						int ty = y + gWall[ i ][ j ].mY - 16;
						vram[ ty * width() + tx ] = 0xff00ff00;
					}
				}
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
