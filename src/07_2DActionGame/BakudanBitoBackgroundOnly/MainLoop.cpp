﻿#include "GameLib/GameLib.h"
#include "MainLoop.h"
#include "Sequence/GameSequence.h"
#include "Sequence/GameOverSequence.h"
#include "Sequence/EndingSequence.h"
#include "Sequence/TitleSequence.h"

MainLoop* MainLoop::mInstance = 0;

void MainLoop::create(){
	assert( !mInstance );
	mInstance = new MainLoop();
}

void MainLoop::destroy(){
	assert( mInstance );
	SAFE_DELETE( mInstance );
}

MainLoop* MainLoop::getInstance(){
	return mInstance;
}

MainLoop::MainLoop() : 
mNextSequence( NEXT_NONE ),
mMode( MODE_NONE ),
mTitle( 0 ),
mGame( 0 ),
mGameOver( 0 ),
mEnding( 0 ){
	//
	mTitle = new Sequence::TitleSequence();
}

MainLoop::~MainLoop(){
	SAFE_DELETE( mTitle );
	SAFE_DELETE( mGame );
	SAFE_DELETE( mGameOver );
	SAFE_DELETE( mEnding );
}

void MainLoop::update(){
	if ( mTitle ){
		mTitle->update( this );
	}else if ( mGame ){
		mGame->update( this );
	}else if ( mGameOver ){
		mGameOver->update( this );
	}else if ( mEnding ){
		mEnding->update( this );
	}else{
		halt( "bakana!" ); //不可能的
	}
	//迁移判断
	//
	switch ( mNextSequence ){
		case NEXT_TITLE:
			assert( !mTitle && ( mGame || mGameOver || mEnding ) ); //检查
			SAFE_DELETE( mGame );
			SAFE_DELETE( mGameOver );
			SAFE_DELETE( mEnding );
			mTitle = new Sequence::TitleSequence();
			break;
		case NEXT_GAME:
			assert( mTitle && !mGameOver && !mGame && !mEnding ); //检查
			SAFE_DELETE( mTitle );
			mGame = new Sequence::GameSequence();
			break;
		case NEXT_GAME_OVER:
			assert( mGame && !mGameOver && !mEnding && !mTitle ); //检查
			SAFE_DELETE( mGame );
			mGameOver = new Sequence::GameOverSequence();
			break;
		case NEXT_ENDING:
			assert( mGame && !mGameOver && !mEnding && !mTitle ); //检查
			SAFE_DELETE( mGame );
			mEnding = new Sequence::EndingSequence();
	}
	mNextSequence = NEXT_NONE; //放回去
}

void MainLoop::moveTo( NextSequence next ){
	assert( mNextSequence == NEXT_NONE ); //没什么
	mNextSequence = next;
}

void MainLoop::setMode( Mode mode ){
	mMode = mode;
}

MainLoop::Mode MainLoop::getMode() const {
	return mMode;
}
