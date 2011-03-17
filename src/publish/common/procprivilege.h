#pragma once
#include <atlbase.h>
#include <atlcoll.h>
#include <atlstr.h>

class CProcPrivilege
{
public:
    CProcPrivilege()
    {
    }

    HRESULT SetPri(BOOL bSet, LPCWSTR lpwszPri)
    {
        BOOL bRet  = FALSE;
        HRESULT hr = S_OK;
        HANDLE hTokenProc = NULL;

        bRet = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hTokenProc);
        if (!bRet)
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        hr = _SetPrivilege(hTokenProc, lpwszPri, bSet);

Exit0:

        if (hTokenProc)
        {
            ::CloseHandle(hTokenProc);
            hTokenProc = NULL;
        }

        return hr;
    }

    HRESULT SetPriSpec( HANDLE hToken, LPCWSTR lpwszPri, BOOL bSet )
    {
        return _SetPrivilege( hToken, lpwszPri, bSet );
    }

    HRESULT EnableShutdown()
    {
        return SetPri( TRUE, SE_SHUTDOWN_NAME );
    }

private:

    HRESULT _SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
    {
        BOOL bRet  = FALSE;
        HRESULT hr = S_OK;

        TOKEN_PRIVILEGES tp;
        LUID luid;

        bRet = ::LookupPrivilegeValue(NULL, lpszPrivilege, &luid);
        if (!bRet)
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        tp.PrivilegeCount       = 1;
        tp.Privileges[0].Luid   = luid;
        if (bEnablePrivilege)
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        else
            tp.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;

        bRet = ::AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            (PTOKEN_PRIVILEGES)NULL,
            (PDWORD)NULL
            );
        if (!bRet)
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }
Exit0:
        return hr;
    }
};