#include "GameLib/GameLib.h"
#include "Track.h"
#include "Note.h"
#include <cmath>
using namespace std;

Track::Track() : mNotes( 0 ), mNoteNumber( 0 ){}

Track::~Track(){
	if ( mNotes ){
		SAFE_DELETE_ARRAY( mNotes );
	}
	mNoteNumber = 0;
}

void Track::setData( const Note* notes, int noteNumber ){
	mNotes = new Note[ noteNumber ];
	for ( int i = 0; i < noteNumber; ++i ){
		mNotes[ i ] = notes[ i ];
	}
	mNoteNumber = noteNumber;
}

void Track::addWave( short* buffer, int bufferSize, int frequency ) const {
	int time = 0; //1/100单位时间
	int pos = 0; //
	for ( int i = 0; i < mNoteNumber; ++i ){
		const Note& n = mNotes[ i ];
		int endTime = time + n.mLength;
		int length = ( endTime * frequency / 100 ) - pos;
		if ( pos + length >= bufferSize ){
			length = bufferSize - pos; //溢出
		}
		if ( n.mVolume > 0 ){ //如果没有声音，请跳过处理
			addNote( 
				buffer + pos,
				n.mVolume * 32767 / 100,
				n.mName,
				length,
				frequency );
		}
		time = endTime;
		pos += length;
	}
}

void Track::addNote( 
short* buffer,
int waveWidth, 
int name, 
int sampleNumber,
int frequency ) const {
	//57将name解释为440Hz。
	double f = 440.0 * pow( 2.0, ( name - 57 ) * ( 1.0 / 12.0 ) );

	//用于计算的PI
	const double pi = 3.1415926535897932384626433832795;
	//正弦波合成

	double a = f * ( 2.0 * pi ) / frequency;
	double b = waveWidth / 2.0;
	double rcpSampleNumber = 1.0 / sampleNumber;

	//有趣的音色合成（傅立叶分析结果（最高7次谐波））
	//可以创建各种声音。
	//
	const double factor[] = { 
		0.5, //基本频率
		0.85, //2倍音
		1.05, //3倍音
		0.85,
		0.4,
		0.3,
		0.1,
		0.0
	};
	//每个谐波的相位（以度为单位）
	const double phase[] = { 
		339.0, //基本频率
		9.0, //2倍音
		18.0, //3倍音
		3.0,
		351.0,
		353.0,
		358.0,
		0,
	};
	//
	const double va = 8.f * ( 2.0 * pi ) / frequency;

	for ( int i = 0; i < sampleNumber; ++i ){
		double posInNote = i * rcpSampleNumber; //音符中的位置
		double tb = b * ( 1.0 - posInNote ); //音量逐渐减小
		tb *= ( 1.0 + posInNote * 0.2 * sin( va * i ) ); //颤音（音符后更强）
		//合成8个正弦波
		double w = 0.0;
		for ( int j = 0; j < 8; ++j ){
			w += tb * sin( a * j * i + ( phase[ j ] / pi ) ) * factor[ j ];
		}
		w += buffer[ i ];
 		buffer[ i ] = static_cast< short >( w );
	}
}
