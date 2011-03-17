/**
* @file    winlogfile.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-02-02 13:33
*/

#include "stdafx.h"
#include "winlogfile.h"

using namespace WinMod;

#define WINMOD_MAX_RECORD_IN_PENDING_QUEUE  5

CWinLogFile::CWinLogFile()
    : m_dwMaxLogLevel(WINMOD_LLVL_DEFAULT)
{
    
}

HRESULT CWinLogFile::CreateAsLogFile(LPCWSTR lpszFilePath)
{
    if (!lpszFilePath)
        return E_POINTER;


    ::DeleteFile(lpszFilePath);
    HRESULT hr = CAtlFile::Create(
        lpszFilePath,
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        OPEN_ALWAYS);
    if (FAILED(hr))
        return hr;


    return S_OK;
}

void CWinLogFile::SetLogLevel(DWORD dwMaxLogLevel)
{
    m_dwMaxLogLevel = dwMaxLogLevel;
}

BOOL CWinLogFile::IsLevelEnabled(DWORD dwLogLevel)
{
    return dwLogLevel <= m_dwMaxLogLevel;
}

LPCSTR CWinLogFile::GetLogLevelNameA(DWORD dwLogLevel)
{
    switch (dwLogLevel)
    {
    case WINMOD_LLVL_ALWAYS:        return "ALWAYS";
    case WINMOD_LLVL_CRITICAL:      return "CRIT";
    case WINMOD_LLVL_ERROR:         return "ERROR";
    case WINMOD_LLVL_WARNING:       return "WARN";
    case WINMOD_LLVL_INFO:          return "INFO";
    case WINMOD_LLVL_DEBUG:         return "DEBUG";
    case WINMOD_LLVL_TRACE:         return "TRACE";
    default:                        return "-";
    }
}


void CWinLogFile::Log(DWORD dwLogLevel, LPCWSTR lpszText)
{
    if (!IsLevelEnabled(dwLogLevel))
        return;

    if (!CAtlFile::m_h || INVALID_HANDLE_VALUE == CAtlFile::m_h)
        return;

    CStringA strLogLine;

    
    SYSTEMTIME  localTime;
    ::GetLocalTime(&localTime);
    strLogLine.AppendFormat(
        "[%04hu-%02hu-%02hu %02hu:%02hu:%02hu.%03hu] ",
        localTime.wYear,
        localTime.wMonth,
        localTime.wDay,
        localTime.wHour,
        localTime.wMinute,
        localTime.wSecond,
        localTime.wMilliseconds);

    
    strLogLine.AppendFormat("[%lu] ", GetCurrentThreadId());

    
    strLogLine.AppendFormat("[%s] ", GetLogLevelNameA(dwLogLevel));

    
    strLogLine.Append(": ");
    strLogLine.Append(CW2A(lpszText));

    
    CHAR chLast = strLogLine[strLogLine.GetLength() - 1];
    if ('\r' != chLast && '\n' != chLast)
    {
        strLogLine.Append("\r\n");
    }


    
    {
        CObjGuard guard(m_queueLock);

        HRESULT hr = CAtlFile::Write(strLogLine, strLogLine.GetLength());
    }
}

void CWinLogFile::LogFormat(DWORD dwLogLevel, LPCWSTR lpszFormat, ...)
{
    if (!IsLevelEnabled(dwLogLevel))
        return;

    va_list argList;
    va_start(argList, lpszFormat);

    LogFormatV(dwLogLevel, lpszFormat, argList);

    va_end( argList );
}

void CWinLogFile::LogFormatV(DWORD dwLogLevel, LPCWSTR lpszFormat, va_list args)
{
    if (!IsLevelEnabled(dwLogLevel))
        return;

    CString strMessage;
    strMessage.FormatV(lpszFormat, args);
    Log(dwLogLevel, strMessage);
}