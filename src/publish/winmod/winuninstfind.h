/**
* @file    winuninstfind.h
* @brief   ...
* @author  bbcallen
* @date    2009-05-22  11:02
*/

#ifndef WINUNINSTFIND_H
#define WINUNINSTFIND_H

#include <atlstr.h>
#include <atlbase.h>
#include "winregkeyfind.h"

NS_WINMOD_BEGIN

class CWinUninstFind
{
public:

    CWinUninstFind();
    virtual ~CWinUninstFind();

    // Operations
public:

    void    Reset();

    BOOL    FindFirstUninst(
        HKEY    hRootKey    = HKEY_LOCAL_MACHINE,
        REGSAM  samDesired  = KEY_ENUMERATE_SUB_KEYS);

    BOOL    FindNextUninst();
    BOOL    IsEndOfFind() const;

    DWORD   GetSystemComponent() const;
    LPCWSTR GetDisplayName() const;
    LPCWSTR GetPublisher() const;
    LPCWSTR GetUninstallString() const;



private:
    // Disabled
    CWinUninstFind(const CWinUninstFind&);
    CWinUninstFind& operator=(const CWinUninstFind&);

public:
    CWinRegKeyFind  m_hRegKeyFind;

protected:
    void    DoReadStandardValues();

    DWORD   m_dwSysmteComponent;
    CString m_strDisplayName;
    CString m_strPublisher;
    CString m_strUninstallString;
};


inline CWinUninstFind::CWinUninstFind()
{
    Reset();
}

inline CWinUninstFind::~CWinUninstFind()
{

}


inline void CWinUninstFind::Reset()
{
    m_hRegKeyFind.Reset();

    m_strDisplayName = L"";
}

inline BOOL CWinUninstFind::IsEndOfFind() const
{
    return m_hRegKeyFind.IsEndOfFind();
}

inline DWORD CWinUninstFind::GetSystemComponent() const
{
    return m_dwSysmteComponent;
}

inline LPCWSTR CWinUninstFind::GetDisplayName() const
{
    return m_strDisplayName;
}

inline LPCWSTR CWinUninstFind::GetPublisher() const
{
    return m_strPublisher;
}

inline LPCWSTR CWinUninstFind::GetUninstallString() const
{
    return m_strUninstallString;
}




NS_WINMOD_END

#endif//WINUNINSTFIND_H