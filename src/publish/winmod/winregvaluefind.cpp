/**
* @file    winregfind.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-04-23  11:13
*/

#include "stdafx.h"
#include "winregvaluefind.h"

#include <assert.h>

using namespace WinMod;

BOOL CWinRegValueFind::FindFirstValueName(HKEY hKeyParent, LPCWSTR lpszKeyName, REGSAM samDesired)
{
    Reset();


    samDesired |= KEY_QUERY_VALUE;
    LONG lRet = m_hKeyEnum.Open(hKeyParent, lpszKeyName, samDesired);
    if (ERROR_SUCCESS != lRet)
        return FALSE;


    return DoEnumRegValue();
}

BOOL CWinRegValueFind::FindNextValueName()
{
    if (IsEndOfFind())
        return FALSE;


    ++m_dwEnumIndex;


    return DoEnumRegValue();
}

BOOL CWinRegValueFind::DoEnumRegValue()
{
    assert(!IsEndOfFind());
    if (IsEndOfFind())
        return FALSE;

    m_lpszeValueName = m_szValueNameBuffer;
    m_szValueNameBuffer[0] = L'\0';
    DWORD dwValueNameLength = _countof(m_szValueNameBuffer) - 1;
    LONG lRet = RegEnumValue(
        m_hKeyEnum,
        m_dwEnumIndex,
        m_szValueNameBuffer,
        &dwValueNameLength,
        NULL, &m_dwType, NULL, NULL);
    if (ERROR_MORE_DATA == lRet)
    {
        m_lpszeValueName = m_strLongValueNameBuffer.GetBuffer(WIN_REG_VALUE_NAME_MAX_BUF_SIZE);
        dwValueNameLength = WIN_REG_VALUE_NAME_MAX_BUF_SIZE - 1;
        lRet = RegEnumValue(
            m_hKeyEnum,
            m_dwEnumIndex,
            m_lpszeValueName,
            &dwValueNameLength,
            NULL, NULL, NULL, NULL);
        if (ERROR_SUCCESS != lRet)
        {
            m_strLongValueNameBuffer.ReleaseBuffer(0);
            Reset();
            return FALSE;
        }
        m_strLongValueNameBuffer.ReleaseBuffer(dwValueNameLength);
    }
    else if (ERROR_SUCCESS != lRet)
    {
        Reset();
        return FALSE;
    }


    m_szValueNameBuffer[_countof(m_szValueNameBuffer) - 1] = L'\0';
    return TRUE;
}

HRESULT CWinRegValueFind::GetStringValue(CString& strValue, DWORD dwCchMaxLen)
{
    assert(!IsEndOfFind());
    if (IsEndOfFind())
        return E_HANDLE;


    LONG lRet = m_hKeyEnum.ExQueryStringValue(GetValueName(), strValue, dwCchMaxLen);
    if (ERROR_SUCCESS != lRet)
        return AtlHresultFromWin32(lRet);


    return S_OK;
}
