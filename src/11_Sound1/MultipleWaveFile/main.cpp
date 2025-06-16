#include "GameLib/Framework.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"
using namespace GameLib::Sound;

Wave gWaveSE;
Wave gWaveBGM;
Player gPlayerSE0;
Player gPlayerSE1;
Player gPlayerBGM;

#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include <sstream>
using namespace std;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		//载入SE
		if ( !gWaveSE ){
			gWaveSE = Sound::Wave::create( "dokaan.wav" );
		}
		//载入BGM
		if ( !gWaveBGM ){
			gWaveBGM = Sound::Wave::create( "charara.wav" );
		}
		//创建SE播放器
		if ( !gPlayerSE0 && !gPlayerSE1 && gWaveSE.isReady() ){ //如果播放器为空并且已完成加载
			if ( !gWaveSE.isError() ){ //检查错误。无法停止读取文件。
				//从1Wave中创建两个玩家
				gPlayerSE0 = Sound::Player::create( gWaveSE );
				gPlayerSE1 = Sound::Player::create( gWaveSE );
			}
		}
		//背景音乐播放器的创建
		if ( !gPlayerBGM && gWaveBGM.isReady() ){ //如果播放器为空并且已完成加载
			if ( !gWaveBGM.isError() ){ //检查错误。无法停止读取文件。
				gPlayerBGM = Sound::Player::create( gWaveBGM );
				gPlayerBGM.play( true ); //循环播放。听起来无休止。
			}
		}
		//SE每80 * 16毫秒交替发出声音。
		if ( gPlayerSE0 && gPlayerSE1 ){
			static int a;
			if ( a % 80 == 0 ){
				gPlayerSE0.setVolume( -10 );
				gPlayerSE0.play(); 
			}
			if ( a % 80 == 40 ){
				gPlayerSE1.setVolume( -20 );
				gPlayerSE1.play(); 
			}
			++a;
		}

		static int v = 0;
		if ( Input::Manager::instance().keyboard().isTriggered( Input::Keyboard::KEY_DOWN ) ){
			--v;
			if ( v < -100 ){
				v = -100;
			}
		}
		if ( Input::Manager::instance().keyboard().isTriggered( Input::Keyboard::KEY_UP ) ){
			++v;
			if ( v > 0 ){
				v = 0;
			}
		}
		ostringstream oss;
		oss << "VOLUME: " << v;
		drawDebugString( 0, 0, oss.str().c_str() );
		if ( gPlayerBGM ){
			gPlayerBGM.setVolume( v );
		}
		if ( isEndRequested() ){
			gWaveSE.release();
			gWaveBGM.release();
			gPlayerSE0.release();
			gPlayerSE1.release();
			gPlayerBGM.release();
		}
	}
}
