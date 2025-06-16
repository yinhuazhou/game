#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	//1000返回每分钟中一帧的时间。返回0到1000的整数，
	//0表示移动开始的瞬间碰撞，1000表示没有发生碰撞
	int getIntersectionTime( int dx, int dy, const Square& b ) const {
		//就像在原点一样移动两个。
		int x = b.mX - mX;
		int y = b.mY - mY;
		//
		int vx = -dx;
		int vy = -dy;
		//
		int s = mHalfSize + b.mHalfSize;
		//在这种情况下，（x，y）+ t（vx，vy）有四条直线
		//x = -s, x = s, y = -s, y = s
		//计算碰撞时间。

		int tx0, tx1, ty0, ty1; //4一个时间
		//x+t*vx=-sとx+t*vx=sより、
		if ( vx == 0 ){
			//找出现在是否相交
			if ( -s < x && x < s ){
				tx0 = -1000; //
				tx1 = 1000;
			}else{ //
				tx0 = tx1 = 1000;
			}
		}else{
			tx0 = ( -s - x ) * 1000 / vx;
			tx1 = ( s - x ) * 1000 / vx;
		}
		//根据y + t * vy = -s和y + t * vy = s，
		if ( vy == 0 ){
			//找出现在是否相交
			if ( -s < y && y < s ){
				ty0 = -1000; //
				ty1 = 1000;
			}else{ //
				ty0 = ty1 = 1000;
			}
		}else{
			ty0 = ( -s - y ) * 1000 / vy;
			ty1 = ( s - y ) * 1000 / vy;
		}
		//将tx0，ty0设为较小的值。以后会更方便。
		int t;
		if ( tx0 > tx1 ){
			t = tx0;
			tx0 = tx1;
			tx1 = t;
		}
		if ( ty0 > ty1 ){
			t = ty0;
			ty0 = ty1;
			ty1 = t;
		}
		t = 1000; //碰撞时间
		//根据tx0检查是否碰撞
		if ( tx0 >= 0 && tx0 < 1000 ){
			//如果y轴在规定的时间处于交叉状态，就是这种情况。
			//这应该满足ty0 <= tx0 <= ty1。
			if ( ty0 <= tx0 && tx0 <= ty1 && tx0 < t ){
				t = tx0;
			}
		}
		//tx1
		if ( tx1 >= 0 && tx1 < 1000 ){
			if ( ty0 <= tx0 && tx0 <= ty1 && tx1 < t ){
				t = tx1;
			}
		}
		//ty0
		if ( ty0 >= 0 && ty0 < 1000 ){
			if ( tx0 <= ty0 && ty0 <= tx1 && ty0 < t ){
				t = ty0;
			}
		}
		//ty1
		if ( ty1 >= 0 && ty1 < 1000 ){
			if ( tx0 <= ty0 && ty0 <= tx1 && ty1 < t ){
				t = ty1;
			}
		}
		return t;
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
			dx = -7;
		}else if ( isKeyOn( 's' ) ){
			dx = 7;
		}
		if ( isKeyOn( 'w' ) ){
			dy = -7;
		}else if ( isKeyOn( 'z' ) ){
			dy = 7;
		}
		unsigned* vram = videoMemory();
		//碰撞处理
		unsigned color = 0xffff0000;
		int time = gPlayer.getIntersectionTime( dx, dy, gWall );
		if ( time < 1000 ){
			color = 0xffffffff;
		}
		//移动
		gPlayer.mX += ( dx * time ) / 1000;
		gPlayer.mY += ( dy * time ) / 1000;
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