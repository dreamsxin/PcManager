////////////////////////////////////////////////////////////////////////////////
//      
//      File for netmon
//      
//      File      : msg_logger.h
//      Version   : 1.0
//      Comment   : 网镖自己的日志记录
//      
//      Create at : 2008-4-7
//      Create by : liupeng
//      
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shlwapi.h"
#include <iostream>
#include <fstream>
#include "time.h"
#include "strsafe.h"
#include <atlconv.h>
using namespace std;

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
class _KNetmonMsgLogger
{
private:
	WCHAR		m_szPath[MAX_PATH];
	WCHAR		m_szOldPath[MAX_PATH];
	WCHAR		m_szNewPath[MAX_PATH];
	BOOL		m_Log;
	CRITICAL_SECTION  m_sec;

public:
	_KNetmonMsgLogger(LPCWSTR oldFile, LPCWSTR logFile):m_Log(FALSE)
	{
		memset(&m_sec, 0, sizeof(CRITICAL_SECTION));
		::InitializeCriticalSection(&m_sec);

		::GetModuleFileName(NULL, m_szOldPath, MAX_PATH);
		WCHAR* pStr = _tcsrchr(m_szOldPath, L'\\');
		if (pStr)
			pStr[1] = 0;

		wcscat_s(m_szOldPath, MAX_PATH, TEXT("log\\"));
		if (!PathFileExists(m_szOldPath))
			::CreateDirectory(m_szOldPath, NULL);

		SYSTEMTIME time = {0};
		::GetSystemTime(&time);
		WCHAR buf[64] = {0};
		swprintf_s(buf, 64, TEXT("%d_%d_%d_%d_%d_%d_"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		wcscat_s(m_szOldPath, MAX_PATH, buf);
		wcscat_s(m_szOldPath, MAX_PATH, oldFile);

		::GetModuleFileName(NULL, m_szNewPath, MAX_PATH);
		pStr = _tcsrchr(m_szNewPath, L'\\');
		if (pStr)
			pStr[1] = 0;
		wcscat_s(m_szNewPath, MAX_PATH, logFile);

		::GetModuleFileName(NULL, m_szPath, MAX_PATH);
		pStr = _tcsrchr(m_szPath, L'\\');
		if (pStr)
			pStr[1] = 0;
		wcscat_s(m_szPath, MAX_PATH, L"debug.log");

		if (::PathFileExists(m_szPath))
			m_Log = TRUE;
	}

	~_KNetmonMsgLogger()
	{
		::DeleteCriticalSection(&m_sec);
	}

	void RemoveLogDir(WCHAR DirSpec[MAX_PATH])
	{
		WIN32_FIND_DATA FindFileData = {0};
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError = 0;

		WCHAR dirSearch[MAX_PATH] = {0};
		wcscpy_s(dirSearch, MAX_PATH, DirSpec);

		WCHAR curDir[MAX_PATH] = {0};
		::GetCurrentDirectory(MAX_PATH, curDir);

		::SetCurrentDirectory(dirSearch);
		wcscat_s(dirSearch, MAX_PATH, TEXT("*"));

		hFind = FindFirstFile(dirSearch, &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE) 
		{
			::SetCurrentDirectory(curDir);
			return;
		} 
		else 
		{
			while (FindNextFile(hFind, &FindFileData) != 0) 
			{
				::DeleteFile(FindFileData.cFileName);
			}

			dwError = GetLastError();
			FindClose(hFind);
			if (dwError != ERROR_NO_MORE_FILES) 
			{
				::SetCurrentDirectory(curDir);
				return ;
			}
		}
		::SetCurrentDirectory(curDir);
	}
	void forceclear()
	{
		WCHAR szLogPath[MAX_PATH] = {0};

		::GetModuleFileName(NULL, szLogPath, MAX_PATH);
		WCHAR* pStr = _tcsrchr(szLogPath, L'\\');
		if (pStr)
			pStr[1] = 0;
		wcscat_s(szLogPath, MAX_PATH, L"log\\");
		RemoveLogDir(szLogPath);
		::RemoveDirectory(szLogPath);
		::DeleteFile(m_szNewPath);
	}

	void clear()
	{
		::CopyFile(m_szNewPath, m_szOldPath, FALSE);
		::DeleteFile(m_szNewPath);
	}

	void log_msg(const wchar_t *format, ...)
	{
		INT nCnt = 0;
		while ( !::TryEnterCriticalSection(&m_sec) ) //已经被其他线程拿住
		{
			nCnt++;
			if (nCnt > 1000)
				return;
			::Sleep(1);
		}

		//EnterCriticalSection(&m_sec);
		do 
		{
			if (!m_Log)
				break;


			HANDLE hFile = NULL;
			if (::PathFileExists(m_szNewPath))
			{
				hFile = ::CreateFile(m_szNewPath,
					GENERIC_READ | GENERIC_WRITE,
					0, //FILE_SHARE_READ|FILE_SHARE_WRITE, 
					NULL, 
					OPEN_EXISTING, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
				if (hFile == INVALID_HANDLE_VALUE)
					break;
			}
			else
			{
				hFile = ::CreateFile(m_szNewPath,
					GENERIC_READ | GENERIC_WRITE,
					0, //FILE_SHARE_READ|FILE_SHARE_WRITE, 
					NULL, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
				if (hFile == INVALID_HANDLE_VALUE)
					break;
				BYTE head[2] = {0xff, 0xfe};
				DWORD nWrite = 0;
				if (!::WriteFile(hFile, head, 2, &nWrite, NULL))
				{
					CloseHandle(hFile);
					break;
				}
			}
				
			::SetFilePointer(hFile, 0, NULL, FILE_END);

			wchar_t           szargs[MAX_PATH*4] = { 0 };
			va_list         args;

			va_start(args, format);
			StringCbVPrintf(szargs, MAX_PATH*4, format, args);
			va_end(args);

			time_t          now;
			struct tm       ptm;
			wchar_t           sztime[100] = { 0 };
			now = time(NULL);
			localtime_s(&ptm, &now);
			_tcsftime(sztime, (sizeof(sztime) / sizeof(sztime[0])), TEXT("%Y-%m-%d %H:%M:%S"), &ptm);

			DWORD nWrite = 0;

			::WriteFile(hFile, sztime, 2*(DWORD)wcslen(sztime), &nWrite, NULL);
			::WriteFile(hFile, TEXT("\t"), 2*(DWORD)wcslen(TEXT("\t")), &nWrite, NULL);
			::WriteFile(hFile, szargs, 2*(DWORD)wcslen(szargs), &nWrite, NULL);
			::WriteFile(hFile, TEXT("\r\n"), 2*(DWORD)wcslen(TEXT("\r\n")), &nWrite, NULL);
			
			if (hFile)
				CloseHandle(hFile);

		} while (false);
		
		LeaveCriticalSection(&m_sec);
	}

	void	nothing()
	{

	}
	void	forcelog()
	{
		m_Log = TRUE;
	}

	static _KNetmonMsgLogger* NetmonInstance()
	{	
		static _KNetmonMsgLogger logger(L"old_netmon.log", L"netmon.log");
		return &logger;
	}

	static _KNetmonMsgLogger* KasInstance()
	{
		static _KNetmonMsgLogger logger(L"old_kas.log", L"kas.log");
		return &logger;
	}

	static _KNetmonMsgLogger* KwsInstance()
	{
		static _KNetmonMsgLogger logger(L"old_kws.log", L"kws.log");
		return &logger;
	}

	static _KNetmonMsgLogger* KwstrayInstance()
	{
		static _KNetmonMsgLogger logger(L"old_kwstray.log", L"kwstray.log");
		return &logger;
	}

	static _KNetmonMsgLogger* KsafetrayInstance()
	{
		static _KNetmonMsgLogger logger(L"old_ksafetray.log", L"ksafetray.log");
		return &logger;
	}
};

#define NETMONLOG		

#ifdef	NETMONLOG
#define netmon_log _KNetmonMsgLogger::NetmonInstance()->log_msg
#define netmon_log_clear _KNetmonMsgLogger::NetmonInstance()->clear
#else
#define netmon_log 
#define netmon_log_clear _KNetmonMsgLogger::NetmonInstance()->nothing
#endif

#define kas_log _KNetmonMsgLogger::KasInstance()->log_msg
#define kas_log_clear _KNetmonMsgLogger::KasInstance()->clear

// #if _DEBUG
#define kws_log	_KNetmonMsgLogger::KwsInstance()->log_msg
#define kws_log_clear _KNetmonMsgLogger::KwsInstance()->clear
#define kws_log_forceclear _KNetmonMsgLogger::KwsInstance()->forceclear
#define kws_log_forcelog _KNetmonMsgLogger::KwsInstance()->forcelog

#define kwstray_log	_KNetmonMsgLogger::KwstrayInstance()->log_msg
#define kwstray_log_clear _KNetmonMsgLogger::KwstrayInstance()->clear
#define kwstray_log_forceclear _KNetmonMsgLogger::KwstrayInstance()->forceclear

#define ksafetray_log	_KNetmonMsgLogger::KsafetrayInstance()->log_msg
#define ksafetray_log_clear _KNetmonMsgLogger::KsafetrayInstance()->clear
#define ksafetray_log_forceclear _KNetmonMsgLogger::KsafetrayInstance()->forceclear

// #else
// #define kws_log	_KNetmonMsgLogger::KwsInstance()->nothing
// #define kws_log_clear _KNetmonMsgLogger::KwsInstance()->nothing
// #endif