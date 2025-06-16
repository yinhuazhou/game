﻿#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H

namespace GameLib{

class Texture;

class Framework{
public:
	Framework();
	///用户定义函数。
	void update();
	///获取实例
	static Framework instance();
	///获取画面宽度
	int width() const;
	///获取画面高度
	int height() const;
	//向Framework发出结束命令
	void requestEnd();
	//检查Framework是否发出结束命令
	bool isEndRequested() const;
	///以毫秒为单位休眠。
	void sleep( int milliSeconds ) const;
	///获取时间（以毫秒计）
	unsigned time() const;
	///获得帧速率
	int frameRate() const;
	///帧速率设置（内部可循环等待）
	void setFrameRate( int );
	///获取上一帧的时间（以毫秒为单位）
	int previousFrameInterval() const;
	///随机数
	int getRandom( int max = 0 );
	//绘制字符
	void drawDebugString( 
		int column, //列（x）
		int row, //行（y）
		const char* string, 
		unsigned color = 0xffffffff );
	///画三角形。由于涉及齐次坐标，所以p含有4个元素。t含有2个元素。
	void drawTriangle3DH(
		const double* p0,
		const double* p1,
		const double* p2,
		const double* t0 = 0,
		const double* t1 = 0,
		const double* t2 = 0,
		unsigned c0 = 0xffffffff,
		unsigned c1 = 0xffffffff,
		unsigned c2 = 0xffffffff );
	//创建纹理
	void createTexture(
		Texture** textureOut,
		const char* filename );
	//丢弃纹理
	void destroyTexture( Texture** );
	//设置纹理
	void setTexture( const Texture* );
	//查找纹理的大小
	void getTextureSizes( const Texture*, int* width, int* height, int* originalWidth, int* originalHeight );
	//混合模式
	enum BlendMode{ 
		BLEND_LINEAR,
		BLEND_ADDITIVE,
		BLEND_OPAQUE,
	};
	void setBlendMode( BlendMode );
	void enableDepthTest( bool );
	void enableDepthWrite( bool );

	//以下库用户不需要知道
	void start( void* windowHandle );
	void preUpdate();
	void postUpdate();
	static void create();
	static void destroy();
};

} //namespace GameLib

#include "GameLib/GameLib.h"

#endif
