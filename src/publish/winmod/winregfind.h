/**
* @file    winregfind.h
* @brief   ...
* @author  bbcallen
* @date    2009-04-23  14:48
*/

#ifndef WINREGFIND_H
#define WINREGFIND_H

#include <assert.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <atlbase.h>
#include "winregkeyfind.h"
#include "winregvaluefind.h"

NS_WINMOD_BEGIN

class CWinRegFindDepthFirst
{
public:
    CWinRegFindDepthFirst();
    virtual ~CWinRegFindDepthFirst();

    BOOL    FindFirstKey(HKEY hKeyParent, LPCWSTR lpszKeyName, REGSAM samDesired = KEY_ENUMERATE_SUB_KEYS);

    BOOL    FindNextKey(REGSAM samDesired = KEY_ENUMERATE_SUB_KEYS);

    BOOL    IsEndOfFind() const;

    LPCWSTR GetSubKeyName() const;

    CString GetFullRegPath() const;

    const FILETIME& GetSubKeyLastWriteTime() const;


private:
    // Disabled
    CWinRegFindDepthFirst(const CWinRegFindDepthFirst&);
    CWinRegFindDepthFirst& operator=(const CWinRegFindDepthFirst&);

public:
    CWinRegKeyFind      m_root;

protected:

    void    Reset();

    typedef CAtlList<CWinRegKeyFind*>  CFindNodeStack;

    CFindNodeStack      m_findNodeStack;

    HKEY                m_hKeyParent;

    static FILETIME     m_ftZero;
};


inline CWinRegFindDepthFirst::CWinRegFindDepthFirst()
{
}

inline CWinRegFindDepthFirst::~CWinRegFindDepthFirst()
{
    Reset();
}

inline BOOL CWinRegFindDepthFirst::IsEndOfFind() const
{
    if (!m_findNodeStack.IsEmpty())
        return FALSE;

    return m_root.IsEndOfFind();
}

inline LPCWSTR CWinRegFindDepthFirst::GetSubKeyName() const
{
    assert(!IsEndOfFind());
    if (IsEndOfFind())
        return NULL;

    CWinRegKeyFind* pTopNode = m_findNodeStack.GetTail();
    assert(pTopNode);
    return pTopNode->GetSubKeyName();
}

inline CString CWinRegFindDepthFirst::GetFullRegPath() const
{
    assert(!IsEndOfFind());
    if (IsEndOfFind())
        return (LPCWSTR)NULL;

    CWinRegKeyFind* pTopNode = m_findNodeStack.GetTail();
    assert(pTopNode);
    return pTopNode->GetFullRegPath();
}

NS_WINMOD_END

#endif//WINREGFIND_H