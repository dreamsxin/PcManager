////////////////////////////////////////////////////////////////////////////////
//      
//      File for url moniter
//      
//      File      : urlmondef.h
//      Version   : 1.0
//      Comment   : 木马网络防火墙一期ipc通信的接口定义
//      
//      Create at : 2010-10-8
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////////
// 简单的ipc通信
struct UrlMonSendUrlNotifyParam
{
	WCHAR		m_strProcessCmdLine[MAX_PATH];				// 进程的cmdline
	char		m_strUrl[1024];								// 
	BYTE		m_md5[16];
	DWORD		m_nPID;										// ProcessID
	DWORD		m_nGroupID;									// GroupID 标识同一个程序的数
	DWORD		m_nSecurityState;							// 水银的安装状态，0=gray,3=black
	DWORD		m_nUrlMode;									// 1： 表示Get，2:表示Post
};

struct UrlMonRefreshWhiteList
{
	
};

struct FluxSetProcessSpeed
{
	ULONG_PTR	nProcessID;
	ULONG		nSendLimit;
	ULONG		nRecvLimit;
	ULONG		nDisable;
	ULONG		bRemember;
};

struct FluxSetProcUnPop
{
	WCHAR		m_ProcPath[MAX_PATH];
	ULONGLONG	m_nModuleID;
	BOOL		m_bUnPop;
};

struct ProcessDisableNetPopInfo
{
	ULONGLONG				m_nCreateID;
	ULONGLONG				m_nProcessID;
	ULONGLONG				m_nModuleID;
	DWORD					m_nTrustMode;
	WCHAR					m_strProcPath[MAX_PATH];
};

struct UrlMonIpcParam
{
	DWORD		m_nCallType;
	union
	{
		UrlMonSendUrlNotifyParam	m_SendUrl;
		UrlMonRefreshWhiteList		m_RefreshWhite;
		FluxSetProcessSpeed			m_SetSpeed;
		ProcessDisableNetPopInfo	m_PopInfo;
		FluxSetProcUnPop			m_UnPop;
	};
};

enum UrlMonCall
{
	Ipc_UrlMon_RefreshWhiteList = 1,
	Ipc_UrlMon_SendUrl			= 2,
	Ipc_NetFlux_ClearDumpStateAndStartUp			= 3,
	Ipc_NetFlux_SetProcessSpeed			= 4,
	Ipc_NetFux_GetPopInfo				= 5,
	Ipc_NetFux_SetUnPop				= 6,
};

#define IPC_PROC_OnUrlSend		TEXT("OnUrlSend")
#define IPC_PROC_URLMON			TEXT("UrlMonIpc")

//////////////////////////////////////////////////////////////////////////

#define URL_MON_CFG_FILE_MAGIC		('ULM')
#define URL_MON_CFG_FILE_FILE		((1 << 16) | 1)

typedef struct __tagUrlMonCfg
{
	DWORD			m_nSize;
	DWORD			m_bEnableUrlMon;		// 是否允许监控
	ULONGLONG		m_nModuleScanCnt;		// 扫描模块的个数
	ULONGLONG		m_nUrlScanCnt;			// 扫描url的个数
	ULONGLONG		m_nBlockUrlCnt;			// 拦截到的黑url的个数
	__int64			m_nLastUrlBlockTime;	// 上次扫描的时间

	DWORD			m_bDisableFluxStastic;	// 是否不允许进行流量统计
	DWORD			m_bDisableDelayScan;	// 关闭开机5分钟不扫描
	DWORD			m_nReserved[99];
}UrlMonCfg;

typedef struct _tagUrlMonCfgFileHeader
{
	DWORD			m_nMagic;				// 'URL_MON_CFG_FILE_MAGIC'
	DWORD			m_nFileVersion;			// 文件版本
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];		// 数据offsetDir
}UrlMonCfgFileHeader;

#define URL_MON_FILE_DATA_CFG		0		// 第一个是cfg

#define URL_MON_MAPDATA_NAME		(TEXT("Global\\6F6B07A7-30DA-4049-9E51-6124F8D51002"))

#define URL_MON_POP					800

//////////////////////////////////////////////////////////////////////////
enum UrlMonUrlSecurity
{
	Url_Unknown			= 0,
	Url_Gray			= 1,
	Url_Safe			= 2,
	Url_Danger			= 3,
};

//////////////////////////////////////////////////////////////////////////
class KUrlMonCfgReader
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;

public:
	KUrlMonCfgReader():m_hFileMap(NULL),
		m_pBufView(NULL)
	{
	}

	~KUrlMonCfgReader()
	{
		Uninit();
	}

	HRESULT	Init()
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		SIZE_T sizeToMap = {0};

		hFileMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, URL_MON_MAPDATA_NAME);
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

	ULONGLONG	GetModuleScanCnt()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return 0;
		return pCfg->m_nModuleScanCnt;
	}
	ULONGLONG	GetBlockUrlCnt()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return 0;
		return pCfg->m_nBlockUrlCnt;
	}

	__int64		GetLastBlockTime()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return 0;
		return pCfg->m_nLastUrlBlockTime;
	}

	BOOL		GetEnable()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return FALSE;
		return pCfg->m_bEnableUrlMon;
	}

	BOOL		SetEnable(BOOL bEnable)
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return FALSE;
		pCfg->m_bEnableUrlMon = bEnable;
		return TRUE;
	}

	BOOL		GetEnableFluxStastic()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return TRUE;
		return !pCfg->m_bDisableFluxStastic;
	}

	BOOL		SetEnableFluxStastic(BOOL bEnable)
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return FALSE;
		pCfg->m_bDisableFluxStastic = !bEnable;
		return TRUE;
	}

	BOOL		GetDisableDelayScan()
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return TRUE;
		return pCfg->m_bDisableDelayScan;
	}

	BOOL		SetDisableDelayScan(BOOL bEnable)
	{
		UrlMonCfg* pCfg = GetCfg();
		if (!pCfg) return FALSE;
		pCfg->m_bDisableDelayScan = bEnable;
		return TRUE;
	}

private:
	UrlMonCfg* GetCfg()
	{
		if (m_pBufView)
		{
			return (UrlMonCfg*)((BYTE*)m_pBufView + ((UrlMonCfgFileHeader*)(m_pBufView))->m_nDataOffset[URL_MON_FILE_DATA_CFG]);
		}
		return NULL;
	}
};
