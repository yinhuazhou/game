#ifndef INCLUDED_CRITICAL_SECTION_H
#define INCLUDED_CRITICAL_SECTION_H

//关键
class CriticalSection{
public:
	CriticalSection();
	void enter();
	void leave();
private:
	int mEntered;
};

#endif
