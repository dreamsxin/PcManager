////////////////////////////////////////////////////////////////////////////////
//      
//      File for performance monitor
//      
//      File      : perfmondef.h
//      Version   : 1.0
//      Comment   : 性能监视相关数据
//      
//      Create at : 2011-1-27
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../common/sharememlock.h"

// 进程使用
struct KProcPerfItem
{
	DWORD					m_nCbSize;
	WCHAR					m_strProcPath[MAX_PATH];// 进程路径
	DWORD					m_nProcessID;			// 进程ID
	ULONGLONG				m_nCreateID;			// 进程创建ID
	DWORD					m_nTrustMode;			// 安全信息
	double					m_nCpuUsage;			// cpu占用率（%）
	ULONGLONG				m_nMemUsage;			// 内存使用
	ULONGLONG				m_nIoCount;				// 磁盘使用 读写次/秒
	DWORD					m_nReserved[20];		// 保留的
};

struct KPocessPerfList
{
	DWORD					m_nSize;
	ShareMemLock			m_lock;
													// 这里可以放system的信息
	DWORD					m_nMaxCnt;
	DWORD					m_nCurrentCnt;
	DWORD					m_nReserved[100];
	KProcPerfItem			m_Items[1];
};

#define PROC_PERF_MON_FILE_MAGIC		('PER')
#define PROC_PERF_MON_FILE_FILE			((1 << 16) | 1)

typedef struct _tagProcPerfMonCacheFileHeader
{
	DWORD			m_nMagic;					// 'PROC_PERF_MON_FILE_MAGIC'
	DWORD			m_nFileVersion;				// 文件版本 PROC_PERF_MON_FILE_FILE
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];			// 数据offsetDir
}ProcPerfMonCacheFileHeader;

#define PROC_PERF_MON_DATA_CACHE		0		// 第一个是cfg

#define PROC_PERF_MON_MAPDATA_NAME		(TEXT("Global\\8FB628AC-11D5-4f91-94A2-F011AA939A5F"))

class KProcPerfMonCacheReader
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;

public:
	KProcPerfMonCacheReader():m_hFileMap(NULL),
		m_pBufView(NULL)
	{
	}

	~KProcPerfMonCacheReader()
	{
		Uninit();
	}

	HRESULT	Init()
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		SIZE_T sizeToMap = {0};

		hFileMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, PROC_PERF_MON_MAPDATA_NAME);
		if (NULL == hFileMap)
		{
			hFileMap = ::OpenFileMapping(FILE_MAP_READ, FALSE, PROC_PERF_MON_MAPDATA_NAME);
			if (NULL == hFileMap)
			{
				printf("OpenFileMapping failed: %d\n", ::GetLastError());
				goto exit0;
			}

			pMapBuf = (BYTE*)::MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);		
			if (!pMapBuf)
			{
				printf("MapViewOfFile failed: %d\n", ::GetLastError());
				goto exit0;
			}
		}
		else
		{
			pMapBuf = (BYTE*)::MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			if (!pMapBuf)
			{
				printf("MapViewOfFile failed: %d\n", ::GetLastError());
				goto exit0;
			}
		}


		m_hFileMap = hFileMap;
		m_pBufView = pMapBuf;
		return S_OK;

exit0:
		if (pMapBuf)
		{
			::UnmapViewOfFile(pMapBuf);
			pMapBuf = NULL;
		}

		if (hFileMap)
		{
			::CloseHandle(hFileMap);
			hFileMap = NULL;
		}

		return E_FAIL;
	}

	HRESULT	Uninit()
	{
		if (m_pBufView && m_hFileMap)
		{
			::UnmapViewOfFile(m_pBufView);
			m_pBufView = NULL;
		}
		if (m_hFileMap)
		{
			::CloseHandle(m_hFileMap);
			m_hFileMap = NULL;
		}
		return S_OK;
	}

	KPocessPerfList*	GetProcessPerfList()
	{
		if (m_pBufView)
		{
			return (KPocessPerfList*)((BYTE*)m_pBufView + ((ProcPerfMonCacheFileHeader*)(m_pBufView))->m_nDataOffset[PROC_PERF_MON_DATA_CACHE]);
		}
		return NULL;
	}
};


