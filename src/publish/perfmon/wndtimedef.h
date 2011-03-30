#pragma once

#include "../common/sharememlock.h"
#include<vector>

struct KWndTimeItem
{
	HWND	hWnd;			//窗口句柄
	DWORD	dwUnActiveTime;	//窗口未活动时间
};

struct KWndTimeList
{
	DWORD					m_nSize;
	ShareMemLock			m_lock;
	DWORD					m_nMaxCnt;
	DWORD					m_nCurrentCnt;
	DWORD					m_nReserved[99];
	KWndTimeItem			m_Items[1];
};

typedef struct _tagWndTimeCacheFileHeader
{
	DWORD			m_nMagic;				// 'WND_TIME_FILE_MAGIC'
	DWORD			m_nFileVersion;			// 文件版本 WND_TIME_FILE_FILE
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];		// 数据offsetDir
}WndTimeCacheFileHeader;

#define WND_TIME_FILE_MAGIC		('WNDT')
#define WND_TIME_FILE_VER		((1 << 16) | 1)
#define WND_TIME_DATA_CACHE		0		// 第一个是cfg
#define WND_TIME_MAPDATA_NAME	(TEXT("Global\\C0174769-14BF-450f-A26B-81D1CAE0B817"))

class KWndTimeCacheReader
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;
public:
	KWndTimeCacheReader():m_hFileMap(NULL),
		m_pBufView(NULL)
	{
	}

	~KWndTimeCacheReader()
	{
		Uninit();
	}

public:

	HRESULT	Init()
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		
		SIZE_T sizeToMap = {0};

		if (m_hFileMap != NULL && m_pBufView != NULL)
			return S_OK;
		else
		{
			if (m_pBufView)
			{
				::UnmapViewOfFile(m_pBufView);
				m_pBufView = NULL;
			}

			if (m_hFileMap)
			{
				::CloseHandle(m_hFileMap);
				m_hFileMap = NULL;
			}
		}

		hFileMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, WND_TIME_MAPDATA_NAME);
		if (NULL == hFileMap)
		{
			printf("OpenFileMapping failed: %d\n", ::GetLastError());
			goto exit0;
		}

		pMapBuf = (BYTE*)::MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (!pMapBuf)
		{
			printf("MapViewOfFile failed: %d\n", ::GetLastError());
			goto exit0;
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

	KWndTimeList*	GetWndTimeList()
	{
		if (m_pBufView)
		{
			return (KWndTimeList*)((BYTE*)m_pBufView + ((WndTimeCacheFileHeader*)(m_pBufView))->m_nDataOffset[WND_TIME_DATA_CACHE]);
		}
		return NULL;
	}

	// 获取当前所有顶层窗口的未活动时间
	BOOL GetWndTimeInfo(std::vector<KWndTimeItem>* pWndTimeList)
	{
		BOOL result = FALSE;

		if (SUCCEEDED(Init()))
		{
			HRESULT hr = 0;
			KWndTimeList* pList = GetWndTimeList();
			BYTE* pBuf = NULL;

			// 为保证快速读取，并且释放锁，这里先使用内存拷贝的方法
			pList->m_lock.LockRead();

			if (pWndTimeList != NULL)
			{

				pWndTimeList->clear();
				KWndTimeItem* pItem = &(pList->m_Items[0]);
				DWORD nCnt = pList->m_nCurrentCnt;

				if (nCnt > pList->m_nMaxCnt)
					nCnt = pList->m_nMaxCnt;

				DWORD nSize = sizeof(KWndTimeItem) * nCnt;
				pBuf = new BYTE[nSize];

				if (pBuf)
					memcpy(pBuf, pItem, nSize);

				pList->m_lock.UnLockRead();

				if (pBuf)
				{
					KWndTimeItem* pItem = (KWndTimeItem*)pBuf;

					for (DWORD i = 0; i < nCnt; i++, pItem++)
					{
						KWndTimeItem wndTimeInfo;
						memcpy(&wndTimeInfo, pItem, sizeof(KWndTimeItem));
						pWndTimeList->push_back( wndTimeInfo );
					}
					delete[] pBuf;
					pBuf = NULL;
				}
			}
			else
			{
				pList->m_lock.UnLockRead();
			}

			result = TRUE;
		}

		return result;
	}
};