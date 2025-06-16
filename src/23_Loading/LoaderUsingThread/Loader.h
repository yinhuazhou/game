#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
#include <fstream>
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
using namespace std;

class File{
public:
	bool isReady() const; //完成加载了吗？
	int getSize() const; //获取文件大小
	const char* getData() const; //内容获取
private:
	File( const char* filename );
	~File();
	friend class Loader;

	string mFilename;
	char* mData;
	int mSize;
};

//Loader::update的调用
class LoadingThread : public GameLib::Threading::Thread{
public:
	LoadingThread(){ start(); }
	~LoadingThread(){ wait(); }
	void operator()();
};

class Loader{
public:
	static Loader* instance();
	static void create();
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update( bool* endOut ); //完成后，将endOut设为true
private:
	Loader();
	Loader( const Loader& ); //
	~Loader();

	//为方便起见，使用数组。
	static const int MAX_FILE_NUMBER = 100;
	File* mFiles[ MAX_FILE_NUMBER ];
	
	LoadingThread* mThread; //线程
	GameLib::Threading::Mutex mLock;
	bool mEndRequest; //完成后设置为true。
	static const int READ_UNIT = 1 * 1024 * 1024; //1MB

	static Loader* mInstance;
};

#endif
