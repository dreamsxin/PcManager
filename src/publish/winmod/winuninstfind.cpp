/**
* @file    winuninstfind.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-05-22  11:02
*/

#include "stdafx.h"
#include "winuninstfind.h"

#include <assert.h>
#include "winmodule.h"

using namespace WinMod;

#define WINMOD_REGKEY_WINDOWS_UNINSTALL         L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"

#define WINMOD_REGVALUE_UNINST_SYSTEMCOMPONENT  L"SystemComponent"  ///< Uninstall\\..\\SystemComponent
#define WINMOD_REGVALUE_UNINST_DISPLAYNAME      L"DisplayName"      ///< Uninstall\\..\\DisplayName
#define WINMOD_REGVALUE_UNINST_PUBLISHER        L"Publisher"        ///< Uninstall\\..\\Publisher
#define WINMOD_REGVALUE_UNINST_UNINSTALLSTRING  L"UninstallString"  ///< Uninstall\\..\\UninstallString

BOOL CWinUninstFind::FindFirstUninst(HKEY hRootKey, REGSAM samDesired)
{
    assert(hRootKey);
    Reset();

    samDesired |= KEY_ENUMERATE_SUB_KEYS;
    BOOL bFind = m_hRegKeyFind.FindFirstSubKey(hRootKey, WINMOD_REGKEY_WINDOWS_UNINSTALL, samDesired);
    if (!bFind)
    {
        Reset();
        return FALSE;
    }

    DoReadStandardValues();
    return TRUE;
}

BOOL CWinUninstFind::FindNextUninst()
{
    if (IsEndOfFind())
        return FALSE;

    BOOL bFind = m_hRegKeyFind.FindNextSubKey();
    if (!bFind)
    {
        Reset();
        return FALSE;
    }

    DoReadStandardValues();
    return TRUE;
}

void CWinUninstFind::DoReadStandardValues()
{
    assert(!IsEndOfFind());
    if (IsEndOfFind())
        return;


    m_dwSysmteComponent     = 0;
    m_strDisplayName        = L"";
    m_strPublisher          = L"";
    m_strUninstallString    = L"";


    CString    strKeyName = m_hRegKeyFind.GetFullRegPath();
    CWinRegKey hRegKeyUninstall;
    LONG lRet = hRegKeyUninstall.Open(m_hRegKeyFind.m_hKeyParent, strKeyName, KEY_QUERY_VALUE);
    if (ERROR_SUCCESS == lRet)
    {   // ..\\Uninstall\\..\\[DisplayName]
        LONG lRet = hRegKeyUninstall.QueryDWORDValue(WINMOD_REGVALUE_UNINST_SYSTEMCOMPONENT, m_dwSysmteComponent);
        if (ERROR_SUCCESS != lRet)
            m_dwSysmteComponent = 0;            // no return

        lRet = hRegKeyUninstall.ExQueryStringValue(WINMOD_REGVALUE_UNINST_DISPLAYNAME, m_strDisplayName);
        if (ERROR_SUCCESS != lRet)
            m_strDisplayName = L"";             // no return

        lRet = hRegKeyUninstall.ExQueryStringValue(WINMOD_REGVALUE_UNINST_PUBLISHER, m_strPublisher);
        if (ERROR_SUCCESS != lRet)
            m_strPublisher = L"";               // no return

        lRet = hRegKeyUninstall.ExQueryStringValue(WINMOD_REGVALUE_UNINST_UNINSTALLSTRING, m_strUninstallString);
        if (ERROR_SUCCESS != lRet)
            m_strUninstallString = L"";         // no return
    }
}