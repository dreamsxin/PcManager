/**
* @file    winfilefinddepthfirst.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-03-06  18:06
*/

#include "stdafx.h"
#include "winfilefinddepthfirst.h"

#include <assert.h>
#include <winioctl.h>
#include <atlfile.h>
#include "winpath.h"
#include "wintokenstack.h"

using namespace WinMod;

typedef struct _WINMOD_REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT  ReparseDataLength;
    USHORT  Reserved;
    union {
        struct {
            USHORT  SubstituteNameOffset;
            USHORT  SubstituteNameLength;
            USHORT  PrintNameOffset;
            USHORT  PrintNameLength;
            ULONG  Flags;
            WCHAR  PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT  SubstituteNameOffset;
            USHORT  SubstituteNameLength;
            USHORT  PrintNameOffset;
            USHORT  PrintNameLength;
            WCHAR  PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    };
} WINMOD_REPARSE_DATA_BUFFER, *PWINMOD_REPARSE_DATA_BUFFER;

#ifndef REPARSE_DATA_BUFFER_HEADER_SIZE
#define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(WINMOD_REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif

#ifndef MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE  ( 16 * 1024 )
#endif

CWinFileFindDepthFirst::CWinFileFindDepthFirst()
    : m_hContext(INVALID_HANDLE_VALUE)
    , m_piFilter(NULL)
    , m_bNeedSkipDirFiles(FALSE)
{

}

CWinFileFindDepthFirst::~CWinFileFindDepthFirst()
{
    Close();
}

BOOL CWinFileFindDepthFirst::FindFirstFile(
    LPCWSTR pszDirectory,
    LPCWSTR pszFileSpec)
{
    Close();

    assert(pszDirectory);
    if (pszDirectory == NULL)
    {
        ::SetLastError(ERROR_BAD_ARGUMENTS);
        return FALSE;
    }



    // get whole path
    m_pathParent.m_strPath = pszDirectory;
    m_pathParent.ExpandFullPathName();
    m_pathParent.ExpandLongPathName();
    m_pathParent.AddUnicodePrefix();
    m_pathParent.AddBackslash();



    if (!pszFileSpec || !*pszFileSpec)
        pszFileSpec = L"*";


    if (!m_pathParent.IsDirectory())
    {
        Close();
        return FALSE;
    }


    if (m_piFilter)
    {   // skip dir tree if need
        if (m_piFilter->NeedSkipDirTree(m_pathParent.m_strPath, NULL))
            return FALSE;
    }


    // begin search
    m_hContext = CWinFileFindApi::FindFirstFileSkipDots(m_pathParent.m_strPath + pszFileSpec, *this);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        Close();
        return FALSE;
    }


    if (m_piFilter)
    {   // check if need skip files
        m_bNeedSkipDirFiles = m_piFilter->NeedSkipDirFiles(m_pathParent.m_strPath);


        if (IsDirectory())
        {   // skip dir if need
            CString strFullPath = GetFullPath();
            if (m_piFilter->NeedSkipDirTree(strFullPath, GetFindDataBuffer()))
                return this->FindNextFileSkipCurrentTree();
        }
        else if (m_bNeedSkipDirFiles)
        {   // skip file if need
            return this->FindNextFile(); 
        }
    }


    return TRUE;
}

BOOL CWinFileFindDepthFirst::FindNextFile()
{
    assert(INVALID_HANDLE_VALUE != m_hContext);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    BOOL bFound = this->FindFirstChildFile();
    if (bFound)
        return TRUE;

    bFound = this->FindNextSiblingFile();
    while (!bFound)
    {
        if (!PopNode())
            return FALSE;

        bFound = this->FindNextSiblingFile();
    }

    return bFound;
}

BOOL CWinFileFindDepthFirst::FindNextFileSkipCurrentTree()
{
    assert(INVALID_HANDLE_VALUE != m_hContext);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    BOOL bFound = this->FindNextSiblingFile();
    if (!bFound)
    {
        if (!PopNode())
            return FALSE;

        bFound = this->FindNextSiblingFile();
    }

    return bFound;
}

BOOL CWinFileFindDepthFirst::FindNextSiblingFile()
{
    assert(INVALID_HANDLE_VALUE != m_hContext);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }


    BOOL bFound = CWinFileFindApi::FindNextFileSkipDots(m_hContext, *this);
    if (!m_piFilter)
        return bFound;


    // ¼ì²é¹ýÂË
    for (NULL; bFound; bFound = CWinFileFindApi::FindNextFileSkipDots(m_hContext, *this))
    {
        if (IsDirectory())
        {   // skip dir if need
            CString strFullPath = this->GetFullPath();
            if (m_piFilter->NeedSkipDirTree(strFullPath, GetFindDataBuffer()))
                continue;
        }
        else if (m_bNeedSkipDirFiles)
        {   // skip file if need
            continue;
        }


        return TRUE;
    }


    return FALSE;
}

BOOL CWinFileFindDepthFirst::FindFirstChildFile(LPCWSTR pszFileSpec)
{
    assert(INVALID_HANDLE_VALUE != m_hContext);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }


    if (!CWinFileFindData::IsDirectory())
        return FALSE;


    if (CWinFileFindData::IsDots())
        return FALSE;


    PushNode();
    m_bNeedSkipDirFiles = FALSE;
    m_hContext          = INVALID_HANDLE_VALUE;


    m_pathParent.RemoveBackslash();


    if (!pszFileSpec || !*pszFileSpec)
        pszFileSpec = L"*";


    m_hContext = CWinFileFindApi::FindFirstFileSkipDots(m_pathParent.m_strPath + L"\\" + pszFileSpec, *this);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        CloseTop();
        PopNode();
        return FALSE;
    }



    if (m_piFilter)
    {   // check if need skip files
        m_bNeedSkipDirFiles = m_piFilter->NeedSkipDirFiles(m_pathParent.m_strPath);


        if (IsDirectory())
        {   // skip dir if need
            CString strFullPath = this->GetFullPath();
            if (m_piFilter->NeedSkipDirTree(strFullPath, GetFindDataBuffer()))
                return this->FindNextSiblingFile();
        }
        else if (m_bNeedSkipDirFiles)
        {   // skip file if need
            return this->FindNextSiblingFile();
        }
    }



    return TRUE;
}

void CWinFileFindDepthFirst::Close()
{
    m_pathParent.m_strPath.Empty();

    while (PopNode())
    {
        NULL;
    }

    CloseTop();
}





CString CWinFileFindDepthFirst::GetFullPath()
{
    CWinPath pathFull = m_pathParent.m_strPath;
    pathFull.Append(CWinFileFindData::GetFileName());
    pathFull.m_strPath.MakeLower();
    return pathFull.m_strPath;
}

CString CWinFileFindDepthFirst::GetParentDirectory()
{
    return m_pathParent.m_strPath;
}





void CWinFileFindDepthFirst::CloseTop()
{
    ResetFindData();

    if (INVALID_HANDLE_VALUE != m_hContext)
    {
        CWinFileFindApi::FindClose(m_hContext);
        m_hContext          = INVALID_HANDLE_VALUE;
        m_bNeedSkipDirFiles = FALSE;
    }
}

void CWinFileFindDepthFirst::PushNode()
{
    assert(INVALID_HANDLE_VALUE != m_hContext);

    m_pathParent.AddBackslash();
    m_pathParent.m_strPath.Append(CWinFileFindData::GetFileName());

    POSITION pos = m_findStack.AddTail();
    CWinFileFindNode& node = m_findStack.GetAt(pos);

    node.m_bNeedSkipDirFiles    = m_bNeedSkipDirFiles;
    node.m_hContext             = m_hContext;
    node.m_findData             = m_findData;
}

BOOL CWinFileFindDepthFirst::PopNode()
{
    if (m_findStack.IsEmpty())
        return FALSE;

    CloseTop();

    CWinFileFindNode& node = m_findStack.GetTail();
    
    m_bNeedSkipDirFiles = node.m_bNeedSkipDirFiles;
    m_hContext          = node.m_hContext;
    m_findData          = node.m_findData;

    m_findStack.RemoveTailNoReturn();

    m_pathParent.RemoveFileSpec();
    return TRUE;
}



BOOL CWinFileFindRecursiveJuctionGuardFilter::NeedSkipDirFiles(LPCWSTR lpszDirectory)
{
    return FALSE;
}

BOOL CWinFileFindRecursiveJuctionGuardFilter::NeedSkipDirTree(LPCWSTR lpszDirectory, WIN32_FIND_DATAW* pFindData)
{
    if (!lpszDirectory || !pFindData)
        return FALSE;

    if (!(pFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        return FALSE;

    if (!(pFindData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
        return FALSE;

    CWinTokenHelper TokenHelper;
    TokenHelper.EnablePrivilege(SE_BACKUP_NAME);

    CAtlFile hDirectory;
    HRESULT hr = hDirectory.Create(
        lpszDirectory, 
        GENERIC_READ,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        OPEN_EXISTING, 
        FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS);
    if (FAILED(hr))
        return FALSE;


    DWORD dwBytes = 0;
    BYTE byBuff[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
    memset(&byBuff, 0, sizeof(byBuff));
    BOOL br = ::DeviceIoControl(
        hDirectory, 
        FSCTL_GET_REPARSE_POINT, 
        NULL,
        0, 
        (LPVOID)byBuff, 
        MAXIMUM_REPARSE_DATA_BUFFER_SIZE, 
        &dwBytes,
        NULL);
    if (!br)
        return FALSE;


    LPWSTR lpszSubstituteName = NULL;
    LPWSTR lpszSubstituteEnd  = NULL;
    PWINMOD_REPARSE_DATA_BUFFER pReparseData = (PWINMOD_REPARSE_DATA_BUFFER)byBuff;
    if (IO_REPARSE_TAG_MOUNT_POINT == pReparseData->ReparseTag)
    {
        lpszSubstituteName = pReparseData->MountPointReparseBuffer.PathBuffer + pReparseData->MountPointReparseBuffer.SubstituteNameOffset / 2;
        lpszSubstituteEnd  = lpszSubstituteName + pReparseData->MountPointReparseBuffer.SubstituteNameLength / 2;
    }
    else if (IO_REPARSE_TAG_SYMLINK == pReparseData->ReparseTag)
    {
        lpszSubstituteName = pReparseData->SymbolicLinkReparseBuffer.PathBuffer + pReparseData->SymbolicLinkReparseBuffer.SubstituteNameOffset / 2;
        lpszSubstituteEnd  = lpszSubstituteName + pReparseData->SymbolicLinkReparseBuffer.SubstituteNameLength / 2;
    }


    if ((BYTE*)lpszSubstituteEnd >= byBuff + MAXIMUM_REPARSE_DATA_BUFFER_SIZE)
        return FALSE;


    *lpszSubstituteEnd = L'\0';
    CWinPath PathLinkDir;
    if (CWinPathApi::IsRelative(lpszSubstituteName))
    {
        PathLinkDir = lpszDirectory;
        PathLinkDir.RemoveFileSpec();
        PathLinkDir.Append(lpszSubstituteName);
        PathLinkDir.ExpandFullPathName();
        PathLinkDir.ExpandNormalizedPathName();
    }
    else
    {
        PathLinkDir = lpszSubstituteName;
        PathLinkDir.ExpandNormalizedPathName();
    }


    if (!m_ReparsePointSet.Lookup(PathLinkDir.m_strPath))
    {
        m_ReparsePointSet.SetAt(PathLinkDir.m_strPath, 0);


        if (!StrStrW(lpszDirectory, PathLinkDir.m_strPath))
            return FALSE;
    }


    return TRUE;
}