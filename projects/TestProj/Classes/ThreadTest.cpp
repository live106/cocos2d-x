#include "ThreadTest.h"
#include <pthread.h>

ThreadTest::ThreadTest(void)
{
}


ThreadTest::~ThreadTest(void)
{
}

void* run(void* data)
{
	char * msg = (char*) data;
    CCLog("%s", msg);
    //sleep(100);

    CCLog("One day past.");
    return NULL;
}

void ThreadTest::createThread()
{
/*
	int PTW32_CDECL pthread_create (pthread_t * tid,
                            const pthread_attr_t * attr,
                            void *(*start) (void *),
                            void *arg);
*/
	pthread_t pid;
	pthread_create(&pid, NULL, run, "I want you!");
	//pthread_detach(pid);
	void * status = NULL;
    pthread_join(pid, &status);

	 CCLog("create end.");
}

