/********************************************************************
* CreatedOn: 2005-8-16   15:01
* FileName: Threads.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef THREADS_H
#define THREADS_H

#ifdef WIN32
#include <windows.h>

#else
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>

#endif

#ifdef WIN32
#define KSTHREAD_MUTEX CRITICAL_SECTION

inline void KSThreadMutexInit(KSTHREAD_MUTEX *pMutex)
{
    InitializeCriticalSection(pMutex);
}
inline void KSThreadMutexDestroy(KSTHREAD_MUTEX *pMutex)
{
    DeleteCriticalSection(pMutex);
}
inline void KSThreadMutexLock(KSTHREAD_MUTEX *pMutex)
{
    EnterCriticalSection(pMutex);
}
inline void KSThreadMutexUnLock(KSTHREAD_MUTEX *pMutex)
{
    LeaveCriticalSection(pMutex);
}

#else

#define KSTHREAD_MUTEX pthread_mutex_t

inline void KSThreadMutexInit(KSTHREAD_MUTEX *pMutex)
{
    pthread_mutex_init(pMutex, NULL);
}
inline void KSThreadMutexDestroy(KSTHREAD_MUTEX *pMutex)
{
    pthread_mutex_destroy(pMutex);
}
inline void KSThreadMutexLock(KSTHREAD_MUTEX *pMutex)
{
    pthread_mutex_lock(pMutex);
}
inline void KSThreadMutexUnLock(KSTHREAD_MUTEX *pMutex)
{
    pthread_mutex_unlock(pMutex);
}

#endif
//////////////////////////////////////////////////////////////////////////
//对临界区进行封装
//由于静态对象初始化顺序的不确定性,该类不能够作为静态对象使用
class KSComAutoThreadMutex
{
public:
	void Init(){}
	void Term(){}
	void Lock() {KSThreadMutexLock(&m_mutex);}
	void Unlock() {KSThreadMutexUnLock(&m_mutex);}
	KSComAutoThreadMutex() {KSThreadMutexInit(&m_mutex);}
	~KSComAutoThreadMutex() {KSThreadMutexDestroy(&m_mutex);}
	
private:
	KSTHREAD_MUTEX m_mutex;
};
//KSComThreadMutex
//可以作为静态对象使用,初始化实例时使用instance = {0}的方法进行静态实例化
//这种实例化的方法要求不能有自定义的构造函数,私有成员变量以及虚函数,参看C++标准8.5.1
struct KSComThreadMutex
{
    volatile int m_nInitFlag;
    KSTHREAD_MUTEX m_mutex;

    void Init() 
    {
        KSThreadMutexInit(&m_mutex);
        m_nInitFlag = 1;
    }
    void Term()
    {        
        KSThreadMutexDestroy(&m_mutex);
    }
    void Lock()
    {
        if (!m_nInitFlag)
            Init();
        KSThreadMutexLock(&m_mutex);
    }
    void Unlock() 
    {
        if (!m_nInitFlag)
            Init();
        KSThreadMutexUnLock(&m_mutex);
    }
};

//////////////////////////////////////////////////////////////////////////
//自动加锁和解锁类
template<typename MutexType>
class KSLockGuard
{
public:
	typedef  MutexType ThreadMutex;
	explicit KSLockGuard(ThreadMutex& rThreadMutex):m_rThreadMutex(rThreadMutex)
	{
		m_rThreadMutex.Lock();
	}
	~KSLockGuard()
	{
		m_rThreadMutex.Unlock();
	}
private:
	ThreadMutex& m_rThreadMutex;
	KSLockGuard(const KSLockGuard&);
	KSLockGuard& operator=(const KSLockGuard&);
};
typedef KSLockGuard<KSComAutoThreadMutex> KSAutoLockGuard;
typedef KSLockGuard<KSComThreadMutex> KSStaticLockGuard;

#endif //THREADS_H
