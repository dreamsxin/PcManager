/**
* @file    winmodule.h
* @brief   ...
* @author  bbcallen
* @date    2009-02-25  11:08
*/

#ifndef WINMODULE_H
#define WINMODULE_H

#include <atlbase.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

class CWinModule
{
public:
    CWinModule();

    CWinModule(CWinModule& h);

    explicit CWinModule(HMODULE h);

    virtual ~CWinModule();

    virtual void FreeLib();




    operator HMODULE() const;

    void    Attach(HMODULE h);

    HMODULE Detach();

    HRESULT LoadLib(LPCWSTR lpDllPath);

    HRESULT LoadLibEx(LPCWSTR lpDllPath, DWORD dwFlag);

    FARPROC GetProcAddr(LPCSTR lpProcName);

    BOOL    IsValid();

    HRESULT HresultFromLastError();

private:
    // disabled
    CWinModule& operator=(CWinModule& h);

private:
    HMODULE m_hLib;
};



inline CWinModule::CWinModule(): m_hLib(NULL)
{

}

inline CWinModule::CWinModule(HMODULE h)
{
    if (m_hLib != h)
    {
        FreeLib();
        m_hLib = h;
    }
}

inline CWinModule::~CWinModule()
{
    FreeLib();
}

inline void CWinModule::FreeLib()
{
    if (m_hLib)
    {
        ::FreeLibrary(m_hLib);
        m_hLib = NULL;
    }
}

inline CWinModule::operator HMODULE() const
{
    return m_hLib;
}

inline void CWinModule::Attach(HMODULE h)
{
    FreeLib();
    m_hLib = h;
}

inline HMODULE CWinModule::Detach()
{
    HMODULE hLib = m_hLib;
    m_hLib = NULL;
    return hLib;
}

inline HRESULT CWinModule::LoadLib(LPCWSTR lpDllPath)
{
    HMODULE hLib = ::LoadLibrary(lpDllPath);
    if (!hLib)
    {
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
    }

    FreeLib();
    m_hLib = hLib;
    return S_OK;
}

inline HRESULT CWinModule::LoadLibEx(LPCWSTR lpDllPath, DWORD dwFlag)
{
    HMODULE hLib = ::LoadLibraryEx(lpDllPath, NULL, dwFlag);
    if (!hLib)
    {
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
    }

    FreeLib();
    m_hLib = hLib;
    return S_OK;
}

inline FARPROC CWinModule::GetProcAddr(LPCSTR lpProcName)
{
    if (!m_hLib)
        return NULL;

    return ::GetProcAddress(m_hLib, lpProcName);
}

inline BOOL CWinModule::IsValid()
{
    return NULL != m_hLib;
}

NS_WINMOD_END

#endif//WINMODULE_H