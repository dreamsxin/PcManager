/**
* @file    filepathtruncater.h
* @brief   ...
* @author  bbcallen
* @date    2010-06-12 13:27
*/

#ifndef FILEPATHTRUNCATER_H
#define FILEPATHTRUNCATER_H

#include "winmod\winpath.h"
#include "skylark2\skylarkbase.h"

NS_SKYLARK_BEGIN

class CFilePathTruncater
{
public:

    static CFilePathTruncater& Instance();

    CFilePathTruncater();

    HRESULT TruncateFilePath(LPCWSTR lpszFilePath, CString& strTruncated);

protected:

    BOOL ContractFilePath(CString& strPath, const CString& strComparePath, LPCWSTR lpszContractAs);

    CWinPath m_Path_Env_Temp;
    CWinPath m_Path_Env_Tmp;
    CWinPath m_Path_Env_ProgramFiles;
    CWinPath m_Path_Env_UserProfile;
    CWinPath m_Path_Env_WinDir;
};

NS_SKYLARK_END

#endif//FILEPATHTRUNCATER_H