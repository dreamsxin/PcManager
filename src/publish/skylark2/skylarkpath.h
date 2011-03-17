/**
* @file    skylarkpath.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-17 11:31
*/

#ifndef SKYLARKPATH_H
#define SKYLARKPATH_H

#include <atlstr.h>
#include "skylark2\skylarkbase.h"

NS_SKYLARK_BEGIN

class CSkylarkPath
{
public:
    static HRESULT GetDataPath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetRuntimeDataPath(CString& strPath, BOOL bCreate = TRUE);

    static HRESULT GetRecyclePath(LPCWSTR lpszRecycleFile, CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetQuarantinePath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetLogPath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetCachePath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetScanTempPath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetTempPath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetAvsPath(CString& strPath, BOOL bCreate = TRUE);
    static HRESULT GetConfigPath(CString& strPath, BOOL bCreate = TRUE);

    static HRESULT CreateFullPath(LPCWSTR lpszFullPath);
};

NS_SKYLARK_END

#endif//SKYLARKPATH_H