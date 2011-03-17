/**
* @file    skylarkpath_userdir.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-08-19 11:08
*/

#include "stdafx.h"
#include "skylarkpath.h"

#include "winmod\winpath.h"
#include "common\utility.h"

NS_SKYLARK_USING

#define SDK_APP_PATH L"KSafe_SDK"

using namespace WinMod;

inline HRESULT CreateSDKAppPath(LPCWSTR lpszSubDir, CString& strPath, BOOL bCreate)
{
    WCHAR szPath[MAX_PATH];
    ::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, bCreate);
    ::PathAppend(szPath, SDK_APP_PATH);

    if (lpszSubDir && *lpszSubDir)
        ::PathAppend(szPath, lpszSubDir);

    strPath = szPath;

    if (bCreate)
    {
        HRESULT hr = CAppPath::Instance().CreateFullPath(strPath);
        if (S_OK == hr)
        {
            hr = CAppPath::Instance().FixFilePrivilege(strPath);
            if (FAILED(hr))
                hr = S_FALSE;
        }
    }

    return S_OK;
}



HRESULT CSkylarkPath::GetDataPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(DATA_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetRuntimeDataPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(LOG_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetRecyclePath(LPCWSTR lpszRecycleFile, CString& strPath, BOOL bCreate)
{
    HRESULT hr = E_FAIL;
    if (bCreate)
    {
        hr = CAppPath::Instance().CreateLeidianRecyclePath(lpszRecycleFile, strPath);
    }
    else
    {
        hr = CAppPath::Instance().GetLeidianRecyclePath(lpszRecycleFile, strPath);
    }

    if (FAILED(hr))
        return hr;


    CWinPathApi::SetFileAces(
        strPath,
        Sids::World(),
        FILE_GENERIC_READ,
        FILE_EXECUTE,
        OBJECT_INHERIT_ACE | CONTAINER_INHERIT_ACE | INHERIT_ONLY_ACE);


    return S_OK;
}

HRESULT CSkylarkPath::GetQuarantinePath(CString& strPath, BOOL bCreate)
{
    if (bCreate)
    {
        return CAppPath::Instance().CreateLeidianQuarantinePath(strPath);
    }
    else
    {
        return CAppPath::Instance().GetLeidianQuarantinePath(strPath);
    }
}

HRESULT CSkylarkPath::GetLogPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(LOG_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetCachePath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianCachePath(strPath, bCreate);
}

HRESULT CSkylarkPath::GetScanTempPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(TMP_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetTempPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(TMP_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetAvsPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(AVS_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::GetConfigPath(CString& strPath, BOOL bCreate)
{
    return CreateSDKAppPath(CFG_DIR, strPath, bCreate);
}

HRESULT CSkylarkPath::CreateFullPath(LPCWSTR lpszFullPath)
{
    return CAppPath::Instance().CreateFullPath(lpszFullPath);
}
