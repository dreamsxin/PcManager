/**
* @file    winlogfile.h
* @brief   ...
* @author  bbcallen
* @date    2010-02-02 13:29
*/

#ifndef WINLOGFILE_H
#define WINLOGFILE_H

#include <atlstr.h>
#include <atlfile.h>
#include <atlcoll.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

#define WINMOD_LLVL_ALWAYS       0              ///< ALWAYS
#define WINMOD_LLVL_CRITICAL     10             ///< CRITICAL
#define WINMOD_LLVL_ERROR        20             ///< ERROR
#define WINMOD_LLVL_WARNING      30             ///< WARNING
#define WINMOD_LLVL_INFO         40             ///< INFO
#define WINMOD_LLVL_DEBUG        50             ///< DEBUG
#define WINMOD_LLVL_TRACE        60             ///< TRACE

#define WINMOD_LLVL_DEFAULT      WINMOD_LLVL_INFO

class CWinLogFile: public CAtlFile
{
public:
    CWinLogFile();

    HRESULT CreateAsLogFile(LPCWSTR lpszFilePath);

    void    SetLogLevel(DWORD dwMaxLogLevel = WINMOD_LLVL_INFO);
    BOOL    IsLevelEnabled(DWORD dwLogLevel);
    LPCSTR  GetLogLevelNameA(DWORD dwLogLevel);
    DWORD   GetMaxLogLevel() {return m_dwMaxLogLevel;}

    void    Log(DWORD dwLogLevel, LPCWSTR lpszText);
    void    LogFormat(DWORD dwLogLevel, LPCWSTR lpszFormat, ...);
    void    LogFormatV(DWORD dwLogLevel, LPCWSTR lpszFormat, va_list args);

private:
    CWinLogFile(CWinLogFile&);
    CWinLogFile& operator=(CWinLogFile&);

protected:
    typedef CComAutoCriticalSection                     CObjLock;
    typedef CComCritSecLock<CComAutoCriticalSection>    CObjGuard;

    CComAutoCriticalSection     m_queueLock;

    DWORD                       m_dwMaxLogLevel;
    CAtlList<CStringA>          m_pendingQueue; ///< store no more than 5 record
};

NS_WINMOD_END

#endif//WINLOGFILE_H