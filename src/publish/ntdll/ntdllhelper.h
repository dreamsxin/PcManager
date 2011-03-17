
#pragma once

#include "winmod/winmodule.h"

namespace NT
{

#define NT_DLL_NAME		L"\\system32\\ntdll.dll"

typedef struct _IO_COUNTERS {
	ULONGLONG  ReadOperationCount;
	ULONGLONG  WriteOperationCount;
	ULONGLONG  OtherOperationCount;
	ULONGLONG ReadTransferCount;
	ULONGLONG WriteTransferCount;
	ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;

//
// Process Virtual Memory Counters
//  NtQueryInformationProcess using ProcessVmCounters
//

typedef struct _VM_COUNTERS {
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} VM_COUNTERS;
typedef VM_COUNTERS *PVM_COUNTERS;

typedef struct _VM_COUNTERS_EX {
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivateUsage;
} VM_COUNTERS_EX;
typedef VM_COUNTERS_EX *PVM_COUNTERS_EX;


typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,          // Note: this is kernel mode only
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessWow64Information,
	ProcessImageFileName,
	ProcessLUIDDeviceMapsEnabled,
	ProcessBreakOnTermination,
	ProcessDebugObjectHandle,
	ProcessDebugFlags,
	ProcessHandleTracing,
	ProcessIoPriority,
	ProcessExecuteFlags,
	ProcessResourceManagement,
	ProcessCookie,
	ProcessImageInformation,
	MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
} PROCESSINFOCLASS;


typedef LONG ( WINAPI *pfnZwQueryInformationProcess)(
							HANDLE ProcessHandle,
							PROCESSINFOCLASS ProcessInformationClass,
							PVOID ProcessInformation,
							ULONG ProcessInformationLength,
							PULONG ReturnLength
);


typedef struct 
{ 
	DWORD dwUnknown1; 
	ULONG uKeMaximumIncrement; 
	ULONG uPageSize; 
	ULONG uMmNumberOfPhysicalPages; 
	ULONG uMmLowestPhysicalPage; 
	ULONG uMmHighestPhysicalPage; 
	ULONG uAllocationGranularity; 
	PVOID pLowestUserAddress; 
	PVOID pMmHighestUserAddress; 
	ULONG uKeActiveProcessors; 
	BYTE bKeNumberProcessors; 
	BYTE bUnknown2; 
	WORD wUnknown3; 
} SYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION
{
	LARGE_INTEGER  IdleTime;                    //CPU空闲时间；
	LARGE_INTEGER  ReadTransferCount;           //I/O读操作数目；
	LARGE_INTEGER  WriteTransferCount;          //I/O写操作数目；
	LARGE_INTEGER  OtherTransferCount;          //I/O其他操作数目；
	ULONG          ReadOperationCount;          //I/O读数据数目；
	ULONG          WriteOperationCount;         //I/O写数据数目；
	ULONG          OtherOperationCount;         //I/O其他操作数据数目；
	ULONG          AvailablePages;              //可获得的页数目；
	ULONG          TotalCommittedPages;         //总共提交页数目；
	ULONG          TotalCommitLimit;            //已提交页数目；
	ULONG          PeakCommitment;              //页提交峰值；
	ULONG          PageFaults;                  //页故障数目；
	ULONG          WriteCopyFaults;             //Copy-On-Write故障数目；
	ULONG          TransitionFaults;            //软页故障数目；
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //需求0故障数；
	ULONG          PagesRead;                   //读页数目；
	ULONG          PageReadIos;                 //读页I/O操作数；
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //已写页文件页数；
	ULONG          PagefilePageWriteIos;        //已写页文件操作数；
	ULONG          MappedFilePagesWritten;      //已写映射文件页数；
	ULONG          MappedFileWriteIos;          //已写映射文件操作数；
	ULONG          PagedPoolUsage;              //分页池使用；
	ULONG          NonPagedPoolUsage;           //非分页池使用；
	ULONG          PagedPoolAllocs;             //分页池分配情况；
	ULONG          PagedPoolFrees;              //分页池释放情况；
	ULONG          NonPagedPoolAllocs;          //非分页池分配情况；
	ULONG          NonPagedPoolFress;           //非分页池释放情况；
	ULONG          TotalFreeSystemPtes;         //系统页表项释放总数；
	ULONG          SystemCodePage;              //操作系统代码页数；
	ULONG          TotalSystemDriverPages;      //可分页驱动程序页数；
	ULONG          TotalSystemCodePages;        //操作系统代码页总数；
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                   
	ULONG          MmSystemCachePage;          //系统缓存页数；
	ULONG          PagedPoolPage;              //分页池页数；
	ULONG          SystemDriverPage;           //可分页驱动页数；
	ULONG          FastReadNoWait;             //异步快速读数目；
	ULONG          FastReadWait;               //同步快速读数目；
	ULONG          FastReadResourceMiss;       //快速读资源冲突数；
	ULONG          FastReadNotPossible;        //快速读失败数；
	ULONG          FastMdlReadNoWait;          //异步MDL快速读数目；
	ULONG          FastMdlReadWait;            //同步MDL快速读数目；
	ULONG          FastMdlReadResourceMiss;    //MDL读资源冲突数；
	ULONG          FastMdlReadNotPossible;     //MDL读失败数；
	ULONG          MapDataNoWait;              //异步映射数据次数；
	ULONG          MapDataWait;                //同步映射数据次数；
	ULONG          MapDataNoWaitMiss;          //异步映射数据冲突次数；
	ULONG          MapDataWaitMiss;            //同步映射数据冲突次数；
	ULONG          PinMappedDataCount;         //牵制映射数据数目；
	ULONG          PinReadNoWait;              //牵制异步读数目；
	ULONG          PinReadWait;                //牵制同步读数目；
	ULONG          PinReadNoWaitMiss;          //牵制异步读冲突数目；
	ULONG          PinReadWaitMiss;            //牵制同步读冲突数目；
	ULONG          CopyReadNoWait;             //异步拷贝读次数；
	ULONG          CopyReadWait;               //同步拷贝读次数；
	ULONG          CopyReadNoWaitMiss;         //异步拷贝读故障次数；
	ULONG          CopyReadWaitMiss;           //同步拷贝读故障次数；
	ULONG          MdlReadNoWait;              //异步MDL读次数；
	ULONG          MdlReadWait;                //同步MDL读次数；
	ULONG          MdlReadNoWaitMiss;          //异步MDL读故障次数；
	ULONG          MdlReadWaitMiss;            //同步MDL读故障次数；
	ULONG          ReadAheadIos;               //向前读操作数目；
	ULONG          LazyWriteIos;               //LAZY写操作数目；
	ULONG          LazyWritePages;             //LAZY写页文件数目；
	ULONG          DataFlushes;                //缓存刷新次数；
	ULONG          DataPages;                  //缓存刷新页数；
	ULONG          ContextSwitches;            //环境切换数目；
	ULONG          FirstLevelTbFills;          //第一层缓冲区填充次数；
	ULONG          SecondLevelTbFills;         //第二层缓冲区填充次数；
	ULONG          SystemCall;                 //系统调用次数；
}SYSTEM_PERFORMANCE_INFORMATION,*PSYSTEM_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION64
{
	LARGE_INTEGER  IdleTime;                    //CPU空闲时间；
	LARGE_INTEGER  ReadTransferCount;           //I/O读操作数目；
	LARGE_INTEGER  WriteTransferCount;          //I/O写操作数目；
	LARGE_INTEGER  OtherTransferCount;          //I/O其他操作数目；
	ULONG          ReadOperationCount;          //I/O读数据数目；
	ULONG          WriteOperationCount;         //I/O写数据数目；
	ULONG          OtherOperationCount;         //I/O其他操作数据数目；
	ULONG          AvailablePages;              //可获得的页数目；
	ULONGLONG      TotalCommittedPages;         //总共提交页数目；
	ULONGLONG      TotalCommitLimit;            //已提交页数目；
	ULONGLONG      PeakCommitment;              //页提交峰值；
	ULONG          PageFaults;                  //页故障数目；
	ULONG          WriteCopyFaults;             //Copy-On-Write故障数目；
	ULONG          TransitionFaults;            //软页故障数目；
	ULONG          Reserved1;
	ULONG          DemandZeroFaults;            //需求0故障数；
	ULONG          PagesRead;                   //读页数目；
	ULONG          PageReadIos;                 //读页I/O操作数；
	ULONG          Reserved2[2];
	ULONG          PagefilePagesWritten;        //已写页文件页数；
	ULONG          PagefilePageWriteIos;        //已写页文件操作数；
	ULONG          MappedFilePagesWritten;      //已写映射文件页数；
	ULONG          MappedFileWriteIos;          //已写映射文件操作数；
	ULONG          PagedPoolUsage;              //分页池使用；
	ULONG          NonPagedPoolUsage;           //非分页池使用；
	ULONG          PagedPoolAllocs;             //分页池分配情况；
	ULONG          PagedPoolFrees;              //分页池释放情况；
	ULONG          NonPagedPoolAllocs;          //非分页池分配情况；
	ULONG          NonPagedPoolFress;           //非分页池释放情况；
	ULONG          TotalFreeSystemPtes;         //系统页表项释放总数；
	ULONG          SystemCodePage;              //操作系统代码页数；
	ULONG          TotalSystemDriverPages;      //可分页驱动程序页数；
	ULONG          TotalSystemCodePages;        //操作系统代码页总数；
	ULONG          SmallNonPagedLookasideListAllocateHits; //
	ULONG          SmallPagedLookasideListAllocateHits;
	ULONG          Reserved3;                   
	ULONG          MmSystemCachePage;          //系统缓存页数；
	ULONG          PagedPoolPage;              //分页池页数；
	ULONG          SystemDriverPage;           //可分页驱动页数；
	ULONG          FastReadNoWait;             //异步快速读数目；
	ULONG          FastReadWait;               //同步快速读数目；
	ULONG          FastReadResourceMiss;       //快速读资源冲突数；
	ULONG          FastReadNotPossible;        //快速读失败数；
	ULONG          FastMdlReadNoWait;          //异步MDL快速读数目；
	ULONG          FastMdlReadWait;            //同步MDL快速读数目；
	ULONG          FastMdlReadResourceMiss;    //MDL读资源冲突数；
	ULONG          FastMdlReadNotPossible;     //MDL读失败数；
	ULONG          MapDataNoWait;              //异步映射数据次数；
	ULONG          MapDataWait;                //同步映射数据次数；
	ULONG          MapDataNoWaitMiss;          //异步映射数据冲突次数；
	ULONG          MapDataWaitMiss;            //同步映射数据冲突次数；
	ULONG          PinMappedDataCount;         //牵制映射数据数目；
	ULONG          PinReadNoWait;              //牵制异步读数目；
	ULONG          PinReadWait;                //牵制同步读数目；
	ULONG          PinReadNoWaitMiss;          //牵制异步读冲突数目；
	ULONG          PinReadWaitMiss;            //牵制同步读冲突数目；
	ULONG          CopyReadNoWait;             //异步拷贝读次数；
	ULONG          CopyReadWait;               //同步拷贝读次数；
	ULONG          CopyReadNoWaitMiss;         //异步拷贝读故障次数；
	ULONG          CopyReadWaitMiss;           //同步拷贝读故障次数；
	ULONG          MdlReadNoWait;              //异步MDL读次数；
	ULONG          MdlReadWait;                //同步MDL读次数；
	ULONG          MdlReadNoWaitMiss;          //异步MDL读故障次数；
	ULONG          MdlReadWaitMiss;            //同步MDL读故障次数；
	ULONG          ReadAheadIos;               //向前读操作数目；
	ULONG          LazyWriteIos;               //LAZY写操作数目；
	ULONG          LazyWritePages;             //LAZY写页文件数目；
	ULONG          DataFlushes;                //缓存刷新次数；
	ULONG          DataPages;                  //缓存刷新页数；
	ULONG          ContextSwitches;            //环境切换数目；
	ULONG          FirstLevelTbFills;          //第一层缓冲区填充次数；
	ULONG          SecondLevelTbFills;         //第二层缓冲区填充次数；
	ULONG          SystemCall;                 //系统调用次数；
}SYSTEM_PERFORMANCE_INFORMATION64,*PSYSTEM_PERFORMANCE_INFORMATION64;

typedef struct 
{ 
	LARGE_INTEGER liKeBootTime; 
	LARGE_INTEGER liKeSystemTime; 
	LARGE_INTEGER liExpTimeZoneBias; 
	ULONG uCurrentTimeZoneId; 
	DWORD dwReserved; 
} SYSTEM_TIME_INFORMATION;


typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemNextEventIdInformation,
	SystemEventIdsInformation,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemPlugPlayBusInformation,
	SystemDockInformation,
	SystemPowerInformation,
	SystemProcessorSpeedInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;



typedef LONG (WINAPI *pfnZwQuerySystemInformation)(
				SYSTEM_INFORMATION_CLASS SystemInformationClass,
				PVOID SystemInformation,
				ULONG SystemInformationLength,
				PULONG ReturnLength
);

class CNTSystem
{
public:
	CNTSystem()
	{
		m_pfQuery = NULL;
	}
	virtual ~CNTSystem(){}

public:
	BOOL GetPerformanceInfo(SYSTEM_PERFORMANCE_INFORMATION& info)
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemPerformanceInformation,&info,sizeof(SYSTEM_PERFORMANCE_INFORMATION),NULL) ); 
	}

	BOOL GetPerformanceInfo_64(SYSTEM_PERFORMANCE_INFORMATION64& info)
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemPerformanceInformation,&info,sizeof(SYSTEM_PERFORMANCE_INFORMATION64),NULL) ); 
	}

	BOOL GetTimeInfo(SYSTEM_TIME_INFORMATION& info)
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemTimeOfDayInformation,&info,sizeof(SYSTEM_TIME_INFORMATION),NULL) ); 
	}

	BOOL GetBasicInfo(SYSTEM_BASIC_INFORMATION& info)
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemBasicInformation,&info,sizeof(SYSTEM_BASIC_INFORMATION),NULL) ); 
	}

protected:
	VOID _Init()
	{
		if (m_pfQuery==NULL)
		{
			TCHAR	szBuffer[MAX_PATH];
			::GetSystemWindowsDirectory(szBuffer,MAX_PATH);
			_tcscat(szBuffer,NT_DLL_NAME);

			m_mod.LoadLib(szBuffer);
			m_pfQuery = (pfnZwQuerySystemInformation)m_mod.GetProcAddr("ZwQuerySystemInformation");
		}
	}
protected:
	WinMod::CWinModule				m_mod;
	pfnZwQuerySystemInformation		m_pfQuery;
};

class CNTProcess
{
public:
	CNTProcess()
	{
		m_pfQuery = NULL;
	}
	virtual ~CNTProcess(){}

public:
	BOOL QueryIOCounter(HANDLE hProcess,IO_COUNTERS& ioConter )
	{
		_Init();

		if (m_pfQuery==NULL)
			return FALSE;

		return (m_pfQuery(hProcess,ProcessIoCounters, &ioConter, sizeof(IO_COUNTERS),NULL) >= 0 );
	}

	BOOL QueryVMMemory(HANDLE hProcess,VM_COUNTERS& vmCounter )
	{
		_Init();

		if (m_pfQuery==NULL)
			return FALSE;

		return (m_pfQuery(hProcess,ProcessVmCounters, &vmCounter, sizeof(VM_COUNTERS),NULL) >= 0 );
	}

protected:
	VOID _Init()
	{
		if (m_pfQuery==NULL)
		{
			TCHAR	szBuffer[MAX_PATH];
			::GetSystemWindowsDirectory(szBuffer,MAX_PATH);
			_tcscat(szBuffer,NT_DLL_NAME);

			m_mod.LoadLib(szBuffer);
			m_pfQuery = (pfnZwQueryInformationProcess)m_mod.GetProcAddr("ZwQueryInformationProcess");
		}
	}

protected:
	WinMod::CWinModule				m_mod;
	pfnZwQueryInformationProcess	m_pfQuery;
};
}

