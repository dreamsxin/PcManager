/**
* @file    bkdbg.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-12-13 10:31
*/

#include "stdafx.h"
#include "bkdbg.h"

#pragma warning(push)
#pragma warning(disable: 4996)  ///< 'function': was declared deprecated


#ifdef _DEBUG
#define BKDBG_LVL_DEFAULT_MAX_LVL   BKDBG_LVL_TRACE
#else
#define BKDBG_LVL_DEFAULT_MAX_LVL   BKDBG_LVL_CRITICAL
#endif


static DWORD g_dwDbgOutput = BKDBG_LVL_DEFAULT_MAX_LVL;



inline CString GetDebugLevelString(DWORD dwDbgLevel)
{
    switch (dwDbgLevel)
    {
    case BKDBG_LVL_DEFAULT:     return L"default";
    case BKDBG_LVL_ALWAYS:      return L"always";
    case BKDBG_LVL_CRITICAL:    return L"crit";
    case BKDBG_LVL_ERROR:       return L"error";
    case BKDBG_LVL_WARNING:     return L"warn";
    case BKDBG_LVL_INFO:        return L"info";
    case BKDBG_LVL_DEBUG:       return L"debug";
    case BKDBG_LVL_TRACE:       return L"trace";
    }

    CString str;
    str.Format(L"%lu", g_dwDbgOutput);
    return str;
}



EXTERN_C BOOL WINAPI BKInitDebugOutput(DWORD dwDbgMaxLevel)
{
    if (BKDBG_LVL_DEFAULT == dwDbgMaxLevel)
    {
        // 根据exe所在目录的配置文件beike.ini, 决定是否输出调试日志
        WCHAR szExePath[MAX_PATH + 1];
        ::GetModuleFileName(NULL, szExePath, MAX_PATH);
        szExePath[MAX_PATH] = L'\0';

        ::PathRemoveFileSpec(szExePath);
        ::PathAddBackslash(szExePath);
        ::PathAppend(szExePath, BEIKE_DBG_INI_FILE);


        // 获取模块名
        WCHAR szDllPath[MAX_PATH + 1];
        ::GetModuleFileName((HMODULE)&__ImageBase, szDllPath, MAX_PATH);
        szDllPath[MAX_PATH] = L'\0';

        LPCWSTR lpFileName = ::PathFindFileName(szDllPath);
        WCHAR szDllName[MAX_PATH + 1];
        ::wcsncpy(szDllName, lpFileName, MAX_PATH);
        szDllName[MAX_PATH] = L'\0';



        // 读取配置
        UINT uRet = ::GetPrivateProfileInt(BEIKE_DBG_INI_APP_DEBUG, szDllName, UINT_MAX, szExePath);
        if (uRet == UINT_MAX)
        {   // 默认设置
            g_dwDbgOutput = BKDBG_LVL_DEFAULT_MAX_LVL;
        }
        else
        {
            g_dwDbgOutput = uRet;
        }



        


        
        LPCWSTR lpszCompileOption = L"Null";
#ifdef _DEBUG
        lpszCompileOption = L"Debug";
#else
        lpszCompileOption = L"Release";
#endif



        BKDebugOutput(
            BKDBG_LVL_INFO,
            L"[beike][%s] %s DebugLevel %s\r\n",
            lpszCompileOption, 
            szDllName,
            GetDebugLevelString(g_dwDbgOutput));
    }


    return g_dwDbgOutput;
}

EXTERN_C BOOL WINAPI BKIsDebugOutputEnabled(DWORD dwDbgLevel)
{
    return dwDbgLevel <= g_dwDbgOutput;
}

EXTERN_C void WINAPI BKDebugEcho(DWORD dwDbgLevel, LPCWSTR lpszFormat)
{
    if (!BKIsDebugOutputEnabled(dwDbgLevel))
        return;

    ::OutputDebugString(lpszFormat);
}

EXTERN_C void WINAPI BKDebugOutput(DWORD dwDbgLevel, LPCWSTR lpszFormat, ...)
{
    if (!BKIsDebugOutputEnabled(dwDbgLevel))
        return;

    va_list argList;
    va_start(argList, lpszFormat);

    BKDebugOutputV(dwDbgLevel, lpszFormat, argList);

    va_end( argList );
}

EXTERN_C void WINAPI BKDebugOutputV(DWORD dwDbgLevel, LPCWSTR lpszFormat, va_list args)
{
    if (!BKIsDebugOutputEnabled(dwDbgLevel))
        return;

    CString strMessage;
    strMessage.FormatV(lpszFormat, args);
    BKDebugEcho(dwDbgLevel, strMessage);
}

#pragma warning(pop)