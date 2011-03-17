/**
* @file    winregkey.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-04-25  15:07
*/

#include "stdafx.h"
#include "winregkey.h"

#include <assert.h>
#include <algorithm>
#include "winpath.h"

using namespace WinMod;

LONG CWinRegKey::ExQueryBinaryValue(LPCWSTR lpszValueName, CAtlArray<BYTE>& buf)
{
    assert(m_hKey);
    if (!m_hKey)
        return ERROR_INVALID_HANDLE;



    ULONG uCbValueLen = 0;
    LONG lRet = CRegKey::QueryBinaryValue(lpszValueName, NULL, &uCbValueLen);
    if (ERROR_SUCCESS != lRet)
        return lRet;


    buf.SetCount( uCbValueLen);
    if (buf.GetCount() < uCbValueLen)
        return ERROR_OUTOFMEMORY; 


    lRet = CRegKey::QueryBinaryValue(lpszValueName, buf.GetData(), &uCbValueLen);
    if (ERROR_SUCCESS != lRet)
        return lRet;


    return ERROR_SUCCESS;
}

LONG CWinRegKey::ExQueryStringValue(LPCWSTR lpszValueName, CString& strValue, DWORD dwCchMaxLen)
{
    assert(m_hKey);
    if (!m_hKey)
        return ERROR_INVALID_HANDLE;


    dwCchMaxLen = max(MAX_PATH, dwCchMaxLen);
    dwCchMaxLen = min(LONG_MAX, dwCchMaxLen);
    if (dwCchMaxLen <= 0)
        return ERROR_MORE_DATA;


    ULONG uCchValueLen = 0;
    LONG lRet = CRegKey::QueryStringValue(lpszValueName, NULL, &uCchValueLen);
    if (ERROR_SUCCESS != lRet)
        return lRet;


    uCchValueLen = max(uCchValueLen, MAX_PATH);
    if (uCchValueLen > dwCchMaxLen)
        return ERROR_MORE_DATA;


    lRet = CRegKey::QueryStringValue(lpszValueName, strValue.GetBuffer(uCchValueLen + 1), &uCchValueLen);
    if (ERROR_SUCCESS != lRet)
    {
        strValue.ReleaseBuffer(0);
        return lRet;
    }
    strValue.ReleaseBuffer(-1);


    return ERROR_SUCCESS;
}

LONG CWinRegKey::ExQueryMultiStringValue(LPCWSTR lpszValueName, CAtlList<CString>& valueList)
{
    assert(m_hKey);
    if (!m_hKey)
        return ERROR_INVALID_HANDLE;



    ULONG uCchValueLen = 0;
    LONG lRet = CRegKey::QueryMultiStringValue(lpszValueName, NULL, &uCchValueLen);
    if (ERROR_SUCCESS != lRet)
        return lRet;


    CAtlArray<WCHAR> buf;
    buf.SetCount( uCchValueLen);
    if (buf.GetCount() < uCchValueLen)
        return ERROR_OUTOFMEMORY;


    lRet = CRegKey::QueryMultiStringValue(lpszValueName, buf.GetData(), &uCchValueLen);
    if (ERROR_SUCCESS != lRet)
    {
        return lRet;
    }
    

    LPCWSTR lpEnd  = buf.GetData() + buf.GetCount();
    LPCWSTR lpNext = buf.GetData();
    while (lpNext && lpNext < lpEnd && *lpNext)
    {
        valueList.AddTail(lpNext);

        lpNext = std::find(lpNext, lpEnd, L'\0');
        if (lpNext == lpEnd)
            break;

        ++lpNext;
    }


    return ERROR_SUCCESS;
}

LONG CWinRegKey::ExQueryExpendedStringValue(LPCWSTR lpszValueName, CString& strValue, DWORD dwCchMaxLen)
{
    LONG lRet = ExQueryStringValue(lpszValueName, strValue, dwCchMaxLen);
    if (ERROR_SUCCESS != lRet)
        return lRet;

    CWinPathApi::ExpandAsAccessiblePath(strValue);
    CWinPathApi::ExpandEnvironmentStrings(strValue);

    return ERROR_SUCCESS;
}