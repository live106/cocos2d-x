
#ifndef __THREADTEST_H__
#define __THREADTEST_H__

#include "cocos2d.h"

USING_NS_CC;

class ThreadTest :
	public CCObject
{
public:
	ThreadTest(void);
	virtual ~ThreadTest(void);

	static void createThread();
};

#endif

