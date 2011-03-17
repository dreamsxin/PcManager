/**
* @file    skylarkpath.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-08-17 17:23
*/

#include "stdafx.h"
#include "skylarkpath.h"

#include "winmod\winpath.h"
#include "common\utility.h"

NS_SKYLARK_USING
using namespace WinMod;

HRESULT CSkylarkPath::GetDataPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianDataPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::GetRuntimeDataPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianLogPath(strPath, bCreate, FALSE);
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
    return CAppPath::Instance().GetLeidianLogPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::GetCachePath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianCachePath(strPath, bCreate);
}

HRESULT CSkylarkPath::GetScanTempPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianTempPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::GetTempPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianTempPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::GetAvsPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianAvsPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::GetConfigPath(CString& strPath, BOOL bCreate)
{
    return CAppPath::Instance().GetLeidianCfgPath(strPath, bCreate, FALSE);
}

HRESULT CSkylarkPath::CreateFullPath(LPCWSTR lpszFullPath)
{
    return CAppPath::Instance().CreateFullPath(lpszFullPath);
}
