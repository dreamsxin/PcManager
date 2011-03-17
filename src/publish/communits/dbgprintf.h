// dummyz@126.com
#pragma once

#define DBG_FMT 0

//#pragma warning(disable : 4995)

#if DBG_FMT

#include <Shlwapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <tchar.h>
#pragma comment(lib, "shlwapi.lib")

inline BOOL CanDebugPrintf()
{
	static int a = 0;
	if ( a == 0 )
	{
#ifdef _DEBUG
		a = 1;
#else
		a = PathFileExists(_T("c:\\enable_dbgprintf")) ? 1 : -1;
#endif
	}

	return (a == 1);
}

inline void DebugPrintf(LPCWSTR lpFormat, ...)
{
	if ( !CanDebugPrintf() ) return;

	const int nBufLen = MAX_PATH * 2;
	WCHAR szBuf[nBufLen];
	va_list ap;
	
	va_start(ap, lpFormat);
	_vsnwprintf(szBuf, nBufLen, lpFormat, ap);
	szBuf[nBufLen - 1] = 0;

	OutputDebugStringW(szBuf);

#if 0
	_tsetlocale(LC_ALL, _T("chs"));
	DWORD dwSessionId = 0;
	TCHAR szFilePath[MAX_PATH];

	ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId);
	_stprintf(szFilePath, _T("c:\\ksafemon_%d.txt"), dwSessionId);

	FILE* lpFile = _tfopen(szFilePath, _T("a+"));
	if ( lpFile != NULL )
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		
		LPCTSTR lpStr = _tcschr(lpFormat, _T('\n'));
		if ( lpStr != NULL )
		{
			lpStr = _T("[%02d:%02d:%02d]%s");
		}
		else
		{
			lpStr = _T("[%02d:%02d:%02d]%s\n");
		}

		_ftprintf(lpFile, lpStr, st.wHour, st.wMinute, st.wSecond, szBuf);
		fclose(lpFile);
	}
#endif

	va_end(ap);
}

#else
#define		DebugPrintf(x, ...)
#endif

class CFunctionTime
{
public:
	CFunctionTime(LPCSTR lpName)
	{
		m_lpFunName = lpName;
		m_dwBeginTime = GetTickCount();
	}

	~CFunctionTime(void)
	{
		DWORD dwTime =  GetTickCount() - m_dwBeginTime;
		DebugPrintf(_T("Function: %S Time: %d ms\n"), m_lpFunName, dwTime);
	}

private:
	LPCSTR	m_lpFunName;
	DWORD	m_dwBeginTime;
};

#if DBG_FMT
#define FUNCTION_TIME_PRINT	CFunctionTime __ftpx_tmp(__FUNCTION__);
#define FUNCTION_TIME_PRINT_(x)	CFunctionTime __ftpx_tmp2(__FUNCTION__#x);
#else
#define FUNCTION_TIME_PRINT	
#define FUNCTION_TIME_PRINT_(x)
#endif
