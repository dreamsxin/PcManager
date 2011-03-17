
#ifndef _LOCKER_INC_
#define _LOCKER_INC_

class CLocker  
{
public:
	CLocker()
	{
		InitializeCriticalSection(&m_lk);
	}

	~CLocker()
	{
		DeleteCriticalSection(&m_lk);
	}

	void Lock() 
	{ 
		EnterCriticalSection(&m_lk);
	}

	void Unlock() 
	{
		LeaveCriticalSection(&m_lk);
	}

	BOOL TryLock()
	{
		return TryEnterCriticalSection(&m_lk);
	}
	
private:
	CRITICAL_SECTION m_lk;
};

class CAutoLocker
{
public:
	CAutoLocker(CLocker& lk) :
	  m_lk(lk)
	{
		m_lk.Lock();
	}

	~CAutoLocker()
	{
		m_lk.Unlock();
	}

protected:
	CLocker& m_lk;
};

#define CAutoLocker_(x)  CAutoLocker __tmp_##x(x)

#endif /* _LOCKER_INC_ */