#ifndef IRUN_OBJECT_DEFINED
#define IRUN_OBJECT_DEFINED

class CThreadPool;

struct IRunObject
{
	virtual void Run() = 0;
	virtual bool AutoDelete() = 0;
	virtual ~IRunObject(){};
	CThreadPool* pThreadPool;
};

#endif