/**
* @file    winfilefinddef.h
* @brief   ...
* @author  bbcallen
* @date    2010-04-21 17:06
*/

#ifndef WINFILEFINDDEF_H
#define WINFILEFINDDEF_H

#include "winmodbase.h"

NS_WINMOD_BEGIN

class IWinFileFindFilter
{
public:
    virtual BOOL NeedSkipDirFiles(LPCWSTR lpszDirectory) = 0;
    virtual BOOL NeedSkipDirTree(LPCWSTR lpszDirectory, WIN32_FIND_DATAW* pFindData) = 0;
};

NS_WINMOD_END

#endif//WINFILEFINDDEF_H