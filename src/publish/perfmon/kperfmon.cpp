////////////////////////////////////////////////////////////////////////////////
//      
//      File for net flux
//      
//      File      : kfluxstastic.h
//      Version   : 1.0
//      Comment   : 流量统计
//      
//      Create at : 2010-12-30
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "kperfmon.h"
#include "atltime.h"
#include "winreg.h"

//////////////////////////////////////////////////////////////////////////
KPerfNameMgr::KPerfNameMgr():m_NameBuf(NULL),m_pNameArray(NULL), m_nMaxNameID(0)
{
	BuildNameMap();

	AddToFixMap(6, 0);
	AddToFixMap(142, 1);
	AddToFixMap(144, 2);
	AddToFixMap(172, 3);
	AddToFixMap(174, 4);
	AddToFixMap(28, 5);
	AddToFixMap(178, 6);
	AddToFixMap(180, 7);
	AddToFixMap(182, 8);
	AddToFixMap(184, 9);
	AddToFixMap(186, 10);
	AddToFixMap(680, 11);
	AddToFixMap(682, 12);
	AddToFixMap(684, 13);
	AddToFixMap(784, 14);
	AddToFixMap(1410, 15);
	AddToFixMap(56, 16);
	AddToFixMap(58, 17);
	AddToFixMap(952, 18);
	AddToFixMap(1412, 19);
	AddToFixMap(1414, 20);
	AddToFixMap(1416, 21);
	AddToFixMap(1418, 22);
	AddToFixMap(1420, 23);
	AddToFixMap(1422, 24);
	AddToFixMap(1424, 25);
	AddToFixMap(1426, 26);
	AddToFixMap(1478, 27);
}

void KPerfNameMgr::AddToFixMap(int nID, int nDataID)
{
	m_FixCounterNameIDToDataID[nID] = nDataID;
}

KPerfNameMgr::~KPerfNameMgr()
{
	if (m_pNameArray)
	{
		m_nMaxNameID = 0;
		delete m_pNameArray;
		m_pNameArray = NULL;
	}

	if (m_NameBuf)
	{
		delete[] m_NameBuf;
		m_NameBuf = NULL;
	}
}

LPTSTR	KPerfNameMgr::GetName(INT nID)
{
	if (nID >= 0 && nID < m_nMaxNameID)
		return m_pNameArray[nID];
	return NULL;
}

INT	KPerfNameMgr::GetDataIDByCounterID(INT nCounterNameID)
{
	LPTSTR strName = GetName(nCounterNameID);
	if (strName)
	{
		for (int i = 0; i < Proc_Perf_Data_Max; i++)
		{
			if (_tcsicmp(strName, m_ProcessDataNames[i]) == 0)
				return i;
		}
	}
	// 使用经验值来做
	if (m_FixCounterNameIDToDataID.find(nCounterNameID) != m_FixCounterNameIDToDataID.end())
		return m_FixCounterNameIDToDataID[nCounterNameID];
	return -1;
}

BOOL KPerfNameMgr::BuildNameMap()
{
	HKEY hKeyPerflib;      // handle to registry key
	HKEY hKeyPerflib009;   // handle to registry key
	DWORD dwMaxValueLen = 0;   // maximum size of key values
	DWORD dwBuffer;        // bytes to allocate for buffers
	DWORD dwBufferSize = sizeof(DWORD);  // size of dwBuffer
	LPTSTR lpCurrentString; // pointer for enumerating data strings
	DWORD dwCounter;       // current counter index
	LONG lResult;          // return value

	// Get the number of Counter items.

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib",
		0,
		KEY_READ,
		&hKeyPerflib) != ERROR_SUCCESS
		)
		return FALSE;

	lResult = RegQueryValueEx( hKeyPerflib,
		L"Last Counter",
		NULL,
		NULL,
		(LPBYTE) &m_nMaxNameID,
		&dwBufferSize );

	RegCloseKey( hKeyPerflib );

	if( lResult != ERROR_SUCCESS )
		return FALSE;

	// Allocate memory for the names array.
	m_nMaxNameID++;
	m_pNameArray = new LPTSTR[m_nMaxNameID];

	if( m_pNameArray == NULL )
		return FALSE;
	memset(m_pNameArray, 0, m_nMaxNameID*sizeof(LPTSTR));
	// Open the key containing the counter and object names.

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009",
		0,
		KEY_READ,
		&hKeyPerflib009) != ERROR_SUCCESS
		)
		return FALSE;

	// Get the size of the largest value in the key (Counter or Help).

	if( RegQueryInfoKey( hKeyPerflib009,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&dwMaxValueLen,
		NULL,
		NULL) != ERROR_SUCCESS
		)
		return FALSE;

	// Allocate memory for the counter and object names.

	dwBuffer = dwMaxValueLen + 1;

	m_NameBuf = new TCHAR[dwBuffer];//(CHAR*)malloc( dwBuffer * sizeof(CHAR) );

	if (m_NameBuf == NULL)
	{
		delete [] m_pNameArray;
		return FALSE;
	}

	// Read the counter value.

	lResult = RegQueryValueEx( hKeyPerflib009,
		L"Counters",
		NULL,
		NULL,
		(LPBYTE)m_NameBuf, &dwBuffer );

	RegCloseKey( hKeyPerflib009 ); 

	if( lResult != ERROR_SUCCESS )
		return FALSE;

	// Load names into an array, by index.

	for( lpCurrentString = m_NameBuf; *lpCurrentString;
		lpCurrentString += (_tcslen(lpCurrentString)+1) )
	{
		dwCounter = _tstol( lpCurrentString );

		lpCurrentString += (_tcslen(lpCurrentString)+1);
		if (dwCounter < m_nMaxNameID)
			m_pNameArray[dwCounter] = (LPTSTR) lpCurrentString;
	}

	return TRUE;
}

LPTSTR KPerfNameMgr::m_ProcessDataNames[Proc_Perf_Data_Max+1] = 
{
	_T("% Processor Time"),
	_T("% User Time"),
	_T("% Privileged Time"),
	_T("Virtual Bytes Peak"),
	_T("Virtual Bytes"),
	_T("Page Faults/sec"),
	_T("Working Set Peak"),
	_T("Working Set"),
	_T("Page File Bytes Peak"),
	_T("Page File Bytes"),
	_T("Private Bytes"),
	_T("Thread Count"),
	_T("Priority Base"),
	_T("Elapsed Time"),
	_T("ID Process"),
	_T("Creating Process ID"),
	_T("Pool Paged Bytes"),
	_T("Pool Nonpaged Bytes"),
	_T("Handle Count"),
	_T("IO Read Operations/sec"),
	_T("IO Write Operations/sec"),
	_T("IO Data Operations/sec"),
	_T("IO Other Operations/sec"),
	_T("IO Read Bytes/sec"),
	_T("IO Write Bytes/sec"),
	_T("IO Data Bytes/sec"),
	_T("IO Other Bytes/sec"),
	_T("Working Set - Private"),
};

//////////////////////////////////////////////////////////////////////////
KProcessPerfData::KProcessPerfData(DWORD nCpuCount):m_nProcessID(-1),
									m_cpuCounterLast(0),
									m_cpuCounterLastTime(0),
									m_memUsage(0),
									m_ioSpeed(0),
									m_ioCounterLast(0),
									m_ioCounterLastTime(0),
									m_timeLast(0),
									m_nCpuCount(nCpuCount),
									m_nTrustMode(0),//默认为未知,
									m_nNewCreate(TRUE)
{
	m_cpuUsage = 0;
}

KProcessPerfData::~KProcessPerfData()
{

}

BOOL KProcessPerfData::UpdateData(PPERF_INSTANCE_DEFINITION  PerfInst,
				PPERF_COUNTER_DEFINITION   ProcessCounters[Proc_Perf_Data_Max],
				PPERF_OBJECT_TYPE PerfObj)
{
	PPERF_COUNTER_DEFINITION pCounter = ProcessCounters[Proc_Perf_Data_Process_ID];
	if (!pCounter) return FALSE;
	m_nProcessID = _GetProcessID(PerfInst, pCounter);
	m_strProcName = (wchar_t*)((PBYTE)PerfInst + PerfInst->NameOffset);

	pCounter = ProcessCounters[Proc_Perf_Data_Elapsed_Time];
	if (!pCounter) return FALSE;
	ULONGLONG nTimePast = 0;
	ULONGLONG nTimeNow = _GetUlonglong(PerfInst, pCounter);
	if (nTimeNow != 0)
	{
		nTimeNow = PerfObj->PerfTime.QuadPart - nTimeNow;
		if (m_timeLast != 0)
			nTimePast = nTimeNow - m_timeLast;
		m_timeLast = nTimeNow;
	}

	// 计算cpu使用率
	pCounter = ProcessCounters[Proc_Perf_Data_Processor_Time];
	if (!pCounter) return FALSE;
	ULONGLONG nCpuCounter = _GetCpuCounter(PerfInst, pCounter);
	if (m_cpuCounterLast == 0)
	{
		m_cpuUsage = 0;
		m_cpuCounterLast = nCpuCounter;
		m_cpuCounterLastTime = PerfObj->PerfTime.QuadPart;
	}
	else
	{
		m_nNewCreate = FALSE;
		if (nCpuCounter >= m_cpuCounterLast)
		{
			ULONGLONG cpuUse = nCpuCounter - m_cpuCounterLast;
			ULONGLONG nTimeSpan = PerfObj->PerfTime.QuadPart - m_cpuCounterLastTime;
			if (nTimeSpan != 0)
			{
				m_cpuUsage = (double)cpuUse * 100.0 / ((double)nTimeSpan * double(m_nCpuCount));
				m_cpuCounterLast = nCpuCounter;
				m_cpuCounterLastTime = PerfObj->PerfTime.QuadPart;
			}
		}
		else
		{
			m_cpuCounterLast = nCpuCounter;
			m_cpuCounterLastTime = PerfObj->PerfTime.QuadPart;
		}
	}

	// 内存使用
	pCounter = ProcessCounters[Proc_Perf_Data_Working_Set_Private];
	if (!pCounter) 
	{
		pCounter = ProcessCounters[Proc_Perf_Data_Working_Set];
		if (!pCounter)
			return FALSE;
	}

	ULONGLONG nWorkingSet = _GetUlonglong(PerfInst, pCounter);
	m_memUsage = nWorkingSet;

	// io读写率
	pCounter = ProcessCounters[Proc_Perf_Data_IO_Data_Per_Sec];
	if (!pCounter) return FALSE;
	ULONGLONG nDataCount = _GetUlonglong(PerfInst, pCounter);
	if (m_ioCounterLast == 0)
	{
		m_ioSpeed = 0;
		m_ioCounterLast = nDataCount;
		m_ioCounterLastTime = PerfObj->PerfTime.QuadPart;
	}
	else
	{
		if (nDataCount > m_ioCounterLast)
		{
			ULONGLONG nDataInc = nDataCount - m_ioCounterLast;
			ULONGLONG nTimeInc = PerfObj->PerfTime.QuadPart - m_ioCounterLastTime;
			if (nTimeInc != 0)
			{
				m_ioSpeed = nDataInc * PerfObj->PerfFreq.QuadPart / nTimeInc;
				m_ioCounterLast = nDataCount;
				m_ioCounterLastTime = PerfObj->PerfTime.QuadPart;
			}
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
KProcessPerfMgr::KProcessPerfMgr():m_PerfData(NULL),
									m_pNameMgr(NULL),
									m_nPerfDataSize(0),
									m_LastUpdateOp(Update_Unknown),
									m_PerfDataLast(NULL),
									m_nPerfDataSizeLast(0),
									m_nMemUsage(0),
									m_pIdleProcessData(0),
									m_nPerfSize(TOTALBYTES),
									m_nCpuNum(0),
									m_pTotalProcessData(NULL)
{

}

KProcessPerfMgr::~KProcessPerfMgr()
{
	
}

HRESULT KProcessPerfMgr::Init()
{
	if (!m_pNameMgr)
	{
		m_pNameMgr = new KPerfNameMgr;
		if (!m_pNameMgr) return E_FAIL;
	}
	SYSTEM_INFO info = {0};
	GetSystemInfo(&info);
	m_nCpuNum = info.dwNumberOfProcessors;
	return S_OK;
}

HRESULT KProcessPerfMgr::UnInit()
{
	if (m_PerfData)
	{
		delete[] m_PerfData;
		m_PerfData = NULL;
	}
	_ClearProcessInfo();

	if (m_pNameMgr)
	{
		delete m_pNameMgr;
		m_pNameMgr = NULL;
	}
	if (m_pIdleProcessData)
	{
		delete m_pIdleProcessData;
		m_pIdleProcessData = NULL;
	}
	if (m_pTotalProcessData)
	{
		delete m_pTotalProcessData;
		m_pTotalProcessData = NULL;
	}

	return S_OK;
}

void KProcessPerfMgr::SwapPerfData()
{
	PPERF_DATA_BLOCK pSwap = m_PerfDataLast;
	m_PerfDataLast = m_PerfData;
	m_PerfData = pSwap;

	DWORD nSwap = m_nPerfDataSizeLast;
	m_nPerfDataSizeLast = m_nPerfDataSize;
	m_nPerfDataSize = nSwap;
}

// 使用api直接获取系统性能数据
void KProcessPerfMgr::UpdateSystemCpuAndMem()
{
	MEMORYSTATUSEX statex = {0};
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	m_nMemUsage = (double)100 - (double)100 * (double)statex.ullAvailPhys/(double)statex.ullTotalPhys;
}

HRESULT KProcessPerfMgr::Update(Perf_Update_Type ut)
{
	UpdateSystemCpuAndMem();

	switch (ut)
	{
	case Update_Only_Process_Perf_Data:
		{
			SwapPerfData();
			if (GetCurrentPerfData())
				CalcOnlyTotalProcessPerfData();
		}
		break;
	case Update_Perf_Data_And_Calc:
		{
			SwapPerfData();
			if (GetCurrentPerfData())
			{
				CalcPerfData();
			}
		}
		break;
	}

	m_LastUpdateOp = ut;
	return S_OK;
}

void KProcessPerfMgr::CalcOnlyTotalProcessPerfData()
{
	if (m_PerfData)
	{
		PPERF_OBJECT_TYPE PerfObj = FirstObject(m_PerfData);
		if (!_UpdateCounterInfo(PerfObj))
			return;

		PPERF_INSTANCE_DEFINITION PerfInst = NULL;
		if( PerfObj->NumInstances > 0 )
		{
			PerfInst = FirstInstance( PerfObj );

			for( int k=0; k < PerfObj->NumInstances; k++ )
			{ 
				wchar_t* pInstName = (wchar_t*)((PBYTE)PerfInst + PerfInst->NameOffset);
				if ((wcsicmp(pInstName, L"Idle") == 0))
				{
					_UpdateProcessIdle(PerfInst, PerfObj);
				}
				else if ((wcsicmp(pInstName, L"_total") == 0))
				{
					_UpdateProcessTotal(PerfInst, PerfObj);
				}
				PerfInst = NextInstance( PerfInst );
			}
		}
	}
}

#define WIDTH 20

KProcessPerfData* KProcessPerfMgr::_UpdateProcessIdle(PPERF_INSTANCE_DEFINITION PerfInst, PPERF_OBJECT_TYPE PerfObj)
{
	if (!m_pIdleProcessData)
		m_pIdleProcessData = new KProcessPerfData(m_nCpuNum);
	if (m_pIdleProcessData)
		m_pIdleProcessData->UpdateData(PerfInst, m_ProcessCounters, PerfObj);
	return m_pIdleProcessData;
}

KProcessPerfData* KProcessPerfMgr::_UpdateProcessTotal(PPERF_INSTANCE_DEFINITION PerfInst, PPERF_OBJECT_TYPE PerfObj)
{
	if (!m_pTotalProcessData)
		m_pTotalProcessData = new KProcessPerfData(m_nCpuNum);
	if (m_pTotalProcessData)
		m_pTotalProcessData->UpdateData(PerfInst, m_ProcessCounters, PerfObj);
	return m_pTotalProcessData;
}

double  KProcessPerfMgr::GetSysCpuUsage()
{
	if (!m_pIdleProcessData) 
		return 0;
	return (double)100 - m_pIdleProcessData->GetCpuUsage();
}

ULONGLONG KProcessPerfMgr::GetTotalIo()
{
	if (!m_pTotalProcessData)
		return 0;
	return m_pTotalProcessData->GetIOSpeed();
}

BOOL KProcessPerfMgr::GetCurrentPerfData()
{
	DWORD nBufSize = m_nPerfDataSize;
	if (m_nPerfDataSize < m_nPerfSize)
	{
		if (m_PerfData)
		{
			delete [] m_PerfData;
			m_PerfData = NULL;
			m_nPerfDataSize = 0;
		}
	}

	if (!m_PerfData)
	{
		m_nPerfDataSize = m_nPerfSize;
		m_PerfData = (PPERF_DATA_BLOCK) new BYTE[ m_nPerfDataSize ];
		if (!m_PerfData)
		{
			m_nPerfDataSize = 0;
			return FALSE;
		}
	}
	nBufSize = m_nPerfDataSize;
	
	while( RegQueryValueEx( HKEY_PERFORMANCE_DATA,
		L"232",
		NULL,
		NULL,
		(LPBYTE) m_PerfData,
		&nBufSize ) == ERROR_MORE_DATA )
	{
		// Get a buffer that is big enough.
		m_nPerfSize <<= 1;
		if (m_PerfData)
		{
			delete [] m_PerfData;
			m_PerfData = 0;
			m_nPerfDataSize = 0;
		}
		m_nPerfDataSize = m_nPerfSize;
		m_PerfData = (PPERF_DATA_BLOCK) new BYTE[ m_nPerfDataSize ];
		if (!m_PerfData)
		{
			m_nPerfDataSize = 0;
			break;
		}
		nBufSize = m_nPerfDataSize;
	}
	
	if (m_PerfData)
	{
		return TRUE;
	}
	else
	{
		m_nPerfDataSize = 0;
		return FALSE;
	}
}

void KProcessPerfMgr::CalcPerfData()
{
	if (m_LastUpdateOp != Update_Perf_Data_And_Calc)
	{
		_ClearProcessInfo();
		if (m_PerfDataLast)
		{
			PPERF_OBJECT_TYPE PerfObj = FirstObject(m_PerfDataLast);
			UpdatePerfData(PerfObj);
		}
	}
	if (m_PerfData)
	{
		PPERF_OBJECT_TYPE PerfObj = FirstObject(m_PerfData);
		UpdatePerfData(PerfObj);
	}
}

void KProcessPerfMgr::UpdatePerfData(PPERF_OBJECT_TYPE PerfObj)
{
	if (!_UpdateCounterInfo(PerfObj))
		return;

	PPERF_INSTANCE_DEFINITION PerfInst = NULL;
	map<DWORD, KProcessPerfData*> newProcessInfo;
	if( PerfObj->NumInstances > 0 )
	{
		PerfInst = FirstInstance( PerfObj );
		double fSumExceptIdle = 0;
		KProcessPerfData* pIdleProcess = NULL;

		for( int k=0; k < PerfObj->NumInstances; k++ )
		{ 
			wchar_t* pInstName = (wchar_t*)((PBYTE)PerfInst + PerfInst->NameOffset);
			if ((wcsicmp(pInstName, L"_total") != 0))
			{
				KProcessPerfData* pProcessData = _UpdateProcessData(PerfInst, PerfObj);
				if (pProcessData)
				{
					newProcessInfo[pProcessData->GetProcessID()] = pProcessData;
					if (pProcessData->GetProcessID() != 0)
						fSumExceptIdle += pProcessData->GetCpuUsage();
					else
						pIdleProcess = pProcessData;
				}

				if ((wcsicmp(pInstName, L"Idle") == 0))
				{
					_UpdateProcessIdle(PerfInst, PerfObj);
				}
			}
			else
			{
				_UpdateProcessTotal(PerfInst, PerfObj);
			}

			PerfInst = NextInstance( PerfInst );
		}
		if (pIdleProcess)
		{
			double idleUsage = (double)100.0 - fSumExceptIdle;
			pIdleProcess->SetCpuUsage(idleUsage);
		}
	}

	_ClearProcessInfo();
	m_ProcessInfo = newProcessInfo;
	newProcessInfo.clear();

}

BOOL KProcessPerfMgr::_UpdateCounterInfo(PPERF_OBJECT_TYPE PerfObj)
{
	PPERF_COUNTER_DEFINITION PerfCntr = NULL, CurCntr = NULL;
	PPERF_COUNTER_BLOCK PtrToCntr = NULL;
	BOOL bShouldBuildMap = FALSE;
	if (m_mapProcessData2CounterID.empty())
		bShouldBuildMap = TRUE;

	memset(&m_ProcessCounters, 0, sizeof(m_ProcessCounters));

	PerfCntr = FirstCounter( PerfObj );
	for (DWORD i = 0; i < PerfObj->NumCounters; i++)
	{
		if (m_mapProcessData2CounterID.find(PerfCntr->CounterNameTitleIndex) != 
			m_mapProcessData2CounterID.end())
		{
			m_ProcessCounters[m_mapProcessData2CounterID[PerfCntr->CounterNameTitleIndex]]
			= PerfCntr;
		}
		else
		{
			if (bShouldBuildMap)
			{
				int nDataID = m_pNameMgr->GetDataIDByCounterID(PerfCntr->CounterNameTitleIndex);
				if (nDataID != -1 && nDataID < Proc_Perf_Data_Max)
				{
					m_mapProcessData2CounterID[PerfCntr->CounterNameTitleIndex] = nDataID;
					m_ProcessCounters[nDataID] = PerfCntr;
				}
				
			}
		}
		PerfCntr = NextCounter(PerfCntr);
	}
	return TRUE;
}

KProcessPerfData* KProcessPerfMgr::_UpdateProcessData(PPERF_INSTANCE_DEFINITION PerfInst, PPERF_OBJECT_TYPE PerfObj)
{
	DWORD nProcessID = _GetProcessID(PerfInst);
	if (nProcessID == -1) return NULL;

	KProcessPerfData* pData = _GetProcessData(nProcessID);
	if (!pData)
		pData = new KProcessPerfData(m_nCpuNum);
	if (pData )
	{
		if (pData->UpdateData(PerfInst, m_ProcessCounters, PerfObj))
			return pData;
		else
			delete pData;
	}
	return NULL;
}

KProcessPerfData* KProcessPerfMgr::_GetProcessData(DWORD nProcessID)
{
	map<DWORD, KProcessPerfData*>::iterator it = m_ProcessInfo.find(nProcessID);
	if (it == m_ProcessInfo.end())
		return NULL;
	else
	{
		KProcessPerfData* pData = it->second;
		m_ProcessInfo.erase(it);
		return pData;
	}

	return NULL;
}

DWORD KProcessPerfMgr::_GetProcessID(PPERF_INSTANCE_DEFINITION PerfInst)
{
	PPERF_COUNTER_DEFINITION pCounter = m_ProcessCounters[Proc_Perf_Data_Process_ID];
	if (!pCounter) return -1;
	LPBYTE pData = GetCounterData(PerfInst, pCounter);
	if (pData) return *((DWORD*)pData);
	return -1;
}

void KProcessPerfMgr::_ClearProcessInfo()
{
	map<DWORD, KProcessPerfData*>::iterator it = m_ProcessInfo.begin();
	while (it != m_ProcessInfo.end())
	{
		delete it->second;
		it++;
	}
	m_ProcessInfo.clear();
}

void KProcessPerfMgr::PrintProcessInfos()
{
	PrintSystemInfo();
	PrintMemInfo();
	PrintCacheInfo();
	if (m_PerfData)
	{
		PPERF_OBJECT_TYPE PerfObj = FirstObject(m_PerfData);
		PPERF_INSTANCE_DEFINITION PerfInst = NULL;
		if( PerfObj->NumInstances > 0 )
		{
			PerfInst = FirstInstance( PerfObj );
			for( int k=0; k < PerfObj->NumInstances; k++ )
			{ 
				wchar_t* pInstName = (wchar_t*)((PBYTE)PerfInst + PerfInst->NameOffset);
				PrintProcessInfo(PerfInst);
				PerfInst = NextInstance( PerfInst );
			}
		}
	}
}

BOOL KProcessPerfMgr::PrintProcessInfo(PPERF_INSTANCE_DEFINITION PerfInst)
{
	PPERF_COUNTER_DEFINITION pCounter = m_ProcessCounters[Proc_Perf_Data_Process_ID];
	if (!pCounter) return FALSE;
	DWORD nProcessID = GetCounterDword(PerfInst, pCounter);
	wchar_t* strProcName = (wchar_t*)((PBYTE)PerfInst + PerfInst->NameOffset);

	printf("process: %S \n", strProcName);
	printf("process id: %d\n", nProcessID);

	pCounter = m_ProcessCounters[Proc_Perf_Data_Virtual_Bytes];
	ULONGLONG nVirtualBytes = GetCounterUlonglong(PerfInst, pCounter);
	printf("process virtual bytes: %*I64d\n", 10, nVirtualBytes);

	pCounter = m_ProcessCounters[Proc_Perf_Data_Working_Set];
	ULONGLONG nWorkingSet = GetCounterUlonglong(PerfInst, pCounter);
	printf("process working set: %*I64d\n", 10, nWorkingSet);

	pCounter = m_ProcessCounters[Proc_Perf_Data_Private_Bytes];
	ULONGLONG nPrivateBytes = GetCounterUlonglong(PerfInst, pCounter);
	printf("process private bytes: %*I64d\n", 10, nPrivateBytes);

	pCounter = m_ProcessCounters[Proc_Perf_Data_Working_Set_Private];
	if (pCounter)
	{
		ULONGLONG nPrivateWorkingSet = GetCounterUlonglong(PerfInst, pCounter);
		printf("process private working set: %*I64d\n", 10, nPrivateWorkingSet);
	}

	// process workingset size
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nProcessID);
	if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
	{
		SIZE_T nMinSize = 0;
		SIZE_T nMaxSize = 0;
		if (GetProcessWorkingSetSize(hProcess, &nMinSize, &nMaxSize))
		{
			printf("process working set: min: %d, max:%d\n", nMinSize, nMaxSize);
		}
		::CloseHandle(hProcess);
	}
	
	printf("----------\n");
	return TRUE;
}

BOOL KProcessPerfMgr::PrintSystemInfo()
{	
	printf("system info\n");
	PrintData(L"2");
	printf("-------------------\n");
	return TRUE;
}

BOOL KProcessPerfMgr::PrintMemInfo()
{
	printf("system mem info\n");
	PrintData(L"4");
	printf("-------------------\n");
	return TRUE;
}
BOOL KProcessPerfMgr::PrintCacheInfo()
{
	printf("system cache info\n");
	PrintData(L"86");
	printf("-------------------\n");
	return TRUE;
}

BOOL KProcessPerfMgr::PrintData(LPCWSTR str)
{
	BYTE sysInfo [TOTALBYTES] = {0};
	DWORD nSize = TOTALBYTES;
	if (RegQueryValueEx( HKEY_PERFORMANCE_DATA,
		str,
		NULL,
		NULL,
		(LPBYTE) sysInfo,
		&nSize ) == ERROR_SUCCESS)
	{
		PPERF_OBJECT_TYPE pObj = FirstObject((PPERF_DATA_BLOCK)sysInfo);
		PrintObj(pObj);
		return TRUE;
	}
	return FALSE;
}

BOOL KProcessPerfMgr::PrintObj(PPERF_OBJECT_TYPE pObj)
{
	if (pObj->NumInstances >= 0)
	{
		// there has instance
	}
	else
	{
		// counter data follow the coutner def
		PPERF_COUNTER_BLOCK pCounterBlock = (PPERF_COUNTER_BLOCK)((PBYTE)pObj + pObj->DefinitionLength);
		PPERF_COUNTER_DEFINITION pCounter = FirstCounter(pObj);
		for (int i = 0; i < pObj->NumCounters; i++)
		{
			LPCWSTR strName = m_pNameMgr->GetName(pCounter->CounterNameTitleIndex);
			if (strName)
			{
				if (pCounter->CounterSize == 4)
				{
					DWORD nData = GetCounterDword(pCounterBlock, pCounter);
					printf("counter %S: %d\n", strName, nData);
				}
				else if (pCounter->CounterSize == 8)
				{
					ULONGLONG nData = GetCounterUlonglong(pCounterBlock, pCounter);
					printf("counter %S: %*I64d\n", strName, 10, nData);
				}
			}
			pCounter = NextCounter(pCounter);
		}
	}
	return TRUE;	
}

BOOL KProcessPerfMgr::PrintTotalInfo()
{
	if (m_pTotalProcessData)
	{
		printf("total process io : %*I64d\n", 10, m_pTotalProcessData->GetIOSpeed());
	}
	return FALSE;
}

