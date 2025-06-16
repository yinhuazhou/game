#include "GameLib/Framework.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"
using namespace GameLib::Sound;

Wave gWave;
Player gPlayer;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		//如果gWave为空，则开始加载
		if ( !gWave ){
			gWave = Sound::Wave::create( "dokaan.wav" );
		}
		if ( !gPlayer && gWave.isReady() ){ //如果播放器为空并且已完成加载
			if ( !gWave.isError() ){ //检查错误。无法停止读取文件。
				gPlayer = Sound::Player::create( gWave );
				gPlayer.play();
			}
		}
		if ( gPlayer ){ //如果创建了gPlayer
			static int a = 0;
			static int b = 1;
			//音量实验
			gPlayer.setVolume( a );
			if ( a == 0 || a == -100 ){
				b *= -1;
			}
			a += b;
			if ( !gPlayer.isPlaying() ){ //结束后释放
				gPlayer.release();
				gWave.release();
			}
		}
		if ( isEndRequested() ){
			gWave.release();
			gPlayer.release();
		}
	}
}
