#include "stdafx.h"

HRESULT CheckCondition(LPCTSTR lpszCondition, int nSize)
{
    BOOL bValue = TRUE, bAnd = TRUE;
    CString strCondition;
    int nPosCurrent = 0;
    int nPosFuncNameStart = 0;
    int nPosFuncNameEnd = 0;
    int nPosFuncParamEnd = 0;

    strCondition.SetString(lpszCondition, nSize);

    do
    {
        CString strFunctionName, strParams;
        int nPosSingleParamStart = 0;
        BOOL bNot = FALSE;
        BOOL bFuncRet = FALSE;

        while (lpszCondition[nPosCurrent] == ' ' || lpszCondition[nPosCurrent] == '\r' || lpszCondition[nPosCurrent] == '\n')
            nPosCurrent ++;

        nPosFuncNameStart = nPosCurrent;
        nPosFuncNameEnd = strCondition.Find(L'(', nPosFuncNameStart);
        if (-1 == nPosFuncNameEnd || nPosFuncNameStart == nPosFuncNameEnd)
            return E_FAIL;

        nPosFuncParamEnd = strCondition.Find(L')', nPosFuncNameEnd + 1);
        if (-1 == nPosFuncParamEnd)
            return E_FAIL;

        strFunctionName = strCondition.Mid(nPosFuncNameStart, nPosFuncNameEnd - nPosFuncNameStart).Trim();
        strParams = strCondition.Mid(nPosFuncNameEnd + 1, nPosFuncParamEnd - nPosFuncNameEnd - 1).Trim();

        if (L'!' == strFunctionName[0])
        {
            bNot = TRUE;
            strFunctionName = strFunctionName.Mid(1);
        }

        if (0 == strFunctionName.CompareNoCase(L"exist_regv"))
        {
            CString strRootKey, strKey, strValue;
            HKEY hKeyRoot = NULL;

            strRootKey = strParams.Tokenize(L",", nPosSingleParamStart).Trim();
            if (-1 == nPosSingleParamStart)
                return E_FAIL;
            strKey = strParams.Tokenize(L",", nPosSingleParamStart).Trim();
            if (-1 == nPosSingleParamStart)
                return E_FAIL;
            strValue = strParams.Tokenize(L",", nPosSingleParamStart).Trim();
            if (-1 == nPosSingleParamStart)
                return E_FAIL;

            if (0 == strRootKey.CompareNoCase(L"HKLM"))
                hKeyRoot = HKEY_LOCAL_MACHINE;
            else if (0 == strRootKey.CompareNoCase(L"HKCU"))
                hKeyRoot = HKEY_CURRENT_USER;
            else if (0 == strRootKey.CompareNoCase(L"HKCR"))
                hKeyRoot = HKEY_CLASSES_ROOT;
            else
                return E_FAIL;

            CRegKey reg;

            reg.Open(hKeyRoot, strKey, KEY_READ);
            if (NULL == reg.m_hKey)
            {
                bFuncRet = FALSE;
            }
            else
            {
                DWORD dwType = 0;
                LONG lRet = reg.QueryValue(strValue, NULL, NULL, NULL);
                bFuncRet = (ERROR_SUCCESS == lRet);
            }
        }
        else if (0 == strFunctionName.CompareNoCase(L"exist_regkey"))
        {
            CString strRootKey, strKey;
            HKEY hKeyRoot = NULL;

            strRootKey = strParams.Tokenize(L",", nPosSingleParamStart).Trim();
            if (-1 == nPosSingleParamStart)
                return E_FAIL;
            strKey = strParams.Tokenize(L",", nPosSingleParamStart).Trim();
            if (-1 == nPosSingleParamStart)
                return E_FAIL;

            if (0 == strRootKey.CompareNoCase(L"HKLM"))
                hKeyRoot = HKEY_LOCAL_MACHINE;
            else if (0 == strRootKey.CompareNoCase(L"HKCU"))
                hKeyRoot = HKEY_CURRENT_USER;
            else if (0 == strRootKey.CompareNoCase(L"HKCR"))
                hKeyRoot = HKEY_CLASSES_ROOT;
            else
                return E_FAIL;

            CRegKey reg;

            reg.Open(hKeyRoot, strKey, KEY_READ);
            bFuncRet = (NULL != reg.m_hKey);
        }
        else if (0 == strFunctionName.CompareNoCase(L"exist_file"))
        {
            CString strPath;

            DWORD dwRet = ::ExpandEnvironmentStrings(strParams.Trim(), strPath.GetBuffer(MAX_PATH + 10), MAX_PATH);
            if (0 == dwRet)
            {
                strPath.ReleaseBuffer(0);
                return E_FAIL;
            }

            strPath.ReleaseBuffer();

            ::SetLastError(ERROR_SUCCESS);
            DWORD dwAttrib = ::GetFileAttributes(strPath);
            bFuncRet = (ERROR_SUCCESS == ::GetLastError());
        }
        else
        {
            return E_FAIL;
        }

        if (bNot)
            bFuncRet = !bFuncRet;

        if (bAnd)
            bValue &= bFuncRet;
        else
            bValue |= bFuncRet;

        nPosCurrent = nPosFuncParamEnd + 1;

        while (lpszCondition[nPosCurrent] == ' ' || lpszCondition[nPosCurrent] == '\r' || lpszCondition[nPosCurrent] == '\n')
            nPosCurrent ++;

        switch (strCondition[nPosCurrent])
        {
        case L'\0':
            return bValue ? S_OK : S_FALSE;
        case L'&':
            bAnd = TRUE;
            break;
        case L'|':
            bAnd = FALSE;
            break;
        default:
            return E_FAIL;
        }

        nPosCurrent ++;
    }
    while (TRUE);
}

HRESULT CheckHomePage(LPCTSTR lpszCondition, int nSize)
{
    CString strCondition = lpszCondition, strCurrentHomePage, strHomePage;
    int nCurrent = 0;

    CRegKey reg;

    reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main", KEY_READ);
    if (NULL == reg.m_hKey)
    {
        return E_FAIL;
    }
    else
    {
        DWORD dwLen = MAX_PATH;
        LONG lRet = reg.QueryStringValue(L"Start Page", strCurrentHomePage.GetBuffer(MAX_PATH + 10), &dwLen);
        if (ERROR_SUCCESS != lRet)
        {
            strCurrentHomePage.ReleaseBuffer(0);
            return E_FAIL;
        }

        strCurrentHomePage.ReleaseBuffer();
    }

    while (TRUE)
    {
        strHomePage = strCondition.Tokenize(L"\r\n", nCurrent).Trim();
        if (-1 == nCurrent)
            return S_FALSE;

        if (0 == strCurrentHomePage.CompareNoCase(strHomePage))
            return S_OK;
    }
}

void SetHomePage(LPCTSTR lpszUrl)
{
    CRegKey reg;

    reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main");
    if (NULL == reg.m_hKey)
        return;

    reg.SetStringValue(L"Start Page", lpszUrl);
}