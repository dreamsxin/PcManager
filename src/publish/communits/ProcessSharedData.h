#pragma once

class CProcessSharedData
{
public:
	CProcessSharedData(void);
	virtual ~CProcessSharedData(void);

	BOOL	Create(LPCTSTR lpName, DWORD dwSize);
	BOOL	Open(LPCTSTR lpName, DWORD dwSize);
	void	Close();

	
	PVOID	Lock(DWORD dwTimeout) const {
		DWORD dwWait = WaitForSingleObject(m_hMutex, dwTimeout);
		return (dwWait == WAIT_OBJECT_0) ? m_lpFileData : NULL;
	}

	void	Unlock() const {
		ReleaseMutex(m_hMutex);
	}
	
	HANDLE	GetLockHandle()  const {
		return m_hMutex;
	}

	PVOID	GetData() const {
		return m_lpFileData;
	}

	DWORD	GetSize() const {
		return m_dwFileSize;
	}

private:
	HANDLE	m_hFileMap;
	LPVOID	m_lpFileData;
	DWORD	m_dwFileSize;
	HANDLE	m_hMutex;
};
