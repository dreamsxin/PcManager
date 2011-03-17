#pragma once


#define X_SUPPORT_CSTRING		1

/* From NTDDK */
#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif

#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth

//
// UNICODE_STRING
//
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
#ifdef MIDL_PASS
	[size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
	PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

//
// ClientId
//
typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

//
// CURDIR
//
typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

//
// RTL_DRIVE_LETTER_CURDIR
//
typedef struct RTL_DRIVE_LETTER_CURDIR
{
	USHORT Flags;
	USHORT Length;
	ULONG TimeStamp;
	UNICODE_STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _PEB_FREE_BLOCK
{
	struct _PEB_FREE_BLOCK* Next;
	ULONG Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

/* RTL_USER_PROCESS_PARAMETERS.Flags */
#define PPF_NORMALIZED (1)

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG  MaximumLength;  //  00h
	ULONG  Length;   //  04h
	ULONG  Flags;   //  08h
	ULONG  DebugFlags;  //  0Ch
	PVOID  ConsoleHandle;  //  10h
	ULONG  ConsoleFlags;  //  14h
	HANDLE  InputHandle;  //  18h
	HANDLE  OutputHandle;  //  1Ch
	HANDLE  ErrorHandle;  //  20h
	CURDIR  CurrentDirectory; //  24h
	UNICODE_STRING DllPath;  //  30h
	UNICODE_STRING ImagePathName;  //  38h
	UNICODE_STRING CommandLine;  //  40h
	PWSTR  Environment;  //  48h
	ULONG  StartingX;  //  4Ch
	ULONG  StartingY;  //  50h
	ULONG  CountX;   //  54h
	ULONG  CountY;   //  58h
	ULONG  CountCharsX;  //  5Ch
	ULONG  CountCharsY;  //  60h
	ULONG  FillAttribute;  //  64h
	ULONG  WindowFlags;  //  68h
	ULONG  ShowWindowFlags; //  6Ch
	UNICODE_STRING WindowTitle;  //  70h
	UNICODE_STRING DesktopInfo;  //  78h
	UNICODE_STRING ShellInfo;  //  80h
	UNICODE_STRING RuntimeInfo;  //  88h
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20]; // 90h
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

#define PEB_BASE        (0x7FFDF000)

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef VOID (__stdcall *PPEBLOCKROUTINE)(PVOID);

typedef struct _PEB
{
	UCHAR InheritedAddressSpace;                     // 00h
	UCHAR ReadImageFileExecOptions;                  // 01h
	UCHAR BeingDebugged;                             // 02h
	UCHAR Spare;                                     // 03h
	PVOID Mutant;                                    // 04h
	PVOID ImageBaseAddress;                          // 08h
	PPEB_LDR_DATA Ldr;                               // 0Ch
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;  // 10h
	PVOID SubSystemData;                             // 14h
	PVOID ProcessHeap;                               // 18h
	PVOID FastPebLock;                               // 1Ch
	PPEBLOCKROUTINE FastPebLockRoutine;              // 20h
	PPEBLOCKROUTINE FastPebUnlockRoutine;            // 24h
	ULONG EnvironmentUpdateCount;                    // 28h
	PVOID* KernelCallbackTable;                      // 2Ch
	PVOID EventLogSection;                           // 30h
	PVOID EventLog;                                  // 34h
	PPEB_FREE_BLOCK FreeList;                        // 38h
	ULONG TlsExpansionCounter;                       // 3Ch
	PVOID TlsBitmap;                                 // 40h
	ULONG TlsBitmapBits[0x2];                        // 44h
	PVOID ReadOnlySharedMemoryBase;                  // 4Ch
	PVOID ReadOnlySharedMemoryHeap;                  // 50h
	PVOID* ReadOnlyStaticServerData;                 // 54h
	PVOID AnsiCodePageData;                          // 58h
	PVOID OemCodePageData;                           // 5Ch
	PVOID UnicodeCaseTableData;                      // 60h
	ULONG NumberOfProcessors;                        // 64h
	ULONG NtGlobalFlag;                              // 68h
	UCHAR Spare2[0x4];                               // 6Ch
	LARGE_INTEGER CriticalSectionTimeout;            // 70h
	ULONG HeapSegmentReserve;                        // 78h
	ULONG HeapSegmentCommit;                         // 7Ch
	ULONG HeapDeCommitTotalFreeThreshold;            // 80h
	ULONG HeapDeCommitFreeBlockThreshold;            // 84h
	ULONG NumberOfHeaps;                             // 88h
	ULONG MaximumNumberOfHeaps;                      // 8Ch
	PVOID** ProcessHeaps;                            // 90h
	PVOID GdiSharedHandleTable;                      // 94h
	PVOID ProcessStarterHelper;                      // 98h
	PVOID GdiDCAttributeList;                        // 9Ch
	PVOID LoaderLock;                                // A0h
	ULONG OSMajorVersion;                            // A4h
	ULONG OSMinorVersion;                            // A8h
	ULONG OSBuildNumber;                             // ACh
	ULONG OSPlatformId;                              // B0h
	ULONG ImageSubSystem;                            // B4h
	ULONG ImageSubSystemMajorVersion;                // B8h
	ULONG ImageSubSystemMinorVersion;                // C0h
	ULONG GdiHandleBuffer[0x22];                     // C4h

	PVOID ProcessWindowStation;                      // ???
} PEB, *PPEB;

typedef struct _GDI_TEB_BATCH
{
	ULONG Offset;
	ULONG HDC;
	ULONG Buffer[0x136];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _NT_TEB
{
	NT_TIB Tib;                         // 00h
	PVOID EnvironmentPointer;           // 1Ch
	CLIENT_ID Cid;                      // 20h
	PVOID ActiveRpcInfo;                // 28h
	PVOID ThreadLocalStoragePointer;    // 2Ch
	PPEB Peb;                           // 30h
	ULONG LastErrorValue;               // 34h
	ULONG CountOfOwnedCriticalSections; // 38h
	PVOID CsrClientThread;              // 3Ch
	PVOID Win32ThreadInfo;              // 40h
	ULONG Win32ClientInfo[0x1F];        // 44h
	PVOID WOW32Reserved;                // C0h
	ULONG CurrentLocale;                // C4h
	ULONG FpSoftwareStatusRegister;     // C8h
	PVOID SystemReserved1[0x36];        // CCh
	PVOID Spare1;                       // 1A4h
	LONG ExceptionCode;                 // 1A8h
	ULONG SpareBytes1[0x28];            // 1ACh
	PVOID SystemReserved2[0xA];         // 1D4h
	GDI_TEB_BATCH GdiTebBatch;          // 1FCh
	ULONG gdiRgn;                       // 6DCh
	ULONG gdiPen;                       // 6E0h
	ULONG gdiBrush;                     // 6E4h
	CLIENT_ID RealClientId;             // 6E8h
	PVOID GdiCachedProcessHandle;       // 6F0h
	ULONG GdiClientPID;                 // 6F4h
	ULONG GdiClientTID;                 // 6F8h
	PVOID GdiThreadLocaleInfo;          // 6FCh
	PVOID UserReserved[5];              // 700h
	PVOID glDispatchTable[0x118];       // 714h
	ULONG glReserved1[0x1A];            // B74h
	PVOID glReserved2;                  // BDCh
	PVOID glSectionInfo;                // BE0h
	PVOID glSection;                    // BE4h
	PVOID glTable;                      // BE8h
	PVOID glCurrentRC;                  // BECh
	PVOID glContext;                    // BF0h
	NTSTATUS LastStatusValue;           // BF4h
	UNICODE_STRING StaticUnicodeString; // BF8h
	WCHAR StaticUnicodeBuffer[0x105];   // C00h
	PVOID DeallocationStack;            // E0Ch
	PVOID TlsSlots[0x40];               // E10h
	LIST_ENTRY TlsLinks;                // F10h
	PVOID Vdm;                          // F18h
	PVOID ReservedForNtRpc;             // F1Ch
	PVOID DbgSsReserved[0x2];           // F20h
	ULONG HardErrorDisabled;            // F28h
	PVOID Instrumentation[0x10];        // F2Ch
	PVOID WinSockData;                  // F6Ch
	ULONG GdiBatchCount;                // F70h
	ULONG Spare2;                       // F74h
	ULONG Spare3;                       // F78h
	ULONG Spare4;                       // F7Ch
	PVOID ReservedForOle;               // F80h
	ULONG WaitingOnLoaderLock;          // F84h

	PVOID StackCommit;                  // F88h
	PVOID StackCommitMax;               // F8Ch
	PVOID StackReserve;                 // F90h

	PVOID MessageQueue;                 // ???
} NT_TEB, *PNT_TEB;


typedef struct _PEB *PPEB;

//
// Thread priority
//

typedef LONG KPRIORITY;

//
// Process Information Classes
//
typedef enum _PROCESSINFOCLASS
{
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
	MaxProcessInfoClass
} PROCESSINFOCLASS;

//
// Basic Process Information
//  NtQueryInformationProcess using ProcessBasicInfo
//

typedef struct _PROCESS_BASIC_INFORMATION {
	NTSTATUS	ExitStatus;
	PPEB		PebBaseAddress;
	ULONG_PTR	AffinityMask;
	KPRIORITY	BasePriority;
	ULONG_PTR	UniqueProcessId;
	ULONG_PTR	InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef LONG (WINAPI *PFNTQUERYINFORMATIONPROCESS)(HANDLE,UINT,PVOID,ULONG,PULONG);

typedef struct _ProcEntry 
{
	DWORD	nProcId;
	TCHAR	szFullPath[MAX_PATH];
	TCHAR	szCommondLine[MAX_PATH];
}PROCESSENTRY, *PPROCESSENTRY;

class CProgressMgr
{
public:
	CProgressMgr();
	virtual ~CProgressMgr();

public:
	void Load();
	void UnLoad();

	BOOL KillByPath(LPCTSTR lpPath);
	BOOL KillByCMD(LPCTSTR lpCommondLine);

#if X_SUPPORT_CSTRING
	BOOL GetFullPath(LPCTSTR lpPath, CString& strFullPath);		// 通过一个部分路径来获取全部路径
#endif

private:

	BOOL KillTask( DWORD nEntryId, BOOL bRemoveFile );
	BOOL KillProcess(DWORD dwProcessId);

	BOOL InitNTQueryApi();
	void GetCommandLine( DWORD dwPid, TCHAR* pszCmd, int nLen );
private:
	std::vector<PROCESSENTRY> m_tasks;
};