/**
* @file    psapimod.h
* @brief   ...
* @author  bbcallen
* @date    2008-12-26  18:01
*/

#ifndef PSAPIMOD_H
#define PSAPIMOD_H

#include <psapi.h>
#include "winmodule.h"

NS_WINMOD_BEGIN

typedef DWORD (WINAPI *PFN_GetProcessImageFileNameW) (
    __in   HANDLE hProcess,
    __out  LPWSTR lpImageFileName,
    __in   DWORD  nSize);

class CWinModule_psapi: public CWinModule
{
public:
    CWinModule_psapi():
        m_pfnGetProcessImageFileNameW(NULL)
    {

    }

    virtual void FreeLib()
    {
        m_pfnGetProcessImageFileNameW = NULL;
        return CWinModule::FreeLib();
    }

    DWORD WINAPI GetProcessImageFileNameW(
        __in   HANDLE hProcess,
        __out  LPWSTR lpImageFileName,
        __in   DWORD  nSize)
    {
        if (NULL == m_pfnGetProcessImageFileNameW)
        {
            m_pfnGetProcessImageFileNameW =
                (PFN_GetProcessImageFileNameW) GetProcAddr("GetProcessImageFileNameW");

            if (!m_pfnGetProcessImageFileNameW)
                return 0;
        }

        return m_pfnGetProcessImageFileNameW(
            hProcess,
            lpImageFileName,
            nSize);
    }

private:
    PFN_GetProcessImageFileNameW m_pfnGetProcessImageFileNameW;
};

NS_WINMOD_END

#endif//PSAPIMOD_H