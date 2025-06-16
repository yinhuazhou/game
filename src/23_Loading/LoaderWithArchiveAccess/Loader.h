#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
#include <map>
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
	friend class Archive;

	string mFilename;
	char* mData;
	int mSize;
};

//档案类
class Archive{
public:
	Archive( const char* archiveName );
	~Archive();
	void read( File* ) ;
private:
	struct Entry{
		int mPosition;
		int mSize;
	};
	int mFileNumber;
	ifstream* mStream;
	map< string, Entry > mEntries;
};

class Loader{
public:
	static Loader* instance();
	//如果从档案中读取，请传入档案名称
	static void create( const char* archiveName = 0 );
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update();
private:
	Loader( const char* filename );
	Loader( const Loader& ); //
	~Loader();

	list< File* > mFiles;
	Archive* mArchive;

	static Loader* mInstance;
};

#endif
