#include "GameLib/Framework.h"
#include "GameLib/Sound/Player.h"
#include "GameLib/FileIO/InFile.h"

using namespace GameLib;
using namespace GameLib::Sound;
using namespace GameLib::FileIO;

InFile gFile;
Player gPlayer;
const int BUFFER_SIZE = 16 * 1024; //建议任何低于此值。如果不超过1/16秒，它将被Player构造函数拒绝。
const int BUFFER_HALFSIZE = BUFFER_SIZE / 2;

int gReadPos = 0;
int gWritePos = 0;
int gWaveSize;
int gWaveStart;

namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){
		if ( !gFile ){
			gFile = InFile::create( "charara.wav" );
			while ( !gFile.isFinished() ){ ; } //等待加载

			//――试着分析Wav文件。

			int channels = gFile.getShort( 22 ); //22从第字节到第二字节的通道数量
			int frequency = gFile.getInt( 24 ); //24从第字节开始4字节频率
			int bitsPerSample = gFile.getShort( 34 ); //34从第字节到第二字节的比特数
			const char* data = gFile.data();
			int pos = 36; //36从兼职开始读
			//跳过不需要的内容
			while ( data[ pos+0 ] != 'd' || data[ pos+1 ] != 'a' || data[ pos+2 ] != 't' || data[ pos+3 ] != 'a' ){ //继续前进，直到找到和“data”并列的东西为止。
				pos += gFile.getInt( pos+4 ); //不需要的尺寸记录在这里
			}
			//终于找到了数据
			gWaveSize = gFile.getInt( pos + 4 ); //数据量是“数据”之后的4个字节
			gWaveStart = pos + 8; //
			if ( gWaveSize + gWaveStart > gFile.size() ){ //有时文件的尺寸是错误的。
				gWaveSize = gFile.size() - gWaveStart; //没办法只能到最后
			}


			//创建玩家
			gPlayer = Player::create( bitsPerSample, BUFFER_SIZE, frequency, channels );

			//填写第一部分
			int restSize = gWaveSize - gReadPos; //剩余文件大小
			int writeSize = ( restSize >= BUFFER_HALFSIZE ) ? BUFFER_HALFSIZE : restSize; //如果它大于缓冲区的一半就这样。如果很小，则取其余所有。
			bool succeeded = gPlayer.write( gWritePos, gFile.data() + 44 + gReadPos, writeSize ); //写。
			if ( !succeeded ){
				cout << "WRITING FAILED!" << endl;
			}
			gReadPos += writeSize;
			gWritePos += writeSize;
			//用0填写不够的数据
			if ( writeSize < BUFFER_HALFSIZE ){
				gPlayer.fillSilence( gWritePos, BUFFER_HALFSIZE - writeSize );
				gWritePos += BUFFER_HALFSIZE - writeSize;
			}
			gPlayer.play(); //在播放流时，将执行循环播放而不会传递任何内容。
		}
		///-----以下每帧执行


		//查看播放位置
		int playPos = gPlayer.position();
		//接近写入指针时（如果写入位置在播放位置之后，并且差值小于缓冲区大小的一半）
		if ( ( gWritePos > playPos ) && ( gWritePos - playPos < BUFFER_HALFSIZE ) ){
			cout << "write=" << gWritePos << "\tplay=" << playPos << "\tread=" << gReadPos << endl;
			//写入。
			int restSize = gWaveSize - gReadPos;
			int writeSize = ( restSize >= BUFFER_HALFSIZE ) ? BUFFER_HALFSIZE : restSize;
			if ( gWritePos >= BUFFER_SIZE ){ //
				gWritePos = 0;
			}
			bool succeeded = gPlayer.write( gWritePos, gFile.data() + 44 + gReadPos, writeSize );
			if ( !succeeded ){
				cout << "WRITING FAILED!" << endl;
			}
			gReadPos += writeSize;
			gWritePos += writeSize;
			//用0填写不够的数据
			if ( writeSize < BUFFER_HALFSIZE ){
				gPlayer.fillSilence( gWritePos, BUFFER_HALFSIZE - writeSize );
				gWritePos += BUFFER_HALFSIZE - writeSize;
			}
		}
		if ( isEndRequested() ){
			gFile.release();
			gPlayer.release();
		}
	}
}
