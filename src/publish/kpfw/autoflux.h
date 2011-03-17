////////////////////////////////////////////////////////////////////////////////
//      
//      File for url moniter
//      
//      File      : autoflux.h
//      Version   : 1.0
//      Comment   : 流量监控自动监控文件保存
//      
//      Create at : 2011-1-5
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../common/sharememlock.h"

struct KAutoFluxItem
{
	DWORD					m_nCbSize;
	WCHAR					m_strProcPath[MAX_PATH];// 进程路径
	DWORD					m_bAutoFlux;			// 是否自动记住限速
	DWORD					m_nSendLimit;			// 发送限速
	DWORD					m_nRecvLimit;			// 接收限速
	DWORD					m_bDisable;				// 禁止访问网络
	DWORD					m_bUnPopOnBlock;		// 禁止网络不弹出泡泡
	DWORD					m_nReserved[5];			
};

struct KAutoFluxList
{
	DWORD					m_nSize;
	ShareMemLock			m_lock;

	DWORD					m_nMaxCnt;
	DWORD					m_nCurrentCnt;
	DWORD					m_nReserved[100];
	KAutoFluxItem			m_Items[1];
};

#define NET_AUTO_FLEX_FILE_MAGIC		('AFF')
#define NET_AUTO_FLEX_FILE_VERSIN		((1 << 16) | 1)

typedef struct _tagAutoFluxCacheFileHeader
{
	DWORD			m_nMagic;					// 'NET_AUTO_FLEX_FILE_MAGIC'
	DWORD			m_nFileVersion;				// 文件版本 NET_AUTO_FLEX_FILE_VERSIN
	DWORD			m_nDataDirs;
	DWORD			m_nDataOffset[20];			// 数据offsetDir
}AutoFluxCacheFileHeader;

#define NET_AUTO_FLEX_DATA_CACHE		0		// 第一个是cfg

#define NET_AUTO_FLEX_MAPDATA_NAME		(TEXT("Global\\58E5D084-6ADA-4a3d-9DA6-882F644DE030"))
