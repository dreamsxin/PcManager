// dummyz@126.com

#pragma once

#include <Psapi.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

//class CPEStdFile; // lpFilePath, hFile
//class CPEModFile; // dwProcess, hModule
class CPEMemFile; // lpMem, dwSize
class CPEMapFile;

class IPEFile
{
protected:
	struct MAP_SECTION
	{
		ULONG	lOffset;
		DWORD	dwTotalSize;
		DWORD	dwDataSize;
	};

public:
	virtual BOOL	IsOpen() = 0;
	virtual	BOOL	IsReadOnly() = 0;
	virtual	BOOL	IsRawPtr() = 0;

	virtual ULONG	GetSize() = 0;
	virtual ULONG	Tell() = 0;
	virtual	ULONG	Seek(LONG lOffset, DWORD dwMethod) = 0;
	virtual ULONG	Read(LPVOID lpBuff, DWORD dwSize) = 0;
	virtual ULONG	Write(LPCVOID lpBuff, DWORD dwSize) = 0;
	virtual	PVOID	GetMapPtr() = 0;

	template<class T> BOOL ReadT(T& r) {
		return Read(&r, sizeof (T)) == sizeof (T);
	}

	template<class T> BOOL WriteT(T& r) {
		return Write(&t, sizeof (T)) == sizeof (T);
	}

	virtual DWORD	GetAttr()
	{
		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////
inline DWORD xxGetFileSize(HANDLE h)
{
	DWORD dwSizeHig;
	DWORD dwSizeLow = GetFileSize(h, &dwSizeHig);
	return (dwSizeHig == 0) ? dwSizeLow : (ULONG_MAX - 1);
}

//////////////////////////////////////////////////////////////////////////

class CPEMemFile : public IPEFile
{
public:
	CPEMemFile() 
	{
		m_bReadOnly = TRUE;
		m_bRawPtr = FALSE;
		m_lpMemBase = NULL;
		m_dwMemSize = 0;
		m_lOffset = 0;
	}

	virtual ~CPEMemFile()
	{
		Close();
	}

	BOOL	Open(LPVOID lpMem, DWORD dwSize, BOOL bReadOnly, BOOL bRawPtr)
	{
		if ( lpMem == NULL || dwSize == 0 )
			return FALSE;

		if ( m_lpMemBase != NULL )
			return FALSE;

		m_lOffset = 0;
		m_lpMemBase = lpMem;
		m_dwMemSize = dwSize;
		m_bRawPtr = bRawPtr;
		m_bReadOnly = bReadOnly;

		return TRUE;
	}
	
	void	Close()
	{
		m_lpMemBase = NULL;
		m_dwMemSize = 0;
		m_bReadOnly = TRUE;
		m_lOffset = 0;
	}

	virtual	BOOL	IsReadOnly() {
		return m_bReadOnly;
	}

	virtual	BOOL	IsRawPtr() {
		return m_bRawPtr;
	}

	virtual BOOL	IsOpen() {
		return (m_lpMemBase != NULL);
	}

	virtual ULONG	GetSize()
	{
		return m_dwMemSize;
	}

	virtual ULONG	Tell()
	{
		assert(m_lpMemBase != NULL);

		return m_lOffset;
	}

	virtual	ULONG	Seek(LONG lOffset, DWORD dwMethod)
	{
		assert(m_lpMemBase != NULL);
		
		switch ( dwMethod )
		{
		case FILE_BEGIN: m_lOffset = lOffset; break;
		case FILE_CURRENT: m_lOffset += lOffset; break;
		case FILE_END: m_lOffset = m_dwMemSize + lOffset; break;
		default:
			return EOF;
		}

		if ( m_lOffset >= m_dwMemSize )
		{
			m_lOffset = m_dwMemSize;
			return EOF;
		}

		return m_lOffset;
	}

	virtual ULONG	Read(LPVOID lpBuff, DWORD dwSize)
	{
		assert(m_lpMemBase != NULL);
		DWORD dwReadBytes = 0;

		if ( m_lOffset + dwSize >= m_dwMemSize )
		{
			dwSize = m_dwMemSize - m_lOffset;
		}

		__try
		{
			memcpy(lpBuff, (PBYTE)m_lpMemBase + m_lOffset, dwSize);
			Seek(dwSize, FILE_CURRENT);

			dwReadBytes = dwSize;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			Seek(0, FILE_END);
			dwReadBytes = 0;
		}

		return dwReadBytes;
	}

	virtual ULONG	Write(LPCVOID lpBuff, DWORD dwSize)
	{
		assert(m_lpMemBase != NULL);
		DWORD dwWrittenBytes = 0;

		if ( m_lOffset + dwSize >= m_dwMemSize )
		{
			dwSize = m_dwMemSize - m_lOffset;
		}

		__try
		{
			memcpy((PBYTE)m_lpMemBase + m_lOffset, lpBuff, dwSize);
			Seek(dwSize, FILE_CURRENT);

			dwWrittenBytes = dwSize;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			Seek(0, FILE_END);
			dwWrittenBytes = 0;
		}

		return dwWrittenBytes;
	}

	PVOID	GetMapPtr() 
	{
		return m_lpMemBase;
	}

protected:
	LPVOID	m_lpMemBase;
	DWORD	m_dwMemSize;
	ULONG	m_lOffset;
	BOOL	m_bRawPtr;
	BOOL	m_bReadOnly;
};

class CPEMapFile : public IPEFile
{
public:
	CPEMapFile() 
	{
		m_bReadOnly = TRUE;
		m_bRawPtr = FALSE;
		m_dwFileAttr = 0;
		
		m_lpMemBase = NULL;
		m_dwMemSize = 0;
	}

	virtual ~CPEMapFile()
	{
		Close();
	}

	BOOL	Open(LPCTSTR lpFilePath, BOOL bReadOnly, BOOL bRawPtr, DWORD dwMaxSize)
	{
		if ( lpFilePath == NULL )
		{
			return FALSE;
		}

		if( m_lpMemBase != NULL )
		{
			return FALSE;
		}

		DWORD dwShareMode = FILE_SHARE_READ;
		DWORD dwDesiredAccess = GENERIC_READ;
		if ( !bReadOnly )
		{
			dwDesiredAccess |= GENERIC_WRITE;
			dwShareMode = 0;
		}

		m_bReadOnly = bReadOnly;
		m_bRawPtr = bRawPtr;
		m_lOffset = 0;

		HANDLE hFile = CreateFile(lpFilePath, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, 0, NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			m_dwFileAttr = GetFileAttributes(lpFilePath);
			m_dwMemSize = xxGetFileSize(hFile);
			m_dwMemSize = min(dwMaxSize, m_dwMemSize);

			DWORD dwProtect = bReadOnly ? PAGE_READONLY : PAGE_READWRITE;
			HANDLE hFileMap = CreateFileMapping(hFile, NULL, dwProtect, 0, m_dwMemSize, NULL);
			CloseHandle(hFile);

			if ( hFileMap != NULL )
			{
				DWORD dwDesiredAccess = bReadOnly ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);
				m_lpMemBase = MapViewOfFile(hFileMap, dwDesiredAccess, 0, 0, m_dwMemSize);
				CloseHandle(hFileMap);

				if ( m_lpMemBase != NULL )
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	
	void	Close()
	{
		if ( m_lpMemBase != NULL )
		{
			UnmapViewOfFile(m_lpMemBase);
			m_lpMemBase = NULL;
		}

		m_dwMemSize = 0;
		m_bReadOnly = TRUE;
		m_lOffset = 0;
	}

	virtual BOOL	IsOpen() {
		return (m_lpMemBase != NULL);
	}

	virtual	BOOL	IsReadOnly() {
		return m_bReadOnly;
	}

	virtual	BOOL	IsRawPtr() {
		return m_bRawPtr;
	}

	virtual ULONG	GetAttr()
	{
		return m_dwFileAttr;
	}

	virtual ULONG	GetSize()
	{
		return m_dwMemSize;
	}

	virtual ULONG	Tell()
	{
		assert(m_lpMemBase != NULL);

		return m_lOffset;
	}

	virtual	ULONG	Seek(LONG lOffset, DWORD dwMethod)
	{
		assert(m_lpMemBase != NULL);

		switch ( dwMethod )
		{
		case FILE_BEGIN: m_lOffset = lOffset; break;
		case FILE_CURRENT: m_lOffset += lOffset; break;
		case FILE_END: m_lOffset = m_dwMemSize + lOffset; break;
		default:
			return EOF;
		}

		if ( m_lOffset >= m_dwMemSize )
		{
			m_lOffset = m_dwMemSize;
			return EOF;
		}

		return m_lOffset;
	}

	virtual ULONG	Read(LPVOID lpBuff, DWORD dwSize)
	{
		assert(m_lpMemBase != NULL);
		DWORD dwReadBytes = 0;

		if ( m_lOffset + dwSize >= m_dwMemSize )
		{
			dwSize = m_dwMemSize - m_lOffset;
		}

		__try
		{
			memcpy(lpBuff, (PBYTE)m_lpMemBase + m_lOffset, dwSize);
			Seek(dwSize, FILE_CURRENT);

			dwReadBytes = dwSize;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			Seek(0, FILE_END);
			dwReadBytes = 0;
		}

		return dwReadBytes;
	}

	virtual ULONG	Write(LPCVOID lpBuff, DWORD dwSize)
	{
		assert(m_lpMemBase != NULL);
		DWORD dwWrittenBytes = 0;

		if ( m_lOffset + dwSize >= m_dwMemSize )
		{
			dwSize = m_dwMemSize - m_lOffset;
		}

		__try
		{
			memcpy((PBYTE)m_lpMemBase + m_lOffset, lpBuff, dwSize);
			Seek(dwSize, FILE_CURRENT);

			dwWrittenBytes = dwSize;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			Seek(0, FILE_END);
			dwWrittenBytes = 0;
		}

		return dwWrittenBytes;
	}
	
	PVOID	GetMapPtr()
	{
		return m_lpMemBase;
	}

protected:
	LPVOID	m_lpMemBase;
	DWORD	m_dwMemSize;
	ULONG	m_lOffset;
	BOOL	m_bRawPtr;
	BOOL	m_bReadOnly;
	DWORD	m_dwFileAttr;
};


class CPEStdFile : public IPEFile
{
public:
	CPEStdFile() 
	{
		m_bReadOnly = TRUE;
		m_bRawPtr = FALSE;
		m_hFile = INVALID_HANDLE_VALUE;
		m_bAttached = FALSE;
		m_dwSize = 0;
		m_dwFileAttr = 0;
	}

	virtual ~CPEStdFile()
	{
		Close();
	}
	
	BOOL	Open(LPCTSTR lpFilePath, BOOL bReadOnly, BOOL bRawPtr)
	{
		if ( lpFilePath == NULL )
		{
			return FALSE;
		}

		if( m_hFile != INVALID_HANDLE_VALUE )
		{
			return FALSE;
		}

		DWORD dwShareMode = FILE_SHARE_READ;
		DWORD dwDesiredAccess = GENERIC_READ;
		if ( !bReadOnly )
		{
			dwDesiredAccess |= GENERIC_WRITE;
			dwShareMode = 0;
		}

		m_bReadOnly = bReadOnly;
		m_bRawPtr = bRawPtr;
		m_bAttached = FALSE;

		m_hFile = CreateFile(lpFilePath, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, 0, NULL);
		if( m_hFile != INVALID_HANDLE_VALUE )
		{
			m_dwFileAttr = GetFileAttributes(lpFilePath);
			m_dwSize = xxGetFileSize(m_hFile);

			return TRUE;
		}

		return FALSE;
	}

	BOOL	Attach(HANDLE hFile, BOOL bReadOnly)
	{
		if ( m_hFile != INVALID_HANDLE_VALUE )
			return FALSE;

		if ( hFile == INVALID_HANDLE_VALUE )
			return FALSE;

		m_bAttached = TRUE;
		m_hFile = hFile;
		m_bReadOnly = bReadOnly;
		m_dwSize = xxGetFileSize(hFile);

		Seek(0, FILE_BEGIN);

		return TRUE;
	}

	void	Detach()
	{
		assert(m_bAttached);

		m_hFile = INVALID_HANDLE_VALUE;
	}

	void	Close()
	{
		if ( m_bAttached )
		{
			Detach();
		}
		else
		{
			if ( m_hFile != INVALID_HANDLE_VALUE )
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}
	}

	virtual BOOL	IsOpen() {
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	virtual	BOOL	IsReadOnly() {
		return m_bReadOnly;
	}

	virtual	BOOL	IsRawPtr() {
		return m_bRawPtr;
	}

	virtual ULONG	GetSize()
	{
		return m_dwSize;
	}

	virtual ULONG	GetAttr()
	{
		return m_dwFileAttr;
	}

	virtual ULONG	Tell()
	{
		assert(m_hFile != INVALID_HANDLE_VALUE);

		return  SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	}

	virtual	ULONG	Seek(LONG lOffset, DWORD dwMethod)
	{
		assert(m_hFile != INVALID_HANDLE_VALUE);

		return SetFilePointer(m_hFile, lOffset, NULL, dwMethod);
	}

	virtual ULONG	Read(LPVOID lpBuff, DWORD dwSize)
	{
		assert(m_hFile != INVALID_HANDLE_VALUE);
		DWORD dwReadBytes = 0;

		if ( ReadFile(m_hFile, lpBuff, dwSize, &dwReadBytes, NULL) )
		{
			return dwReadBytes;
		}

		return 0;
	}

	virtual ULONG	Write(LPCVOID lpBuff, DWORD dwSize)
	{
		assert(m_hFile != INVALID_HANDLE_VALUE);
		DWORD dwWrittenBytes = 0;

		if ( m_bReadOnly )
			return FALSE;
		
		if ( WriteFile(m_hFile, lpBuff, dwSize, &dwWrittenBytes, NULL) )
		{
			return dwWrittenBytes;
		}

		return 0;
	}

	virtual	PVOID	GetMapPtr() {
		return NULL;
	}

protected:
	HANDLE	m_hFile;
	BOOL	m_bAttached;
	DWORD	m_dwSize;
	BOOL	m_bRawPtr;
	BOOL	m_bReadOnly;
	DWORD	m_dwFileAttr;
};

/*
class CPEModFile : public IPEFile
{
public:
	CPEModFile()
	{
		m_bReadOnly = TRUE;
		m_bRawPtr = FALSE;
		m_hProcess = NULL;
		m_lpImageBase = NULL;
		m_dwImageSize = 0;
	}

	virtual ~CPEModFile()
	{
		Close();
	}

	BOOL Open(DWORD dwProcessID, HMODULE hModule, BOOL bReadOnly, BOOL bRawPtr)
	{
		if ( m_hProcess != NULL )
			return FALSE;

		DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
		if ( !bReadOnly )
			dwDesiredAccess |= PROCESS_VM_WRITE;

		m_hProcess = OpenProcess(dwDesiredAccess, FALSE, dwProcessID);
		if ( m_hProcess != NULL )
		{
			MODULEINFO mi = { 0 };

			if ( GetModuleInformation(m_hProcess, hModule, &mi, sizeof (mi)) )
			{
				m_lOffset = 0;
				m_bReadOnly = bReadOnly;
				m_bRawPtr = bRawPtr;
				m_lpImageBase = mi.lpBaseOfDll;
				m_dwImageSize = mi.SizeOfImage;

				return TRUE;
			}

			Close();
		}

		return FALSE;
	}

	BOOL Open(DWORD dwProcessID, LPVOID lpImageBase, DWORD dwImageSize, BOOL bReadOnly, BOOL bRawPtr)
	{
		if ( m_hProcess != NULL )
			return FALSE;
	
		DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
		if ( !bReadOnly )
			dwDesiredAccess |= PROCESS_VM_WRITE;

		m_hProcess = OpenProcess(dwDesiredAccess, FALSE, dwProcessID);
		if ( m_hProcess != NULL )
		{
			m_lOffset = 0;
			m_bReadOnly = bReadOnly;
			m_bRawPtr = bRawPtr;

			m_lpImageBase = lpImageBase;
			m_dwImageSize = dwImageSize;

			return TRUE;
		}

		return FALSE;
	}

	void Close()
	{
		if ( m_hProcess != NULL )
		{
			m_lpImageBase = NULL;
			m_dwImageSize = 0;
			m_lOffset = 0;

			CloseHandle(m_hProcess);
			m_hProcess = NULL;
		}
	}

	virtual BOOL	IsOpen() {
		return (m_hProcess != NULL);
	}

	virtual	BOOL	IsReadOnly() {
		return m_bReadOnly;
	}

	virtual	BOOL	IsRawPtr() {
		return m_bRawPtr;
	}

	virtual ULONG	GetSize()
	{
		return m_dwImageSize;
	}

	virtual ULONG	Tell() 
	{
		assert(m_hProcess != NULL);

		return m_lOffset;
	}

	virtual	ULONG	Seek(LONG lOffset, DWORD dwMethod)
	{
		assert(m_hProcess != NULL);

		switch ( dwMethod )
		{
		case FILE_BEGIN: m_lOffset = lOffset; break;
		case FILE_CURRENT: m_lOffset += lOffset; break;
		case FILE_END: m_lOffset = m_dwImageSize + lOffset; break;
		default:
			return EOF;
		}

		if ( m_lOffset >= m_dwImageSize )
		{
			m_lOffset = m_dwImageSize;
			return EOF;
		}

		return m_lOffset;
	}

	virtual ULONG	Read(LPVOID lpBuff, DWORD dwSize)
	{
		assert(m_hProcess != NULL);
		DWORD dwReadBytes = 0;

		if ( m_lOffset + dwSize >= m_dwImageSize )
		{
			dwSize = m_dwImageSize - m_lOffset;
		}

		PBYTE lpReadOffset = (PBYTE)m_lpImageBase + m_lOffset;
		if ( ReadProcessMemory(m_hProcess, lpReadOffset, lpBuff, dwSize, &dwReadBytes) )
		{
			Seek(dwReadBytes, FILE_CURRENT);
			return dwReadBytes;
		}

		return 0;
	}

	virtual ULONG	Write(LPCVOID lpBuff, DWORD dwSize)
	{
		assert(m_hProcess != NULL);
		DWORD dwWritteBytes = 0;

		if ( m_lOffset + dwSize >= m_dwImageSize )
		{
			dwSize = m_dwImageSize - m_lOffset;
		}

		PBYTE lpWriteOffset = (PBYTE)m_lpImageBase + m_lOffset;
		if ( WriteProcessMemory(m_hProcess, lpWriteOffset, lpBuff, dwSize, &dwWritteBytes) )
		{
			Seek(dwWritteBytes, FILE_CURRENT);
			return dwWritteBytes;
		}

		return 0;
	}

protected:
	HANDLE	m_hProcess;
	LPVOID	m_lpImageBase;
	DWORD	m_dwImageSize;
	ULONG	m_lOffset;
	BOOL	m_bRawPtr;
	BOOL	m_bReadOnly;
};
*/