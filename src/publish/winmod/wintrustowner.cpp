/**
* @file    wintrustowner.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-04-09  11:56
*/

#include "stdafx.h"
#include "wintrustowner.h"

#include <atlsecurity.h>

using namespace WinMod;

BOOL CWinTrustOwner::IsTrustedOwner(HANDLE hFile)
{
    CSid sidOwner;

    BOOL bRet = ::AtlGetOwnerSid(hFile, SE_FILE_OBJECT, &sidOwner);
    if (!bRet)
        return FALSE;

    LPCWSTR lpszAccount = sidOwner.AccountName();
    if (!lpszAccount)
        return FALSE;

    if (0 != wcscmp(lpszAccount, L"TrustedInstaller"))
        return FALSE;

    return TRUE;
}