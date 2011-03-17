////////////////////////////////////////////////////////////////////////////////
//      
//      File for url moniter
//      
//      File      : netfluxdef.h
//      Version   : 1.0
//      Comment   : 流量控制数据结构定义
//      
//      Create at : 2010-10-8
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../common/sharememlock.h"
#include<vector>

#ifndef DEFFLOWMODENAME			
#define DEFFLOWMODENAME _T("{990CC23C-6E4D-4be7-9E2D-F5791768B038}_flow")
#endif

enum enumProcessMode
{
	enumProcessModeHasNetFlow		= 1, //只返回存在网络流量的进程
	enumProcessModeHasLocationFlow	= 2, //只返回存在本地流量的进程
	enumProcessModeHasAll			= 3, //返回所有流量的进程
	enumProcessModeDisabled			= 4  //返回已被禁止网络访问的进程
};

struct KPFWFLUX
{
	// 接收速度
	ULONGLONG	nRecvSpeed;

	// 发送速度
	ULONGLONG	nSendSpeed;

	// 总计接受的流量
	ULONGLONG	nTotalRecv;

	// 总计发送的流量
	ULONGLONG	nTotalSend;

	// 本地接收速度
	ULONGLONG	nRecvSpeedLocal;

	// 本地发送速度
	ULONGLONG	nSendSpeedLocal;

	// 本地总计接受的流量
	ULONGLONG	nTotalRecvLocal;

	// 本地总计发送的流量
	ULONGLONG	nTotalSendLocal;
};

struct KProcFluxItem
{
	DWORD					m_nCbSize;
	WCHAR					m_strProcPath[MAX_PATH];// 进程路径
	ULONG_PTR				m_nProcessID;			// 进程ID
	ULONGLONG				m_nCreateID;			// 进程创建ID
	DWORD					m_nTrustMode;			// 进程的安全性
	KPFWFLUX				m_nFlux;				// 流量信息
	DWORD					m_nCanNotKill;			// 是否可以杀掉进程
	DWORD					m_nSendLimit;			// 发送限速
	DWORD					m_nRecvLimit;			// 接收限速
	DWORD					m_nDisable;				// 禁止访问
	DWORD					m_bAutoFixed;			// 是否自动限速的
	DWORD					m_nReserved[5];
};

struct KProcessFluxList
{
	DWORD					m_nSize;
	ShareMemLock			m_lock;
	KPFWFLUX				m_SysFlux;
	DWORD					m_nMaxCnt;
	DWORD					m_nCurrentCnt;
	DWORD					m_nProcessPopCount;
	DWORD					m_nReserved[99];
	KProcFluxItem			m_Items[1];
};

#define NET_FLEX_FILE_MAGIC		('NFX')
#define NET_FLEXFILE_FILE		((1 << 16) | 1)

typedef struct _tagNetFluxCacheFileHeader
{
	DWORD			m_nMagic;				// 'NET_FLEX_FILE_MAGIC'
	DWORD			m_nFileVersion;			// 文件版本 NET_FLEXFILE_FILE
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];		// 数据offsetDir
}NetFluxCacheFileHeader;

#define NET_FLUX_DATA_CACHE		0		// 第一个是cfg

#define NET_FLUX_MAPDATA_NAME		(TEXT("Global\\88C620CF-5D6D-4873-8673-D85BA2499B05"))

//////////////////////////////////////////////////////////////////////////
class KNetFluxCacheReader
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;
	HANDLE				m_hFileMapInfo;
	LPVOID				m_pBufViewInfo;
public:
	KNetFluxCacheReader():m_hFileMap(NULL),
		m_pBufView(NULL),
		m_hFileMapInfo(NULL),
		m_pBufViewInfo(NULL)
	{
	}

	~KNetFluxCacheReader()
	{
		Uninit();
	}

public:

	HRESULT	Init()
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		HANDLE hFileMapInfo = NULL;
		BYTE* pMapBufInfo = NULL;
		SIZE_T sizeToMap = {0};

		if (m_hFileMap != NULL && m_pBufView != NULL && m_hFileMapInfo != NULL && m_pBufViewInfo != NULL)
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

			if (m_pBufViewInfo)
			{
				::UnmapViewOfFile(m_pBufViewInfo);
				m_pBufViewInfo = NULL;
			}

			if (m_hFileMapInfo)
			{
				::CloseHandle(m_hFileMapInfo);
				m_hFileMapInfo = NULL;
			}
		}

		hFileMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NET_FLUX_MAPDATA_NAME);
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


		hFileMapInfo = ::CreateFileMapping((HANDLE)0xFFFFFFFF, 
											NULL, 
											PAGE_READWRITE, 
											0, 
											sizeof(int), 
											DEFFLOWMODENAME);
		if (NULL == hFileMapInfo)
		{
			printf("CreateFileMapping failed: %d\n", ::GetLastError());
			goto exit0;
		}

		pMapBufInfo = (BYTE*)::MapViewOfFile(hFileMapInfo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (!pMapBufInfo)
		{
			printf("MapViewOfFile failed: %d\n", ::GetLastError());
			goto exit0;
		}

		m_hFileMapInfo = hFileMapInfo;
		m_pBufViewInfo = pMapBufInfo;
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

		if (pMapBufInfo)
		{
			::UnmapViewOfFile(pMapBufInfo);
			pMapBufInfo = NULL;
		}

		if (hFileMapInfo)
		{
			::CloseHandle(hFileMapInfo);
			hFileMapInfo = NULL;
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

		if (m_pBufViewInfo && m_hFileMapInfo)
		{
			::UnmapViewOfFile(m_pBufViewInfo);
			m_pBufView = NULL;
		}
		if (m_hFileMapInfo)
		{
			::CloseHandle(m_hFileMapInfo);
			m_hFileMapInfo = NULL;
		}
		return S_OK;
	}

	KProcessFluxList*	GetFluxList()
	{
		if (m_pBufView)
		{
			return (KProcessFluxList*)((BYTE*)m_pBufView + ((NetFluxCacheFileHeader*)(m_pBufView))->m_nDataOffset[NET_FLUX_DATA_CACHE]);
		}
		return NULL;
	}

	static KNetFluxCacheReader& Instance()
	{
		static KNetFluxCacheReader _ins;
		return	_ins;
	}

	BOOL SetProcessFlowMode(IN int nProcessMode)
	{
		if (m_pBufViewInfo)
		{
			memcpy(m_pBufViewInfo, (void *)&nProcessMode, sizeof(int));
			return TRUE;
		}

		return FALSE;
	}

	int GetProcessFlowMode(void)
	{
		if (m_pBufViewInfo != NULL)
			return (*((int *)m_pBufViewInfo));
		
		return enumProcessModeHasAll;
	}

	// 获取系统的流量信息
	BOOL GetSystemNetFlow(KPFWFLUX& sysFlux)
	{
		
		BOOL bRet = _GetProcessesFluxInfo(sysFlux, NULL);
		int nMode = GetProcessFlowMode();
		if (nMode == enumProcessModeHasNetFlow)
		{
			sysFlux.nTotalRecv = sysFlux.nTotalRecv - sysFlux.nTotalRecvLocal;
			sysFlux.nTotalSend = sysFlux.nTotalSend - sysFlux.nTotalSendLocal;
			sysFlux.nSendSpeed = sysFlux.nSendSpeed - sysFlux.nSendSpeedLocal;
			sysFlux.nRecvSpeed = sysFlux.nRecvSpeed - sysFlux.nRecvSpeedLocal;
		}
		else if (nMode == enumProcessModeHasLocationFlow)
		{
			sysFlux.nTotalRecv = sysFlux.nTotalRecvLocal;
			sysFlux.nTotalSend = sysFlux.nTotalSendLocal;
			sysFlux.nSendSpeed = sysFlux.nSendSpeedLocal;
			sysFlux.nRecvSpeed = sysFlux.nRecvSpeedLocal;
		}

		return bRet;
	}


	// 获取系统和当前进程的流量信息
	BOOL GetProcessesFluxInfo(KPFWFLUX& sysFlux, std::vector<KProcFluxItem>* pProcessesList, int nProcessMode = enumProcessModeHasAll)
	{
		if (SUCCEEDED(Init()))
			SetProcessFlowMode(nProcessMode);

		return _GetProcessesFluxInfo(sysFlux, pProcessesList, nProcessMode);
		
	}

	// 获取系统和当前进程的流量信息
	BOOL _GetProcessesFluxInfo(KPFWFLUX& sysFlux, std::vector<KProcFluxItem>* pProcessesList, int nProcessMode = enumProcessModeHasAll)
	{
		BOOL result = FALSE;

		if (SUCCEEDED(Init()))
		{
			HRESULT hr = 0;
			KProcessFluxList* pList = GetFluxList();
			BYTE* pBuf = NULL;

			// 为保证快速读取，并且释放锁，这里先使用内存拷贝的方法
			pList->m_lock.LockRead();

			sysFlux = pList->m_SysFlux;

			if (pProcessesList != NULL)
			{

				pProcessesList->clear();
				KProcFluxItem* pItem = &(pList->m_Items[0]);
				DWORD nCnt = pList->m_nCurrentCnt;

				if (nCnt > pList->m_nMaxCnt)
					nCnt = pList->m_nMaxCnt;

				DWORD nSize = sizeof(KProcFluxItem) * nCnt;
				pBuf = new BYTE[nSize];

				if (pBuf)
					memcpy(pBuf, pItem, nSize);

				pList->m_lock.UnLockRead();

				if (pBuf)
				{
					KProcFluxItem* pItem = (KProcFluxItem*)pBuf;

					for (DWORD i = 0; i < nCnt; i++, pItem++)
					{
						if (nProcessMode == enumProcessModeHasNetFlow)
						{
							if (pItem->m_nFlux.nTotalRecv - pItem->m_nFlux.nTotalRecvLocal <= 0)
								continue;
							else
							{
								pItem->m_nFlux.nTotalRecv = pItem->m_nFlux.nTotalRecv - pItem->m_nFlux.nTotalRecvLocal;
								pItem->m_nFlux.nTotalSend = pItem->m_nFlux.nTotalSend - pItem->m_nFlux.nTotalSendLocal;
								pItem->m_nFlux.nSendSpeed = pItem->m_nFlux.nSendSpeed - pItem->m_nFlux.nSendSpeedLocal;
								pItem->m_nFlux.nRecvSpeed = pItem->m_nFlux.nRecvSpeed - pItem->m_nFlux.nRecvSpeedLocal;
							}
						}
						else if (nProcessMode == enumProcessModeHasLocationFlow)
						{
							if (pItem->m_nFlux.nTotalRecvLocal <= 0)
								continue;
							else
							{
								pItem->m_nFlux.nTotalRecv = pItem->m_nFlux.nTotalRecvLocal;
								pItem->m_nFlux.nTotalSend = pItem->m_nFlux.nTotalSendLocal;
								pItem->m_nFlux.nSendSpeed = pItem->m_nFlux.nSendSpeedLocal;
								pItem->m_nFlux.nRecvSpeed = pItem->m_nFlux.nRecvSpeedLocal;
							}
						}
						else if (nProcessMode == enumProcessModeDisabled)
						{
							if (pItem->m_nDisable == 0)
								continue;
							else
							{
								pItem->m_nFlux.nTotalRecv = pItem->m_nFlux.nTotalRecvLocal;
								pItem->m_nFlux.nTotalSend = pItem->m_nFlux.nTotalSendLocal;
								pItem->m_nFlux.nSendSpeed = pItem->m_nFlux.nSendSpeedLocal;
								pItem->m_nFlux.nRecvSpeed = pItem->m_nFlux.nRecvSpeedLocal;
							}
						}

						KProcFluxItem processinfo;

						memcpy(&processinfo, pItem, sizeof(KProcFluxItem));

						pProcessesList->push_back( processinfo );
					}

					if (nProcessMode == enumProcessModeHasNetFlow)
					{
						sysFlux.nTotalRecv = sysFlux.nTotalRecv - sysFlux.nTotalRecvLocal;
						sysFlux.nTotalSend = sysFlux.nTotalSend - sysFlux.nTotalSendLocal;
						sysFlux.nSendSpeed = sysFlux.nSendSpeed - sysFlux.nSendSpeedLocal;
						sysFlux.nRecvSpeed = sysFlux.nRecvSpeed - sysFlux.nRecvSpeedLocal;
					}
					else if (nProcessMode == enumProcessModeHasLocationFlow)
					{
						sysFlux.nTotalRecv = sysFlux.nTotalRecvLocal;
						sysFlux.nTotalSend = sysFlux.nTotalSendLocal;
						sysFlux.nSendSpeed = sysFlux.nSendSpeedLocal;
						sysFlux.nRecvSpeed = sysFlux.nRecvSpeedLocal;
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

//__declspec(selectany) LPVOID KNetFluxCacheReader::m_pBufViewInfo = NULL;