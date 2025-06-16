#include "GameLib/Framework.h"

//乐谱定义
struct Note{
	int mName; //do re mi。0是65Hz。24是所谓的do。
	int mLength; // 单位为100分之1秒
	int mVolume; // 0从100
};

const int C = 24;
const int Cis = 25;
const int D = 26;
const int Dis = 27;
const int E = 28;
const int F = 29;
const int Fis = 30;
const int G = 31;
const int Gis = 32;
const int A = 33;
const int Ais = 34;
const int H = 35;

//右手乐谱
const Note gTrack1[] = {
	{ 0, 150, 0 }, //
	{ D, 50, 15 },

	{ G, 50, 17 },
	{ A, 50, 20 },
	{ H, 50, 22 },
	{ C+12, 50, 25 },

	{ D+12, 50, 30 },
	{ C+12, 25, 25 },
	{ H, 25, 22 },
	{ A, 50, 20 },
	{ D+12, 50, 22 },

	{ G+12, 50, 35 },
	{ Fis+12, 50, 32 },
	{ E+12, 50, 20 },
	{ D+12, 50, 28 },

	{ D+12, 12, 20 },
	{ Cis+12, 13, 20 },
	{ D+12, 12, 20 },
	{ Cis+12, 13, 20 },
	{ H, 25, 20 },
	{ Cis+12, 25, 20 },
	{ A, 25, 25 },
	{ H, 25, 15 },
	{ G, 25, 15 },
	{ A, 25, 15 },

	{ A+12, 100, 15 },
	{ A+12, 100, 15 },

	{ H+12, 100, 20 },
	{ H+12, 100, 20 },

	{ A+12, 50, 25 },
	{ H+12, 50, 25 },
	{ A+12, 50, 22 },
	{ G+12, 50, 20 },

	{ G+12, 12, 15 },
	{ Fis+12, 13, 15 },
	{ G+12, 12, 15 },
	{ Fis+12, 13, 15 },
	{ E+12, 25, 15 },
	{ Fis+12, 25, 15 },
	{ D+12, 100, 12 },
};

//左手乐谱
const Note gTrack2[] = {
	{ 0, 200, 0 }, //

	{ G-12, 100, 10 },
	{ G, 100, 10 },

	{ G, 50, 10 },
	{ Fis, 25, 10 },
	{ E, 25, 10 },
	{ Fis, 50, 10 },
	{ D, 50, 10 },

	{ E, 50, 10 },
	{ Fis, 50, 10 },
	{ G, 50, 10 },
	{ E, 50, 10 },

	{ A, 50, 10 },
	{ G, 50, 10 },
	{ Fis, 50, 10 },
	{ E, 50, 10 },

	{ Fis, 50, 10 },
	{ A, 50, 12 },
	{ D+12, 50, 15 },
	{ Cis+12, 50, 12 },

	{ H, 50, 15 },
	{ D+12, 50, 20 },
	{ G+12, 50, 25 },
	{ Fis+12, 50, 20 },

	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 10, 10 },
	{ D+12, 10, 10 },
	{ E+12, 10, 10 },
	{ D+12, 10, 10 },
	{ Cis+12, 10, 10 },
	{ H, 25, 10 },
	{ Cis+12, 25, 10 },

	{ D+12, 25, 15 },
	{ A, 25, 15 },
	{ G, 25, 12 },
	{ A, 25, 12 },
	{ Fis, 100, 10 },
};

void addSound( 
short* data, //输出
int waveWidth, //波长=音量
int note, //音符编号0为261Hz。12是522 Hz的do
int sampleNumber ){ //数据数
	//频率表。
	int frequencyTable[] = {
		261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 494,
	};
	int octave = note / 12; //确定八度。
	int f = frequencyTable[ note % 12 ]; //

	//2移到八度以下
	octave -= 2;
	if ( octave >= 0 ){
		f <<= octave; //希望octave为2就变成4倍，不过这是左移2位
	}else{
		f >>= -octave;
	}

	int m = waveWidth * f * 2 / 44100 ; //

	//三角波
	int period = 44100 / f; //
	//然后只需填入数据
	for ( int i = 0; i < sampleNumber; ++i ){
		int j = i % period;
		int w = ( j < period / 2 ) ? ( j*m ) : ( 2*waveWidth - j*m );
		w += data[ i ];
		data[ i ] = static_cast< short >( w ); 
	}
}

bool gFirstFrame = true;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){ //仅处理第一帧
			gFirstFrame = false;
			short* wave = new short[ 44100 * 60 ]; //暂时1分。如果数组很大再new

			//填零
			for ( int i = 0; i < 44100 * 60; ++i ){
				wave[ i ] = 0;
			}

			//解释乐谱。
			//1第三轨
			int time = 0;
			int noteNumber = sizeof( gTrack1 ) / sizeof( Note ); //得到音符的数量
			for ( int i = 0; i < noteNumber; ++i ){
				const Note& n = gTrack1[ i ];
				if ( n.mVolume > 0 ){ //如果没有声音，请跳过处理
					addSound( 
						wave + time, 
						n.mVolume * 300,
						n.mName,
						n.mLength * 441 );
				}
				time += n.mLength * 441;
			}
			//2第三轨
			time = 0;
			noteNumber = sizeof( gTrack2 ) / sizeof( Note ); //得到音符的数量
			for ( int i = 0; i < noteNumber; ++i ){
				const Note& n = gTrack2[ i ];
				if ( n.mVolume > 0 ){ //如果没有声音，请跳过处理
					addSound( 
						wave + time, 
						n.mVolume * 300,
						n.mName,
						n.mLength * 441 );
				}
				time += n.mLength * 441;
			}
			playSound( wave, 44100 * 60 );
			SAFE_DELETE_ARRAY( wave );
		}
	}
}