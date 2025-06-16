#ifndef INCLUDED_BITSTREAM_H
#define INCLUDED_BITSTREAM_H

//读取专用
class IBitStream{
public:
	//设置区域和构造
	explicit IBitStream( const unsigned char* buffer ); //
	void read( unsigned char* data, int sizeInBit ); //读取指定数量的字节
	bool read(); //1读取比特，如果是1则返回真。
	unsigned readU4(); //读取unsigned int
	int readS4(); //读取signed int
	int positionInByte() const; //返回以字节为单位的位置。
private:
	const unsigned char* mBuffer;
	int mPosition; //位单位。
};

//写入专用
class OBitStream{
public:
	//设置区域和构造
	explicit OBitStream( unsigned char* buffer ); //
	void write( const unsigned char* data, int sizeInBit );//写入指定的位数
	void write( bool ); //1位写入
	void write( unsigned ); //读取unsigned
	void write( int ); //读取int
	int sizeInByte() const; //返回字节大小。
private:
	unsigned char* mBuffer;
	int mPosition; //位单位。
};

#endif

