/**
* @file    sltokenstack.h
* @brief   ...
* @author  bbcallen
* @date    2008-12-26  22:07
*/

#ifndef SLTOKENSTACK_H
#define SLTOKENSTACK_H

#include <atlcoll.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

class CWinTokenHelper
{
public:
    HRESULT EnablePrivilege(LPCWSTR lpszPrivilege);

    HRESULT DisablePrivilege(LPCWSTR lpszPrivilege);
};



class CWinTokenStack: public CWinTokenHelper
{
public:
    CWinTokenStack();

    ~CWinTokenStack();

    HRESULT PushToken();

    HRESULT PopToken();

    HRESULT PopAllToken();

protected:
    CAtlList<HANDLE>   m_tokenStack;
};

NS_WINMOD_END

#endif//SLTOKENSTACK_H
