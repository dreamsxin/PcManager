/**
* @file    winprocessenumerator.h
* @brief   ...
* @author  bbcallen
* @date    2009-02-25  11:27
*/

#ifndef WINPROCESSENUMERATOR_H
#define WINPROCESSENUMERATOR_H

#include <psapi.h>
#include <tlhelp32.h>

#include <atlbase.h>
#include <atlcoll.h>
#include <atlstr.h>
#include "winmodbase.h"
#include "wintokenstack.h"
#include "psapimod.h"

NS_WINMOD_BEGIN

#define UNICODE_MAX_PATH 32768


class CWinProcessApi
{
public:
    static HRESULT ObtainImpersonateToken(DWORD dwProcessId, HANDLE& hTokenObtain);

    static HRESULT ExecuteAsDesktopUser(
        LPCWSTR                 lpszExePath,
        LPCWSTR                 lpszCommandLine,
        DWORD                   dwDesktopProcessId = 0,
        PROCESS_INFORMATION*    pProcessInformation = NULL);

    static HRESULT ObtainExplorerToken(HANDLE& hTokenObtain);
};



class CWinModuleEnumerator
{
public:
    HRESULT EnumAllModules(DWORD dwProcessID);
    BOOL    FindFirstModule();
    BOOL    FindNextModule();

    BOOL    EndOfModules();
    HMODULE GetModule();
    HRESULT GetModulePath(CString& strPath);

protected:
    void    Reset();

    CHandle             m_hProc;
    CAtlArray<HMODULE>  m_moduleList;
    size_t              m_moduleIndex;
};




class CWinProcessEnumerator
{
public:
    CWinProcessEnumerator();


    HRESULT EnumAllProcesses();
    BOOL    FindFirstProcess();
    BOOL    FindNextProcess();

    BOOL    EndOfProcesses();
    DWORD   GetProcessID();


    HRESULT GetProcessPath(CString& strProcessPath);

    HRESULT GetProcessImageName(CString& strProcessImageName);

    HRESULT GetProcessName(CString& strProcessName);

    HRESULT ObtainImpersonateToken(HANDLE& hTokenObtain);


protected:

    void    Reset();

    CAtlArray<DWORD>    m_procList;
    size_t              m_procIndex;

    CWinModule_psapi    dll_psapi;
};



class CWinProcessToolHelpEnumerator
{
public:
    CWinProcessToolHelpEnumerator();

    BOOL    FindProcessID(DWORD dwProcessID);

    HRESULT EnumAllProcesses();
    BOOL    FindFirstProcess();
    BOOL    FindNextProcess();

    BOOL    EndOfProcesses();
    DWORD   GetProcessID();
    HRESULT GetProcessName(CString& strProcessName);

private:
    CHandle         m_hProcessSnap;
    PROCESSENTRY32  m_pe32;
    BOOL            m_bValieProcEntry;
};

NS_WINMOD_END

#endif//WINPROCESSENUMERATOR_H