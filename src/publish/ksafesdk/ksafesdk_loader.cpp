/**
* @file    ksafesdk_loader.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-08-06 11:34
*/

#include <windows.h>
#include "ksafesdk_def.h"
#include "ksafesdk_loader.h"

#pragma warning(disable: 4996)

static HRESULT __stdcall notimpl_ksafeInitialize(PFN_ksafeCallbackW pCallback) { return E_NOTIMPL;}
static HRESULT __stdcall notimpl_ksafeUninitialize() { return E_NOTIMPL;}
static HRESULT __stdcall notimpl_ksafeScanFileW(
    /*[in]*/  const wchar_t*    pScanFileName,
    /*[in]*/  unsigned long     dwScanningMode,
    /*[in]*/  unsigned long     dwCleaningMode,
    /*[in]*/  void*             pUserContext,
    /*[out]*/ KSAFE_RESULT*     pResult) {return E_NOTIMPL;}
static HRESULT __stdcall notimpl_ksafeFlushAsyncScan() {return E_NOTIMPL;}


#define DEF_KSAFESDK_FUNC(func__, name__)                   \
    PFN_##func__ func__ = notimpl_##func__;                 \
    static const char * const g_funcname_##func__ = name__;


DEF_KSAFESDK_FUNC(ksafeInitialize,      "ksafeInitialize")
DEF_KSAFESDK_FUNC(ksafeUninitialize,    "ksafeUninitialize")
DEF_KSAFESDK_FUNC(ksafeScanFileW,       "ksafeScanFileW")
DEF_KSAFESDK_FUNC(ksafeFlushAsyncScan,  "ksafeFlushAsyncScan")



void ClearDLLFunctions()
{
#define CLR_KSAFESDK_FUNC(func__) func__ = notimpl_##func__;

    CLR_KSAFESDK_FUNC(ksafeInitialize)
    CLR_KSAFESDK_FUNC(ksafeUninitialize)
    CLR_KSAFESDK_FUNC(ksafeScanFileW)
    CLR_KSAFESDK_FUNC(ksafeFlushAsyncScan)
}



bool LoadDLLFunctions(HMODULE hModule)
{
    bool bRet = true;

#define GET_KSAFESDK_FUNC(func__)                                           \
    func__ = (PFN_##func__) GetProcAddress(hModule, g_funcname_##func__);     \
    if (!func__) {bRet = false;}

    GET_KSAFESDK_FUNC(ksafeInitialize)
    GET_KSAFESDK_FUNC(ksafeUninitialize)
    GET_KSAFESDK_FUNC(ksafeScanFileW)
    GET_KSAFESDK_FUNC(ksafeFlushAsyncScan)

    if (!bRet)
        ClearDLLFunctions();

    return bRet;
}


static const wchar_t * const    G_WSZ_MODULE_NAME       = L"ksafesdk.dll";
static unsigned long            g_dwLoadModuleCounter   = 0;
static HMODULE                  g_hModule               = 0;


//////////////////////////////////////////////////////////////////////////////

HRESULT ksafeLoadW(const wchar_t *pLoadFolder)
{
    if (0 == g_dwLoadModuleCounter)
    {
        wchar_t wszFullDLLPath[MAX_PATH + 1];
        *wszFullDLLPath = 0;
        if (pLoadFolder && *pLoadFolder)
        {
            if (wcslen(pLoadFolder) + wcslen(G_WSZ_MODULE_NAME) >= MAX_PATH) 
                return E_INVALIDARG;

            wcscpy(wszFullDLLPath, pLoadFolder);
            if (wszFullDLLPath[ wcslen(wszFullDLLPath)-1] != L'\\')
                wcscat(wszFullDLLPath, L"\\");
        }
        else
        {
            if (!GetModuleFileNameW( 0, wszFullDLLPath, MAX_PATH))
                return GetLastError() ? HRESULT_FROM_WIN32(GetLastError()) : E_FAIL;

            *(wcsrchr(wszFullDLLPath, L'\\') + 1) = 0;
            if (wcslen(wszFullDLLPath) + wcslen(G_WSZ_MODULE_NAME) >= _MAX_PATH)
                return E_INVALIDARG;
        }
        wcscat(wszFullDLLPath, G_WSZ_MODULE_NAME);

        g_hModule = LoadLibraryExW(wszFullDLLPath, 0, LOAD_WITH_ALTERED_SEARCH_PATH );
        if (!g_hModule)
            return GetLastError() ? HRESULT_FROM_WIN32(GetLastError()) : E_FAIL;

        if (!LoadDLLFunctions(g_hModule))
        {
            FreeLibrary(g_hModule);
            return GetLastError() ? HRESULT_FROM_WIN32(GetLastError()) : E_FAIL;
        }
    }

    g_dwLoadModuleCounter++;
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////

HRESULT ksafeUnload()
{
    if (g_dwLoadModuleCounter == 0)
    {
        return E_UNEXPECTED;
    }
    else if (g_dwLoadModuleCounter == 1)
    {
        ClearDLLFunctions();
        if (!FreeLibrary( g_hModule ) )
        {
            return GetLastError() ? HRESULT_FROM_WIN32(GetLastError()) : E_FAIL;
        }

        g_hModule = 0;
    }

    g_dwLoadModuleCounter--;
    return S_OK;
}
