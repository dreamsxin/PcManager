///////////////////////////////////////////////////////////////
//
//	Filename: 	KDllLock.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2008-2-20  17:22
//	Comment:	
//
///////////////////////////////////////////////////////////////

#ifndef _KDllLock_h_
#define _KDllLock_h_




class KDllLock
{
public:
	static KDllLock* GetInstance()
	{
		static KDllLock instance;
		return &instance;
	}


	UINT Lock()
	{
		::InterlockedIncrement( &m_ulLockCount );
		return m_ulLockCount;
	}

	ULONG UnLock()
	{
		::InterlockedDecrement( &m_ulLockCount );
		return m_ulLockCount;
	}


	ULONG GetLockCount() const
	{
		return m_ulLockCount;
	}

private:

	KDllLock()
	{
		m_ulLockCount = 0;
	}

	LONG m_ulLockCount;
};



#endif // _KDllLock_h_