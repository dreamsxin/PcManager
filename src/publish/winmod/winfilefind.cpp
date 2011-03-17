/**
* @file    winfilefind.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-03-06  11:17
*/

#include "stdafx.h"
#include "winfilefind.h"

#include <assert.h>

using namespace WinMod;

CWinFileFind::CWinFileFind()
    : m_hContext(INVALID_HANDLE_VALUE)
{
}

CWinFileFind::~CWinFileFind()
{
    Close();
}

BOOL CWinFileFind::FindFirstFile(LPCTSTR pszName)
{
    Close();

    assert(pszName);
    if (pszName == NULL)
    {
        ::SetLastError(ERROR_BAD_ARGUMENTS);
        return FALSE;
    }

    m_hContext = CWinFileFindApi::FindFirstFileSkipDots(pszName, *this);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        Close();
        return FALSE;
    }

    return TRUE;
}

BOOL CWinFileFind::FindNextFile()
{
    assert(INVALID_HANDLE_VALUE != m_hContext);
    if (INVALID_HANDLE_VALUE == m_hContext)
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    return CWinFileFindApi::FindNextFileSkipDots(m_hContext, *this);
}

void CWinFileFind::Close()
{
    ResetFindData();

    if (INVALID_HANDLE_VALUE != m_hContext)
    {
        CWinFileFindApi::FindClose(m_hContext);
        m_hContext = INVALID_HANDLE_VALUE;
    }
}

HANDLE CWinFileFind::GetHandle()
{
    return m_hContext;
}