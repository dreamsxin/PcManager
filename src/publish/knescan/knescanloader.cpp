/**
* @file    knescanloader.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-07-08 11:02
*/

#include "stdafx.h"
#include "knescanloader.h"

#include "winmod\winpath.h"

using namespace WinMod;

static HMODULE g_hMod_knescan = NULL;

static PFN_NewKnEScan   g_pfnNewKnEScan  = NULL;
static PFN_FreeKnEScan  g_pfnFreeKnEScan = NULL;

EXTERN_C HRESULT WINAPI KnEScanLoad(LPCWSTR lpszEngineDir)
{
    if (g_hMod_knescan)
        return S_FALSE;


    CWinPath pathEngineDir;
    if (lpszEngineDir)
    {
        pathEngineDir = lpszEngineDir;
    }
    else
    {
        pathEngineDir.GetModuleFileName((HMODULE)&__ImageBase);
        pathEngineDir.RemoveFileSpec();
    }


    pathEngineDir.AddBackslash();
    g_hMod_knescan = ::LoadLibrary(pathEngineDir.m_strPath + KNESCAN_DLL_NAME);
    if (!g_hMod_knescan)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    g_pfnNewKnEScan = (PFN_NewKnEScan)GetProcAddress(g_hMod_knescan, KNESCAN_PFN_NewKnEScanObject);
    g_pfnFreeKnEScan = (PFN_FreeKnEScan)GetProcAddress(g_hMod_knescan, KNESCAN_PFN_FreeKnEScanObject);


    return S_OK;
}

EXTERN_C HRESULT WINAPI KnEScanFile(DWORD dwSource, LPCTSTR lpFilePath, KnEScanResult& Result)
{
    if (!lpFilePath || !*lpFilePath)
        return E_INVALIDARG;


    if (!g_pfnNewKnEScan || !g_pfnFreeKnEScan)
        return E_HANDLE;


    IKnEScan* piKnEScan = g_pfnNewKnEScan();
    if (!piKnEScan)
        return E_POINTER;


    HRESULT hrScan = piKnEScan->Scan(dwSource, lpFilePath, Result);


    g_pfnFreeKnEScan(&piKnEScan);


    return hrScan;
}