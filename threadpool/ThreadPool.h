#ifndef THREAD_POOL_CLASS
#define THREAD_POOL_CLASS
#pragma warning( disable : 4786) // remove irritating STL warnings

#include <windows.h>
#include <list>
#include <map>
#include "RunObject.h"

#define POOL_SIZE		  10
#define SHUTDOWN_EVT_NAME _T("PoolEventShutdown")
using namespace std;

// info about functions which require servicing will be saved using this struct.
typedef struct tagFunctionData
{
	LPTHREAD_START_ROUTINE lpStartAddress;
	union 
	{
		IRunObject* runObject;
		LPVOID pData;
	};	
} _FunctionData;

// // info about threads in the pool will be saved using this struct.
typedef struct tagThreadData
{
	bool	bFree;
	HANDLE	WaitHandle;
	HANDLE	hThread;
	DWORD	dwThreadId;
} _ThreadData;

// info about all threads belonging to this pool will be stored in this map
typedef map<DWORD, _ThreadData, less<DWORD>, allocator<_ThreadData> > ThreadMap;

// all functions passed in by clients will be initially stored in this list.
typedef list<_FunctionData, allocator<_FunctionData> > FunctionList;

// this decides whether a function is added to the front or back of the list.
enum ThreadPriority
{
	High,
	Low
};

typedef struct UserPoolData
{
	LPVOID pData;
	CThreadPool* pThreadPool;
} _tagUserPoolData;

enum PoolState
{
	Ready, // has been created
	Destroying, // in the process of getting destroyed, no request is processed / accepted
	Destroyed // Destroyed, no threads are available, request can still be queued
};

class CThreadPool
{
private:
	#ifdef USE_WIN32API_THREAD
	static DWORD WINAPI _ThreadProc(LPVOID);
	#else
	static UINT __stdcall _ThreadProc(LPVOID pParam);
	#endif
	
	FunctionList m_functionList;
	ThreadMap m_threads;

	int		m_nPoolSize;
	int		m_nWaitForThreadsToDieMS; // In milli-seconds
	TCHAR	m_szPoolName[256];
	HANDLE	m_hNotifyShutdown; // notifies threads that a new function 
							   // is added
	volatile PoolState	 m_state;
	volatile static long m_lInstance;
	
	CRITICAL_SECTION m_csFuncList;
	CRITICAL_SECTION m_csThreads;

	bool	GetThreadProc(DWORD dwThreadId, LPTHREAD_START_ROUTINE&, 
						  LPVOID*, IRunObject**); 
	
	void	FinishNotify(DWORD dwThreadId);
	void	BusyNotify(DWORD dwThreadId);
	void	ReleaseMemory();
		
	HANDLE	GetWaitHandle(DWORD dwThreadId);
	HANDLE	GetShutdownHandle();

public:
	CThreadPool(int nPoolSize = POOL_SIZE, bool bCreateNow = true, int nWaitTimeForThreadsToComplete = 5000);
	virtual ~CThreadPool();
	bool	Create();	// creates the thread pool
	void	Destroy();	// destroy the thread pool
	
	int		GetPoolSize();
	void	SetPoolSize(int);
	
	bool	Run(LPTHREAD_START_ROUTINE pFunc, LPVOID pData, 
				ThreadPriority priority = Low);

	bool	Run(IRunObject*, ThreadPriority priority = Low);

	bool	CheckThreadStop();

	int		GetWorkingThreadCount();

	PoolState GetState();
};
//------------------------------------------------------------------------------
#endif