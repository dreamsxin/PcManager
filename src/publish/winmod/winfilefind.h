/**
* @file    winfilefind.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-06  11:30
*/

#ifndef WINFILEFIND_H
#define WINFILEFIND_H

#include <atlstr.h>
#include <atlcoll.h>
#include "winmodbase.h"
#include "winfilefinddata.h"

NS_WINMOD_BEGIN

// auto skip . and ..
class CWinFileFind: public CWinFileFindData
{
public:
    CWinFileFind();
    ~CWinFileFind();

// Operations
public:
    BOOL    FindFirstFile(LPCTSTR pszName);
    BOOL    FindNextFile();

    void    Close();
    HANDLE  GetHandle();

private:

    // Disabled
    CWinFileFind(const CWinFileFind&);
    CWinFileFind& operator=(const CWinFileFind&);

protected:
    HANDLE  m_hContext;
};

NS_WINMOD_END

#endif//WINFILEFIND_H