#include "GameLib/Framework.h"

void createSound( 
short* data, //输出
int waveWidth, //波长=音量
int note, //音符编号0为262Hz。12是522 Hz的do
int sampleNumber ){ //数据数
	//频率表。
	int frequencyTable[] = {
		261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 494,
	};
	int octave = note / 12; //确定八度。
	int f = frequencyTable[ note % 12 ]; //
	f <<= octave; //希望octave为2就变成4倍，不过这是左移2位

	//计算乘数乘以i


	//转换为乘以i的数字（请参见上面的公式）
	int m = waveWidth * f / 44100;

	//然后只需填入数据
	for ( int i = 0; i < sampleNumber; ++i ){
		data[ i ] = static_cast< short >( (i*m) % waveWidth );
	}
}

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		static int a = 0;
		static int b = 0;
		if ( a++ % 120 == 0 ){
			short wave[ 44100 ];
			//Domisomidoo，还记得音乐课的内容吗
			createSound( wave + 44100*0/5, 20000, b+0, 44100/5 );
			createSound( wave + 44100*1/5, 20000, b+4, 44100/5 );
			createSound( wave + 44100*2/5, 20000, b+7, 44100/5 );
			createSound( wave + 44100*3/5, 20000, b+4, 44100/5 );
			createSound( wave + 44100*4/5, 20000, b+0, 44100/5 );

			playSound( wave, 44100 );
			++b;
		}
	}
}