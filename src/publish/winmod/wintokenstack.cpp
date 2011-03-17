/**
* @file    wintokenstack.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-02-25  11:27
*/

#include "stdafx.h"
#include "wintokenstack.h"

#include <assert.h>
#include <atlsecurity.h>

using namespace WinMod;

HRESULT CWinTokenHelper::EnablePrivilege(LPCWSTR lpszPrivilege)
{
    assert(lpszPrivilege);


    CAccessToken token;
    BOOL br = token.GetEffectiveToken(TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    br = token.EnablePrivilege(lpszPrivilege);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    return S_OK;
}

HRESULT CWinTokenHelper::DisablePrivilege(LPCWSTR lpszPrivilege)
{
    assert(lpszPrivilege);


    CAccessToken token;
    BOOL br = token.GetEffectiveToken(TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    br = token.DisablePrivilege(lpszPrivilege);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    return S_OK;
}










CWinTokenStack::CWinTokenStack()
{

}

CWinTokenStack::~CWinTokenStack()
{
    PopAllToken();
}

HRESULT CWinTokenStack::PushToken()
{
    CAccessToken token;
    token.GetThreadToken(TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY);
 
    

    // no impersonate token, so impersonate self and push NULL;
    if(!::ImpersonateSelf(SecurityImpersonation))
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;



    // push previous token
    m_tokenStack.AddTail(token.Detach());
    return S_OK;
}

HRESULT CWinTokenStack::PopToken()
{
    assert(!m_tokenStack.IsEmpty());
    if (m_tokenStack.IsEmpty())
        return E_FAIL;



    if (m_tokenStack.GetTail())
    {   // revert to stack base token
        CAccessToken token;
        token.Attach(m_tokenStack.RemoveTail());

        
        bool br = token.Revert();
        if (!br)
            return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
    }
    else
    {   // revert to self
        BOOL br = ::RevertToSelf();
        if (!br)
            return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
    }
    



    return S_OK;
}

HRESULT CWinTokenStack::PopAllToken()
{
    // release all tokens
    while (m_tokenStack.GetCount() > 1)
    {
        HANDLE hToken = m_tokenStack.RemoveTail();
        if (hToken)
            ::CloseHandle(hToken);
    }


    // revert to stack base token
    if (!m_tokenStack.IsEmpty())
        return PopToken();


    return S_FALSE;
}
