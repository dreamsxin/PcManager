/**
* @file    winfilefinddata.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-03-06  22:33
*/

#include "stdafx.h"
#include "winfilefinddata.h"

#include "winpath.h"

using namespace WinMod;

CWinFileFindData::CWinFileFindData()
{
    ResetFindData();
}

CWinFileFindData::~CWinFileFindData()
{
}

WIN32_FIND_DATAW* CWinFileFindData::GetFindDataBuffer()
{
    return &m_findData;
}






CString CWinFileFindData::GetFileNameBase() const
{
    CWinPath pathFileNameBase = m_findData.cFileName;

    pathFileNameBase.RemoveExtension();
    pathFileNameBase.StripPath();

    return pathFileNameBase.m_strPath;
}

CString CWinFileFindData::GetFileNameExt() const
{
    return CWinPathApi::FindExtension(m_findData.cFileName);
}






BOOL CWinFileFindData::MatchesMask(DWORD dwMask) const
{
	return (!!(m_findData.dwFileAttributes & dwMask));
}

BOOL CWinFileFindData::IsDots() const
{
	// return TRUE if the file name is "." or ".." and
	// the file is a directory
    if (!IsDirectory())
        return FALSE;
    
    return CWinPathApi::IsDots(m_findData.cFileName);
}

ULONGLONG CWinFileFindData::GetFileSize() const
{
    ULARGE_INTEGER nFileSize;

    nFileSize.LowPart  = m_findData.nFileSizeLow;
    nFileSize.HighPart = m_findData.nFileSizeHigh;

    return nFileSize.QuadPart;
}

void CWinFileFindData::ResetFindData()
{
    memset(&m_findData, 0, sizeof(WIN32_FIND_DATA));
}













HANDLE CWinFileFindApi::FindFirstFileSkipDots(LPCWSTR lpFileName, CWinFileFindData& findData)
{
    HANDLE hContext = ::FindFirstFile(lpFileName, findData.GetFindDataBuffer());
    if (INVALID_HANDLE_VALUE == hContext)
        return hContext;

    if (findData.IsDots())
    {
        BOOL bFound = CWinFileFindApi::FindNextFileSkipDots(hContext, findData);
        if (!bFound)
        {
            ::FindClose(hContext);
            return INVALID_HANDLE_VALUE;
        }
    }

    return hContext;
}

BOOL CWinFileFindApi::FindNextFileSkipDots(HANDLE hContext, CWinFileFindData& findData)
{
    BOOL bFound = ::FindNextFile(hContext, findData.GetFindDataBuffer());
    while (bFound && findData.IsDots())
    {
        bFound = ::FindNextFile(hContext, findData.GetFindDataBuffer());
    }

    if (!bFound)
        return FALSE;

    if (findData.IsDots())
    {
        ::SetLastError(ERROR_FILE_NOT_FOUND);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinFileFindApi::FindClose(HANDLE hContext)
{
    return ::FindClose(hContext);
}