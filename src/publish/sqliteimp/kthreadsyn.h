////////////////////////////////////////////////////////////////
//
//	Filename: 	kthreadsyn.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-10  18: 53
//	Comment:	从ksethread.h精简改造而来
//
///////////////////////////////////////////////////////////////
#pragma once

#include <assert.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
class KSYNRefCnt
{
public:
	KSYNRefCnt() : m_lValue(0) {}
	KSYNRefCnt(long lValue) : m_lValue(lValue) {}

	long operator++() 
	{
		return InterlockedIncrement(&m_lValue);
	}
	long operator--() 
	{
		return InterlockedDecrement(&m_lValue);
	}
	long operator=(long lValue) { return (m_lValue = lValue); }
	operator long() const { return m_lValue; }
private:
	long operator++(int);
	long operator--(int);
	volatile long m_lValue;
};

//////////////////////////////////////////////////////////////////////////
class KSYNEvent
{
public:
	KSYNEvent():m_handle(NULL)
	{
	}

	KSYNEvent(
		bool bManualReset, 
		bool bInitialState, 
		const TCHAR* pszName = NULL, 
		LPSECURITY_ATTRIBUTES psa = NULL):m_handle(NULL)
	{
		Init(bManualReset, bInitialState, pszName, psa);
	}

	~KSYNEvent()
	{
		UnInit();
	}

	int Init(
		bool bManualReset, 
		bool bInitialState, 
		const TCHAR* pszName = NULL, 
		LPSECURITY_ATTRIBUTES psa = NULL)
	{
		m_handle = ::CreateEvent(psa, bManualReset, bInitialState, pszName);
		return (m_handle != NULL) ? 0 : ::GetLastError();
	}

	int UnInit()
	{
		int nRetCode = 0;
		if (m_handle)
		{
			nRetCode = ::CloseHandle(m_handle) ? 0 : ::GetLastError();
			m_handle = NULL;
		}
		return nRetCode;
	}

	int Reset()
	{
		return ::ResetEvent(m_handle) ? 0 : ::GetLastError();
	}

	int Wait(DWORD dwTimeWait = INFINITE)
	{
		int nRetCode = -1;
		if (m_handle)
		{
			nRetCode = ::WaitForSingleObject(m_handle, dwTimeWait);
		}
		return nRetCode;
	}

	int Signal()
	{
		return ::SetEvent(m_handle) ? 0 : ::GetLastError();
	}
private:
	HANDLE m_handle;
};

//////////////////////////////////////////////////////////////////////////
class KSYNMutex
{
public:
	KSYNMutex()
	{
		::InitializeCriticalSection(&m_cs);
	}

	~KSYNMutex()
	{
		::DeleteCriticalSection(&m_cs);
	}

	bool TryLock()
	{
		return ::TryEnterCriticalSection(&m_cs) == TRUE;
	}

	int Lock()
	{
		::EnterCriticalSection(&m_cs);
		return 0;

	}

	int UnLock()
	{
		::LeaveCriticalSection(&m_cs);
		return 0;
	}
private:
	CRITICAL_SECTION m_cs;
};

//////////////////////////////////////////////////////////////////////////
class KSYNKernelMutex
{
public:
	KSYNKernelMutex():m_handle(NULL)
	{
	}

	KSYNKernelMutex(const TCHAR* pszName, LPSECURITY_ATTRIBUTES psa = NULL)
	{
		Init(pszName, psa);
	}

	~KSYNKernelMutex()
	{
		UnInit();
	}

	int Init(const TCHAR* pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
	{
		m_handle = ::CreateMutex(psa, false, pszName);
		assert(m_handle);
		return (m_handle != NULL) ? 0 : -1;
	}

	int UnInit()
	{
		int nRetCode = 0;
		if (m_handle)
		{
			nRetCode = ::CloseHandle(m_handle) ? 0 : ::GetLastError();
			m_handle = NULL;
		}
		return nRetCode;
	}

	bool TryLock()
	{
		return Lock(0) != WAIT_TIMEOUT;
	}

	int Lock(DWORD dwWaitTime = INFINITE)
	{
		return ::WaitForSingleObject(m_handle, dwWaitTime);
	}

	int UnLock()
	{
		return ::ReleaseMutex(m_handle) ? 0 : ::GetLastError();
	}

private:
	HANDLE m_handle;
};

template<class MutexType>
class KSYNLockGuard
{
public:
	typedef  MutexType ThreadMutex;

	explicit KSYNLockGuard(ThreadMutex& rThreadMutex):m_rThreadMutex(rThreadMutex)
	{
		m_rThreadMutex.Lock();
	}

	~KSYNLockGuard()
	{
		m_rThreadMutex.UnLock();
	}

private:
	ThreadMutex& m_rThreadMutex;
	KSYNLockGuard(const KSYNLockGuard&);
	KSYNLockGuard& operator=(const KSYNLockGuard&);
};

typedef KSYNLockGuard<KSYNMutex> KSYNGuard;
typedef KSYNLockGuard<KSYNKernelMutex> KSYNKernelGuard;

inline bool IsOSVersionLaterNT(void)
{
	bool  bVersionLaterNT = true;
	OSVERSIONINFOEX osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&osvi);
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		//os is windows9x
		bVersionLaterNT = false;
	}
	else if(osvi.dwMajorVersion <= 4)
	{
		//os is windowsNT
		bVersionLaterNT = false;
	}
	else if(osvi.dwMajorVersion >= 5 )
	{
		//os is windows2000 or later
		bVersionLaterNT = true;
	}
	return bVersionLaterNT;
}

inline void GetKernelMutexName(std::wstring &strMutexName, const wchar_t* pszMutexName)
{
	if (IsOSVersionLaterNT())
	{
		strMutexName = L"Global\\";
		strMutexName += pszMutexName;
	}
	else
	{
		strMutexName = pszMutexName;
	}
}