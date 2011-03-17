////////////////////////////////////////////////////////////////////////////////
//      
//      File for url moniter
//      
//      File      : netstastic.h
//      Version   : 1.0
//      Comment   : 流量统计数据结构定义
//      
//      Create at : 2011-1-5
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "netfluxdef.h"
#include "../common/sharememlock.h"

struct KFluxStasticProcItem
{
	DWORD					m_nCbSize;
	WCHAR					m_strProcPath[MAX_PATH];// 进程路径
	ULONGLONG				m_nTotalRecv;			// 总计接受的流量
	ULONGLONG				m_nTotalSend;			// 总计发送的流量
};

struct KStasticFluxProcessList
{
	DWORD					m_nSize;
	ShareMemLock			m_lock;
	ULONGLONG				m_nTotalRecv;			// 总计接受的流量
	ULONGLONG				m_nTotalSend;			// 总计发送的流量
	__int64					m_nTimeWatch;			// 监控时间
	__int64					m_nTimeTodayStart;		// 今天截止时间点
	__int64					m_nTimeTodayLastTime;	// 今天截止时间点

	DWORD					m_nMaxCnt;
	DWORD					m_nCurrentCnt;
	DWORD					m_nReserved[100];
	KFluxStasticProcItem	m_Items[1];
};

#define NET_FLEX_STASTIC_FILE_MAGIC		('NFS')
#define NET_FLEX_STASTIC_FILE_FILE		((1 << 16) | 1)

typedef struct _tagNetStasticFluxCacheFileHeader
{
	DWORD			m_nMagic;					// 'NET_FLEX_STASTIC_FILE_MAGIC'
	DWORD			m_nFileVersion;				// 文件版本 NET_FLEX_STASTIC_FILE_FILE
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];			// 数据offsetDir
}NetFluxStasticCacheFileHeader;

#define NET_FLUX_STASTIC_DATA_CACHE		0		// 第一个是cfg

#define NET_FLUX_STASTIC_MAPDATA_NAME		(TEXT("Global\\E7FBFB64-A045-4a9f-A057-9A88B439CCD3"))

class KNetFluxStasticCacheReader
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;

public:
	KNetFluxStasticCacheReader():m_hFileMap(NULL),
		m_pBufView(NULL)
	{
	}

	~KNetFluxStasticCacheReader()
	{
		Uninit();
	}

	HRESULT	Init()
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		SIZE_T sizeToMap = {0};

		hFileMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NET_FLUX_STASTIC_MAPDATA_NAME);
		if (NULL == hFileMap)
		{
			hFileMap = ::OpenFileMapping(FILE_MAP_READ, FALSE, NET_FLUX_STASTIC_MAPDATA_NAME);
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

	KStasticFluxProcessList*	GetStasticFluxList()
	{
		if (m_pBufView)
		{
			return (KStasticFluxProcessList*)((BYTE*)m_pBufView + ((NetFluxCacheFileHeader*)(m_pBufView))->m_nDataOffset[NET_FLUX_STASTIC_DATA_CACHE]);
		}
		return NULL;
	}
};


