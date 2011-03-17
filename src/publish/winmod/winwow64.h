/**
* @file    winwow64.h
* @brief   ...
* @author  bbcallen
* @date    2010-06-09 21:17
*/

#ifndef WINWOW64_H
#define WINWOW64_H

#include "winmodbase.h"
#include "winpath.h"
#include "winosver.h"
#include "kernel32mod.h"

NS_WINMOD_BEGIN

class CWinWow64
{
public:
    CWinWow64();

    BOOL    IsWow64() const {return m_bIsWow64;}

    HRESULT Wow64FsRedirection(LPCWSTR lpszWow64Path, CString& strRealPath, BOOL bNormalized = FALSE) const;
    HRESULT Wow64FsReverseRedirection(LPCWSTR lpszRealPath, CString& strWow64Path, BOOL bNormalized = FALSE) const;

    BOOL    Wow64IsX64Path(LPCWSTR lpszRealPath, BOOL bNormalized = FALSE) const;

    BOOL    IsExemptSubDir(LPCWSTR lpszSubDir) const;

    const CWinPath& GetPathSystem32()           {return m_PathSystem32;}
    const CWinPath& GetPathProgramFiles()       {return m_PathProgramFiles;}
    const CWinPath& GetPathSysWow64()           {return m_PathSysWow64;}
    const CWinPath& GetPathProgramFilesX86()    {return m_PathProgramFilesX86;}

protected:

    BOOL m_bIsWow64;
    BOOL m_bRedirectionIsValid;

    // real path
    CWinPath m_PathSystem32;
    CWinPath m_PathProgramFiles;

    // wow64 path
    CWinPath m_PathSysWow64;
    CWinPath m_PathProgramFilesX86;
};



inline CWinWow64::CWinWow64(): m_bIsWow64(FALSE), m_bRedirectionIsValid(FALSE)
{
    if (!CWinModule_kernel32::IsWow64Process(::GetCurrentProcess(), &m_bIsWow64))
    {
        m_bIsWow64 = FALSE;
        return;
    }



    m_PathSystem32 = L"%windir%\\System32";        // c:\windows\system32
    m_PathSystem32.ExpandEnvironmentStrings();
    m_PathSystem32.ExpandNormalizedPathName();
    m_PathSystem32.AddBackslash();

    m_PathProgramFiles = L"%ProgramW6432%";            // c:\program files
    m_PathProgramFiles.ExpandEnvironmentStrings();
    m_PathProgramFiles.ExpandNormalizedPathName();
    m_PathProgramFiles.AddBackslash();

    m_PathSysWow64 = L"%windir%\\SysWow64";        // c:\windows\syswow64
    m_PathSysWow64.ExpandEnvironmentStrings();
    m_PathSysWow64.ExpandNormalizedPathName();
    m_PathSysWow64.AddBackslash();

    m_PathProgramFilesX86 = L"%ProgramFiles(x86)%";       // c:\program files (x86)
    m_PathProgramFilesX86.ExpandEnvironmentStrings();
    m_PathProgramFilesX86.ExpandNormalizedPathName();
    m_PathProgramFilesX86.AddBackslash();




    if (!m_PathSystem32.m_strPath.IsEmpty() &&
        !m_PathProgramFiles.m_strPath.IsEmpty() &&
        !m_PathSysWow64.m_strPath.IsEmpty() &&
        !m_PathProgramFilesX86.m_strPath.IsEmpty())
    {
        m_bRedirectionIsValid = TRUE;
        return;
    }
}

inline HRESULT CWinWow64::Wow64FsRedirection(LPCWSTR lpszWow64Path, CString& strRealPath, BOOL bNormalized) const
{
    if (!m_bIsWow64 || !m_bRedirectionIsValid)
    {
        strRealPath.Empty();
        return S_FALSE;
    }


    if (!lpszWow64Path || !*lpszWow64Path)
    {
        strRealPath.Empty();
        return S_FALSE;
    }


    CWinPath PathWow64 = lpszWow64Path;
    if (!bNormalized)
        PathWow64.ExpandNormalizedPathName();


    if (0 == StrCmpNW(PathWow64.m_strPath, m_PathSystem32.m_strPath, m_PathSystem32.m_strPath.GetLength()))
    {
        LPCWSTR lpszSubPath = (LPCWSTR)PathWow64.m_strPath + m_PathSystem32.m_strPath.GetLength();
        if (!IsExemptSubDir(lpszSubPath))
        {
            strRealPath = m_PathSysWow64.m_strPath;
            strRealPath.Append(lpszSubPath);
            return S_OK;
        }
    }


    if (0 == StrCmpNW(PathWow64.m_strPath, m_PathProgramFiles.m_strPath, m_PathProgramFiles.m_strPath.GetLength()))
    {
        LPCWSTR lpszSubPath = (LPCWSTR)PathWow64.m_strPath + m_PathProgramFiles.m_strPath.GetLength();
        strRealPath = m_PathProgramFilesX86.m_strPath;
        strRealPath.Append(lpszSubPath);
        return S_OK;
    }


    strRealPath = PathWow64;
    return S_FALSE;
}

inline HRESULT CWinWow64::Wow64FsReverseRedirection(LPCWSTR lpszRealPath, CString& strWow64Path, BOOL bNormalized) const
{
    if (!lpszRealPath || !*lpszRealPath)
    {
        strWow64Path.Empty();
        return S_FALSE;
    }


    CWinPath PathReal = lpszRealPath;
    if (!bNormalized)
        PathReal.ExpandNormalizedPathName();


    if (0 == StrCmpNW(PathReal.m_strPath, m_PathSysWow64.m_strPath, m_PathSysWow64.m_strPath.GetLength()))
    {
        LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathSysWow64.m_strPath.GetLength();
        if (!IsExemptSubDir(lpszSubPath))
        {
            strWow64Path = m_PathSystem32.m_strPath;
            strWow64Path.Append(lpszSubPath);
            return S_OK;
        }
    }


    if (0 == StrCmpNW(PathReal.m_strPath, m_PathProgramFilesX86.m_strPath, m_PathProgramFilesX86.m_strPath.GetLength()))
    {
        LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathProgramFilesX86.m_strPath.GetLength();
        strWow64Path = m_PathProgramFiles.m_strPath;
        strWow64Path.Append(lpszSubPath);
        return S_OK;
    }


    strWow64Path = PathReal;
    return S_FALSE;
}

inline BOOL CWinWow64::Wow64IsX64Path(LPCWSTR lpszRealPath, BOOL bNormalized) const
{
    if (!lpszRealPath || !*lpszRealPath)
    {
        return FALSE;
    }


    CWinPath PathReal = lpszRealPath;
    if (!bNormalized)
        PathReal.ExpandNormalizedPathName();


    if (0 == StrCmpNW(PathReal.m_strPath, m_PathSysWow64.m_strPath, m_PathSysWow64.m_strPath.GetLength()))
    {
        LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathSysWow64.m_strPath.GetLength();
        if (!IsExemptSubDir(lpszSubPath))
        {
            return TRUE;
        }
    }


    if (0 == StrCmpNW(PathReal.m_strPath, m_PathProgramFilesX86.m_strPath, m_PathProgramFilesX86.m_strPath.GetLength()))
    {
        return TRUE;
    }


    return FALSE;
}

inline BOOL CWinWow64::IsExemptSubDir(LPCWSTR lpszSubDir) const
{
    if (!lpszSubDir || !*lpszSubDir)
        return FALSE;





    if (0 == StrCmpNW(lpszSubDir, L"catroot\\", (int)wcslen(L"catroot\\")))
        return TRUE;

    if (0 == StrCmpNW(lpszSubDir, L"catroot2\\", (int)wcslen(L"catroot2\\")))
        return TRUE;

    if (0 == StrCmpNW(lpszSubDir, L"drivers\\etc\\", (int)wcslen(L"drivers\\etc\\")))
        return TRUE;

    if (0 == StrCmpNW(lpszSubDir, L"logfiles\\", (int)wcslen(L"logfiles\\")))
        return TRUE;

    if (0 == StrCmpNW(lpszSubDir, L"spool\\", (int)wcslen(L"spool\\")))
        return TRUE;





    if (!CWinOSVer::IsWin7OrLater())
        return FALSE;

    if (0 == StrCmpNW(lpszSubDir, L"driversstore\\", (int)wcslen(L"driversstore\\")))
        return TRUE;

    return FALSE;
}












class CWinWow64FsRedirectionGuard
{
public:
    CWinWow64FsRedirectionGuard();
    ~CWinWow64FsRedirectionGuard();

    HRESULT Enable();
    HRESULT Disable();
    HRESULT Revert();

protected:
    BOOL  m_bChanged;
    PVOID m_OldValue;
};

inline CWinWow64FsRedirectionGuard::CWinWow64FsRedirectionGuard(): m_bChanged(FALSE), m_OldValue(NULL)
{
    
}

inline CWinWow64FsRedirectionGuard::~CWinWow64FsRedirectionGuard()
{
    Revert();
}

inline HRESULT CWinWow64FsRedirectionGuard::Enable()
{
    if (!m_bChanged)
    {
        HRESULT hr = Disable();
        if (FAILED(hr))
            return hr;
    }


    BOOL br = CWinModule_kernel32::Wow64EnableWow64FsRedirection(TRUE);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    m_bChanged = TRUE;
    return S_OK;
}

inline HRESULT CWinWow64FsRedirectionGuard::Disable()
{
    BOOL br = FALSE;
    if (m_bChanged)
    {
        br = CWinModule_kernel32::Wow64EnableWow64FsRedirection(FALSE);
    }
    else
    {
        br = CWinModule_kernel32::Wow64DisableWow64FsRedirection(&m_OldValue);
    }


    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    m_bChanged = TRUE;
    return S_OK;
}

inline HRESULT CWinWow64FsRedirectionGuard::Revert()
{
    if (!m_bChanged)
        return S_FALSE;
    

    BOOL br = CWinModule_kernel32::Wow64RevertWow64FsRedirection(m_OldValue);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    m_OldValue = NULL;
    m_bChanged = FALSE;
    return S_OK;
}











NS_WINMOD_END

#endif//WINWOW64_H