/**
* @file    winrunnable.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-27  15:54
*/

#ifndef WINRUNNABLE_H
#define WINRUNNABLE_H

#include <atlsync.h>
#include <atlbase.h>
#include "winthread.h"

NS_WINMOD_BEGIN

/**
* @brief abstract class for thread
*/
class AWinRunnable: public IWinRunnable
{
public:
    AWinRunnable();
    virtual ~AWinRunnable() {}

    void    CloseThread();
    HRESULT StartRunning();

    DWORD   WaitExit(DWORD dwMilliseconds);
    BOOL    IsExit();
    DWORD   GetExitCode(DWORD dwDefaultCode = 0);

protected:

    // override this function to implement thread routine
    virtual DWORD STDMETHODCALLTYPE Run() = 0;

protected:
    CWinThread  m_hThread;  
};



inline AWinRunnable::AWinRunnable()
{

}


inline void AWinRunnable::CloseThread()
{
    m_hThread.Close();
}

inline HRESULT AWinRunnable::StartRunning()
{
    if (m_hThread && !m_hThread.IsExit())
    {
        return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);
    }
        
    m_hThread.Close();

    return m_hThread.Create(this);
}

inline DWORD AWinRunnable::WaitExit(DWORD dwMilliseconds)
{
    return m_hThread.WaitExit(dwMilliseconds);
}

inline BOOL AWinRunnable::IsExit()
{
    return m_hThread.IsExit();
}

inline DWORD AWinRunnable::GetExitCode(DWORD dwDefaultCode)
{
    return m_hThread.GetExitCode(dwDefaultCode);
}



NS_WINMOD_END

#endif//WINRUNNABLE_H