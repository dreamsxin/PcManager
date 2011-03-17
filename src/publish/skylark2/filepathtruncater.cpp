/**
* @file    filepathtruncater.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-06-12 13:32
*/

#include "stdafx.h"
#include "filepathtruncater.h"

NS_SKYLARK_USING

CFilePathTruncater& CFilePathTruncater::Instance()
{
    static CFilePathTruncater s_Instance;
    return s_Instance;
}

CFilePathTruncater::CFilePathTruncater()
{
    m_Path_Env_Temp = L"%Temp%";
    m_Path_Env_Temp.ExpandEnvironmentStrings();
    m_Path_Env_Temp.ExpandNormalizedPathName();
    m_Path_Env_Temp.AddBackslash();

    m_Path_Env_Tmp = L"%Tmp%";
    m_Path_Env_Tmp.ExpandEnvironmentStrings();
    m_Path_Env_Tmp.ExpandNormalizedPathName();
    m_Path_Env_Tmp.AddBackslash();

    m_Path_Env_ProgramFiles = L"%ProgramFiles%";
    m_Path_Env_ProgramFiles.ExpandEnvironmentStrings();
    m_Path_Env_ProgramFiles.ExpandNormalizedPathName();
    m_Path_Env_ProgramFiles.AddBackslash();

    m_Path_Env_UserProfile = L"%UserProfile%";
    m_Path_Env_UserProfile.ExpandEnvironmentStrings();
    m_Path_Env_UserProfile.ExpandNormalizedPathName();
    m_Path_Env_UserProfile.AddBackslash();

    m_Path_Env_WinDir = L"%WinDir%";
    m_Path_Env_WinDir.ExpandEnvironmentStrings();
    m_Path_Env_WinDir.ExpandNormalizedPathName();
    m_Path_Env_WinDir.AddBackslash();;
}

HRESULT CFilePathTruncater::TruncateFilePath(LPCWSTR lpszFilePath, CString& strTruncated)
{
    CWinPath PathTruncate = lpszFilePath;

    if (PathTruncate.m_strPath.IsEmpty())
        ;
    else if (ContractFilePath(PathTruncate.m_strPath, m_Path_Env_Temp.m_strPath, L"%temp%\\"))
        ;
    else if (ContractFilePath(PathTruncate.m_strPath, m_Path_Env_Tmp.m_strPath, L"%tmp%\\"))
        ;
    else if (ContractFilePath(PathTruncate.m_strPath, m_Path_Env_ProgramFiles.m_strPath, L"%programfiles%\\"))
        ;
    else if (ContractFilePath(PathTruncate.m_strPath, m_Path_Env_UserProfile.m_strPath, L"%userprofile%\\"))
        ;
    else if (ContractFilePath(PathTruncate.m_strPath, m_Path_Env_WinDir.m_strPath, L"%windir%\\"))
        ;
    else
        PathTruncate.RemoveUnicodePrefix();

    strTruncated = PathTruncate;
    if (strTruncated.GetLength() > MAX_PATH)
        strTruncated = CString(L"*") + strTruncated.Right(MAX_PATH - 1);

    return S_OK;
}

BOOL CFilePathTruncater::ContractFilePath(CString& strPath, const CString& strComparePath, LPCWSTR lpszContractAs)
{
    if (strPath.GetLength() <= strComparePath.GetLength())
        return FALSE;

    if (0 != StrCmpNW(strPath, strComparePath, strComparePath.GetLength()))
        return FALSE;
    
    strPath = CString(lpszContractAs) + strPath.Mid(strComparePath.GetLength());
    return TRUE;
}