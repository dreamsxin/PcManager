// yykingking@126.com
//
#include "stdafx.h"
#include "ProcessMgr.h"
#include <strsafe.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

typedef LONG (WINAPI *PFNTQUERYINFORMATIONPROCESS)(HANDLE,UINT,PVOID,ULONG,PULONG);
PFNTQUERYINFORMATIONPROCESS	NtQueryInformationProcess = NULL;

CProgressMgr::CProgressMgr()
{
	InitNTQueryApi();
}

CProgressMgr::~CProgressMgr()
{
	UnLoad();
}

void CProgressMgr::Load()
{
	m_tasks.clear();

	DWORD*	arrayPid = new DWORD[1024];
	DWORD	cbNeeds  = 0;

	::EnumProcesses(arrayPid,1024,&cbNeeds);

	cbNeeds /= 4;
	for ( DWORD nIndex = 0; nIndex < cbNeeds; nIndex++)
	{
		if ( arrayPid[nIndex] <= 8 )
			continue;

		HANDLE	hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, arrayPid[nIndex] );

		if ( hProcess != NULL )
		{
			PROCESSENTRY	psEntry;
			memset(&psEntry,0,sizeof(PROCESSENTRY));

			::GetModuleFileNameEx( hProcess, NULL, psEntry.szFullPath, MAX_PATH);
			GetCommandLine(arrayPid[nIndex], psEntry.szCommondLine, MAX_PATH);

			::CloseHandle(hProcess);
			psEntry.nProcId = arrayPid[nIndex];
			m_tasks.push_back(psEntry);
		}
	}
	delete arrayPid;
}

void CProgressMgr::UnLoad()
{
	m_tasks.clear();
}

BOOL CProgressMgr::InitNTQueryApi()
{
	if(!NtQueryInformationProcess)
	{
		HMODULE hNtDll = GetModuleHandle( _T("ntdll.dll") );
		if(hNtDll)
			NtQueryInformationProcess = (PFNTQUERYINFORMATIONPROCESS)GetProcAddress(
			hNtDll, "NtQueryInformationProcess");

		if(!NtQueryInformationProcess)
			return FALSE;
	}

	return TRUE;
}

void CProgressMgr::GetCommandLine( DWORD dwPid, TCHAR* pszCmd, int nLen )
{
	HANDLE	hProc;
	DWORD	dwRet;
	LONG	nStatus;
	PEB		peb;
	BOOL	bRet = FALSE;
	WCHAR	wsPath[MAX_PATH] = {0};
	PROCESS_BASIC_INFORMATION	pbi;
	RTL_USER_PROCESS_PARAMETERS	rupp;

	if( NtQueryInformationProcess == NULL)
		return ;

	hProc = OpenProcess( PROCESS_VM_READ | PROCESS_QUERY_INFORMATION|PROCESS_VM_OPERATION, FALSE, dwPid);
	if ( hProc == NULL )
		return ;

	nStatus = NtQueryInformationProcess( hProc, ProcessBasicInformation, (PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION), NULL);

	__try
	{
		if(ReadProcessMemory(hProc, (LPVOID)pbi.PebBaseAddress, &peb, sizeof(PEB), &dwRet))
		{
			if(ReadProcessMemory(hProc, (LPVOID)peb.ProcessParameters, &rupp, sizeof(rupp), &dwRet))
			{
				if( rupp.CommandLine.Length < sizeof(wsPath)/sizeof(WCHAR) && rupp.CommandLine.Length > 0  ) 
				{
					if(ReadProcessMemory(hProc, (LPVOID)rupp.CommandLine.Buffer, wsPath, rupp.CommandLine.Length, &dwRet) )
					{
#if _UNICODE
						_tcsncpy_s( pszCmd, nLen, wsPath, rupp.CommandLine.Length);
#else
						WideCharToMultiByte( CP_ACP, 0, wsPath, rupp.CommandLine.Length, pszCmd, nLen, NULL, NULL );
#endif
					}
				}
			}
		}
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
	}

	CloseHandle(hProc);
	return ;
}

BOOL CProgressMgr::KillByPath( LPCTSTR lpPath )
{
	if ( m_tasks.size() == 0 )
		return FALSE;

	for ( DWORD nIndex = 0; nIndex < m_tasks.size(); nIndex++)
	{
		if ( m_tasks[nIndex].nProcId != 0 && StrStrI(m_tasks[nIndex].szFullPath, lpPath) != 0 )
		{
			return KillTask( nIndex, TRUE);
		}
	}
	return FALSE;
}

BOOL CProgressMgr::KillByCMD( LPCTSTR lpCommondLine )
{
	if ( m_tasks.size() == 0 )
		return FALSE;

	for ( DWORD nIndex = 0; nIndex < m_tasks.size(); nIndex++)
	{
		if ( m_tasks[nIndex].nProcId != 0 && StrStrI(m_tasks[nIndex].szCommondLine, lpCommondLine) != 0 )
		{
			return KillTask( nIndex, TRUE);
		}
	}
	return FALSE;
}

BOOL CProgressMgr::KillTask( DWORD nEntryId, BOOL bRemoveFile )
{
	if ( nEntryId >= m_tasks.size() )
		return FALSE;
	
	if ( m_tasks[nEntryId].nProcId == GetCurrentProcessId() || m_tasks[nEntryId].nProcId == 0 )
		return FALSE;

	BOOL	bKill = FALSE;
	HANDLE	hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_tasks[nEntryId].nProcId );
	if ( hProcess )
	{
		bKill = ::TerminateProcess( hProcess, 0);
		::CloseHandle(hProcess);
	}
	if (bKill)
		m_tasks[nEntryId].nProcId = 0;

	return bKill;
}


#if X_SUPPORT_CSTRING
BOOL CProgressMgr::GetFullPath( LPCTSTR lpPath, CString& strFullPath )
{
	if ( m_tasks.size() == 0 )
		return FALSE;

	for ( DWORD nIndex = 0; nIndex < m_tasks.size(); nIndex++)
	{
		if ( m_tasks[nIndex].nProcId != 0 && StrStrI(m_tasks[nIndex].szFullPath, lpPath) != 0 )
		{
			strFullPath = m_tasks[nIndex].szFullPath;

			return TRUE;
		}
	}
	return FALSE;
}
#endif
