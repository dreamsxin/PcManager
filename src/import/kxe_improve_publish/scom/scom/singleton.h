/********************************************************************
* CreatedOn: 2005-8-16   15:03
* FileName: Singleton.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SINGLETON_H
#define SINGLETON_H
#include "Threads.h"
#include <cassert>

//////////////////////////////////////////////////////////////////////////
//这个单件模板不能被静态(或全局)对象的构造函数调用,因为在静态对象的构造函数执行时,
//KSComAutoThreadMutex对象不能保证被初始化了,但是在ctr运行后(进入main函数或dllmain函数后),类的静态成员已经初始化
//可以正常调用
template<class T>
class SingletonHolder
{
public:
	typedef  T* volatile  _PtrInstanceType;

	static T& Instance();
private:
	SingletonHolder(){};
	SingletonHolder(const SingletonHolder&){};
	SingletonHolder& operator = (const SingletonHolder&){};
	//	~SingletonHolder()
	//	{
	//		m_pInstance = 0;
	//		m_destroyed = true;
	//	};
	static void MakeInstance();
	static void DestroySingleton();
	static void OnDeadReference();

	struct _Lock 
	{
		_Lock()
		{
			m_Lock.Lock();
		}
		~_Lock()
		{
			m_Lock.Unlock();
		}
	};
	friend struct _Lock;

private:
	static KSComAutoThreadMutex m_Lock;
	static _PtrInstanceType m_pInstance;
	static  bool m_destroyed;
};
//////////////////////////////////////////////////////////////////////////
// init data
template<class T>
KSComAutoThreadMutex SingletonHolder<T>::m_Lock;

template<class T>
typename SingletonHolder<T>::_PtrInstanceType
SingletonHolder<T>::m_pInstance = 0;

template<class T>
bool SingletonHolder<T>::m_destroyed = 0;

template<class T>
inline T& SingletonHolder<T>::Instance()
{
	if (!m_pInstance)
	{
		MakeInstance();
	}
	return *m_pInstance;
}
//////////////////////////////////////////////////////////////////////////
//MakeInstance()
template<class T>
inline void SingletonHolder<T>::MakeInstance()
{
	//KSClassLockGuard<T> guard;
	_Lock guard;
	if (!m_pInstance)
	{
		if (m_destroyed)
		{
			OnDeadReference();
		}
		//static T TheInstance;
		//m_pInstance = &TheInstance;
		m_pInstance = new T;
		m_destroyed = false;
		atexit(DestroySingleton);
	}
}
//////////////////////////////////////////////////////////////////////////
//OnDeadReference()
template<class T>
inline void SingletonHolder<T>::OnDeadReference()
{
	throw std::runtime_error("Dead Reference Detected");
}

template<class T>
inline void SingletonHolder<T>::DestroySingleton()
{
	delete m_pInstance;
	m_pInstance = 0;
	m_destroyed = true;
}

//////////////////////////////////////////////////////////////////////////
//这个单件模板可以被静态(或全局)对象的构造函数调用,因为KSComThreadMutex没有构造函数,
//是通过内部的标记判断是否初始化,如果没有初始化则进行初始化,但是由于第一次调用到时才进行
//KSComThreadMutex的初始化,当不是被静态(或全局)对象的构造函数调用时,如果多线程同时调用
//Instance()函数,有被初始化两次的可能.
template<class T>
class SingletonHolder_Static
{
public:
	typedef  T* volatile  _PtrInstanceType;

	static T& Instance();
private:
	SingletonHolder_Static(){};
	SingletonHolder_Static(const SingletonHolder_Static&){};
	SingletonHolder_Static& operator = (const SingletonHolder_Static&){};
//	~SingletonHolder()
//	{
//		m_pInstance = 0;
//		m_destroyed = true;
//	};
	static void MakeInstance();
	static void DestroySingleton();
	static void OnDeadReference();

    	struct _Lock 
    	{
        	_Lock()
        	{
            		m_Lock.Lock();
        	}
        	~_Lock()
        	{
            		m_Lock.Unlock();
		}
    	};
    	friend struct _Lock;

private:
    static KSComThreadMutex m_Lock;
	static _PtrInstanceType m_pInstance;
	static  bool m_destroyed;
};
//////////////////////////////////////////////////////////////////////////
// init data
template<class T>
KSComThreadMutex SingletonHolder_Static<T>::m_Lock = {0};

template<class T>
typename SingletonHolder_Static<T>::_PtrInstanceType
SingletonHolder_Static<T>::m_pInstance = 0;

template<class T>
bool SingletonHolder_Static<T>::m_destroyed = 0;

template<class T>
inline T& SingletonHolder_Static<T>::Instance()
{
	if (!m_pInstance)
	{
		MakeInstance();
	}
	return *m_pInstance;
}
//////////////////////////////////////////////////////////////////////////
//MakeInstance()
template<class T>
inline void SingletonHolder_Static<T>::MakeInstance()
{
	//KSClassLockGuard<T> guard;
	_Lock guard;
	if (!m_pInstance)
	{
		if (m_destroyed)
		{
			OnDeadReference();
		}
		//static T TheInstance;
		//m_pInstance = &TheInstance;
		m_pInstance = new T;
		m_destroyed = false;
		atexit(DestroySingleton);
	}
}
//////////////////////////////////////////////////////////////////////////
//OnDeadReference()
template<class T>
inline void SingletonHolder_Static<T>::OnDeadReference()
{
	throw std::runtime_error("Dead Reference Detected");
}

template<class T>
inline void SingletonHolder_Static<T>::DestroySingleton()
{
	delete m_pInstance;
	m_pInstance = 0;
	m_Lock.Term();
	m_destroyed = true;
}

#endif //SINGLETON_H