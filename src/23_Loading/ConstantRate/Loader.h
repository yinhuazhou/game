#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
#include <fstream>
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

class Loader{
public:
	static Loader* instance();
	static void create();
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update();
private:
	Loader();
	Loader( const Loader& ); //
	~Loader();

	list< File* > mFiles;
	
	File* mCurrentFile; //当前正在处理的文件
	ifstream* mStream; //正在读取的文件
	int mFileSize; //当前加载文件的大小
	int mFilePosition; //当前加载文件的位置
	char* mData; //加载缓冲区
	static const int READ_UNIT = 128 * 1024; //128KB

	static Loader* mInstance;
};

#endif
