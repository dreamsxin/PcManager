/**
* @file    winhttpmodhandle.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 15:33
*/

#ifndef WINHTTPMODHANDLE_H
#define WINHTTPMODHANDLE_H

#include <assert.h>
#include "winmodbase.h"
#include "winhttpmodport.h"

NS_WINMOD_BEGIN

class CWinHttpHandle
{
public:
    CWinHttpHandle();
    virtual ~CWinHttpHandle();


    operator        HINTERNET() const;
    void            Attach(HINTERNET h, BOOL bOwn = TRUE);
    HINTERNET       Detach();
    virtual void    Close() {if (m_bOwn && m_h != NULL) {::WinHttpCloseHandle(m_h); m_h = NULL;}}

    HRESULT         DoSetOptionDWORD(DWORD dwOption, DWORD  dwValue);
    HRESULT         DoGetOptionDWORD(DWORD dwOption, DWORD& dwValue);

private:
    // denied
    CWinHttpHandle(CWinHttpHandle& h);
    CWinHttpHandle(HINTERNET h);
    CWinHttpHandle& operator=(CWinHttpHandle& h);

public:
    BOOL        m_bOwn;
    HINTERNET   m_h;
};


inline CWinHttpHandle::CWinHttpHandle(): m_bOwn(TRUE), m_h(NULL)
{
}

inline CWinHttpHandle::~CWinHttpHandle()
{
    if (m_h != NULL)
    {
        Close();
    }
}

inline CWinHttpHandle::operator HANDLE() const
{
    return(m_h);
}

inline void CWinHttpHandle::Attach(HANDLE h, BOOL bOwn)
{
    assert(m_h == NULL);
    m_h     = h;    // Copy
    m_bOwn  = bOwn; // Take ownership ?
}

inline HANDLE CWinHttpHandle::Detach()
{
    HANDLE h;

    h = m_h;  // Release ownership
    m_h = NULL;

    return(h);
}

inline HRESULT CWinHttpHandle::DoSetOptionDWORD(DWORD dwOption, DWORD  dwValue)
{
    assert(m_h);
    BOOL br = ::WinHttpSetOption(m_h, dwOption, &dwValue, sizeof(DWORD));
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpHandle::DoGetOptionDWORD(DWORD dwOption, DWORD& dwValue)
{
    assert(m_h);
    DWORD dwBufLen = sizeof(DWORD);
    BOOL br = ::WinHttpQueryOption(m_h, dwOption, &dwValue, &dwBufLen);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

NS_WINMOD_END

#endif//WINHTTPMODHANDLE_H