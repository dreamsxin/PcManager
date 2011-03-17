
#pragma once
#include "Psapi.h"

struct CallBackStruct
{
	int nTotalProc;
	int nFreeProc;

	CallBackStruct(int a, int b)
	{
		nTotalProc = a;
		nFreeProc = b;
	}
};

typedef void (*PFNCallBack)(CallBackStruct& param, void* pContext);

class CSysPerfOpt
{
public:
	~CSysPerfOpt(){
		m_pfnCallBack	= NULL;
		m_pContext		= NULL;
	};


	static CSysPerfOpt* Instance()
	{
		static CSysPerfOpt perfopt;
		return &perfopt;
	}

	//////////////////////////////////////////////////////////////////////////
	//强制释放内存
	ULONGLONG	ForceFreeMem( BOOL bCallBack = TRUE )
	{
		ULONGLONG uFreeSize = 0;
		MEMORYSTATUSEX statex = {0};
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx (&statex);
		ULONGLONG nMem = statex.ullAvailPhys;

		DWORD nProcessID[1000] = {0};
		DWORD nCount = sizeof(nProcessID);
		DWORD nReturn = 0;
		int nTotalCount = 0, nFix = 0;
		if (EnumProcesses(nProcessID, nCount, &nReturn))
		{
			HANDLE hToken = NULL; 
			TOKEN_PRIVILEGES tkp = {0}; 

			if (!OpenProcessToken(GetCurrentProcess(), 
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
				goto Exit0;

			LookupPrivilegeValue(NULL, SE_DEBUG_NAME, 
				&tkp.Privileges[0].Luid); 
			tkp.PrivilegeCount = 1;  
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
				(PTOKEN_PRIVILEGES)NULL, 0); 

			if (GetLastError() != ERROR_SUCCESS) 
				goto Exit0;

			nTotalCount = nReturn /sizeof(DWORD);

			if( bCallBack )
			{
				if (NULL != m_pfnCallBack)
					m_pfnCallBack(CallBackStruct(nTotalCount, nFix), m_pContext);
			}

			
			for (DWORD i = 0; i < nTotalCount; i++)
			{
				DWORD nID = nProcessID[i];
				HANDLE hHandle = ::OpenProcess(PROCESS_SET_QUOTA, FALSE, nID);
				if (hHandle)
				{
					::SetProcessWorkingSetSize(hHandle, -1, -1);
					::CloseHandle(hHandle);
				}

				if( bCallBack )
				{
					if (NULL != m_pfnCallBack)
						m_pfnCallBack(CallBackStruct(nTotalCount, ++nFix), m_pContext);
				}
			}
			if (hToken)
			{
				::CloseHandle(hToken);
				hToken = NULL;
			}
		}

		GlobalMemoryStatusEx (&statex);
		if (statex.ullAvailPhys > nMem)
		{
			uFreeSize = statex.ullAvailPhys - nMem;
			//printf ("There are %*I64d %sbytes was freed.\n",
			//	WIDTH, uFreeSize/DIV, divisor);
		}

Exit0:
		if( bCallBack )
		{
			if (NULL != m_pfnCallBack)
				m_pfnCallBack(CallBackStruct(nTotalCount, nFix), m_pContext);
		}

		return uFreeSize;
	}

	PFNCallBack SetCallBack(PFNCallBack pfn, void* pContext)
	{
		m_pfnCallBack = pfn;
		PFNCallBack p = m_pfnCallBack;
		m_pContext = pContext;

		return p;
	}
protected:
private:
	CSysPerfOpt(){}


	PFNCallBack		m_pfnCallBack;
	void*			m_pContext;
};

