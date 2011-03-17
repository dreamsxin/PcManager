/**
* @file    winmodinethandle.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:09
*/

#ifndef WINMODINETHANDLE_H
#define WINMODINETHANDLE_H

#include <assert.h>
#include <wininet.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

class CInetHandle
{
public:
    CInetHandle();
    virtual ~CInetHandle();


    operator        HINTERNET() const;
    void            Attach(HINTERNET h, BOOL bOwn = TRUE);
    HINTERNET       Detach();
    virtual void    Close() {if (m_bOwn && m_h != NULL) {::InternetCloseHandle(m_h); m_h = NULL;}}

    HRESULT         DoSetOptionDWORD(DWORD dwOption, DWORD  dwValue);
    HRESULT         DoGetOptionDWORD(DWORD dwOption, DWORD& dwValue);

private:
    // denied
    CInetHandle(CInetHandle& h);
    CInetHandle(HINTERNET h);
    CInetHandle& operator=(CInetHandle& h);

public:
    BOOL        m_bOwn;
    HINTERNET   m_h;
};


inline CInetHandle::CInetHandle(): m_bOwn(TRUE), m_h(NULL)
{
}

inline CInetHandle::~CInetHandle()
{
    if (m_h != NULL)
    {
        Close();
    }
}

inline CInetHandle::operator HANDLE() const
{
    return(m_h);
}

inline void CInetHandle::Attach(HANDLE h, BOOL bOwn)
{
    assert(m_h == NULL);
    m_h     = h;    // Copy
    m_bOwn  = bOwn; // Take ownership ?
}

inline HANDLE CInetHandle::Detach()
{
    HANDLE h;

    h = m_h;  // Release ownership
    m_h = NULL;

    return(h);
}

inline HRESULT CInetHandle::DoSetOptionDWORD(DWORD dwOption, DWORD  dwValue)
{
    assert(m_h);
    BOOL br = ::InternetSetOption(m_h, dwOption, &dwValue, sizeof(DWORD));
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHandle::DoGetOptionDWORD(DWORD dwOption, DWORD& dwValue)
{
    assert(m_h);
    DWORD dwBufLen = sizeof(DWORD);
    BOOL br = ::InternetQueryOption(m_h, dwOption, &dwValue, &dwBufLen);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

NS_WINMOD_END

#endif//WINMODINETHANDLE_H