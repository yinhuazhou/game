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

const Note gNotes[] = {
	{ E, 25, 20 },
	{ Dis, 25, 20 },

	{ E, 25, 20 },
	{ Dis, 25, 20 },
	{ E, 25, 20 },
	{ H-12, 25, 20 },
	{ D, 25, 20 },
	{ C, 25, 20 },

	{ A-12, 75, 20 },
	{ C-12, 25, 20 },
	{ E-12, 25, 20 },
	{ A-12, 25, 20 },

	{ H-12, 75, 20 },
	{ E-12, 25, 20 },
	{ Gis-12, 25, 20 },
	{ H-12, 25, 20 },

	{ C, 75, 20 },
};
const int gNoteNumber = 17;

void createSound( 
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

	//计算乘数乘以i


	//转换为乘以i的数字（请参见上面的公式）
	int m = waveWidth * f / 44100;

	//然后只需填入数据
	for ( int i = 0; i < sampleNumber; ++i ){
		data[ i ] = static_cast< short >( (i*m) % waveWidth );
	}
}

bool gFirstFrame = true;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){ //仅处理第一帧
			gFirstFrame = false;
			short* wave = new short[ 44100 * 60 ]; //暂时1分。如果数组很大再new

			//解释乐谱。
			int time = 0;
			for ( int i = 0; i < gNoteNumber; ++i ){
				const Note& n = gNotes[ i ];
				createSound( 
					wave + time, 
					n.mVolume * 300,
					n.mName,
					n.mLength * 441 );
				time += n.mLength * 441;
			}
			//剩余时间设置0
			for ( int i = time; i < 44100 * 60; ++i ){
				wave[ i ] = 0;
			}
			playSound( wave, 44100 * 60 );
			SAFE_DELETE_ARRAY( wave );
		}
	}
}