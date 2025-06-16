#ifndef INCLUDED_NOTE_H
#define INCLUDED_NOTE_H

struct Note{
public:
	int mName; //do re mi。0是65Hz。24是所谓的do。
	int mLength; // 单位为100分之1秒
	int mVolume; // 0从100
};

#endif
