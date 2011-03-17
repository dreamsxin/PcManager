////////////////////////////////////////////////////////////////////////////////
//      
//      File for perf mon
//      
//      File      : ksimpleperf.h
//      Version   : 1.0
//      Comment   : 简单系统性能监视，获取cpu和内存使用
//      
//      Create at : 2011-2-14
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <winternl.h>

//////////////////////////////////////////////////////////////////////////
typedef struct _SYSTEM_PERFORMANCE_INFORMATION_DETAIL
{
	LARGE_INTEGER liIdleTime;
	LARGE_INTEGER IoReadTransferCount;
	LARGE_INTEGER IoWriteTransferCount;
	LARGE_INTEGER IoOtherTransferCount;
	ULONG IoReadOperationCount;
	ULONG IoWriteOperationCount;
	ULONG IoOtherOperationCount;
	ULONG AvailablePages;
	ULONG CommittedPages;
	ULONG CommitLimit;
	ULONG PeakCommitment;
	ULONG PageFaultCount;
	ULONG CopyOnWriteCount;
	ULONG TransitionCount;
	ULONG CacheTransitionCount;
	ULONG DemandZeroCount;
	ULONG PageReadCount;
	ULONG PageReadIoCount;
	ULONG CacheReadCount;
	ULONG CacheIoCount;
	ULONG DirtyPagesWriteCount;
	ULONG DirtyWriteIoCount;
	ULONG MappedPagesWriteCount;
	ULONG MappedWriteIoCount;
	ULONG PagedPoolPages;
	ULONG NonPagedPoolPages;
	ULONG PagedPoolAllocs;
	ULONG PagedPoolFrees;
	ULONG NonPagedPoolAllocs;
	ULONG NonPagedPoolFrees;
	ULONG FreeSystemPtes;
	ULONG ResidentSystemCodePage;
	ULONG TotalSystemDriverPages;
	ULONG TotalSystemCodePages;
	ULONG NonPagedPoolLookasideHits;
	ULONG PagedPoolLookasideHits;
	ULONG Spare3Count;
	ULONG ResidentSystemCachePage;
	ULONG ResidentPagedPoolPage;
	ULONG ResidentSystemDriverPage;
	ULONG CcFastReadNoWait;
	ULONG CcFastReadWait;
	ULONG CcFastReadResourceMiss;
	ULONG CcFastReadNotPossible;
	ULONG CcFastMdlReadNoWait;
	ULONG CcFastMdlReadWait;
	ULONG CcFastMdlReadResourceMiss;
	ULONG CcFastMdlReadNotPossible;
	ULONG CcMapDataNoWait;
	ULONG CcMapDataWait;
	ULONG CcMapDataNoWaitMiss;
	ULONG CcMapDataWaitMiss;
	ULONG CcPinMappedDataCount;
	ULONG CcPinReadNoWait;
	ULONG CcPinReadWait;
	ULONG CcPinReadNoWaitMiss;
	ULONG CcPinReadWaitMiss;
	ULONG CcCopyReadNoWait;
	ULONG CcCopyReadWait;
	ULONG CcCopyReadNoWaitMiss;
	ULONG CcCopyReadWaitMiss;
	ULONG CcMdlReadNoWait;
	ULONG CcMdlReadWait;
	ULONG CcMdlReadNoWaitMiss;
	ULONG CcMdlReadWaitMiss;
	ULONG CcReadAheadIos;
	ULONG CcLazyWriteIos;
	ULONG CcLazyWritePages;
	ULONG CcDataFlushes;
	ULONG CcDataPages;
	ULONG ContextSwitches;
	ULONG FirstLevelTbFills;
	ULONG SecondLevelTbFills;
	ULONG SystemCalls;
}SYSTEM_PERFORMANCE_INFORMATION_DETAIL;

typedef
NTSTATUS
__stdcall
NtQuerySystemInformationFun (
							 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
							 OUT PVOID SystemInformation,
							 IN ULONG SystemInformationLength,
							 OUT PULONG ReturnLength OPTIONAL
						  );

typedef struct
{
	LARGE_INTEGER BootTime;
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER TimeZoneBias;
	ULONG uCurrentTimeZoneId;
	DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;


class KSimplePerfMgr
{
public:
	KSimplePerfMgr();
	~KSimplePerfMgr();

	HRESULT	Init();
	HRESULT	UnInit();
	DWORD GetSysMemUsage();
	DWORD GetSysCpuUsage();

	VOID	Update();

private:
	void	UpdateSystemCpuAndMem();

private:
	DWORD									m_dwMemUsage;
	DWORD									m_dwCpuUsage;

	SYSTEM_PERFORMANCE_INFORMATION_DETAIL	m_LastInfo;
	SYSTEM_TIME_INFORMATION					m_LastTime;
	DWORD                                   m_dwLastTickCount;

	HANDLE									m_hModule;
	NtQuerySystemInformationFun*			m_pfnNtQuerySystemInformation;
	DWORD                                   m_dwNumberProcessors;
};