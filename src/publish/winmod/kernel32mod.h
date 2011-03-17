/**
* @file    kernel32mod.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-04 17:39
*/

#ifndef KERNEL32MOD_H
#define KERNEL32MOD_H

#include <psapi.h>

typedef void (WINAPI *PFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
typedef BOOL (WINAPI *PFN_GetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
typedef BOOL (WINAPI *PFN_IsWow64Process)(HANDLE, PBOOL);

typedef BOOL (WINAPI *PFN_Wow64DisableWow64FsRedirection)(PVOID* OldValue);
typedef BOOL (WINAPI *PFN_Wow64RevertWow64FsRedirection)(PVOID OldValue);

typedef BOOLEAN (WINAPI *PFN_Wow64EnableWow64FsRedirection)(BOOLEAN Wow64FsEnableRedirection);


NS_WINMOD_BEGIN

class CWinModule_kernel32
{
public:

    //void WINAPI GetNativeSystemInfo(__out  LPSYSTEM_INFO lpSystemInfo)
    //{
    //    if (NULL == m_pfnGetNativeSystemInfo)
    //    {
    //        m_pfnGetNativeSystemInfo =
    //            (PFN_GetNativeSystemInfo) GetProcAddr("GetNativeSystemInfo");

    //        if (!m_pfnGetNativeSystemInfo)
    //        {
    //            ::GetSystemInfo(lpSystemInfo);
    //            return;
    //        }
    //    }

    //    return m_pfnGetNativeSystemInfo(lpSystemInfo);
    //}

    //BOOL WINAPI GetProductInfo(
    //    __in   DWORD dwOSMajorVersion,
    //    __in   DWORD dwOSMinorVersion,
    //    __in   DWORD dwSpMajorVersion,
    //    __in   DWORD dwSpMinorVersion,
    //    __out  PDWORD pdwReturnedProductType
    //    )
    //{
    //    if (NULL == m_pfnGetProductInfo)
    //    {
    //        m_pfnGetProductInfo = (PFN_GetProductInfo) GetProcAddr("GetProductInfo");

    //        if (!m_pfnGetProductInfo)
    //        {
    //            return FALSE;
    //        }
    //    }

    //    return m_pfnGetProductInfo(
    //        dwOSMajorVersion,
    //        dwOSMinorVersion,
    //        dwSpMajorVersion,
    //        dwSpMinorVersion,
    //        pdwReturnedProductType);
    //}

    static BOOL WINAPI IsWow64Process(HANDLE hProcess, BOOL* pbIsWow64Process)
    {
        if (!m_pfnIsWow64Process)
        {
            m_pfnIsWow64Process = (PFN_IsWow64Process)GetProcAddr("IsWow64Process");
            if (NULL == m_pfnIsWow64Process)
                return FALSE;
        }

        return m_pfnIsWow64Process(hProcess, pbIsWow64Process);
    }

    static BOOL WINAPI Wow64DisableWow64FsRedirection(PVOID* OldValue)
    {
        if (!m_pfnWow64DisableWow64FsRedirection)
        {
            m_pfnWow64DisableWow64FsRedirection = (PFN_Wow64DisableWow64FsRedirection)GetProcAddr("Wow64DisableWow64FsRedirection");
            if (NULL == m_pfnWow64DisableWow64FsRedirection)
                return FALSE;
        }

        return m_pfnWow64DisableWow64FsRedirection(OldValue);
    }

    static BOOL WINAPI Wow64RevertWow64FsRedirection(PVOID OldValue)
    {
        if (!m_pfnWow64RevertWow64FsRedirection)
        {
            m_pfnWow64RevertWow64FsRedirection = (PFN_Wow64RevertWow64FsRedirection)GetProcAddr("Wow64RevertWow64FsRedirection");
            if (NULL == m_pfnWow64RevertWow64FsRedirection)
                return FALSE;
        }

        return m_pfnWow64RevertWow64FsRedirection(OldValue);
    }

    static BOOLEAN WINAPI Wow64EnableWow64FsRedirection(BOOLEAN Wow64FsEnableRedirection)
    {
        if (!m_pfnWow64EnableWow64FsRedirection)
        {
            m_pfnWow64EnableWow64FsRedirection = (PFN_Wow64EnableWow64FsRedirection)GetProcAddr("Wow64EnableWow64FsRedirection");
            if (NULL == m_pfnWow64EnableWow64FsRedirection)
                return FALSE;
        }

        return m_pfnWow64EnableWow64FsRedirection(Wow64FsEnableRedirection);
    }



    static HMODULE GetModHandle()
    {
        if (m_hKernel32)
            return m_hKernel32;

        m_hKernel32 = ::GetModuleHandle(L"kernel32");
        return m_hKernel32;
    }


    static void* GetProcAddr(LPCSTR lpProcName)
    {
        HMODULE hKernel32 = GetModHandle();
        if (!hKernel32)
            return NULL;

        return ::GetProcAddress(hKernel32, lpProcName);
    }

private:
    static HMODULE m_hKernel32;
    
    static PFN_GetNativeSystemInfo      m_pfnGetNativeSystemInfo;
    static PFN_GetProductInfo           m_pfnGetProductInfo;
    static PFN_IsWow64Process           m_pfnIsWow64Process;

    static PFN_Wow64DisableWow64FsRedirection   m_pfnWow64DisableWow64FsRedirection;
    static PFN_Wow64RevertWow64FsRedirection    m_pfnWow64RevertWow64FsRedirection;
    static PFN_Wow64EnableWow64FsRedirection    m_pfnWow64EnableWow64FsRedirection;
};



__declspec(selectany) HMODULE CWinModule_kernel32::m_hKernel32 = NULL;

__declspec(selectany) PFN_GetNativeSystemInfo   CWinModule_kernel32::m_pfnGetNativeSystemInfo = NULL;
__declspec(selectany) PFN_GetProductInfo        CWinModule_kernel32::m_pfnGetProductInfo = NULL;
__declspec(selectany) PFN_IsWow64Process        CWinModule_kernel32::m_pfnIsWow64Process = NULL;

__declspec(selectany) PFN_Wow64DisableWow64FsRedirection    CWinModule_kernel32::m_pfnWow64DisableWow64FsRedirection = NULL;
__declspec(selectany) PFN_Wow64RevertWow64FsRedirection     CWinModule_kernel32::m_pfnWow64RevertWow64FsRedirection = NULL;
__declspec(selectany) PFN_Wow64EnableWow64FsRedirection     CWinModule_kernel32::m_pfnWow64EnableWow64FsRedirection = NULL;



NS_WINMOD_END

#endif//KERNEL32MOD_H