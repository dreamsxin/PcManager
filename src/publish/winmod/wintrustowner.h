/**
* @file    wintrustowner.h
* @brief   ...
* @author  bbcallen
* @date    2009-04-09  11:33
*/

#ifndef WINTRUSTOWNER_H
#define WINTRUSTOWNER_H

#include "winmodbase.h"

NS_WINMOD_BEGIN

class CWinTrustOwner
{
public:
    static BOOL IsTrustedOwner(HANDLE hFile);
};

NS_WINMOD_END

#endif//WINTRUSTOWNER_H