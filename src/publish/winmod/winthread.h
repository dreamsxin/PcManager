/**
* @file    winthreadbase.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-27  16:06
*/

#ifndef WINTHREADBASE_H
#define WINTHREADBASE_H

#include <assert.h>
#include <atlbase.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

class IWinRunnable
{
public:
    virtual DWORD   STDMETHODCALLTYPE Run() = 0;
};






class CWinThread: public CHandle
{
public:

    HRESULT Create(IWinRunnable* piRunnable);
    HRESULT CreateNoCRT(IWinRunnable* piRunnable);

    DWORD   WaitExit(DWORD dwMilliseconds);
    BOOL    IsExit();
    //void    Terminate(DWORD dwExitCode = 1);

    DWORD   Suspend();
    DWORD   Resume();


    BOOL    SetPriority(int nPriority);
    int     GetPriority();


    DWORD   GetExitCode(DWORD dwDefaultCode = 0);

private:

    // for CRT thread API
    static unsigned int __stdcall RunThreadFunc(void* pParam);

    // for Win32 thread API
    static DWORD WINAPI RunThreadFuncNoCRT(LPVOID pParam);
};













#ifdef _ATL_MIN_CRT

inline HRESULT CWinThread::Create(IWinRunnable* piRunnable)
{
    return CreateNoCRT(piRunnable);
}

#else

inline HRESULT CWinThread::Create(IWinRunnable* piRunnable)
{
    assert(!m_h);
    assert(piRunnable);
    if (m_h)
        return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);

    if (!piRunnable)
        return E_POINTER;

    // use _beginthreadex for initialization of c runtime lib
    m_h = (HANDLE)_beginthreadex(NULL, 0, RunThreadFunc, piRunnable, CREATE_SUSPENDED, NULL);
    if (!m_h)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    Resume();
    return S_OK;
}

#endif//_ATL_MIN_CRT


inline HRESULT CWinThread::CreateNoCRT(IWinRunnable* piRunnable)
{
    assert(!m_h);
    assert(piRunnable);
    if (m_h)
        return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);

    if (!piRunnable)
        return E_POINTER;

    m_h = ::CreateThread(NULL, 0, RunThreadFuncNoCRT, piRunnable, CREATE_SUSPENDED, NULL);
    if (!m_h)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    Resume();
    return S_OK;
}


inline DWORD CWinThread::WaitExit(DWORD dwMilliseconds)
{
    return ::WaitForSingleObject(m_h, dwMilliseconds);
}

inline BOOL CWinThread::IsExit()
{
    return WAIT_TIMEOUT != WaitExit(0);
}

//inline void CWinThread::Terminate(DWORD dwExitCode)
//{
//    assert(m_h);
//    ::TerminateThread(m_h, dwExitCode);
//}


inline DWORD CWinThread::Suspend()
{
    assert(m_h);
    return ::SuspendThread(m_h);
}

inline DWORD CWinThread::Resume()
{
    return ::ResumeThread(m_h);
}


inline BOOL CWinThread::SetPriority(int nPriority)
{
    assert(m_h);
    return ::SetThreadPriority(m_h, nPriority);
}

inline int CWinThread::GetPriority()
{
    assert(m_h);
    return ::GetThreadPriority(m_h);
}


inline DWORD CWinThread::GetExitCode(DWORD dwDefaultCode)
{
    assert(m_h);
    DWORD dwExitCode;
    if (::GetExitCodeThread(m_h, &dwExitCode))
        return dwExitCode;

    return dwDefaultCode;
}




inline unsigned int __stdcall CWinThread::RunThreadFunc(void* pParam)
{
    DWORD dwRet = RunThreadFuncNoCRT(pParam);

#ifndef _ATL_MIN_CRT
    _endthreadex(dwRet);
#endif//_ATL_MIN_CRT

    return DWORD(dwRet);
}

inline DWORD WINAPI CWinThread::RunThreadFuncNoCRT(LPVOID pParam)
{
    IWinRunnable* pThis = (IWinRunnable*)pParam;

    if (pThis)
        return pThis->Run();

    return 0;
}





NS_WINMOD_END

#endif//WINTHREADBASE_H