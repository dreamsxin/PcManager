////////////////////////////////////////////////////////////////////////////////
//      
//      File for url cache manager
//      
//      File      : sharememlock.h
//      Version   : 1.0
//      Comment   : 提供一个访问共享内存的锁
//      
//      Create at : 2010-2-3
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////

#pragma once 

//////////////////////////////////////////////////////////////////////////
// 本锁的特性：
// 多个线程可以同时读取
// 当有写的要求时，新的读取请求被挂起，直到本次写完，即写入优先
// 只能有一个线程在写

typedef struct _tagShareMemLock
{
	volatile LONG			m_nLock;
	volatile LONG			m_nReadCnt;
	volatile LONG			m_nWrite;

	volatile DWORD			m_nDbgLockProcessID;
	volatile DWORD			m_nDbgLockThreadID;
	volatile DWORD			m_nDbgLockTime;

	void	InitLock()
	{
		m_nLock = 0;
		m_nReadCnt = 0;
		m_nWrite = 0;
	}

	BOOL	TryLockReadCntLock()
	{
		return (::InterlockedCompareExchange(&m_nLock, 1, 0) == 0);
	}

	BOOL	UnlockReadCntLock()
	{
		m_nLock = 0;
		return TRUE;
	}

	BOOL	TryLockWriteLock()
	{
		return (::InterlockedCompareExchange(&m_nWrite, 1, 0) == 0);
	}

	BOOL	UnlockWriteLock()
	{
		m_nWrite = 0;
		return TRUE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	BOOL	TryLockRead()
	{
		// 如果有修改请求，那么优先考虑修改
		if (m_nWrite)
			return FALSE;

		// 如果已经在修改了，那么先等等，锁之外的判断不
		// 准确，但是可以起到加速的作用
		if (m_nReadCnt < 0)
			return FALSE;

		// 开始进行
		if (TryLockReadCntLock())
		{
			if (m_nReadCnt >= 0)
			{
				m_nReadCnt ++;
				UnlockReadCntLock();
				return TRUE;
			}
			else
			{
				UnlockReadCntLock();
				return FALSE;
			}
		}
		return FALSE;
	}

	BOOL	LockRead()
	{
		while (!TryLockRead())
			::Sleep(1);
		return TRUE;
	}

	void	UnLockRead()
	{
		while (!TryLockReadCntLock())
			::Sleep(1);
		m_nReadCnt --;
		UnlockReadCntLock();
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL	TryLockWrite()
	{
		// 如果有人在读，那么再等等，锁之外的判断不准确
		// 但是可以起到加速作用
		if (m_nReadCnt > 0)
			return FALSE;

		if (TryLockReadCntLock())
		{
			// 确保没有人读，也没有人写
			if (m_nReadCnt == 0)
			{
				m_nReadCnt = -10000;
				UnlockReadCntLock();
				return TRUE;
			}
			else
			{
				UnlockReadCntLock();
				return FALSE;
			}
		}
		return FALSE;
	}

	BOOL	LockWrite()
	{
		// 保证没有其他人在写
		while (!TryLockWriteLock())
			::Sleep(1);

		// 保证其他人没有在读，也没有在写
		while (!TryLockWrite())
			::Sleep(1);
		return TRUE;
	}

	void	UnLockWrite()
	{
		while (!TryLockReadCntLock())
			::Sleep(1);
		m_nReadCnt = 0;
		UnlockReadCntLock();

		UnlockWriteLock();
	}
}ShareMemLock;