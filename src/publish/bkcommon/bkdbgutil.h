/**
* @file    bkdbgutil.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-17 13:07
*/

#ifndef BKDBGUTIL_H
#define BKDBGUTIL_H

#include "bkdbg.h"

class CBKDebugTraceGuard
{
public:
    CBKDebugTraceGuard(DWORD dbgLevel);
    ~CBKDebugTraceGuard();

    void EndTrace(LPCWSTR lpszFormat, ...);
    void EndTraceV(LPCWSTR lpszFormat, va_list args);

    void FormatMessageAbort(LPCWSTR lpszFormat, ...);
    void FormatMessageAbortV(LPCWSTR lpszFormat, va_list args);

public:
    DWORD   m_dwDbgLevel;

protected:
    CString m_strMessageAbort;
};


inline CBKDebugTraceGuard::CBKDebugTraceGuard(DWORD dbgLevel)
    : m_dwDbgLevel(dbgLevel)
{

}

inline CBKDebugTraceGuard::~CBKDebugTraceGuard()
{
    if (m_strMessageAbort.IsEmpty())
        return;

    BKDebugEcho(m_dwDbgLevel, m_strMessageAbort);
}

inline void CBKDebugTraceGuard::EndTrace(LPCWSTR lpszFormat, ...)
{
    if (!BKIsDebugOutputEnabled(m_dwDbgLevel))
        return;

    va_list argList;
    va_start(argList, lpszFormat);

    EndTraceV(lpszFormat, argList);

    va_end( argList );
}

inline void CBKDebugTraceGuard::EndTraceV(LPCWSTR lpszFormat, va_list args)
{
    if (!BKIsDebugOutputEnabled(m_dwDbgLevel))
        return;

    // 清除退出消息
    m_strMessageAbort.Empty();

    CString strMessage;
    strMessage.FormatV(lpszFormat, args);
    BKDebugEcho(m_dwDbgLevel, strMessage);
}

inline void CBKDebugTraceGuard::FormatMessageAbort(LPCWSTR lpszFormat, ...)
{
    if (!BKIsDebugOutputEnabled(m_dwDbgLevel))
        return;

    va_list argList;
    va_start(argList, lpszFormat);

    FormatMessageAbortV(lpszFormat, argList);

    va_end( argList );
}

inline void CBKDebugTraceGuard::FormatMessageAbortV(LPCWSTR lpszFormat, va_list args)
{
    if (!BKIsDebugOutputEnabled(m_dwDbgLevel))
        return;

    m_strMessageAbort.FormatV(lpszFormat, args);
}






class CBKDebugIniter
{
public:
    CBKDebugIniter()
    {
        BKInitDebugOutput();
    }
};






#endif//BKDBGUTIL_H